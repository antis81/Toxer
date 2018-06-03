# HowTo build Toxer for Sailfish OS

Before building, make sure SailfishOS SDK (aka MerSDK) is installed and accessible via SSH.

By default the build script is setup to build for the emulator. Make sure using the correct SailfishOS version by setting `SFVER`.

## Step 1: Prepare the SDK and install the basic build environment.

```bash
# ssh into the Build-SDK
# TODO: encapsulate this step
ssh -p 2222 -i ~/SailfishOS/vmshare/ssh/private_keys/engine/mersdk mersdk@localhost

# then inside the SDK, install compiler and build environment
export SFVER='2.2.0.29'
sb2 -t SailfishOS-$SFVER-armv7hl -m sdk-install -R zypper install gcc gcc-c++ autoconf automake make libtool`
`sb2 -t "SailfishOS-$SFVER-i486" -m sdk-install -R zypper install gcc gcc-c++ autoconf automake make libtool`

# you can leave the ssh session now (press CTRL+D)
```

## Step 2: Setup the Toxer specific build environment.

```bash
# IMPORTANT:
Edit the "env" file located in this folder and set SFVER to the correct SDK version
# Example:
SFVER='2.0.0.29'

# Optionally you can also adjust the toxcore & sodium versions to your needs here.
```

## Step 3: Run the preparation scripts.

```bash
# build & install toxcore + sodium libs in MerSDK before building the package
./dev-prepare.sh

# copy the project sources
./package.sh
```

## Step 4: Build Toxer RPM package manually inside the SDK

```bash
# TODO: This step can be automated as well
# ssh to MerSDK again (see Step 1)

# Finally build the RPM package from inside the SDK.
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
