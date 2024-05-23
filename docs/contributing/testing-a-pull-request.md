# Testing a Pull Request (i.e. Running Custom Code)

If you are testing a pull request, or for other reasons need to replace the 
"official" code with code that's in a PR, or a different branch or fork, this 
page offers two methods: using Composer Patches, and using Composer to 
require the branch and/or fork.

This documentation applies to Drupal modules, themes, and recipes, or any 
other project that is managed by Composer.

!!! note "Can't I just `git clone`?"
    When managing a site with Composer, it's very fragile to use commands 
    like `git clone` to obtain code, as it can be overwritten by 
    a `composer update`. 

## Applying a Patch using Composer Patches

This method is best for testing pull requests, because it's very easy to get a 
patch from a pull request. If the desired code is not the subject of a 
PR, you can still use this method but must generate a `.patch` file yourself.
You may wish to use the other method, as it is more dynamic - see 
section **Using Composer to require a fork or branch**. 

Run the following commands from within your site's root folder. They will 
update your `composer.json` file.

```shell
# Enable Composer Patches
composer config allow-plugins.cweagans/composer-patches true
composer require cweagans/composer-patches
```

For the next step, prepare the following replacement tokens:

* `MY_PACKAGE`: The full Composer name of the package to patch. It 
  takes the form [vendor name]/[project name]. Example: 
  `drupal/controlled_access_terms`
* `MY_ISSUE_TITLE`: A descriptive way to identify what the patch is for. 
  Best practice is to include a link to the related issue - 
  especially if you're going to keep this patch around for a while.
  Example: `Updated config format https://github.com/Islandora/controlled_access_terms/issues/117`
* `MY_PATCH_LOCATION`: Where to access the patch. See below.

To get the URL of a patch for a PR, go to the PR's main URL, and append
`.patch` to the URL. Make sure that your URL ends with `pull/XX.patch` 
and not `pull/XX/files.patch` - the latter will not work.

If you don't have a PR, you could create a patch using `diff`. However, this 
patch will be static and will need to be updated manually if your code 
changes. You can put patches in a folder in your root directory such as `
[COMPOSER_ROOT]/assets/` and then the patch location would be 
`assets/my_patch_name.patch`.

This one-liner, with the substitutions above, will add the patch to your 
`composer.json`:

```shell
# Add patch to composer.json
composer config extra.patches --merge --json '{"MY_PACKAGE": {"MY_ISSUE_TITLE": "MY_PATCH_LOCATION"}}'

```

Or you could manually edit `composer.json` so it contains the following
(`...` denotes omitted content):
```json
{
  ...
  extra: {
    ...    
    patches: {
      "MY_PACKAGE": {
        "MY_ISSUE_TITLE": "MY_PATCH_LOCATION"
      }
    }
  }
}
```

Then, update your package (recall e.g. `drupal/controlled_access_terms`) 
using Composer:

```shell
composer update MY_PACKAGE
```

The patch should apply, and then you will be running a patched version! If 
you're using a dynamic patch, then running `composer update` again should 
pull in changes to the code.

## Using Composer to require a fork or branch

This method is best if you don't have a pull request open for the code.

### Step 1: Add a repository (if necessary)

If your code is on a fork, then you will need to add a repository to 
Composer so that it knows where to get your package.

If the code that you want to test is a different branch/tag on the same 
repository that you're currently getting your code from, then you do not 
need to add a repository.

Prepare the following replacement tokens before adding a repository:

* `REPO_NAME`: a name for this repository (mandatory if using the composer 
  one-liner), e.g. `rosiel-islandora`
* `REPO_URL`: the URL to the repository, e.g. `https://github.com/rosiel/islandora`

```shell
# Add custom repo
composer config repo.REPO_NAME vcs REPO_URL
```
Your `composer.json` file should now contain

```json
{
  ...
  "repositories": {
    ...
    "REPO_NAME": {
      "type": "vcs",
      "url": "REPO_URL"
    }
  }
}
```

!!! note "Order of precedence of repositories"
    If you have a matching version spec (e.g. branch name) that's available 
    from multiple repositories, e.g. both islandora's Gitlab and your personal 
    fork both have an `enable-hocr` branch, then the repository that's first 
    in the list in composer.json will take precedence.

### Step 2: Require theß custom branch

This step could be as simple as
```shell
composer require MY_PACKAGE:dev-MY_BRANCH_NAME
```

with the following replacements:

* `MY_PACKAGE`: the full Composer name of the package. Example: 
`drupal/islandora`
* `MY_BRANCH_NAME`: the name of the branch you want to run. Example: 
  `testing-fedora-6`. Note that in the case that your branch name is 
  "version-like" for example `2.x`, then the `dev` goes at the end, as in
  `2.x-dev`, instead of preceding the branch name as in the template above.
 

However, if your component is a dependency of another component, then you 
will probably need to use an alias. This allows your custom code to "act as" 
a version that will meet the requirements of your other component. For 
example, if the `drupal/islandora_mirador` package requires 
`drupal/islandora:^2.4.1`, then using Composer to require the `enable-hocr` 
branch of `drupal/islandora` will not meet the requirements. Instead, use 
`as` to provide an alias, to a version that will match the constraints. Note 
the quotes around the package name and version spec. This takes the form:

```shell
composer require "MY_PACKAGE:dev-MY_BRANCH_NAME as ALIAS"
```
For example:
```shell
composer require "drupal/islandora:dev-enable-hocr as 2.12.1"
```

That will install the specified branch and allow it to work with your
dependencies.

## To reset these changes

### ... using Composer Patches

When you no longer need to be applying the patch, simply remove it from the 
`patches:` section of `composer.json` (and of course, take care to ensure the 
json remains valid, by adjusting commas!) and run `composer update MY_PACKAGE`.

### ... using Composer require

When you no longer want to pull from a separate branch or fork, reset the 
version constraint back to what it used to be, or, if your package was not 
originally in `composer.json` (because it is required as a dependency by 
another package), you can delete the requirement from your `composer.json`. 
Then run `composer update MY_PACKAGE`. If you added a repository, it's 
safest to delete the repository, as it could lead to you getting stale 
branches from a fork rather than the desired active code from the canonical 
repository.

More great information is available in the [Composer Documentation](https://getcomposer.org/doc/). 