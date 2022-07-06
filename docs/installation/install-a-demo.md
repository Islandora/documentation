# Docker Demo

This demo, available to download [here](https://drive.google.com/file/d/1mA3sLIoqYlSlUgvJ0N05NpCQJ0nW6N1m/view?usp=sharing), is loaded with sample content and configurations to demonstrate
features of Islandora and provide a starting point for exploration. 

Download [this file](https://drive.google.com/file/d/1mA3sLIoqYlSlUgvJ0N05NpCQJ0nW6N1m/view?usp=sharing), and open and follow the instructions in the README.md.html file (also shown below). Thanks, and happy building!


Requirements
==============================================================

Before you can set up **Islandora Sandbox** locally you must first install a few
software packages.

Docker Desktop
--------------------------------------------------------------

[Docker Desktop](https://www.docker.com/products/docker-desktop) provides a
graphical user interface for all major platforms.

<https://www.docker.com/products/docker-desktop>

Follow the link above and select the appropriate system and follow the
installation instructions.

**If you already have Docker Desktop installed,** make sure that the **Extensions** 
feature is listed in the left sidebar. If it is not, you need to update to the newest 
version of Docker Desktop. This is required to add the Portainer extension.

Portainer Extension
--------------------------------------------------------------

Once you have worker [Docker Desktop](#dockerdesktop) installation, you can add
the [Portainer](https://portainer.io/) extension:

1. Open Docker Desktop.
2. From the Dashboard, select Add Extensions in the menu bar.
3. The Extensions Marketplace opens.
4. Browse the available extensions.
5. Select **Portainer**
6. Click Install.

![install-portainer](../assets/docker_demo_Install_Portainer.gif)

For more information on Portainer refer to its
[documentation](https://docs.portainer.io/).

Setup
==============================================================

Assuming you have all the [Requirements](#requirements) installed
and running. You can now setup **Islandora Sandbox**.

Generate docker-compose.yml
--------------------------------------------------------------

In this package there are some platform specific scripts. Please **double
click** on the appropriate script for your platform:

| Platform  | Script        |
| :-------- | :------------ |
| Linux     | setup.command |
| Macintosh | setup.command |
| Windows   | setup.bat     |
[Table [states]: Platform specific setup script.]

This will install a local certificate and generate a `docker-compose.yml` file.
During execution of the script, you will be **prompted for your admin password**, 
in order to install the local certificate.

_**Note for Mac users:**_ If when executing the script, you are shown a pop-up warning
that "setup.command cannot be opened", please navigate to _System Preferences >
Security and Privacy_. Here, you can click "Allow anyway" and then re-try running
the script. This may need to be done more than once.

Create Stack
--------------------------------------------------------------

Once that has completed successfully, you can open **Portainer** via the Docker
Desktop Dashboard. 

1. Open Docker Desktop.
2. Click on Portainer.
3. If you see the Quick Setup Environment Wizard, click “Get Started". Otherwise, select your local environment under "Environments".
4. Open the sidebar.
5. Click on **Stacks**.
6. Click **Add Stack**.
7. Type in the name **sandbox** (_lowercase_).
8. Select **Upload from your Computer**.
9. Upload the **docker-compose.yml** file you generated via the setup script.
10. Click on **Deploy the Stack**. The button will then display _Deployment in progress_  - do not click away while this message is shown.

This may take a **very long** time if this is the first time setting up
**Islandora Sandbox** as it must download all the Docker Images from the
internet. 

![install-sandbox](../assets/docker_demo_Install_Sandbox.gif)

Wait for Install
--------------------------------------------------------------

**Islandora Sandbox** still needs to install and configure `Drupal`, which may
take upwards of _10-15 minutes_. You can check on the install process by doing
the following.

1. Open Docker Desktop.
2. Click on Portainer.
3. Open the sidebar.
4. Click on **Stacks**.
5. Click on **sandbox**.
6. Click on the Logs icon for **Drupal**.
7. Examine the log output waiting for the message below.

When completed you should see the following output at the end of the log.

```
#####################
# Install Completed #
#####################
```

You can now visit your local **Islandora Sandbox** in your browser of choice at:

<https://sandbox.islandora.dev>

![sandbox](../assets/docker_demo_Sandbox.png)

The login credentials can be found
[here](https://github.com/Islandora/documentation/wiki/Sandbox.Islandora.ca-online-credentials)

Reinstall
==============================================================

If you want to re-install **Islandora Sandbox**, and restore it to its original
state., please perform the following steps:

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

![reinstall-sandbox](../assets/docker_demo_Reinstall_Sandbox.gif)

Uninstall
==============================================================

If you want to completely uninstall **Islandora Sandbox**, please perform the
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

![uninstall-sandbox](../assets/docker_demo_Uninstall_Sandbox.gif)
