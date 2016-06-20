#!/bin/bash
echo " Installing Twig C extentions"
cd /home/vagrant
composer require twig/twig:~1.0
cd /home/vagrant/vendor/twig/twig/ext/twig
phpize
./configure
make
make install
sed -i '$iextension=/usr/lib/php/20131226/twig.so' /etc/php/5.6/apache2/php.ini

