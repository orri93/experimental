import QtQuick 2.14
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4

Dialog  {
  id: sandboxDialog

  property string titleText: qsTr("Sandbox Settings")

  title: titleText

  height: 300
  width: 600

  standardButtons: StandardButton.Ok | StandardButton.Cancel

  SandboxTabView {
    id: sandboxTabView
  }

  onVisibleChanged: {
    console.log("Dialog visible " + visible)
  }

  onAccepted: {
    console.log("Saving the date ")
  }

 }
