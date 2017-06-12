import QtQuick 2.0
import QtQuick.Layouts 1.1

import com.tox.qmlcomponents 1.0
import com.tox.qmltypes 1.0

Rectangle {
    id: root

    implicitWidth: 50
    implicitHeight: 30

    color: Style.color.alternateBase

    ToxProfileQuery {
        id: toxProfile

        function statusIcon() {
            if (isOnline()) {
                switch (statusInt()) {
                case ToxTypes.Unknown:
                case ToxTypes.Away: return Style.icon.away;
                case ToxTypes.Busy: return Style.icon.busy;
                case ToxTypes.Ready: return Style.icon.online;
                }

                return Style.icon.away;
            } else {
                return Style.icon.offline;
            }
        }

        onIsOnlineChanged: {
            selfInfo.statusLight.source = statusIcon();
        }
        onStatusChanged: {
            selfInfo.statusLight.source = statusIcon();
        }
        onUserNameChanged: {
            selfInfo.name.text = userName;
        }
        onStatusMessageChanged: {
            selfInfo.statusMessage = statusMessage;
        }
    }

    Connections {
        target: toxProfile
    }

    FriendDelegate {
        id: selfInfo

        anchors.fill: parent
        anchors.margins: parent.height * 0.05
        anchors.bottomMargin: 3

        // TODO: avatar, name and message should be editable.
        //       a separate flyout dialog makes sense to retain the ui
        //       scalable to any screen format

        avatar.source: {
            var url = Toxer.avatarsUrl() + "/" +
                    toxProfile.publicKeyStr().toUpperCase() + ".png"
            return Toxer.exists(url) ? url : Style.icon.noAvatar;
        }

        name.text: toxProfile.userName();
        statusMessage.text: toxProfile.statusMessage()
        statusLight.source: toxProfile.statusIcon()
    }
}
