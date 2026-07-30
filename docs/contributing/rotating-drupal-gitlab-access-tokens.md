# Rotating Drupal.org GitLab Access Tokens

Islandora develops its Drupal modules on GitHub and mirrors them to Drupal.org
GitLab for distribution in the `drupal/*` Composer namespace. Each mirror uses a
Drupal.org GitLab project access token stored in its GitHub repository as the
Actions secret `GITLAB_PASSWORD`.

Each project access token has an expiration date and is currently rotated about
once a year. Follow the expiration date shown in GitLab and rotate the tokens for
all six projects when Drupal.org GitLab sends an expiration reminder.

## Required access

The committer performing the rotation needs:

- Maintainer or Owner access to each project on Drupal.org GitLab.
- Permission to update Actions secrets in each GitHub repository.

Coordinate with the other committers before starting so that only one person
rotates the tokens.

!!! warning "Handle each token as a secret"
    Rotating a token immediately invalidates the old token, and GitLab displays
    the new token only once. Complete both the GitLab and GitHub steps for one
    project before moving to the next project. Never save the token in an issue,
    chat message, or other shared location, and never reuse a project's token for
    another project.

## Projects

| Module | GitHub repository | Drupal.org GitLab project |
|---|---|---|
| Islandora | [Islandora/islandora](https://github.com/Islandora/islandora) | [project/islandora](https://git.drupalcode.org/project/islandora) |
| Islandora Mirador | [Islandora/islandora_mirador](https://github.com/Islandora/islandora_mirador) | [project/islandora_mirador](https://git.drupalcode.org/project/islandora_mirador) |
| OpenSeadragon | [Islandora/openseadragon](https://github.com/Islandora/openseadragon) | [project/openseadragon](https://git.drupalcode.org/project/openseadragon) |
| Controlled Access Terms | [Islandora/controlled_access_terms](https://github.com/Islandora/controlled_access_terms) | [project/controlled_access_terms](https://git.drupalcode.org/project/controlled_access_terms) |
| Advanced Search | [Islandora/advanced_search](https://github.com/Islandora/advanced_search) | [project/advanced_search](https://git.drupalcode.org/project/advanced_search) |
| JSON-LD REST Services | [Islandora/jsonld](https://github.com/Islandora/jsonld) | [project/jsonld](https://git.drupalcode.org/project/jsonld) |

## Rotate an access token

Repeat this procedure for every row in the [project list](#projects):

1. Sign in to [Drupal.org GitLab](https://git.drupalcode.org/) and open the
   project's Drupal.org GitLab link.
1. Select **Settings** >> **Access tokens**.
1. In *Active project access tokens*, identify the token used by the GitHub
   mirror. Verify its recorded name, role, scopes, and recent use before selecting
   **Rotate** (the circular-arrows action). If more than one token is active and
   the mirror token cannot be identified, confirm it with another committer
   rather than guessing.
1. Select **Rotate** in the confirmation dialog.
1. Copy the new token immediately. Keep this page open until the matching GitHub
   secret has been updated.
1. Open the project's matching GitHub repository from the table above.
1. Select **Settings** >> **Secrets and variables** >> **Actions**.
1. Under *Repository secrets*, select the edit action for `GITLAB_PASSWORD`.
1. Paste the new token into the *Secret* field and select **Update secret**.
1. Confirm that GitHub shows a current update time for `GITLAB_PASSWORD`.
1. Return to GitLab and confirm that the replacement token is active. Note its
   expiration date.
1. In GitHub **Actions**, rerun the most recent branch-based mirror workflow, if
   that option is available, and confirm that it succeeds. The workflow is
   defined in `.github/workflows/gitlab-mirror.yml`.
1. Continue with the next project.

If the new token is lost before the GitHub secret is updated, rotate the active
token again. Do not leave the repository configured with an old or incomplete
token.

## Verify the mirrors

After all six secrets have been updated:

- Confirm that every project in the table was completed.
- For any project whose workflow could not be rerun immediately, confirm that the
  next normal branch or tag push completes the mirror workflow successfully.
- Confirm that the tested branch or tag appears in the matching Drupal.org GitLab
  project.

For details about the interfaces used in this procedure, see GitLab's
[project access token documentation](https://docs.gitlab.com/user/project/settings/project_access_tokens/)
and GitHub's documentation for
[using secrets in GitHub Actions](https://docs.github.com/en/actions/how-tos/write-workflows/choose-what-workflows-do/use-secrets).
