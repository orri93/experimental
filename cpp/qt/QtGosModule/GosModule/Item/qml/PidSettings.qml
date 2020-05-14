import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import FloatValidator 1.0

GroupBox {
  property alias kp: kpInput
  property alias integral: integralText
  property alias kiInput: kiInput
  property alias kdInput: kdInput

  property string realPlaceholderText: qsTr("0.0")

  property color labelTextColor: "white"
  property color inputTextColor: "beige"
  property color informationTextColor: "burlywood"

  property int labelFontPointSize:  8
  property int inputFontPointSize: 10
  property int informationFontPointSize: 10

  property real labelLeftMargin: 2.0
  property real inputTextLeftMargin: 5.0
  property real informationTextLeftMargin: 5.0

  title: qsTr("PID")

  FloatValidator {
    id: floatValidator
  }

  GridLayout {
    columns: 2
    rows: 2

    ColumnLayout {
      id: kpColumn
      Layout.fillWidth: true
      Layout.column: 0
      Layout.row: 0
      Label {
        text: qsTr("Kp")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      TextInput {
        id: kpInput
        Layout.leftMargin: inputTextLeftMargin
        Layout.fillWidth: true
        color: inputTextColor
        font.pointSize: inputFontPointSize
        validator: floatValidator
      }
    }

    ColumnLayout {
      id: integralColumn
      Layout.fillWidth: true
      Layout.column: 1
      Layout.row: 0
      Label {
        text: qsTr("Integral")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      Text {
        id: integralText
        text: realPlaceholderText
        color: informationTextColor
        Layout.leftMargin: informationTextLeftMargin
        font.pointSize: informationFontPointSize
        Layout.fillWidth: true
      }
    }

    ColumnLayout {
      id: kiColumn
      Layout.fillWidth: true
      Layout.column: 0
      Layout.row: 1
      Label {
        text: qsTr("Ki")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      TextInput {
        id: kiInput
        Layout.leftMargin: inputTextLeftMargin
        Layout.fillWidth: true
        color: inputTextColor
        font.pointSize: inputFontPointSize
        validator: floatValidator
      }
    }

    ColumnLayout {
      id: kdColumn
      Layout.fillWidth: true
      Layout.column: 1
      Layout.row: 1
      Label {
        text: qsTr("Kd")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      TextInput {
        id: kdInput
        Layout.leftMargin: inputTextLeftMargin
        Layout.fillWidth: true
        color: inputTextColor
        font.pointSize: inputFontPointSize
        validator: floatValidator
      }
    }
  }
}


