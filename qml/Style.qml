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

pragma Singleton
import QtQuick 2.2

QtObject {
    property bool lightTheme: false
    property double fontPointSize: 8

    readonly property string iconResource: {
        return "qrc:/res/images/" + (lightTheme ? "light" : "dark")
    }

    property QtObject color: QtObject {
        property color base: Qt.hsla(0.0, 0.0, 0.1)
        property color alternateBase: Qt.lighter(base, 1.6)
        property color text: lightTheme ? "#333333" : "#cccccc"
    }

    property QtObject statusColor: QtObject {
        property color disabled: "gray"
        property color enabled: "#80ff80"
        property color pending: "#bbbb80"
        property color active: "#ff8080"
    }

    property QtObject userStatusColor: QtObject {
        property color offline: "gray"
        property color online: "#80ff80"
        property color away: "#bbbb80"
        property color busy: "#ff8080"
    }

    property QtObject icon: QtObject {
        property url noAvatar: iconResource + "/contact.svg"
        property url offline: iconResource + "/dot_offline.svg"
        property url online: iconResource + "/dot_online.svg"
        property url away: iconResource + "/dot_away.svg"
        property url busy: iconResource + "/dot_busy.svg"
        property url settings: iconResource + "/settings.svg"
        property url add: iconResource + "/add.svg"
    }
}
