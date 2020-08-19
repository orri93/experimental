import QtQuick 2.4

QuickUiExampleForm {
  anchors.fill: parent
  button.onClicked: messageDialog.show(qsTr("Button pressed"))
}
