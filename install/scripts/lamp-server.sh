#!/bin/sh

echo "Installing LAMP server packages"

PACKAGES="libwrap0 ssl-cert libterm-readkey-perl mysql-client-5.5 libdbi-perl libmysqlclient18 mysql-server-core-5.5 mysql-common apache2 mysql-server-5.5 mysql-client-core-5.5 tcpd libaio1 mysql-server libdbd-mysql-perl libhtml-template-perl php5.6 libapache2-mod-php5.6"

apt-get -qq install -y $PACKAGES

usermod -a -G www-data vagrant

chown -R vagrant:vagrant islandora
