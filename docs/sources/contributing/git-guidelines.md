In general we follow the [[Git guidelines|https://wiki.duraspace.org/display/FCREPO/Git+Guidelines+and+Best+Practices#GitGuidelinesandBestPractices-CommitMessages]] for the fcrepo project since they are sensible. They go into a lot of detail about line endings which we don't need to worry about as much at the moment. Specifically we should try and do the following, however:

# Commit Messages

Commit messages should follow the guidelines described in detail at [here](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html).

In summary:

* First line: Github issue number in all caps (if applicable), followed by a brief description (~ 50 characters)
* Second line: blank
* Following lines: more detailed description, line-wrapped at 72 characters. May contain multiple paragraphs, separated by blank lines. Link to the Github issue, if applicable.
Use the present tense when writing messages, i.e. "Fix bug, apply patch", not "Fixed bug, applied patch."

Using the git command line tool you can add multi-line commit messages by typing a \ at the end of a line and continuing to type. E.g.,
```console
$ git commit -m "ISLANDORA-110 Cannot delete content model when there is only one left.\
\
Remove logic that prevents delete widget from being drawn."
$ 
```
# Tags

Each major release of Islandora has a tag associated with it. This tag will point to a commit in a given release branch. This is the code actually released in the zip files on a given release's release notes page. This allows us to have an easily accessible archive of past releases.

# Branches

## Main Project Branches

Many of our projects are dependent on Drupal, and we have projects with multiple `master` branches, one for each supported version of Drupal, the main branches for each projects repository are named after the version of Drupal that is supported by that version of the project. For example Islandora has `6.x` for Drupal 6 and `7.x` for Drupal 7. These branches should be treated as the `master` branch for each version, and not developed on top of as is mentioned above.  All of our projects should follow this naming convention for their `master` branch for consistency. 

There are also release branches associated with each of the master branches. These were named after the master branch they correspond to. For example: `6.x-release` and `7.x-release`. These branches were an archive of the "blessed" released code. They exist mainly so that we could do things like modify the module.info files for releases and cherry-pick commits from the master branch to backport bugfixes into a release.  With Islandora 7.x-1.3 we have moved to a new approach where we will have a new branch for each release.  When simple pull requests will not work for bringing code into these branches we will not require cherry-pick`d commits; simply re-commited edits will be acceptable. It is however highly advisable to use cherry-pick for anything other than the most trivial pull requests.

## Issue / Topic Branches

All Github issues should be worked on in separate git branches. The branch name should be the same as the Github issue number, including all-caps, so ISLANDORA-153, ISLANDORA-118, etc.

Example: `git checkout -b 7.x-2.x-ISLANDORA-977` or `git checkout -b 7.x-2.1-ISLANDORA-977`

When working on a branch, before committing and merging it's best to pull updates from the 7.x branch or current release branch (ex. 7.x-1.4) into your own branch. This makes for more graceful merge logic and more intuitive-looking github graphs. This is discussed extensively in the [[Development in a local branch|https://wiki.duraspace.org/display/FCREPO/Git+Guidelines+and+Best+Practices#GitGuidelinesandBestPractices-Developmentinalocalbranch]] section of the fcrepo git guidelines.

When ready to have a branch merged into 7.x and/or release branch, the Github ticket should be resolved with a status of 'Ready to test'. We will discuss the changes at the weekly committers call and then if agreed upon the release manager will merge the changes into master and delete the topic branch. Once the merge with 7.x and/or release branch is complete, change the fix status of the Github ticket to 'Ready for release'.

## Pull requests

When creating a pull request, if the pull request is not trivial, please make sure to provide an appropriate amount of information with the pull request. Once the request is made, Travis-CI will be initiated. Travis-CI will run PHP Codesniffer, Drupal Coding Standards, PHP Copy/Paste Detector, and any module specific tests. When those tests pass, a community reviewer will review the pull request. If a week goes by without a maintainer noticing a pull request, please mention @manez or @ruebot in a comment on the pull request and they will shepherd the process.

Community members who have push/merge permissions on a repository should **never** push directly to a repo, nor merge their own pull requests. 
