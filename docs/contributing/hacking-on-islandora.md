# Hacking on Islandora 8

One of the goals of the Islandora 8 project is to lower the barrier to entry for working on and with Islandora. A shared, consistent environment amongst developers is one of the best ways to do this. To get started hacking on Islandora 8 with your very own development environment, all you need to do follow these steps:

1. Install [VirtualBox](https://www.virtualbox.org/)
2. Install [Vagrant](https://www.vagrantup.com/)
3. Fork [Islandora CLAW](https://github.com/Islandora-CLAW/CLAW) and clone it onto your machine using the main branch
```bash
~ $ git clone -b main https://github.com/your_github_name/islandora.git
```
OR
If you're already a developer and have previously forked and cloned Islandora CLAW, just add the main branch from Islandora-CLAW to your existing checkout:
```bash
~ $ cd /path/to/islandora
/path/to/CLAW $ git remote add claw https://github.com/Islandora-CLAW/CLAW.git
/path/to/CLAW $ git fetch claw
/path/to/CLAW $ git checkout main
```
5. Navigate into the `install` directory of your main checkout
```bash
~ $ cd /path/to/CLAW/install
```
6. Build your vm using `vagrant up`
```bash
/path/to/CLAW/install $ vagrant up
```

Sit back and relax as your development environment is created for you! In a few minutes you’ll have a brand new Islandora CLAW install that you can use and abuse as much as you’d like. If you ever ruin your environment (trust me, it’ll happen), you can always destroy it and bring up a new one at any time.

```bash
~ $ cd /path/to/CLAW/install
/path/to/CLAW/install $ vagrant destroy
/path/to/CLAW/install $ vagrant up
```

## Logging in to your development environment

You can ssh into your development environment at any time by doing the following:

```bash
~ $ cd /path/to/islandora/install
/path/to/islandora/install $ vagrant ssh
Welcome to Ubuntu 14.04.2 LTS (GNU/Linux 3.13.0-45-generic x86_64)

 * Documentation:  https://help.ubuntu.com/

 System information disabled due to load higher than 1.0

  Get cloud support with Ubuntu Advantage Cloud Guest:
    http://www.ubuntu.com/business/services/cloud


Last login: Fri Jul 17 13:51:38 2015 from 10.0.2.2
vagrant@islandora:~$
```

You'll be logged in as the vagrant user, with your GitHub repo in your home directory.

## Port Forwarding

Many ports are forwarded from your virtual machine to your personal computer, so care must be taken that there are no conflicts. Here's a list of all the ports that get forwarded:
- Apache: 80 -> 8000
- Tomcat: 8080 -> 8080
- Karaf: 8181 -> 8181
- MySQL: 3306 -> 3306

At any point in time, you can edit how the ports are forwarded by clicking on the Port Forwarding button in your virtual machine's Network settings in VirtualBox.

You can also edit the section of the Vagrantfile that controls the default port forwarding, which looks something like this:
```ruby
  config.vm.network :forwarded_port, guest: 8080, host: 8080 # Tomcat
  config.vm.network :forwarded_port, guest: 8181, host: 8181 # Karaf
  config.vm.network :forwarded_port, guest: 3306, host: 3306 # MySQL
  config.vm.network :forwarded_port, guest: 5432, host: 5432 # PostgreSQL
  config.vm.network :forwarded_port, guest: 80, host: 8000 # Apache
```

## Important locations

- Drupal: `/var/www/html/drupal`
- Apache Logs: `/var/log/apache2/error.log`
- Tomcat: `/var/lib/tomcat7`
    - Logs: `/var/log/tomcat7`
    - Fedora 4: `/var/lib/tomcat7/webapps/fcrepo`
    - Solr: `/var/lib/tomcat7/webapps/solr`
    - BlazeGraph: `/var/lib/tomcat7/webapps/bigdata`
    - FcrepoCamelToolbox: `/var/lib/tomcat7/webapps/fcrepo-camel-toolbox`
- Karaf: `/opt/karaf`
    - Logs: `/opt/karaf/data/log`
    - Configuration: `/opt/karaf/etc`
- CLAW: `/home/vagrant/CLAW`

## Shared folders

By default, the vagrant environment uses a shared folder between your computer and the virtual machine it has created. This folder points to your Islandora CLAW GitHub repo on your computer and is available at `~/CLAW` (`/home/vagrant/CLAW`) on the virtual machine. On top of that, in Drupal, `sites/all/modules/islandora` is a symlink pointing to `~/CLAW/islandora`.  This has a few profound consequences:

- You can use the IDE you're comfortable with on your own machine, without the need to scp/rsync code over to the development environment
- Changes to Drupal module code are automatically reflected on your Drupal site since that code is symlinked (although you may need to be ssh'd in to `drush cc all` on occassion)
- When testing the Vagrant install after adding changes, the code from your fork and your branch on your computer is used to generate the virtual machine

## Compiling Camel Bundles

Despite being nothing but Blueprint xml files, Camel projects still have to be compiled using Maven. The `install` directive is the default, so all one has to do is navigate to the appropriate directory and issue a `mvn` in the command line.  For example, to compile the collection service:
```bash
~ $ cd /path/to/CLAW/Alpaca/services/collection-service
/path/to/CLAW/Alpaca/serices/collection-service $ mvn
```

The only caveat here is that this is best done on the virtual machine itself. While your virtual machine was being built, it's Karaf installation was set to monitor the vagrant user's Maven repository for changes, and autodeploy in response to `mvn install`. So while you can always run Maven from your own machine, it will get installed in your personal Maven repository on your machine. So to take advantage of the auto-redeploy capabilities of Karaf, you need to do this as the vagrant user.

If for any reason, you have to shutdown or restart Karaf, it will cease to monitor the vagrant user's Maven repository. In order to turn this back on, you'll have to issue the appropriate commands to get monitoring working again. The easiest way to do this is to re-run the karaf script the install process uses to set up monitoring by executing `/opt/karaf/bin/client < ~/islandora/install/karaf/monitor.script`.
