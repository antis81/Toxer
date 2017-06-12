import QtQuick 2.0

Loader {
    id: root

    property url defaultView: ""

    width: 100
    height: 100

    focus: true

    onSourceChanged: {
        if (!root.source) {
            root.source = defaultView
        }
    }

    onItemChanged: {
        if (item && typeof(item.viewLoader) !== "undefined") {
            item.viewLoader = root;
        }
    }

    Component.onCompleted: {
        root.source = root.defaultView;
    }
}
