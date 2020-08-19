import QtQuick 2.14
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Tab {
  id: uitm

  width: parent.width
  height: parent.height

  title: qsTr("Ui")

  ColumnLayout {
    DialogUiInput {
      id: uiInputItems
    }
    DialogUiOutput {
      id: uiOutputItems
    }
    DialogUiPid {
      id: uiPidItems
    }
  }
}
