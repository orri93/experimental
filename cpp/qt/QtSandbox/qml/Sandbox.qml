import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4

Dialog  {
  property string titleText: qsTr("Black Box Settings")

  title: titleText

  width: 800
  height: 480

  property color dialogColor: "#404040"


  standardButtons: StandardButton.Ok | StandardButton.Cancel

  onAccepted: {
    console.log("Saving the date ")
  }

  contentItem: Rectangle {
    anchors.fill: parent
    color: dialogColor
    implicitWidth: 400
    implicitHeight: 100

    GroupBox {
      title: "Testing"
      Column {
        spacing: 20
        Label {
          text: "Label"
        }
      }
    }

    GridLayout {

      columns: 1
      rows: 4

      Text {
        text: titleText
        color: titleTextColor
        Layout.column: 0
        Layout.row: 0
        Layout.fillWidth: false
        Layout.leftMargin: titleLeftMargin
        Layout.topMargin: titleVerticalMargin
        Layout.bottomMargin: titleVerticalMargin
        Layout.alignment: Qt.AlignTop
        font.pointSize: titleFontPointSize
      }

    }
  }
}


