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

import QtQuick 2.0

import com.tox.qmlcomponents 1.0
import com.tox.qmltypes 1.0

import "base" as Base
import "controls" as Controls

Base.View {
    id : root

    ListView {
        id: friendsView

        property bool compactMode: false

        ToxFriendQuery {
            id: toxFriends

            function statusIcon(friendIndex) {
                if (isOnline(friendIndex)) {
                    switch (statusInt(friendIndex)) {
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
        }

        anchors.fill: parent

        // TODO: initialize model by toxFriends.friends()
        model: toxFriends.count()

        spacing: compactMode ? 2 : 8
        delegate: FriendDelegate {
            id: friendDelegate

            height: friendsView.compactMode ? 22 : 36
            width: friendsView.width

            name.text: toxFriends.name(modelData)

            statusMessage.visible: !friendsView.compactMode
            statusMessage.text: toxFriends.statusMessage(modelData)

            avatar.source: {
                var url = Toxer.avatarsUrl() + "/" +
                        toxFriends.publicKeyStr(modelData).toUpperCase() + ".png"
                return Toxer.exists(url) ? url : Style.icon.noAvatar
            }

            statusLight.source: toxFriends.statusIcon(modelData);

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    friendsView.currentIndex = index;
                }

                onDoubleClicked: {
                    if (viewLoader) {
                        viewLoader.setSource("MessengerView.qml", {
                                                 friendIndex: index
                                             });
                    } else {
                        console.warn("No ViewLoader assigned to View item " +
                                     root)
                    }
                }
            }

            Connections {
                // TODO: define the connections inside TFQ instead of delegate
                target: toxFriends
                onIsOnlineChanged: {
                    if (index === modelData) {
                        // TODO: get icon by connection state
                        statusLight.source = toxFriends.statusIcon(index);
                    }
                }
                onStatusChanged: {
                    if (index === modelData) {
                        // TODO: get icon by state enum
                        statusLight.source = toxFriends.statusIcon(index);
                    }
                }
                onNameChanged: {
                    if (index === modelData) {
                        friendDelegate.name.text = name;
                    }
                }
                onStatusMessageChanged: {
                    if (index === modelData) {
                        friendDelegate.statusMessage.text = statusMessage;
                    }
                }
            }
        }

        clip: true
        focus: true
        highlightFollowsCurrentItem: false
        highlight: Rectangle {
            width: friendsView.currentItem.width
            height: friendsView.currentItem.height
            border.color: "#80FFFFFF";
            color: Qt.darker("#33000000", 1.2)
            radius: 5
            y: friendsView.currentItem.y
            z: friendsView.currentItem.z + 1

            Behavior on y {
                NumberAnimation { duration: 120 }
            }
        }
    }
}
