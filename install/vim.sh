HOME_DIR=$1

# Get vundle
git clone https://github.com/gmarik/Vundle.vim.git "$HOME_DIR/.vim/bundle/Vundle.vim"

# Copy over the vimrc
cp "$HOME_DIR/islandora/install/.vimrc" "$HOME_DIR/.vimrc"

# Install plugins using vundle 
vim -c "PluginInstall" -c "qa"

# Compile YouCompleteMe
apt-get -y install build-essential cmake python-dev
cd "$HOME_DIR/.vim/bundle/YouCompleteMe"
./install.sh

# Deps for eclipse (basically... X)
apt-get -y install xvfb

# Get eclipse and put it in the home directory
cd $HOME_DIR
wget -O eclipse.tar.gz http://eclipse.mirror.rafal.ca/technology/epp/downloads/release/luna/SR1a/eclipse-java-luna-SR1a-linux-gtk-x86_64.tar.gz
tar zxvf eclipse.tar.gz

# Start X (run as the vagrant user)
sudo -u vagrant Xvfb :1 -screen 0 1024x768x24 &

# Install dependencies for installer
DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://download.eclipse.org/releases/luna/ \
  -installIU org.eclipse.cdt.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://download.eclipse.org/releases/luna/ \
  -installIU org.eclipse.dltk.core.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://download.eclipse.org/releases/luna/ \
  -installIU org.eclipse.dltk.ruby.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://download.eclipse.org/releases/luna/ \
  -installIU org.eclipse.jdt.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://download.eclipse.org/releases/luna/ \
  -installIU org.eclipse.php.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://download.eclipse.org/releases/luna/ \
  -installIU org.eclipse.wst.web_ui.feature.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository https://dl-ssl.google.com/android/eclipse/ \
  -installIU com.android.ide.eclipse.adt.feature.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://dist.springsource.org/release/GRECLIPSE/e4.4/ \
  -installIU org.codehaus.groovy.eclipse.feature.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://pydev.org/updates \
  -installIU org.python.pydev.feature.feature.group 

DISPLAY=:1 "$HOME_DIR/eclipse/eclipse" -nosplash -consolelog -debug \
  -application org.eclipse.equinox.p2.director \
  -repository http://download.scala-ide.org/sdk/lithium/e44/scala211/dev/site \
  -installIU org.scala-ide.sdt.feature.feature.group 

# Get Eclim
wget -O ~/eclim_2.4.1.jar http://sourceforge.net/projects/eclim/files/eclim/2.4.1/eclim_2.4.1.jar/download

# chown things back to the vagrant user before installing eclim
chown vagrant:vagrant ~/eclim_2.4.1.jar
chown -R vagrant:vagrant "$HOME_DIR/eclipse"
chown -R vagrant:vagrant "$HOME_DIR/eclim"
chown -R vagrant:vagrant "$HOME_DIR/.vim"

# Build and install from source (run as the vagrant user)
sudo -u vagrant java -Dvim.files=$HOME/.vim -Declipse.home=$HOME/eclipse -jar eclim_2.4.1.jar install

# Start Eclim (run as the vagrant user)
sudo -u vagrant DISPLAY=:1 "$HOME_DIR/eclipse/eclimd" -b

# Wait a few minutes for all the crazy output to stop... and enjoy!
