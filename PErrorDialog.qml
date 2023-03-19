import QtQuick 2.15
import QtQuick.Window 2.15
Window {
    id : root
    modality: Qt.WindowModal
    width : 100
    height : 40
    Column {
        padding : 20
        spacing: 20
        anchors.fill : parent
    Text {
        verticalAlignment: TextEdit.AlignVCenter
        height : 30
        width : parent.width
        wrapMode: Text.WordWrap
        text: qsTr("Oops, something bad happened. Please restart application and start again")
    }
    PButton {
        height: 30
        text : "Ok"
        onClicked : () => {root.close()}
    }
    }
}
