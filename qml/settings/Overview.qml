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

import "../base" as Base
import "../controls" as Controls

Base.View {
    id: root

    ListModel {
        id: menuModel

        ListElement {
            caption: qsTr("Appearance")
            description: qsTr("Appearance settings")
            page: "Appearance.qml"
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Flow {
            id: menu

            Layout.fillWidth: true
            Layout.minimumHeight: 24

            Controls.FlatButton {
                text: qsTr("Back")
                onClicked: { closing(); }
            }

            Repeater {
                id: menuCtl

                model: menuModel

                Controls.FlatButton {
                    text: caption
                    Accessible.description: description

                    onClicked: {
                        settingsPage.source = page
                    }
                }
            }
        }

        Loader {
            id: settingsPage

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Component.onCompleted: {
            // TODO: activate first menu entry
        }
    }
}
