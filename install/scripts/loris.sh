#!/bin/bash
echo "Installing Loris IIIF Image Server"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

# Kakadu
if [ ! -f "$DOWNLOAD_DIR/adore-djatoka.tar.gz" ]; then
  echo "Downloading Adore-Djatoka"
  wget -q -O "$DOWNLOAD_DIR/adore-djatoka.tar.gz" "http://downloads.sourceforge.net/project/djatoka/djatoka/1.1/adore-djatoka-1.1.tar.gz"
fi

if [ $(grep -c 'Listen 8383' /etc/apache2/ports.conf) != 1 ]; then
  echo "Adding Listen 8383 to Apache ports.conf"
  sed -i '/Listen 80$/a \Listen 8383' /etc/apache2/ports.conf
fi

cd /opt
mkdir djatoka
cd /tmp || exit
cp "$DOWNLOAD_DIR/adore-djatoka.tar.gz" /tmp
tar -xzvf adore-djatoka.tar.gz
cd adore-djatoka-1.1 || exit
mv -v ./* /opt/djatoka
ln -s /opt/djatoka/bin/Linux-x86-64/kdu_compress /usr/local/bin/kdu_compress
ln -s /opt/djatoka/bin/Linux-x86-64/kdu_expand /usr/local/bin/kdu_expand

#Dependencies
apt-get -y install python-pip python-setuptools
pip install --upgrade pip
pip uninstall PIL
pip uninstall Pillow
apt-get -y purge python-imaging
apt-get -y install libjpeg-turbo8-dev libfreetype6-dev zlib1g-dev liblcms2-dev liblcms2-utils libtiff5-dev python-dev libwebp-dev libapache2-mod-wsgi
pip install Pillow
pip install Werkzeug
pip install configobj
pip install requests
pip install mock
pip install responses
useradd -d /var/www/loris2 -s /sbin/false loris
a2enmod headers expires
cp "$HOME_DIR/islandora/install/configs/002-loris.conf" "/etc/apache2/sites-enabled/"
cp "$HOME_DIR/islandora/install/configs/kdu_libs.conf" "/etc/ld.so.conf.d/kdu_libs.conf"
ldconfig
mkdir /usr/local/share/images

#Install Loris
cd /opt
git clone https://github.com/loris-imageserver/loris.git
cd /opt/loris
cp -R /opt/loris/tests/img/* /usr/local/share/images
chown -hR loris:loris /usr/local/share/images
python setup.py install
service apache2 restart
