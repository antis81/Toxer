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

import "controls" as Controls

ColumnLayout {
    id: root

    Item {
        // spacer
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    Controls.TextInput {
        id: txtProfileName

        Layout.fillWidth: true
        Layout.minimumHeight: 34

        placeholderText: qsTr("Enter a profile name.")
    }

    Controls.TextInput {
        id: txtPassword

        Layout.fillWidth: true
        Layout.minimumHeight: 34

        echoMode: TextInput.Password
        placeholderText: qsTr("Enter the profile's password.")
    }

    Controls.TextInput {
        id: txtRepeatPassword

        Layout.fillWidth: true
        Layout.minimumHeight: 34

        background: txtRepeatPassword.text === txtPassword.text ? "transparent"
                                                                : "#803030"

        echoMode: TextInput.Password
        placeholderText: qsTr("Retype the profile's password.")
    }

    Item {
        // spacer
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    Row {
        Layout.alignment: Qt.AlignRight

        spacing: 5

        Controls.FlatButton {
            id: btnCreate

            enabled: txtProfileName.length > 0 && txtPassword.length > 0 &&
                     txtRepeatPassword.text === txtPassword.text

            padding: 15
            text: qsTr("Create Profile")

            onClicked: {
                Toxer.createProfile(txtProfileName.text, txtPassword.text);
            }
        }
    }
}
