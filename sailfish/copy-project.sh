#!/bin/bash

set -e

# setup local environment
SCRIPT_DIR=$(dirname `readlink -f $0`)
source "$SCRIPT_DIR/env.local"
TOXER_PRJ=${TOXER_PRJ:-"$SCRIPT_DIR/.."}
export TOXER_PRJ=$(readlink -f $TOXER_PRJ)

# create directory structure required to build sailfish rpm
echo -en "Creating project tree in $SCRIPT_DIR \t"
rsync -a \
  --exclude ".git*" \
  --exclude "build" \
  --exclude "qml" \
  --exclude "res" \
  --exclude "sailfish"  \
  --exclude "sailfish-ui" \
  --exclude "scripts" \
  --exclude "*.md" \
  --exclude "*.user*" \
  --exclude "webres" \
  $TOXER_PRJ/ $SCRIPT_DIR/ >/dev/null
echo "OK"

# copy SailfishOS UI
echo -en "Creating UI resource tree in $SCRIPT_DIR/qml \t"
mkdir -p $SCRIPT_DIR/qml/res
rsync -a $TOXER_PRJ/sailfish-ui/ $SCRIPT_DIR/qml >/dev/null
rsync -a \
  --exclude ".git*" \
  --exclude "*.qrc" \
  $TOXER_PRJ/res/ $SCRIPT_DIR/qml/res >/dev/null
echo "OK"
