#!/bin/bash

set -e

# setup local environment
SCRIPT_DIR=$(dirname `readlink -f $0`)
source "$SCRIPT_DIR/env.local"
TOXER_PRJ=${TOXER_PRJ:-"$SCRIPT_DIR/.."}
export TOXER_PRJ=$(readlink -f $TOXER_PRJ)

# create directory structure required to build sailfish rpm
echo -en "Creating CMake/C++ project tree in $SCRIPT_DIR \t"
rsync -a \
  --exclude ".git*" \
  --exclude "build" \
  --exclude "qml" \
  --exclude "res" \
  --exclude "sailfish"  \
  --exclude "scripts" \
  --exclude "*.md" \
  --exclude "*.qrc*" \
  --exclude "*.user*" \
  --exclude "webres" \
  $TOXER_PRJ/ $SCRIPT_DIR/ >/dev/null
echo "OK"
