import QtQuick 2.0

import ".." // QTBUG-34418

Text {
    renderType: Text.NativeRendering
    maximumLineCount: 1
    wrapMode: Text.NoWrap
    textFormat: Text.PlainText
    elide: Text.ElideRight
    color: Style.color.text
    font.pointSize: Style.fontPointSize
}
