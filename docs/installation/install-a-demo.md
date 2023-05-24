# Docker Demo

To test out Islandora, there is [a Starter-Site based demo available in Google Drive](https://drive.google.com/file/d/1LrIe12eYAZjD4A8_cqf61RXv7mtMJRHH/view?usp=share_link). It is loaded with sample content to demonstrate
features of Islandora and provide a starting point for exploration. 

Download your demo and use the instructions below to install. It is recommended that you have a minimum of 4GB of hard drive space available on your machine before getting started. It will take some time to build the image, do not be surprised if it takes at least 25 minutes. Thanks, and happy building!
<br>

Installation Video Tutorial
==============================================================
This material is also covered in: [How to Install Docker Desktop and Set up Starter Site (video tutorial)](https://www.youtube.com/watch?v=xhJft9K-YMU).
<br>

Requirements
==============================================================
Before you can set up your demo locally you must first install a few software packages.


Docker Desktop
--------------------------------------------------------------

[Docker Desktop](https://www.docker.com/products/docker-desktop) provides a graphical user interface for all major platforms.

<https://www.docker.com/products/docker-desktop>

Follow the link above and select the appropriate system and follow the installation instructions.

Portainer Extension
--------------------------------------------------------------
Once you have a working [Docker Desktop](#dockerdesktop) installation, you can add
the [Portainer](https://portainer.io/) extension:

1. Open Docker Desktop.
2. From the Dashboard, select Add Extensions in the menu bar.
3. The Extensions Marketplace opens.
4. Browse the available extensions.
5. Select **Portainer**
6. Click Install.

For more information on Portainer refer to its
[documentation](https://docs.portainer.io/).

Setup
==============================================================
Assuming you have all the [Requirements](#requirements) installed
and running. You can now setup your demo. 

Get a Yaml file
--------------------------------------------------------------
Download the [Starter-Site-based demo from Google Drive](https://drive.google.com/file/d/1LrIe12eYAZjD4A8_cqf61RXv7mtMJRHH/view?usp=share_link) and find the `docker-compose.yml` file provided inside. This will be used in the next step.


Create Stack
--------------------------------------------------------------
Open **Portainer** via the Docker Desktop Dashboard. 

1. Open Docker Desktop.
2. Click on Portainer.
3. If you see the Quick Setup Environment Wizard, click “Get Started" and then select the "local" environment. Otherwise, select your local environment under "Environments".
4. Open the sidebar.
5. Click on **Stacks**.
6. Click **Add Stack**.
7. Type in the name **sandbox** (_lowercase_).
8. Select **Upload from your Computer**.
9. Upload the **docker-compose.yml** file you downloaded.
10. Click on **Deploy the Stack**. The button will then display _Deployment in progress_  - do not click away while this message is shown.

This may take a **very long** time if this is the first time setting up
the demo, as it must download all the Docker Images from the
internet. 

Once it is completed, you will see the list of stacks, now listing 'sandbox' among them.


Wait for Install
--------------------------------------------------------------
After deploying the stack has completed, the demo still needs to install and configure `Drupal`, which may
take upwards of _10-15 minutes_. You can check on the install process by doing
the following.

1. Open Docker Desktop.
2. Click on Portainer.
3. Open the sidebar.
4. Click on **Stacks**.
5. Click on **sandbox**.
6. Click on the Logs button ('page' icon) for the **Drupal** container - the one named something like `sandbox-drupal-1`
7. Examine the log output waiting for the message below.

When completed you should see the following output at the end of the log.

```
#####################
# Install Completed #
#####################
```

You can now visit your local starter-site demo in your browser of choice at:

<http://islandora.io/>


The starter-site demo login credentials 

username: admin

password: password
<br>

Reinstall
==============================================================
If you want to re-install your demo and restore it to its original
state, please perform the following steps:

1. Open Docker Desktop.
2. Click on Portainer.
3. Open the sidebar.
4. Click on **Stacks**.
5. Click on **sandbox**.
6. Click on **Stop this Stack**.
7. Click on **Volumes** in the sidebar.
8. Search for **sandbox**.
9. Ensure that all items are listed.
10. Select all items.
11. Click on **Remove**.
12. Click on **Stacks** in the sidebar.
13. Click on **sandbox**.
14. Click on **Start this Stack**.
15. Wait for the installation to complete like in the [Wait for Install Section](#waitforinstall)
<br>

Uninstall
==============================================================
If you want to completely uninstall your demo, please perform the
following steps:

1. Open Docker Desktop.
2. Click on Portainer.
3. Open the sidebar.
4. Click on **Stacks**.
5. Click on **sandbox**.
6. Click on **Delete this Stack**.
7. Click on **Volumes** in the sidebar.
8. Search for **sandbox**.
9. Ensure that all items are listed.
10. Select all items.
11. Click on **Remove**.
<br>

Troubleshooting
==============================================================
These troubleshooting issues and solutions for Docker Desktop have arised from and been used with Windows 10.

"Docker Failed to Start" Error Message
--------------------------------------------------------------
For Windows 10, you may receive the "Docker Failed to Start" Error message. To resolve this:

* Download WSL 2 from Step 4 in [these instructions](https://learn.microsoft.com/en-ca/windows/wsl/install-manual#step-4---download-the-linux-kernel-update-package)--do not do any other steps in these instructions.
* Under Windows ‘Firewall & Network Protection’, ‘Allow an app through the firewall’, and ‘Change settings’ to select the ‘Private’ box for ‘Virtual Machine Monitoring’.
* Restart computer for all changes to take effect.


No Content Showing on islandora.io
--------------------------------------------------------------
If no content is showing up in your local installation when on islandora.io in a browser, you may need to wait longer for the deployment process to complete.
1. Go to ‘containers’, click the ‘logs’ for ‘sandbox-drupal-1’ to see the deployment process. When completed you will see: 
```
#####################
# Install Completed #
#####################
```
* This can take anywhere from 5 minutes to over an hour.


Docker Desktop and Virtual Machine (Vmmem in Task Manager) Taking up too much CPU and RAM
--------------------------------------------------------------
If Docker Desktop and Virtual Machine (Vmmem in Task Manager) are taking up too much CPU and RAM, you can create a .wslconfig file to limit how much of your computers resources are allocated to these processes:
1. Open Notepad++ and type the following:
```
[wsl2]
memory=4GB
processors=2
```
2. Modify the numbers to reflect how much you want to allocate to Docker Desktop (i.e. if you only have 4GB of RAM then allocate 2GB).
     * Save As and select ‘All File Types’ from the dropdown.
     * Name the file x.wslconfig and save it.
     * Go to the location of the saved file and rename it, removing the ‘x’.
     * The file should look like this:
![wslconfigimage](https://github.com/Islandora/documentation/assets/122306010/6833bc2b-0151-4053-b902-b764c1e44cc4)
3. Put the .wslconfig file in the appropriate location.
    * In your File Explorer address bar type ‘%UserProfile% and hit enter.
    * Move the .wslconfig file to this folder.
4. Restart your computer and start up Docker Desktop. It will now be using only the specified memory and processors.
