#!/bin/bash

# Get the end portion of the TRAVIS_REPO_SLUG for the branch name.
DELIMITED_SLUG = $(echo $TRAVIS_REPO_SLUG | tr "/" "\n")
CURRENT_REPO = "${DELIMITED_SLUG[1]}"

# Git business
export VERBOSE_DIR = $HOME/sites/default/files/simpletest/verbose
cd $HOME
git clone https://islandora-logger:$LOGGER_PW@github.com/Islandora/islandora_travis_logs.git
cd islandora_travis_logs
git checkout -B $CURRENT_REPO

# Out with the old, in with the new
git rm $HOME/islandora_travis_logs/*.*
cp $VERBOSE_DIR/*.* $HOME/islandora_travis_logs
git add -A
git commit -m "Job: $TRAVIS_JOB_NUMBER Commit: $TRAVIS_COMMIT"
git push origin $CURRENT_REPO
