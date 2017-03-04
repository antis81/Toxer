/*
 * This file is part of the Toxer application, a Tox messenger client.
 *
 * Copyright (c) 2017 Nils Fenner <nils@macgitver.org>
 *
 * This software is licensed under the terms of the MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

import com.tox.qmlcomponents 1.0
import com.tox.qmltypes 1.0

import "." // QTBUG-34418
import "controls" as Controls

Controls.Page {
    id: root

    width: Math.min(280, Screen.width)
    height: Math.min(430, Screen.height)

    readonly property url defaultView: "FriendsView.qml"

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
            selfInfo.statusLight = statusIcon();
        }
        onUserNameChanged: {
            selfInfo.name.text = userName;
        }
        onStatusMessageChanged: {
            selfInfo.statusMessage = statusMessage;
        }
    }

    ExclusiveGroup {
        id: mainToolButtons

        onCurrentChanged: {
            if (current === btnSettings) {
                view.source = "settings/Overview.qml";
            } else if (current === addIcon) {
                view.source = "AddFriendView.qml";
            } else {
                view.source = root.defaultView;
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        Rectangle {
            id: selfView

            Layout.fillWidth: true
            Layout.minimumHeight: Math.max(root.height * 0.1, 40)
            Layout.maximumHeight: 55

            color: Style.color.alternateBase

            FriendDelegate {
                id: selfInfo

                anchors.fill: parent
                anchors.margins: parent.height * 0.1

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

        Loader {
            id: view

            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: {
                view.source = root.defaultView;
            }

            Connections {
                target: view.item
                onClosing: {
                    view.source = root.defaultView;
                    mainToolButtons.current = null;
                }
            }
        }

        Rectangle {
            id: quickActions

            Layout.fillWidth: true
            Layout.minimumHeight: Math.max(root.height * 0.05, 26);
            Layout.maximumHeight: 32

            color: Style.color.alternateBase

            RowLayout {
                anchors.fill: parent

                spacing: 0

                ToolButton {
                    id: btnSettings

                    Layout.fillHeight: true
                    width: height
                    Layout.margins: 2

                    checkable: true
                    exclusiveGroup: mainToolButtons

                    clip: true
                    iconSource: Style.icon.settings

                    NumberAnimation {
                        id: rotAni

                        target: btnSettings
                        property: "rotation"
                        duration: 2000
                        from: 0
                        to: 360
                        loops: Animation.Infinite
                    }

                    onHoveredChanged: {
                        if (hovered) {
                            if (!rotAni.running) {
                                rotAni.start();
                            }
                        } else {
                            if (rotAni.running) {
                                rotAni.stop();
                            }
                        }
                    }
                }

                TextField {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 1

                    font.pointSize: Style.fontPointSize;
                    placeholderText: qsTr("Search friend…");
                }

                ToolButton {
                    id: addIcon

                    Layout.fillHeight: true
                    width: height
                    Layout.margins: 2

                    clip: true
                    iconSource: Style.icon.add

                    checkable: true
                    exclusiveGroup: mainToolButtons

                    NumberAnimation {
                        id: pushAni

                        target: addIcon
                        property: "scale"
                        duration: 200
                        alwaysRunToEnd: true
                    }

                    onHoveredChanged: {
                        pushAni.stop();
                        pushAni.to = hovered ? 0.7 : 1.0;
                        pushAni.restart();
                    }
                }
            }
        }
    }
}
