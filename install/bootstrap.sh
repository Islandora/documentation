###
# BASICS
###

cd /home/vagrant

# Update
apt-get -y update && apt-get -y upgrade

# SSH
apt-get -y install openssh-server

# Build tools
apt-get -y install build-essential

# Git vim
apt-get -y install git vim

# Java
apt-get -y install openjdk-7-jdk

# Maven
apt-get -y install maven

# Tomcat
apt-get -y install tomcat7
usermod -a -G tomcat7 vagrant

# Wget and curl
apt-get -y install wget curl

# More helpful packages
apt-get -y install htop tree zsh

# Set some params so it's non-interactive for the lamp-server install
debconf-set-selections <<< 'mysql-server mysql-server/root_password password islandora'
debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password islandora'
debconf-set-selections <<< "postfix postfix/mailname string islandora-fedora4.org"
debconf-set-selections <<< "postfix postfix/main_mailer_type string 'Internet Site'"

# Lamp server
tasksel install lamp-server
usermod -a -G www-data vagrant

# Get the repo
git clone -b 7.x-2.x https://github.com/Islandora-Labs/islandora.git
