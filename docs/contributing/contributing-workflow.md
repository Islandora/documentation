### Terms:
- **User**: Anyone who uses Islandora whether or not they participate in the community directly.
- **Contributor**: Anyone who contributes in any form to Islandora (code, documentation, posts on the lists, etc).
- **Committer**: Individuals with merging privileges, and binding votes on procedural, code modification, and release issues, etc. (further outlined here).
- **Community**: All of the above.

## Overview
Choosing how we work together is an important privilege the Islandora community offers to its contributors.  Our workflow is built around engagement and consensus to encourage high quality code that is expected to meet defined standards.  All contributors are expected to follow the defined workflow.  Any contributor can propose changes to the workflow at any time by initiating a conversation on the mailing list.  Any proposed changes must be confirmed by the committers by way of an Apache rules vote in order to take effect.

## Social Dynamics
We operate under the [Islandora Community Code of Conduct](https://www.islandora.ca/code-of-conduct). Some additional general observations to keep in mind when interacting with others in this workflow:

- We are all volunteers
  - Time/attention of others is a limited resource.
  - You cannot impose on others what to do.
  - You should be motivated to do things by yourself.
- Common need is the driving force.
  - Friendly cooperation is how its done
  - Your contribution should not overstrain others.
  - Your contribution is valuable when others appreciate it.
- Islandora is constantly improving
  - You are expected to make high quality contributions
  - You must be able to adapt to change

## Workflow:

#### General Guidelines:
- Identify if a Github issue is needed:
  - Minor grammar / php warning: no ticket required
  - Feature or Fix beyond the minimal: Github Issue
  - Unsure? Play it safe and make a Github Issue
- Prioritizing pull requests:
  - If there is an urgent need for the pull request to be addressed quickly, indicate the need in the pull request template or a comment.
  - Complexity should also be taken into account when evaluating how quickly to merge a pull request. Changes that affect core modules or make extensive changes should receive more review and testing.
- All interested parties should be satisfied before something is merged; no hard numbers. If you know who is likely to be interested, tag them. Tag the creator of the issue if possible. Make a reasonable effort.
- If a pull request languishes without response when one is needed, tag @Islandora/8-x-committers (or @Islandora-Devops/committers if you’re working on install code) with a reminder and/or put the issue on the agenda for the next Islandora Tech Call
- All contributions to GitHub must be accompanied by either an Individual Contributor License or a Corporate Contributor License covering the contributor.

#### Development Workflow:
- Create a Github Issue if none exists
  - Assign the issue to yourself or request it to be assigned in a comment on the issue.  Be sure to tag @Islandora/8-x-committers to bring attention to it.
- Perform development on a ‘feature’ branch.  Give your branch a name that describes the issue or feature.  Using something like ‘issue-xxx‘ and including the issue number is always a safe bet if you don’t know what to name it.
- When the code is ready for review, issue a pull request on Github from your feature branch into the development branch (‘8.x-1.x’ for Drupal modules and ‘dev’ for everything else)
- Continuous integration checks need to be satisfied before the code can be merged
  - Coding standards checks and copy/paste mess detection must be passing before code can be merged.
  - Automated tests are also expected to pass before code can be merged, however at times there can be extenuating circumstances preventing this. For example, sometimes there is a disconnect between running tests locally versus on the continuous integration server. If tests run locally and everyone involved in the pull request is in agreement, the check for automated tests can be ignored. But this isn’t a common occurrence.
  - Code coverage checks are more of a guideline than a strict requirement. A reasonable effort to provide tests for contributed code is expected.  However, demanding 100% code coverage for all contributed code is unreasonable.  If tests have been provided or updated and everyone involved in the pull request is in agreement, code coverage checks can be ignored.
- The code is reviewed and tested by the community, with all feedback and further development performed within the pull request.
- Once all feedback has been addressed, all applicable status checks have passed, and all interested parties are satisfied, the code can be merged by a committer
  - Pull requests should not be merged by committers who have provided code in the pull request.  Pull requests should also not be merged by committers who are peers in the same organization as those who provided code.
  - Always [“Squash and Merge”](https://help.github.com/en/articles/about-pull-request-merges#squash-and-merge-your-pull-request-commits), please.
- Close the Github issue
  - If you add the keyword “fixes” or “resolves” on the same line when referencing the Github Issue in the PR description, it will auto-close the issue when the PR is merged.
  - Otherwise, make a comment referencing the new commit and close the issue manually.

