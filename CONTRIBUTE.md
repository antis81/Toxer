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
TODO

## Code-Style
The good news is: Coding style is very relaxed. There's only the following few simple rules to follow:

* Indent by four (4) leading **spaces** (no tabs)
* Put brackets on **every** conditional or loop block:
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

* break at **80 columns** - highly improves readability:
```c++
this->is_a_very_long_function(with, too, much, parameters, just, to, show,
                              how, to, break, a, line);
char* longText = "And this is how you would break a long text spreading"
                 " multiple kilometers in length (and so on)";
```

# Contribute unit tests
TODO

# Contribute test/build/release scripts (CI/CD)
TODO
