#!/bin/bash

set -e

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
		exit 1
fi

if [ "$1" != "i486" ] && [ "$1" != "armv7hl" ]; then
    echo "Invalid target"
		exit 2
fi

SCRIPT_DIR=$(dirname `readlink -f $0`)
source $SCRIPT_DIR/env

if [ ! -d "$FAKEDIR" ]
then
	echo -en "Creating fake root $TARGET \t"
	mkdir -p "$FAKEDIR"
	echo "OK"
else
	echo -en "Cleaning fake root $TARGET \t"
	rm -rf "$FAKEDIR"/*
	echo "OK"
fi

rm -rf "$SODIUMDIR" && mkdir -p "$SODIUMDIR"
rm -rf "$TOXCOREDIR" && mkdir -p "$TOXCOREDIR"

echo -en "Getting libsodium \t"
curl -s -L "$SODIUMSRC" | tar xz -C "$SODIUMDIR" --strip-components=1 &> /dev/null
echo "OK"

echo -en "Getting toxcore \t"
curl -s -L "$TOXCORESRC" | tar xz -C "$TOXCOREDIR" --strip-components=1 &> /dev/null
echo "OK"

# LIBSODIUM
cd "$SODIUMDIR"
echo -en "Building libsodium \t"
sb2 -t SailfishOS-$TARGET -m sdk-build autoreconf -i &> "$TOXDIR/output.log"
sb2 -t SailfishOS-$TARGET -m sdk-build ./configure --prefix "$FAKEDIR" &> "$TOXDIR/output.log"
sb2 -t SailfishOS-$TARGET -m sdk-build make clean &> "$TOXDIR/output.log"
sb2 -t SailfishOS-$TARGET -m sdk-build make -j $THREADS &> "$TOXDIR/output.log"
echo "OK"
echo -en "Installing libsodium to $TARGET \t"
sb2 -t SailfishOS-$TARGET -m sdk-build make install &> "$TOXDIR/output.log"
echo "OK"
cd "$TOXDIR"

# TOXCORE
cd "$TOXCOREDIR"
echo -en "Building toxcore \t"
sb2 -t SailfishOS-$TARGET -m sdk-build autoreconf -i &> "$TOXDIR/output.log"
sb2 -t SailfishOS-$TARGET -m sdk-build ./configure --with-pic --prefix "$FAKEDIR" --with-libsodium-headers="$SODIUMDIR/src/libsodium/include" --with-libsodium-libs="$SODIUMLIBSDIR" &> "$TOXDIR/output.log"
sb2 -t SailfishOS-$TARGET -m sdk-build make clean &> "$TOXDIR/output.log"
sb2 -t SailfishOS-$TARGET -m sdk-build make -j $THREADS &> "$TOXDIR/output.log"
echo "OK"
echo -en "Installing toxcore to $TARGET \t"
sb2 -t SailfishOS-$TARGET -m sdk-build make install &> "$TOXDIR/output.log"
echo "OK"
