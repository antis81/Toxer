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

import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

import com.tox.qmlcomponents 1.0
import com.tox.qmltypes 1.0
import com.toxer.settings 1.0

import ".." // QTBUG-34418
import "../controls" as Controls

Page {
    id: root

    readonly property url defaultView: "FriendsView.qml"
    property alias toxProfile: toxProfile
    property alias uiSettings: uiSettings

    ToxProfileQuery {
        id: toxProfile

        function statusIcon() {
            if (isOnline()) {
                switch (statusInt()) {
                case ToxTypes.Unknown:
                case ToxTypes.Away: return Style.icon.away;
                case ToxTypes.Busy: return Style.icon.busy;
                case ToxTypes.Ready: return Style.icon.online;
                }

                return Style.icon.away;
            } else {
                return Style.icon.offline;
            }
        }
    }

    UiSettings {
        id: uiSettings

        onAppLayoutChanged: {
            Toxer.reloadUi();
        }
    }
}
