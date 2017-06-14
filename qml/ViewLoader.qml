import QtQuick 2.0

Loader {
    id: root

    property url defaultView: ""

    width: 100
    height: 100

    focus: true

    onItemChanged: {
        if (item) {
            if (typeof(item.viewLoader) !== "undefined") {
                item.viewLoader = root;
            }
        } else if (defaultView){
            root.source = root.defaultView
        }
    }

    Component.onCompleted: {
        root.source = root.defaultView;
    }
}
