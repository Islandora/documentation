# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!
VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  # All Vagrant configuration is done here. The most common configuration
  # options are documented and commented below. For a complete reference,
  # please see the online documentation at vagrantup.com.
  config.vm.provider "virtualbox" do |v|
    v.name = "Islandora 7.x-2.x"
  end
  config.vm.hostname = "islandora"

  # Every Vagrant virtual environment requires a box to build off of.
  config.vm.box = "ubuntu/trusty64"

  # Setup the shared folder
  home_dir = "/home/vagrant"
  config.vm.synced_folder "../", home_dir + "/islandora"

  config.vm.network :forwarded_port, guest: 8080, host: 8080 # Tomcat
  config.vm.network :forwarded_port, guest: 8181, host: 8181 # Karaf
  config.vm.network :forwarded_port, guest: 3306, host: 3306 # MySQL
  config.vm.network :forwarded_port, guest: 5432, host: 5432 # PostgreSQL
  config.vm.network :forwarded_port, guest: 80, host: 8000 # Apache

  config.vm.provider "virtualbox" do |vb|
    vb.customize ["modifyvm", :id, "--memory", '2048']
  end

  config.vm.provision :shell, inline: "sudo sed -i '/tty/!s/mesg n/tty -s \\&\\& mesg n/' /root/.profile", :privileged =>false
  config.vm.provision :shell, :path => "./scripts/bootstrap.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/solr.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/drupal.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/fcrepo.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/blazegraph.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/islandora-commands.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/karaf.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/hawtio.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/camel.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/fcrepo-camel-toolbox.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/fcrepo-camel.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/activemq.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/config.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/islandora-component.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/sync.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/services.sh", :args => home_dir
  config.vm.provision :shell, :path => "./scripts/post-install.sh", :args => home_dir
end
