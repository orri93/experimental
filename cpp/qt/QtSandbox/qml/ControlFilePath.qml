import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0


ColumnLayout {

  property string fileNameText: qsTr("file")

  property string filePath: filePathTextEdit.text

  property real fileTextInputWidth: 320.0

  property real browseButtonWidth: 160.0
  property real browseButtonHeight: 120.0


  FileDialog {
    id: fileDialog
    visible: false
    height: 600
    width: 800
    defaultSuffix: "log"
    title: "Please choose a " + fileNameText
    onAccepted: {
      filePathTextEdit.text = fileDialog.fileUrl;
      console.log("You chose: " + fileDialog.fileUrl)
      Qt.quit()
    }
    onRejected: {
      console.log("Canceled")
      Qt.quit()
    }
  }

  RowLayout {
    TextEdit {
      id: filePathTextEdit
      Layout.preferredWidth: fileTextInputWidth
      Layout.fillHeight: true
    }
    Button {
      text: qsTr("Browse")
      width: browseButtonWidth
      height: browseButtonHeight
      onClicked: {
        fileDialog.open();
      }
    }
  }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
