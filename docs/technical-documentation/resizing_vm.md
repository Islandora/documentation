## Resize vagrant machine
To expand virtual machine's harddrive for testing of larger files. Once the VM has started, you'll need to `halt` the VM, download and run the script, tell it what size (in MB) and then start the VM.
The last step `vagrant ssh --command "sudo resize2fs /dev/sda1"` is a check. It should return there was nothing to do. If you already provisioned you VM you can skip the 2 steps with provisioning in them.

```shell
# Skip this if you VM is already provisioned.
$ vagrant up --no-provision  <-- Exclude if already running and provisioned.

$ vagrant halt

# Download and run. This will default to the correct name (just press enter) then give the size.
# Example: `350000` is equal to 350GB

$ wget https://gist.githubusercontent.com/DonRichards/6dc6c81ae9fc22cba8d7a57b90ab1509/raw/98cd6daeafe86c8e830495f9b46eb04e590f4b4f/expand_disk.py
$ chmod +x expand_disk.py
$ python expand_disk.py
$ vagrant up --no-provision

# This step isn't needed but acts as a check to verify it worked.
$ vagrant ssh --command "sudo resize2fs /dev/sda1"

# Skip this if you VM is already provisioned.
$ vagrant provision           <-- Exclude if already provisioned.
```

### Troubleshooting expand_disk.py
You may need to remove the "resized" version. Assuming your VM location is `~/VirtualBox\ VMs`
```shell
$ rm -rf ~/VirtualBox\ VMs/Islandora\ CLAW\ Ansible_resized
```
