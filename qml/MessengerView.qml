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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import com.tox.qmlcomponents 1.0

import "base" as Base
import "controls" as Controls

Base.View {
    id: root

    property int friendIndex: undefined

    width: 320
    height: 600

    ToxProfileQuery { id: toxProfile }
    ToxMessenger {
        id: toxMessenger

        onMessage: {
            // TODO: This is only for testing purpose.
            //       Implement a little message db!
            if (index === root.friendIndex) {
                messageModel.addMessageItem(toxMessenger.publicKeyStr(index),
                                            message);
            }
        }
    }

    SplitView {
        id: layout

        anchors.fill: parent
        orientation: Qt.Vertical

        ListView {
            id:messageBox

            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 2
            clip: true

            model: MessageModel {
                id: messageModel
            }
            delegate: MessengerDelegate {
                width: messageBox.width
            }

            // auto-scroll to last item
            currentIndex: messageModel.count - 1
        }

        RowLayout {
            id: inputLayout

            Layout.minimumHeight: 40
            Layout.maximumHeight: parent.height * 0.4
            spacing: 0
            clip: true

            Controls.TextEdit {
                id: messageInput

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 40

                placeholderText: qsTr("Type your message here...")
                wrapMode: TextEdit.WordWrap

                Keys.onReturnPressed: {
                    if (event.modifiers === Qt.NoModifier) {
                        root.sendMessage();
                    } else if (event.modifiers === Qt.ShiftModifier) {
                        text += "\n";
                        cursorPosition = length
                    }
                }
            }

            Controls.FlatButton {
                id: btnSendMessage

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 20
                Layout.maximumWidth: root.width * 0.2

                border { width: 1; color: "#6697E7"}
                Accessible.defaultButton: true
                enabled: messageInput.text
                text: qsTr("Send")

                onClicked: {
                    root.sendMessage();
                }
            }
        }
    }

    function sendMessage() {
        var message = messageInput.text;
        if (message) {
            toxMessenger.sendMessage(friendIndex, message);

            // TODO: This is only for testing purposes.
            //       Implement a little message db!
            messageModel.addMessageItem(toxProfile.publicKeyStr(),
                                        message);
            messageInput.clear();
        }
    }

    Component.onCompleted: {
        messageInput.forceActiveFocus();
    }
}
