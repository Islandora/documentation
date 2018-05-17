# Introduction

Islandora CLAW makes use of [drupal-project](https://github.com/drupal-composer/drupal-project), a composer template for Drupal projects. We augment it with Islandora CLAW specific changes, and need to occasionally pull in upstream changes. The process below outlines how we will do it in a consistent manner.

# Pull in upstream changes

1. Clone a fork of our fork to your or your institution's GitHub organization:
<br />  `git clone fork`
2. Add the drupal-composer repository as a remote:
<br /> `git remote add upstream https://github.com/drupal-composer/drupal-project.git`
3. Fetch everything:
<br /> `git fetch --all`
4. Create a branch to pull in changes that is based off the Islandora-CLAW 8.x-1.x branch: 
<br /> `git checkout -b sync-upstream`
5. Rebase upstream changes:
<br /> `git rebase upstream/8.x` (fix any merge conflicts, and then `git rebase --continue`)
6. Push to your fork:
<br /> `git push origin sync-upstream`
7. Create pull request
