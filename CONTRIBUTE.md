# How to contribute to Toxer
We are happy for any contributions to the project and strive to keep hurdles as low as possible. The first thing you should do, if you want to contribute, is to choose for a category. The following chapters help you with where to start and how to do it right.

# Contribute documentation
Documentation is part of communication with the users and contributors. While communication is a very important part in the process, we like to avoid over-documenting with "write once, read never" documentation. By nature, a user interface should be designed self-explanatory. However, if you think about e.g. on a tutorial on how to use the client, please keep in mind, the UI might be heavily changing (especially in pre-alpha) and screenshots become incompatible too easily. The `Readme.md` file should cover the most important part and be clear from there.

### Code-Documentation (comments)
The code is documented via Doxygen comments. Those shall be above each entity (method, class, …) and always in the definition (the `.cpp` file). For example a class should be documented like this:

file: `myclass.cpp`
```c++
// license note

#include "myclass.h"
// further includes

/**
@class MyClass
@brief This class does something.

Detailed description of the class.


@struct MyStruct
@brief This struc does something.

Detailed description of the struct.


@enum MyClass::ExampleEnum
@brief This enumeration reflects something.
@var One    first value
@var Two    second value

Detailed description of the enumeration.


@fn MyClass::someSignal
@brief A static/inline function, signal or callback.

Detailed description of the function.
*/

// Continue with the first function, which is usually the class constructor

/**
@brief constructor
@param[in] parameter1        the first parameter
@param[in,out] parameter2    the second parameter

Detailed description
*/
MyClass::MyClass(int parameter1, int& parameter2) {
    // do something with the parameters
}
```

**What you shouldn't do:**

* inline comments are obsolete and an absolute exception
    * `// TODO: something` comments are fine, but should be rare -> if some code is not finished, it is TODO by nature
* documenting UI related code usually does not make sense
* leading stars -> make it harder to read, write and parse

# Contribute localization (l10n)
TODO

# Contribute code
Having a great idea to improve Toxer? Feel welcome. However, if you want to contribute code, there is naturally some things you should learn first. 

## Code with style
If this is the first project you are contributing to, you should know coders care about "how readable code is". We call this the "Code-Style" and every project has one. The good news for Toxer is: We are using a relaxed Code-Style. There's only the following few rules that apply.

### Rule \1: break at **80 columns**

Example:
```c++
this->is_a_very_long_function(with, too, much, parameters, just, to, show,
                              how, to, break, a, line);
char* longText = "And this is how you would break a long text spreading"
                 " multiple kilometers in length (and so on)";
```

### Rule \#2: Indent by leading **spaces** (no tabs)
For short and single-line commands - which you alwas prefer - indent by four (4) spaces. If your command gets longer, you may want to indent to the opening brace or bracket. Examples:

```c++
// example 1:
int i =
    thisIntegerIsInitializedByAMethodNameDefinitelyTooLongToFitIntoOneLine();

// example 2:
int i = init("This integer is initialized by a short method, but the"
             " parameter would not fit into one line.");

// example 3 (variant of example 2):
int i = init(
    "This integer is initialized by a short method, but the parameter would not"
    " fit into one line.");

// Now please use your brain and think of your own. :)
```

### Rule \#3: Enclose **every** single- or multi-line block with brackets (`{}`)

```c++
if (a==1) {
    // do something
} else {
    // do something else
}

forever() {
    // loop until judgement day
}
```

## Build "Toxer"
If you have not yet read the [Code-Style](#code-with-style), please do so before heading on with this chapter. After that, you are all set up and ready to download the code and start building the application.

### Pre-Requisites

Library/Binaries | Minimum Version
---- | ----
Qt | >= 5.6
CMake | >= 3.5
toktok/c-toxcore | >= 0.1.3

If you have these installed, you are all set up and the fun can begin!

### Get the code
Clone the Git source repository:
```bash
git clone https://gitlab.com/antis81/Toxer.git
```

### Build

The processs to building Toxer is pretty much straight forward and requires no specific knowledge to get you started. As a reminder: Please make sure the [Pre-Requisites](#pre-requisites) are met.

```bash
# go to project directory…
cd /path/to/Toxer

# …and fetch the toktok/c-toxcore sources into "lib/toxcore" directory
./scripts/getToxCore.sh
```

From here you have two options to build.

1. The easiest and recommended way to build from source on your desktop system is by using Qt Creator and open the "CMakeLists.txt" project file. From here you will be guided nicely through the project configuration.

2. Rather want to build directly from Terminal? Then continue with the following steps:

```bash
# setup your Qt environment (e.g. Qt 5.6 pre-build with gcc_64)
export QT_DIR=/path/to/Qt/gcc_64
export PATH=$QT_DIR/bin:$PATH

# probably also required for CMake to find the QtQuick modules
# export Qt5_DIR=$QT_DIR/lib/cmake

# finally cmake/make the project
mkdir build && cd build
cmake ..
make

# you should find the resulting "toxer" binary inside the "build/bin" directory
# still in the build directory, start toxer with:
bin/toxer
```

# Contribute unit tests
TODO

# Contribute test/build/release scripts (CI/CD)
The Toxer build & release system is available online at [Toxer builds](https://build.tox.chat/view/toxer).

If you want to help release Toxer for a platform, please contact the Tox community.
