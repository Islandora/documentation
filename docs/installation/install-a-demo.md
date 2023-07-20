# Portainer Docker Demo

To test out Islandora, there is [a Starter-Site based Portainer demo available](https://github.com/Islandora-Devops/sandbox/releases) (Download the .zip file). This demo is loaded with sample content to demonstrate
features of Islandora and provide a starting point for exploration. Follow the instructions in the README.md.html file provided in the download.

It is recommended that you have a minimum of 4GB of hard drive space available on your machine before getting started. It will take some time to build the image, do not be surprised if it takes a long time. Thanks, and happy building!
<br>

Installation Video Tutorial
==============================================================
The README.md.html is the most up to date version of the installation documentation, but you can also view this video: [How to Install Docker Desktop and Set up Starter Site (video tutorial)](https://www.youtube.com/watch?v=xhJft9K-YMU).

Troubleshooting
==============================================================
These troubleshooting issues and solutions for Docker Desktop have arised from and been used with Windows 10.

"Docker Failed to Start" Error Message
--------------------------------------------------------------
For Windows 10, you may receive the "Docker Failed to Start" Error message. To resolve this:

* Download WSL 2 from Step 4 in [these instructions](https://learn.microsoft.com/en-ca/windows/wsl/install-manual#step-4---download-the-linux-kernel-update-package), but do not do any other steps in these instructions.
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
