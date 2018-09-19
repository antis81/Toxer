#!/bin/sh
set -e

# Installs the libraries and headers of Tox.

SCRIPTDIR=$(dirname `readlink -f $0`)

system=${$1:-'linux'}
if [ "system" = "linux" ] ; then
  source "$SCRIPTDIR/$system/env"
fi

TOXCORE="$SCRIPTDIR/toxcore-$TOX_VERSION"
TOXCORE_BUILD="$TOXCORE/_build"
LOGBUILD=$SCRIPTDIR/toxcore_build.log
LOGINSTALL=$SCRIPTDIR/toxcore_install.log

if [ -f "$LIBS/libtoxcore.so" ] ; then
  echo "Removing previously installed toxcore libraries…"
  sudo rm -f $LIBS/libtoxcore.*
  sudo rm -f $LIBS/pkgconfig/toxcore.pc
  echo "…and headers…"
  sudo rm -rf $TOX_HEADERS
fi
if [ -d "$TOXCORE" ] ; then
  echo "Removing existing toxcore sources in ${TOXCORE}…"
  rm -rf $TOXCORE
fi
mkdir -p $TOXCORE
curl -L $TOX_ARCHIVE | tar xz -C $TOXCORE --strip-components=1

# build it
echo "Building toxcore in ${TOXCORE_BUILD}…"
mkdir -p $TOXCORE_BUILD
cd $TOXCORE_BUILD
cmake \
  -DBUILD_AV_TEST=OFF \
  -DENABLE_STATIC=OFF \
  .. > $LOGBUILD
make -j$(nproc) -s >> $LOGBUILD
sudo make -s install > $LOGINSTALL
make clean
cd -
