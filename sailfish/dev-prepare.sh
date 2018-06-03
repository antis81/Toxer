#!/bin/bash

# Exit on errors
set -e

SCRIPT_DIR=$(dirname `readlink -f $0`)
SSH_KEYDIR="~/SailfishOS/vmshare/ssh/private_keys/engine/mersdk"

# prepare extra
mkdir -p $SCRIPT_DIR/extra

# Prepare build dir and script
echo -en "Preparing build dir \t"
ssh -p 2222 -i "$SSH_KEYDIR" mersdk@localhost 'mkdir -p ~/tox' > /dev/null
scp -P 2222 -i "$SSH_KEYDIR" $SCRIPT_DIR/build-libs.sh mersdk@localhost:~/tox > /dev/null
scp -P 2222 -i "$SSH_KEYDIR" $SCRIPT_DIR/env mersdk@localhost:~/tox > /dev/null
echo "OK"

# Build
ssh -p 2222 -i "$SSH_KEYDIR" mersdk@localhost "cd ~/tox; ./build-libs.sh i486 && ./build-libs.sh armv7hl"

# Copy everything where it's needed
echo -en "Copying libs and headers \t"
scp -r -P 2222 -i "$SSH_KEYDIR" "mersdk@localhost:~/tox/*i486" $SCRIPT_DIR/extra/i486  > /dev/null
scp -r -P 2222 -i "$SSH_KEYDIR" "mersdk@localhost:~/tox/*armv7hl" $SCRIPT_DIR/extra/armv7hl > /dev/null
# Clean up unneeded files
# i486
rm -rf $SCRIPT_DIR/extra/i486/bin > /dev/null
rm -rf $SCRIPT_DIR/extra/i486/lib/pkgconfig > /dev/null
#rm -f $SCRIPT_DIR/extra/i486/lib/*.so* > /dev/null
#rm -f $SCRIPT_DIR/extra/i486/lib/*.la > /dev/null
# arm
rm -rf $SCRIPT_DIR/extra/armv7hl/bin > /dev/null
rm -rf $SCRIPT_DIR/extra/armv7hl/lib/pkgconfig > /dev/null
#rm -f $SCRIPT_DIR/extra/armv7hl/lib/*.so* > /dev/null
#rm -f $SCRIPT_DIR/extra/armv7hl/lib/*.la > /dev/null
echo "OK"
