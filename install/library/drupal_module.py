#!/usr/bin/python

def main():
    module = AnsibleModule(
        argument_spec = dict(
            name  = dict(required=True, aliases=['module'], type="list"),
            state = dict(default='present', choices=['present', 'enabled']),
            chdir = dict(default='/var/www/html/drupal'),
        ),
        supports_check_mode = False
    )

    # Change to the given directory
    os.chdir(os.path.abspath(os.path.expanduser(module.params['chdir'])))

    # Gather information
    state = module.params['state']
    drupal_modules = module.params['name']

    cmd = 'drush pm-info --fields=extension,status --format=csv'
    rc, out, err = module.run_command(cmd)
    drupal_module_info = {}
    for row in out.splitlines():
        fields = row.split(',')
        extension = fields[0]
        status = fields[1]
        drupal_module_info[extension] = status

    results = {}
    results['results'] = []
    results['msg'] = ''
    results['rc'] = 0

    # Options from Drush: disabled, enabled, not installed
    if state == 'present':

        for drupal_module in drupal_modules:
            if drupal_module not in drupal_module_info:
                cmd = 'drush dl -y %s' % drupal_module
                rc, out, err = module.run_command(cmd)
                results['rc'] = rc
                results['results'].append(out)
                results['msg'] = err
                if rc != 0:
                    module.fail_json(msg='Error from drush: %s: %s' % (cmd, err))

    elif state == 'enabled':
        for drupal_module in drupal_modules:
            drupal_module_exists = drupal_module in drupal_module_info
            if not drupal_module_exists or drupal_module_info[drupal_module] != 'enabled':
                cmd = 'drush en -y %s' % drupal_module
                rc, out, err = module.run_command(cmd)
                results['rc'] = rc
                results['results'].append(out)
                results['msg'] = err
                if rc != 0:
                    module.fail_json(msg='Error from drush: %s: %s' % (cmd, err))

    module.exit_json(**results)

from ansible.module_utils.basic import *
from subprocess import call
if __name__ == '__main__':
    main()
