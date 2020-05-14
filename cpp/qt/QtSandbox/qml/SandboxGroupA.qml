import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

GroupBox {

  property string groupTitle: qsTr("Timing")

  signal applyToControllerChanged(bool value)

  property color groupTextColor: "navy"
  property color labelTextColor: "midnightblue"

  property int labelFontPointSize:  8
  property int informationFontPointSize: 10

  property real labelLeftMargin: 2.0
  property real informationTextLeftMargin: 5.0

  property real groupTitleLocation: 10.0

  property real preferredWidth: 140.0

  label: Label {
    x: groupTitleLocation
    text: groupTitle
    color: groupTextColor
  }

  function setApplyToController(value) {
    intervalApplyCheckBox.checked = value;
  }

  GridLayout {
    columns: 2
    rows: 1

    ColumnLayout {
      id: intervalColumn
      Layout.fillWidth: true
      Layout.column: 0
      Layout.row: 0
      Label {
        text: qsTr("Interval")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      ComboBox {
        id: intervalComboBox
        Layout.fillWidth: true
        onCurrentIndexChanged: {
          intervalIndexChanged(currentIndex);
        }
      }
    }

    ColumnLayout {
      id: intervalApplyColumn
      Layout.fillWidth: true
      Layout.column: 1
      Layout.row: 0
      Label {
        text: qsTr("Apply to controller")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      CheckBox {
        id: intervalApplyCheckBox
        checked: true
        onCheckedChanged: {
          applyToControllerChanged(checked);
        }
      }
    }
    SpinBox {
      id: spinBox
      antialiasing: false
      enabled: true
      value: 12
    }
  }
}
