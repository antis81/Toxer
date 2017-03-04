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

import "." as Controls
import ".."

Rectangle {
    id: root

    property int padding: 15

    property alias text: contentLabel.text
    property color hoverColor: Style.color.alternateBase

    signal clicked

    Accessible.role: Accessible.Button
    Accessible.name: contentLabel.text
    Accessible.defaultButton: false
    Accessible.checkable: false
    Accessible.onPressAction: {
        clicked();
    }

    implicitWidth: contentLabel.width + padding
    implicitHeight: contentLabel.height + padding

    color: mouseArea.containsMouse ? hoverColor : Style.color.base
    border.color: "transparent"
    border.width: 2
    radius: 3


    Controls.Text {
        id: contentLabel

        anchors.centerIn: parent

        color: Style.color.text
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            root.Accessible.pressAction();
        }
    }

    SequentialAnimation on border.color {
        loops: Animation.Infinite
        running: root.Accessible.defaultButton

        ColorAnimation {
            from: Style.color.base
            to: Qt.lighter(root.color, 1.8)
            duration: 800
        }
        ColorAnimation {
            from: Qt.lighter(root.color, 1.8)
            to: Style.color.base
            duration: 800
        }
    }
}
