import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

ColumnLayout {
  id: qtExperimentalPanel

  Layout.fillWidth: true
  Layout.fillHeight: true

  property string titleText: qsTr("Qt Experimental")

  property color titleTextColor: qsTr("floralwhite")
  property color labelTextColor: qsTr("white")
  property color inputTextColor: qsTr("beige")
  property color informationTextColor: qsTr("burlywood")

  property int titleFontPointSize: 18
  property int labelFontPointSize:  8
  property int inputFontPointSize: 10
  property int informationFontPointSize: 10

  property real titleTopMargin: 10.0
  property real titleLeftMargin: 10.0
  property real labelLeftMargin: 2.0
  property real inputTextLeftMargin: 5.0
  property real informationTextLeftMargin: 5.0

  GridLayout {
    columns: 2
    rows: 2

    ColumnLayout {
      id: titelColumn
      Layout.fillWidth: true
      Layout.columnSpan: 2
      Layout.column: 0
      Layout.row: 0

      Text {
        text: titleText
        color: titleTextColor
        anchors.top: parent.top
        Layout.fillWidth: false
        Layout.topMargin: titleTopMargin
        Layout.leftMargin: titleLeftMargin
        font.pointSize: titleFontPointSize
      }
    }

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
        onEditingFinished: {
          orchestration.kpText = text
        }
      }
      Connections {
        target: orchestration
        onKpChanged: {
          kpInput.text = orchestration.kpText
        }
      }
    }
  }
}
