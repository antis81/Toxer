#!/bin/sh -e

SCRIPT_DIR=$(dirname `readlink -f -- $0`)

PROJECT_DIR=${PROJECT_DIR:-"$SCRIPT_DIR/.."}
GIT_REMOTE=${GIT_REMOTE:-'https://github.com/toktok/c-toxcore.git'}
GIT_BRANCH='v0.1.6'
GIT_DEST_DIR=${GIT_DEST_DIR:-"$PROJECT_DIR/lib/toxcore"}

if [ -d "$GIT_DEST_DIR" ] ; then
  echo "Deleting existing clone at $GIT_DEST_DIR"
  rm -rf "$GIT_DEST_DIR"
fi

git clone -q --progress --no-checkout --depth=2 --branch="$GIT_BRANCH" \
  $GIT_REMOTE $GIT_DEST_DIR
git -C "$GIT_DEST_DIR" checkout

echo "Cloned toxcore sources to $GIT_DEST_DIR."
