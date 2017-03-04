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
import ".." // QTBUG-34418

Rectangle {
    id: slider

    property real from: 0.0
    property real to: 1.0
    property real stepSize: 0.01
    property real value: 0.5

    property alias text: text.text

    width: 100
    height: 30

    Accessible.role: Accessible.Slider
    Accessible.focusable: true

    border.color: Style.color.alternateBase
    border.width: 1

    gradient: Gradient {
        GradientStop { position: 0.0; color: Style.color.base }
        GradientStop { position: 0.38; color: Style.color.alternateBase }
    }

    Rectangle {
        id: indicator

        x: slider.border.width
        y: slider.border.width

        width: {
            var w = slider.width - 2 * slider.border.width;
            var range = to - from;
            var v = (slider.value - from) / range;
            return v * w;
        }
        height: slider.height - 2 * slider.border.width
        radius: 2
        gradient: Gradient {
            GradientStop { position: 0.0; color: Style.color.alternateBase }
            GradientStop { position: 0.38; color: Style.color.base }
        }
    }

    Controls.Text {
        id: text

        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onMouseXChanged: {
            var pos = mouseX / (slider.width - 2 * slider.border.width);
            var range = to - from;
            var v = (range < 1 ? pos * range : pos / range) + from;
            slider.value = Math.max(Math.min(v, slider.to), slider.from);
        }
    }
}
