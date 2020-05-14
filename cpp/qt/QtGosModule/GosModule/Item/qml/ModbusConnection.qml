import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

GroupBox {
  property alias serialPort: serialPortComboBox
  property alias serialBaud: serialBaudComboBox
  property alias connectDisconnect: connectDisconnectButton

  property color labelTextColor: "white"
  property color informationTextColor: "burlywood"

  property int labelFontPointSize:  8
  property int informationFontPointSize: 10

  property real labelLeftMargin: 2.0
  property real informationTextLeftMargin: 5.0

  title: qsTr("Connection")

  GridLayout {
    columns: 2
    rows: 3

    ColumnLayout {
      Layout.column: 0
      Layout.row: 0
      Label {
        text: qsTr("Serial Port")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      ComboBox {
        id: serialPortComboBox
        Layout.fillWidth: true
        Layout.column: 0
        Layout.row: 0
      }
    }

    ColumnLayout {
      Layout.column: 1
      Layout.row: 0

      Label {
        text: qsTr("Serial Baud")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
        Layout.column: 1
        Layout.row: 0
      }
      ComboBox {
        id: serialBaudComboBox
        Layout.fillWidth: true
        Layout.column: 1
        Layout.row: 0
        model: serialBaudModel
      }
    }

    Button {
      id: connectDisconnectButton
      Layout.fillWidth: true
      Layout.column: 0
      Layout.row: 1
      text: qsTr("Connect")
    }

    ColumnLayout {
      Layout.column: 1
      Layout.row: 1

      Label {
        text: qsTr("Status")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
        Layout.column: 1
        Layout.row: 0
      }
      Text {
        id: connectionStatusText
        text: qsTr("Starting")
        color: informationTextColor
        Layout.leftMargin: informationTextLeftMargin
        font.pointSize: informationFontPointSize
        Layout.fillWidth: true
      }
    }
  }
}


