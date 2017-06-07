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
import QtQuick.Window 2.0

import "animations" as Animations
import "base" as Base
import "controls" as Controls

Base.Page {
    id: root

    width: Math.min(600, Screen.width)
    height: Math.min(400, Screen.height)

    RowLayout {
        id: pageLayout

        anchors.fill: parent
        spacing: 0

        ColumnLayout {
            Layout.minimumWidth: 140
            Layout.maximumWidth: 240
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 0

            Item {
                // spacer
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Image {
                id: logo

                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: parent.width / 1.8
                Layout.preferredHeight: Layout.preferredWidth
                Layout.maximumWidth: 120
                Layout.maximumHeight: Layout.maximumWidth

                sourceSize: Qt.size(width, height)
                source: "qrc:/res/images/dark/login_logo.svg"
            }

            Item {
                // spacer
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ListView {
                id: pageSelector

                property real spikeWidth: 36

                Layout.fillWidth: true
                height: 48 * count
                spacing: 0
                interactive: false

                model: ListModel {
                    ListElement {
                        name: qsTr("Create Profile")
                        page: "CreateProfile.qml"
                    }
                    ListElement {
                        name: qsTr("Load Profile")
                        page: "SelectProfile.qml"
                    }
                }

                delegate: Text {
                    height: font.pixelSize + 36
                    width: pageSelector.width

                    renderType: Text.NativeRendering
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    color: "#bbb"
                    text: name

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            pageSelector.currentIndex = index
                        }
                    }
                }

                focus: false
                highlightFollowsCurrentItem: false
                highlight: Canvas {
                    width: pageSelector.currentItem.width +
                           pageSelector.spikeWidth
                    height: pageSelector.currentItem.height
                    onPaint: {
                        var bgColor = Qt.lighter(color, 1.8);
                        var borderColor = "transparent";
                        var cy = height / 2;

                        var ctx = getContext("2d");
                        ctx.save();

                        ctx.beginPath();
                        ctx.moveTo(0,0);
                        ctx.lineTo(width - pageSelector.spikeWidth,
                                   ctx.lineWidth);
                        ctx.lineTo(width, cy);
                        ctx.lineTo(width - pageSelector.spikeWidth,
                                   height - ctx.lineWidth);
                        ctx.strokeStyle = borderColor;
                        ctx.lineTo(ctx.lineWidth, height - ctx.lineWidth);
                        ctx.lineTo(ctx.lineWidth, ctx.lineWidth);
                        ctx.stroke();
                        var grd = ctx.createLinearGradient(0, 0, width * 0.8,
                                                           0);
                        grd.addColorStop(0, "transparent");
                        grd.addColorStop(1, bgColor)
                        ctx.fillStyle = grd;
                        ctx.fill();

                        ctx.restore();
                    }

                    y: pageSelector.currentItem.y

                    Behavior on y {
                        NumberAnimation { duration: 140 }
                    }
                }

                onCurrentIndexChanged: {
                    var item = model.get(currentIndex);
                    pageLoader.setSource(item.page);
                }

                Component.onCompleted: {
                    currentIndex = 1
                }
            }

            Column {
                Layout.fillWidth: true

                Text {
                    id: txtToxerVersion

                    width: parent.width

                    renderType: Text.NativeRendering
                    color: "#bbb"
                    maximumLineCount: 1
                    elide: Text.ElideRight
                    text: Qt.application.name + " version " +
                          Qt.application.version
                }

                Text {
                    id: txtToxcoreVersion

                    width: parent.width

                    renderType: Text.NativeRendering
                    color: "#bbb"
                    maximumLineCount: 1
                    elide: Text.ElideRight
                    text: "Tox version " + Toxer.toxVersionString()
                }
            }
        }

        Loader {
            id: pageLoader

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 9
        }

        Animations.Ringing {
            target: logo
            duration: 100
            loops: 12
            running: true
        }
    }
}
