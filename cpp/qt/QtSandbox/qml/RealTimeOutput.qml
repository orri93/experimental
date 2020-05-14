import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import QtQuick.Extras 1.4

GroupBox {

  property string groupTitle: qsTr("Output")

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

  GridLayout {
    columns: 2
    rows: 1

    ColumnLayout {
      id: tempertureColumn
      Layout.fillWidth: true
      Layout.column: 0
      Layout.row: 0
      Label {
        text: qsTr("Temperture")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      Gauge {
        id: tempertureGauge
        maximumValue: 100.00
      }
    }

    ColumnLayout {
      id: outputColumn
      Layout.fillWidth: true
      Layout.column: 1
      Layout.row: 0
      Label {
        text: qsTr("Output")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      Gauge {
          id: outputGauge
      }
    }
  }
}
