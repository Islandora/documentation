#!/bin/sh

# Globals
readonly INSTALL_DIR=$1

# Checks if the given command exists
command_exists() {
	  command -v "$@" > /dev/null 2>&1
}

# Install Docker if not present
install_docker() {
    if ! command_exists docker; then
        curl -sSL https://get.docker.com/ | sh;
    fi
}

# Checks if the given files differ
files_differ() {
    diff "$@" > /dev/null; [ $? -ne 0 ]
}

# Use provided configuration, only update if changed.
config_docker() {
    local orig=/etc/default/docker
    local new=$INSTALL_DIR/files/docker
    # Check if the configuration is different
    if files_differ $orig $new; then
       # Apply new configuration
       sudo cp $new $orig
       # Change requires docker to be restarted
       sudo service docker restart
    fi
}

# Main entry point
main() {
    install_docker
    config_docker
}
main $@