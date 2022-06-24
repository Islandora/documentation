# Welcome!

If you are reading this document then you are interested in contributing to Islandora. All contributions are welcome: use-cases, documentation, code, patches, bug reports, feature requests, etc. You do not need to be a programmer to speak up!

## Weekly Open Tech Call

The Islandora Tech Call meets each Wednesday at 1:00 PM Eastern. You can view meeting agendas, notes, and call-in information [here](https://github.com/Islandora/islandora-community/wiki/Weekly-Open-Tech-Call). Anybody is welcome to join the call, and add items to the agenda.

## Github

You can use Github to:

- report bugs
- request documentation or report unclear documentation
- request features
- submit use cases

See more about onboarding to the Islandora community, including Islandora on Github, on the islandora-community Onboarding page [here](https://github.com/Islandora/islandora-community/wiki/Onboarding-Checklist#general-onboarding).

### Report a bug
To report a bug you should [open an issue in the Islandora repository](https://github.com/Islandora/documentation/issues/new/choose), selecting the **Bug report** template. 

Please summarize the bug along best practice guidelines outlined in the template. In order to help us understand and fix the bug it would be great if you could provide us with:

1. The steps to reproduce the bug. This includes information about e.g. the Islandora version you were using along with the versions of stack components.
2. The expected behavior.
3. The actual, incorrect behavior.

Feel free to search the issue queue for existing issues (aka tickets) that already describe the problem; if there is such a ticket please add your information as a comment.

**If you want to provide a pull along with your bug report:**

That is great! In this case please send us a pull request as described in the section _Create a pull request_  below.

### Documentation

You can contribute documentation in two different ways. One way is to open an issue [here](https://github.com/Islandora/documentation/issues/new/choose), using the **Documentation** template. This template will ask for further information about where your documentation request belongs and what it should contain.
Another way is by pull request, which is the same process as [Contribute Code](https://github.com/Islandora/documentation/blob/main/CONTRIBUTING.md#contribute-code) and requires the same Contributor License Agreements. All documentation resides in [`docs`](https://github.com/Islandora/documentation/tree/main/docs).

### Request a new feature

To request a new feature, [open an issue in the Islandora repository](https://github.com/Islandora/documentation/issues/new/choose) using the **Feature request** template. Summarize the desired functionality, making sure to note whether the feature request is a change to existing behaviour of islandora or if it is introducing a new addition or behaviour. Feature requests may be supported by specific use cases. To create supporting Use case issues and link them to your feature request, see the _Use cases_ section below. 


### Use cases
If you would like to submit a use case to the Islandora project, please submit an issue [here](https://github.com/Islandora/documentation/issues/new/choose) using the **Use Case template**. You may want to create a user story to support your use case. Read more about user stories [here](https://en.wikipedia.org/wiki/User_story#Principle).

### Contribute code

Before you set out to contribute code or documentation you will need to have completed a [Contributor License Agreement](https://github.com/Islandora/islandora-community/wiki/Onboarding-Checklist#contributor-license-agreements) or be covered by a [Corporate Contributor License Agreement](https://github.com/Islandora/islandora-community/wiki/Onboarding-Checklist#contributor-license-agreements). 

_If you are interested in contributing code to Islandora but do not know where to begin:_

In this case you should [browse open issues](https://github.com/Islandora/documentation/issues) and check out [use cases](https://github.com/Islandora/documentation/labels/Type%3A%20use%20case).

If you are contributing Drupal code, it must adhere to [Drupal Coding Standards](https://www.drupal.org/coding-standards); Travis CI will check for this on pull requests.

Contributions to the Islandora codebase should be sent as GitHub pull requests. See section _Create a pull request_ below for details. If there is any problem with the pull request we can work through it using the commenting features of GitHub.

* For _small patches_, feel free to submit pull requests directly for those patches.
* For _larger code contributions_, please use the following process. The idea behind this process is to prevent any wasted work and catch design issues early on.

    1. [Open an issue](https://github.com/Islandora/documentation/issues/new), prepending "Enhancement:" in the title if a similar issue does not exist already. If a similar issue does exist, then you may consider participating in the work on the existing issue.
    2. Comment on the issue with your plan for implementing the issue. Explain what pieces of the codebase you are going to touch and how everything is going to fit together.
    3. Islandora committers will work with you on the design to make sure you are on the right track.
    4. Implement your issue, create a pull request (see below), and iterate from there.

### Create a pull request

Take a look at [Creating a pull request](https://help.github.com/articles/creating-a-pull-request). In a nutshell you need to:

1. [Fork](https://help.github.com/articles/fork-a-repo) this repository to your personal or institutional GitHub account (depending on the CLA you are working under). Be cautious of which branches you work from though (you'll want to base your work off main, or for Drupal modules use the most recent version branch). See [Fork a repo](https://help.github.com/articles/fork-a-repo) for detailed instructions.
2. Commit any changes to your fork.
3. Send a [pull request](https://help.github.com/articles/creating-a-pull-request) using the [pull request template](https://github.com/Islandora/documentation/blob/main/.github/PULL_REQUEST_TEMPLATE.md) to the Islandora GitHub repository that you forked in step 1.  If your pull request is related to an existing issue -- for instance, because you reported a [bug/issue](https://github.com/Islandora/documentation/issues) earlier -- prefix the title of your pull request with the corresponding issue number (e.g. `issue-123: ...`). Please also include a reference to the issue in the description of the pull. This can be done by using '#' plus the issue number like so '#123', also try to pick an appropriate name for the branch in which you're issuing the pull request from.

You may want to read [Syncing a fork](https://help.github.com/articles/syncing-a-fork) for instructions on how to keep your fork up to date with the latest changes of the upstream (official) repository.

## License Agreements

The Islandora Foundation requires that contributors complete a [Contributor License Agreement](https://github.com/Islandora/islandora-community/wiki/Onboarding-Checklist#contributor-license-agreements) or be covered by a [Corporate Contributor License Agreement](https://github.com/Islandora/islandora-community/wiki/Onboarding-Checklist#contributor-license-agreements). The signed copy of the license agreement should be sent to <a href="mailto:community@islandora.ca?Subject=Contributor%20License%20Agreement" target="_top">community@islandora.ca</a>. This license is for your protection as a contributor as well as the protection of the Foundation and its users; it does not change your rights to use your own contributions for any other purpose.
