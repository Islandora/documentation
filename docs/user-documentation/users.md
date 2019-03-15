# Managing Users

## How to Add a User
1. Log in to Drupal. You will need the Administrator role to Manage Users.
1. Click the **Manage** tab from the top toolbar.
1. Select the **People** tab from the resulting menu to bring up the *People* page.
![Alt text](../assets/users_people.png "People page")
1. Click the blue **Add user** button to bring up the *Add user* page.
1. Enter all required fields, as indicated by red asterisks, in the *Add user* page.
1. In the *Roles* section, click on the roles that the new user will need.
  - Click on "Administrator" if the new user will need to be able to run adminsitrative tasks in Drupal or Islandora 8.
  - Click on "fedoraAdmin" if the new user will need to be able to run certain tasks in Fedora. This is required by the WebAC authentication so Islandora 8 and Fedora can communicate.
1. Review other settings available for a new user such as:
  - Site language
    - *English* selected by default.
  - Contact Settings section
    - *Personal contact form* selected by default.
1. Click on the **Create New Account** button at the bottom of the *Add user* page to finish adding a new user.

To review/edit the permission for each role, in the *People* page click the **Permissions** tab in the set of tabs above the **Add user** button.

## How to Create a New User Role
1. Log in to Drupal. You will need the Administrator role to edit or add roles.
1. Click the **Manage** tab from the top toolbar.
1. Select the **People** tab from the resulting menu.
1. Click the **Roles** tab in the set of tabs above the **Add user** button.
1. Click the blue **Add role** button to bring up the *Add role* page.
![Alt text](../assets/users_people_roles.png "Roles page")
1. Name the role in a way that it can be disambiguated from related activities or similar groups.
    * For example: use the course code for a particular class of students working on a particular collection.
1. Click the blue **Save** button.

## How to Edit Role Permissions
1. Log in to Drupal. You will need the Administrator role to edit permissions.
1. Click the **Manage** tab from the top toolbar.
1. Select the **People** tab from the resulting menu.
1. Click the **Roles** tab in the set of tabs above the **Add user** button.
![Alt text](../assets/users_people_roles.png "Roles page")
1. To edit the permissions for a role click the **Edit** dropdown menu to the right of a role and select **Edit Permissions** to bring up the *Edit role* page.
1. Scroll down or search for options that have an *Islandora* prefix or contain the word *Islandora*. For example, *Islandora Access: Create terms*.
![Alt text](../assets/users_permissions.png "Permissions page")

## Further Reading on Managing Users in Drupal

For more information on managing users in Drupal visit the section
[Managing User Accounts](https://www.drupal.org/docs/user_guide/en/user-chapter.html) of Drupal.org.
