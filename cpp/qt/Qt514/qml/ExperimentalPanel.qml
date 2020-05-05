import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import FloatValidator 1.0

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

  FloatValidator {
    id: floatValidator
  }

  Component.onCompleted: {
    backend.panelCompleted();
  }

  GridLayout {
    Layout.alignment: Qt.AlignTop
    Layout.fillWidth: true
    Layout.fillHeight: true
    columns: 2
    rows: 4

    ColumnLayout {
      id: titelColumn
      Layout.fillWidth: true
      Layout.columnSpan: 2
      Layout.column: 0
      Layout.row: 0

      Text {
        text: titleText
        color: titleTextColor
        //anchors.top: parent.top
        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: false
        Layout.topMargin: titleTopMargin
        Layout.leftMargin: titleLeftMargin
        font.pointSize: titleFontPointSize
      }
    }

    ColumnLayout {
      id: textColumn
      Layout.fillWidth: true
      Layout.columnSpan: 2
      Layout.column: 0
      Layout.row: 1
      Label {
        text: qsTr("Text")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      TextInput {
        id: textInput
        Layout.leftMargin: inputTextLeftMargin
        Layout.fillWidth: true
        color: inputTextColor
        font.pointSize: inputFontPointSize
        validator: floatValidator
        onEditingFinished: {
          backend.text = text
        }
      }
      Connections {
        target: backend
        onTextChanged: {
          textInput.text = backend.text
        }
      }
    }

    ColumnLayout {
      id: intervalColumn
      Layout.fillWidth: true
      Layout.column: 0
      Layout.row: 2
      Label {
        text: qsTr("Interval")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      ComboBox {
        id: intervalComboBox
        Layout.fillWidth: true
        model: intervalModel
        onCurrentIndexChanged: {
          backend.intervalIndex = currentIndex;
        }
      }
    }

    ColumnLayout {
      Layout.fillWidth: true
      Layout.column: 1
      Layout.row: 2
      Label {
        text: qsTr("Interval")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      Text {
        id: intervalText
        color: informationTextColor
        Layout.leftMargin: informationTextLeftMargin
        font.pointSize: informationFontPointSize
        Layout.minimumWidth: 130
      }
    }

    ColumnLayout {
      Layout.fillWidth: true
      Layout.columnSpan: 2
      Layout.column: 0
      Layout.row: 3
      Label {
        text: qsTr("Status")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      Text {
        id: statusText
        text: qsTr("Loading")
        color: informationTextColor
        Layout.leftMargin: informationTextLeftMargin
        font.pointSize: informationFontPointSize
        Layout.minimumWidth: 130
      }
    }

    Connections {
      target: backend
      onIntervalChanged: {
        intervalText.text = backend.interval.toString();
        intervalComboBox.currentIndex = backend.intervalIndex;
      }
    }

    Connections {
      target: backend
      onCompleted: {
        statusText.text = "Backend Completed";
      }
    }


  }
}
