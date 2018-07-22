# Toxer: The Tox-messenger that simply fits.
Toxer aims to become the framework of choice for QML based Tox client implementations. Currently a builtin user interface comes with it.

![! WARNING: EXPERIMENTAL !](webres/experimental.png  "ToxerIsExperimental")

**Please note our baby is just born and learning to walk!**

## Resources:

* Contact:
    * **Tox protocol (a.k.a toxcore)** based questions/suggestions are answered by our competent Tox enthusiasts on IRC channels [#tox-dev](irc://irc.freenode.net/#tox-dev) and [#toktok](irc://irc.freenode.net/#toktok)
    * For feature requests on Toxer, please file an issue or [contribute](CONTRIBUTE.md).

## Downloads (binary releases)

Currently Toxer has to be built from source. If you want to help out, please contact the community as written in the [Resources](#resources) section.

## Usage

Toxer framework is fairly simple to use! All Tox data is accessed via query objects. Currently we have `ToxProfileQuery` and `ToxFriendQuery` to perform these tasks.

Example to query Tox friend data in QML:

```qml
import QtQuick 2.0

import toxer.

MyItem {
    // note: A Tox profile must be initialized before for this example to work.
    ToxFriendQuery {
        id: tfq
        onMessage: {
            console.log("Friend " + tfq.name(index) + " says: " + message);
        }
    }

    Component.onCompleted {
        // note: the password can be derived from a TextField or something and is not saved by Toxer in any way!
        Toxer.activateProfile("my_profile.tox", "my_secret_password");
        console.log("I have as many as " + tfq.count + " friends on Tox. Yay!");
    }
}
```

## Progress

The following table provides a good overview of what is there and what is missing in order to provide the basic functionality.

Feature | Progress | Notes
---- | ---- | ----
Encryption | 100% | --
Profile-Management | 95% | todo: profile import
Tox-Bootstrap | 95% | randomize nodes
Friends | 90% | todo: add/remove friend
Chat | 30% | todo: message management (history etc.)
Group-Chat | 0% | ??
Styles/Themes | 95% | todo: save style settings; support background images

# License
The Toxer application is freely usable under the terms of the MIT license. The licenses of the underlying libraries (e.g. Qt and toxcore) apply to these components. If you plan to re-use the code, please contact the Tox community.
