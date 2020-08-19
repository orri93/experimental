import QtQuick 2.14
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4

Dialog  {
  id: uiDialog

  property string titleText: qsTr("Ui Settings")

  title: titleText

  height: 500
  width: 680

  standardButtons: StandardButton.Ok | StandardButton.Cancel

  UiTabView {
    id: uiTabView
  }

  onAccepted: {
    console.log("Saving the date ")
    backend.applyUiDialog();
  }

  onRejected: {
    console.log("Rejecting the date ")
    backend.rejectUiDialog();
  }
}
