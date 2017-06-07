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
import QtQuick.Controls.Styles 1.4

import com.toxer.settings 1.0

import ".." // QTBUG-34418
import "../controls" as Controls

Column {
    id: root

    width: 400
    height: 300

    spacing: 10

    UiSettings { id: uiSettings }

    Column {
        width: parent.width

        ComboBox {
            width: parent.width

            model: ListModel {
                ListElement {
                    text: qsTr("Single-Window layout")
                    description: qsTr("Views are shown in a single window \
                                       side by side")
                    value: UiSettings.Split
                }
                ListElement {
                    text: qsTr("Slim Layout")
                    description: qsTr("Slim layout meant for small screens \
                                       like in pocket-sized devices.")
                    value: UiSettings.Slim
                }
            }

            onActivated: {
                var item = model.get(index);
                uiSettings.set_app_layout(item.value);
            }

            Component.onCompleted: {
                var v = uiSettings.app_layout_int();
                for(var i = 0; i < count; i++) {
                    if (parseInt(model.get(i).value) === v) {
                        currentIndex = i;
                        break;
                    }
                }
            }
        }

        Controls.Text {
            text: qsTr("Base Color");
        }

        Controls.Slider {
            width: parent.width

            text: qsTr("Hue")
            value: Style.color.base.hslHue

            onValueChanged: {
                Style.color.base.hslHue = value
            }
        }

        Controls.Slider {
            width: parent.width

            text: qsTr("Saturation")
            value: Style.color.base.hslSaturation

            onValueChanged: {
                Style.color.base.hslSaturation = value
            }
        }

        Controls.Slider {
            width: parent.width

            text: qsTr("Lightness");
            from: 0.1
            to: 0.9
            value: Style.color.base.hslLightness

            onValueChanged: {
                Style.color.base.hslLightness = value;
            }
        }
    }

    CheckBox {
        style: CheckBoxStyle {
            label: Controls.Text {
                text: qsTr("Light Theme")
            }
        }

        checked: Style.lightTheme

        onCheckedChanged: {
            Style.lightTheme = checked;
        }
    }
}
