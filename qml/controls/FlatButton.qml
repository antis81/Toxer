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

import "." as Controls
import ".." // QTBUG-34418

Rectangle {
    id: root

    property int padding: 15

    property alias contentIcon: icon
    property alias contentLabel: contentLabel

    property alias text: contentLabel.text
    property alias iconSource: icon.source
    property ExclusiveGroup exclusiveGroup: null
    property color hoverColor: {
        var c = Style.color.base;
        return Qt.hsla(c.hslHue + 0.2, c.hslSaturation,
                       c.hslLightness + (c.hslLightness <= 0.5 ? 0.1 : -0.1));
    }
    readonly property bool hovered: mouseArea.hoverEnabled &&
                                    mouseArea.containsMouse
    property bool checked: Accessible.checkable && Accessible.checked;

    signal clicked

    Accessible.role: Accessible.Button
    Accessible.name: contentLabel.text
    onCheckedChanged: {
        if (exclusiveGroup) {
            exclusiveGroup.current = checked ? this : null;
        }
    }
    Accessible.onPressAction: {
        if (Accessible.checkable) {
            Accessible.checked = !Accessible.checked;
        } else {
            clicked();
        }
    }

    implicitWidth: contentLabel.width + padding
    implicitHeight: contentLabel.height + padding

    color: hovered ? hoverColor : "transparent"
    border.color: "transparent"
    border.width: 3
    radius: 3
    opacity: enabled ? 1 : 0.2

    Behavior on opacity {
        NumberAnimation { duration: 300 }
    }

    Image {
        id: icon

        anchors.centerIn: parent
        sourceSize: Qt.size(width, height)
        Component.onCompleted: {
            height = Math.min(width, height);
            width = height;
        }
    }

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
            from: border.color
            to: hoverColor
            duration: 800
        }
        ColorAnimation {
            from: hoverColor
            to: border.color
            duration: 800
        }
    }
}
