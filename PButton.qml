import QtQuick 2.15
Rectangle {
 id : root
 property var onClicked : () => console.log("Clicked")
 property alias text : text.text
 height: 30
 width: height * 2
 color: "lightblue"
 anchors.margins: 10
 MouseArea {
     id : sensor
     anchors.fill: parent
        onClicked: root.onClicked()
        Text {
            id : text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
}
}
