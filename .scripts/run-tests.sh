#!/bin/bash

# Wrapper for the test executing function so we only have to change it in one place.
# The module name gets passed in as a command line arg.
php core/scripts/run-tests.sh --suppress-deprecations --url http://127.0.0.1:8282 --verbose --php `which php` --module "$1"
