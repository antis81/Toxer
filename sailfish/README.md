# HowTo build Toxer for Sailfish OS

Before building, make sure SailfishOS SDK (aka MerSDK) is installed and accessible via SSH.

By default the build script is setup to build for the emulator. Make sure using the correct SailfishOS version by setting `SFVER`.

## Step 1: Prepare the SDK and Install the Basic Build Environment

```bash
# ssh into the Build-SDK
ssh -p 2222 -i ~/SailfishOS/vmshare/ssh/private_keys/engine/mersdk mersdk@localhost

# once inside the SDK, install compiler and build environment
export SFVER='2.2.0.29'
sb2 -t SailfishOS-$SFVER-armv7hl -m sdk-install -R zypper install gcc gcc-c++ autoconf automake make libtool`
`sb2 -t "SailfishOS-$SFVER-i486" -m sdk-install -R zypper install gcc gcc-c++ autoconf automake make libtool`

# you can leave the ssh session now (press CTRL+D)
```

## Step 2: Setup the Host Build Environment

Edit the SDK environment located in `mersdk/env`:

```bash
SFVER='2.2.0.29' # set to your installed version of MerSDK
# You can also adjust the toxcore & sodium versions to your needs.
```

Run the preparation scripts:

```bash
# build & install toxcore + sodium libs in MerSDK before building the package
./setup-mersdk.sh

# copy the local project files
./copy-project.sh
```

## Step 3: Build Toxer RPM Package in the SDK

```bash
# ssh to MerSDK (see Step 1)

# Finally build the RPM package in the SDK.
cd /home/src1/Toxer/sailfish  # cd to /your/Toxer/sailfish directory
mb2 -t <your_sailfish_target> build
```

Note this is an early version of the build setup. I plan making this more simple over time.

# Variable Reference

The following variables can be used to configure the build to your needs:

Variable | Description
---- | ----
ARCH | (currently unused) ~~target architecture [armv7hl, i486]~~
SFVER | SailfishOS SDK version (e.g. 2.2.0.29)
TOXER\_PRJ | path to the Toxer project directory (default: parent directory "..")
