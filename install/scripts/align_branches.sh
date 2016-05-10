#!/bin/sh
# whikloj - 2016-03-22
#
# Changes the version for islandora/resource-service or
# islandora/transaction-service to 'dev-<current branch>'

if [ $# -gt 0 ]; then
  # Take branch name from arguments
  CURRENT_BRANCH="$1"
else
  # Currently selected branch in git
  CURRENT_BRANCH=$(git branch | grep '*' | cut -d' ' -f2)
fi
# List of directories to change composer.json in 
AFFECTS_DIR=". CollectionService TransactionService"

echo "Altering composer.json to use branch 'dev-${CURRENT_BRANCH}'..."
for i in $AFFECTS_DIR; do
  mv $i/composer.json $i/composer.json_bkup
  awk -v BRANCH=$CURRENT_BRANCH '/islandora\/(resource|transaction|collection)/ { print gensub(/(.+):\s*"dev-[^"]+/, "\\1: \"dev-"BRANCH, 1)} !/islandora\/(resource|transaction|collection)/ { print $0}' $i/composer.json_bkup > $i/composer.json
done
echo "Done"

