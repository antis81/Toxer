#!/bin/bash

set -e

# setup script and project paths
SCRIPT_DIR=$(dirname `readlink -f $0`)
source $SCRIPT_DIR/env
TOXER_PRJ=${TOXER_PRJ:-"$SCRIPT_DIR/.."}
export TOXER_PRJ=$(readlink -f $TOXER_PRJ)
export TARGET=$SFVER-$ARCH

# create directory structure required to build sailfish rpm
echo -en "creating rpm tree in $SCRIPT_DIR\t"
rsync -a \
  --exclude ".git*" \
  --exclude "build" \
  --exclude "qml" \
  --exclude "sailfish"  \
  --exclude "scripts" \
  --exclude "*.md" \
  --exclude "*.user*" \
  $TOXER_PRJ/ \
  $SCRIPT_DIR/
echo "OK"

# build rpm package from parent directory
cd $TOXER_PRJ
# TODO: ssh to mersdk
#mb2 -t SailfishOS-$TARGET build
cd -

