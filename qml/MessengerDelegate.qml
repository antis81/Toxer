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
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

import com.tox.qmlcomponents 1.0

import "." // QTBUG-34418
import "controls" as Controls


RowLayout {
    id: root

    readonly property bool isMe: pk === toxProfile.publicKeyStr()

    spacing: 5
    layoutDirection : root.isMe ? Qt.RightToLeft : Qt.LeftToRight

    Item {
        id:picItem

        width: Math.min(Math.max(root.width * 0.1, 26), 36)
        height: width
        Layout.alignment: Qt.AlignCenter

        Image {
            id: image

            anchors.fill: parent

            visible: false
            sourceSize: Qt.size(width, height)
            source: {
                var url = Toxer.avatarsUrl() + "/" + pk.toUpperCase() +
                        ".png"
                return Toxer.exists(url) ? url : Style.icon.noAvatar
            }
        }
        Rectangle {
            id: mask

            anchors.fill: image
            color: "blue"
            radius: width / 2
            clip: true
            visible: false
        }
        OpacityMask {
            anchors.fill: mask
            source: image
            maskSource: mask
        }
    }

    Rectangle {
        id: messageRec

        Layout.fillWidth: true
        Layout.minimumWidth: 100
        implicitHeight: messageText.paintedHeight +
                        messageText.anchors.topMargin +
                        messageText.anchors.bottomMargin
        Layout.alignment: Qt.AlignTop

        color: root.isMe ? "#5E97EB" : "#424954"
        radius: 4

        Controls.Text {
            id: messageText

            text: message
            color: "#F6F6F9"

            anchors.fill: parent
            anchors.margins: 4

            maximumLineCount: 50
            wrapMode: Text.WordWrap
            elide: Text.ElideNone
        }
    }

    Controls.Text {
        id:messageTime

        Layout.alignment: Qt.AlignTop

        text: Qt.formatTime(new Date(ts), "hh:mm:ss")
    }
}
