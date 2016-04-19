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

# Add repo for Oracle's JDK 8, rationale #L36-38
add-apt-repository -y ppa:webupd8team/java
# Add repo for PHP 5.6
add-apt-repository -y ppa:ondrej/php5-5.6

# Update all the things.
apt-get -y -qq update && apt-get -y -qq upgrade

# SSH
apt-get -y -qq install openssh-server

# Build tools
apt-get -y -qq install build-essential

# Git vim
apt-get -y -qq install git vim

# Java
## There is no Java8 OpenJDK right now in the Ubuntu repos
## http://askubuntu.com/questions/464755/how-to-install-openjdk-8-on-14-04-lts
## We'll use Oracle Java8 for now.
# Java (Oracle)
apt-get -qq install -y software-properties-common
apt-get -qq install -y python-software-properties
echo debconf shared/accepted-oracle-license-v1-1 select true | sudo debconf-set-selections
echo debconf shared/accepted-oracle-license-v1-1 seen true | sudo debconf-set-selections
apt-get -qq install -y oracle-java8-installer
update-java-alternatives -s java-8-oracle
apt-get -qq install -y oracle-java8-set-default
export JAVA_HOME=/usr/lib/jvm/java-8-oracle

# Maven
apt-get -y -qq install maven

# Tomcat
apt-get -y -qq install tomcat7 tomcat7-admin
usermod -a -G tomcat7 vagrant
sed -i '$i<user username="islandora" password="islandora" roles="manager-gui"/>' /etc/tomcat7/tomcat-users.xml

# Make the ingest directory
if [ ! -d "/mnt/ingest" ]; then
  mkdir /mnt/ingest
fi

chown -R tomcat7:tomcat7 /mnt/ingest

# Wget and curl
apt-get -y -qq install wget curl

# Bug fix for Ubuntu 14.04 with zsh 5.0.2 -- https://bugs.launchpad.net/ubuntu/+source/zsh/+bug/1242108
export MAN_FILES
MAN_FILES=$(wget -qO- "http://sourceforge.net/projects/zsh/files/zsh/5.0.2/zsh-5.0.2.tar.gz/download" \
  | tar xz -C /usr/share/man/man1/ --wildcards "zsh-5.0.2/Doc/*.1" --strip-components=2)
for MAN_FILE in $MAN_FILES; do gzip /usr/share/man/man1/"${MAN_FILE##*/}"; done

# More helpful packages
apt-get -y -qq install htop tree zsh fish

# Set some params so it's non-interactive for the lamp-server install
debconf-set-selections <<< 'mysql-server mysql-server/root_password password islandora'
debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password islandora'
debconf-set-selections <<< "postfix postfix/mailname string islandora-fedora4.org"
debconf-set-selections <<< "postfix postfix/main_mailer_type string 'Internet Site'"

# Set JAVA_HOME -- Java8 set-default does not seem to do this.
sed -i 's|#JAVA_HOME=/usr/lib/jvm/openjdk-6-jdk|JAVA_HOME=/usr/lib/jvm/java-8-oracle|g' /etc/default/tomcat7
