#!/bin/bash

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -d "$DOWNLOAD_DIR" ]; then
  mkdir -p "$DOWNLOAD_DIR"
fi

cd "$HOME_DIR"

# Set apt-get for non-interactive mode
export DEBIAN_FRONTEND=noninteractive

cp "$HOME_DIR"/islandora/install/configs/motd /etc/motd

# Update all the things.
apt-get -y -qq update && apt-get -y -qq upgrade

# SSH
apt-get -y -qq install openssh-server

# Build tools
apt-get -y -qq install build-essential

# Git vim
apt-get -y -qq install git vim

# Java
apt-get -y install openjdk-8-jdk openjdk-8-jdk-headless openjdk-8-jre
sed -i '$iJAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64' /etc/environment
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

# Maven
apt-get -y -qq install maven

# Tomcat
apt-get -y -qq install tomcat7 tomcat7-admin
usermod -a -G tomcat7 vagrant
sed -i '$i<user username="islandora" password="islandora" roles="manager-gui"/>' /etc/tomcat7/tomcat-users.xml

# Wget and curl
apt-get -y -qq install wget curl

# Bug fix for Ubuntu 14.04 with zsh 5.0.2 -- https://bugs.launchpad.net/ubuntu/+source/zsh/+bug/1242108
export MAN_FILES
MAN_FILES=$(wget -qO- "http://sourceforge.net/projects/zsh/files/zsh/5.0.2/zsh-5.0.2.tar.gz/download" \
  | tar xz -C /usr/share/man/man1/ --wildcards "zsh-5.0.2/Doc/*.1" --strip-components=2)
for MAN_FILE in $MAN_FILES; do gzip /usr/share/man/man1/"${MAN_FILE##*/}"; done

# More helpful packages
apt-get -y -qq install htop tree zsh fish unzip

# Set some params so it's non-interactive for the lamp-server install
debconf-set-selections <<< 'mysql-server mysql-server/root_password password islandora'
debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password islandora'
debconf-set-selections <<< "postfix postfix/mailname string islandora-fedora4.org"
debconf-set-selections <<< "postfix postfix/main_mailer_type string 'Internet Site'"

# Set JAVA_HOME -- Java8 set-default does not seem to do this.
sed -i 's|#JAVA_HOME=/usr/lib/jvm/openjdk-6-jdk|JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64|g' /etc/default/tomcat7
