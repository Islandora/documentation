#!/bin/sh

echo "Installing LAMP server packages"

PACKAGES="libwrap0 ssl-cert libterm-readkey-perl mysql-client libdbi-perl libmysqlclient20 mysql-client-core-5.7 mysql-common apache2 mysql-server mysql-server-core-5.7 tcpd libaio1 mysql-server libdbd-mysql-perl libhtml-template-perl php7.0 php7.0-dev libapache2-mod-php7.0 php7.0-mbstring"

apt-get -qq install -y $PACKAGES

usermod -a -G www-data vagrant

chown -R vagrant:vagrant islandora
