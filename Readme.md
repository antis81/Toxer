# Toxer: The Tox framework that simply fits Qt(Quick).
Toxer is a quality framework for building Qt/QML based Tox applications. The framework itself does not provide a user interface.

![! WARNING: EXPERIMENTAL !](webres/experimental.png  "ToxerIsExperimental")

**Lots of features are still not implemented. Feel welcome to contribute your ideas!**

## Resources:

* Contact:
    * **Tox protocol (a.k.a toxcore)** based questions/suggestions are answered by our competent Tox enthusiasts on IRC channels [#tox-dev](irc://irc.freenode.net/#tox-dev) and [#toktok](irc://irc.freenode.net/#toktok)
    * For feature requests on Toxer, please file an issue or [contribute](CONTRIBUTE.md).

## Downloads (binary releases)

Currently Toxer has to be built from source. If you want to help out, please contact the community as written in the [Resources](#resources) section.

## Usage

Toxer framework integrates fairly smooth with Qt applications! Tox data is represented via easy to use query objects.

I would like to introduce two of them, namely `ToxProfileQuery` and `ToxFriendQuery` to perform quite common tasks like showing a friend's online status.

```qml
import QtQuick 2.0

import com.tox.qmlcomponents

MyItem {
    ToxFriendQuery {
        id: tfq
        onMessage: {
            console.log("Friend " + tfq.name(index) + " says: " + message);
        }
    }

    Component.onCompleted {
        // notes:
        // 1. For the record: The password used here is an example.
        //    It is required to decrypt the Tox profile locally(!) on your device.
        //    Toxer respects your privacy and does in no way save your password.
        //    You can input the password in a PasswordField or similar text field!
        // 2. You can either create an empty profile or open an existing one.
        //    For the example we assume an existing profile "my_profile.tox" on disk.
        Toxer.activateProfile("my_profile.tox", "my_secret_password");
        console.log("I have as many as " + tfq.count + " friends on Tox. Yay!");
    }
}
```

Once you get the hang of it, using Toxer object mechanisms becomes easy and smooth.

A more advanced UI integration is waiting here: https://gitlab.com/antis81/Toxer-UI

## Progress

The following table provides an overview of what is there and what is yet to be done.

Feature | Progress | Notes
---- | ---- | ----
Profile-Management | 95% | --
Add/Remove Friends | 0% | TODO
1v1 Chat | 30% | 1v1 chat works
Group Chat | 0% | TODO
Styles/Themes | 95% | works; some settings are not persistent yet

# License
The Toxer application is freely usable under the terms of the MIT license. The licenses of the underlying libraries (e.g. Qt and toxcore) apply to these components. If you plan to re-use the code, please contact the Tox community.
