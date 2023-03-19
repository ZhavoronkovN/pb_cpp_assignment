import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    PErrorDialog { id : error_dialog }
    Column{
        width: parent.width
        padding: 10
        spacing: 20
        Row{
            spacing: 20
            height : 40
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                verticalAlignment: TextEdit.AlignVCenter
                height: parent.height
                text: qsTr("Current directory : ")
            }

            Rectangle {
                width: 400
                height: parent.height
                border.width : 1
          TextEdit {
                anchors.leftMargin : 10
                anchors.rightMargin : 10
                anchors.fill : parent
                verticalAlignment: TextEdit.AlignVCenter
                id : edit_path
                text: qsTr(start_path)
                
            }
            }
  

            PButton {
                height: parent.height
                onClicked : () => {Manager.refresh(edit_path.text)}
                text : "Refresh"
            }
        }
        Row {
                spacing: 20
                height : 30
                width:parent.width
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    height: parent.height
                    text: qsTr("File name")
                    width: parent.width * 0.3
                }
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr("Type")
                    width: parent.width * 0.05
                }
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr("Ext")
                    width: parent.width * 0.05
                }
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr("File size")
                    width: parent.width * 0.1
                }
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr("Action")
                    width: parent.width * 0.15
                }
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr("Status")
                    width: parent.width * 0.15
                }

            }
        Column {
            width : parent.width
        ListView {
            height : 400
            width:parent.width
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter
            model: Manager.items
            delegate: Row {
                spacing: 20
                height : 30
                width:parent.width
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    height: parent.height
                    text: qsTr(modelData.name)
                    width: parent.width * 0.3
                }
                Text {
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr(modelData.is_file?"File":"Not a file")
                    width: parent.width * 0.05
                }
                Text {
                    visible: modelData.is_file
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr(modelData.ext)
                    width: parent.width * 0.05
                }
                Text {
                    visible: modelData.is_file
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr(Math.round(modelData.size/1024) + "KB")
                    width: parent.width * 0.1
                }
                PButton {
                    visible : modelData.ext === ".barch"
                    height: parent.height
                    onClicked : () => {Manager.decode(modelData.full_path)}
                    text : "Decode"
                    width: parent.width * 0.15
                }
                PButton {
                    visible: modelData.ext === ".bmp"
                    height: parent.height
                    onClicked : () => {Manager.encode(modelData.full_path)}
                    text : "Encode"
                    width: parent.width * 0.15
                }
                Text {
                    id : wait_text
                    visible: modelData.in_work
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: parent.height
                    text: qsTr((modelData.ext === ".bmp"?"Encoding":"Decoding") +", please wait...")
                    width: parent.width * 0.15
                }

            }
        }
        }
    }
}
