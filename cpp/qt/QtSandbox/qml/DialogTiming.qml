import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

GroupBox {
  property string groupTitle: qsTr("Timing")

  property color groupTextColor: "gray"
  property color labelTextColor: "white"
  property color inputTextColor: "beige"
  property color informationTextColor: "burlywood"
  property color unitTextColor: "cornsilk"

  property int labelFontPointSize:  8
  property int inputFontPointSize: 10
  property int informationFontPointSize: 10
  property int unitFontPointSize: 8

  property real labelLeftMargin: 2.0
  property real inputTextLeftMargin: 5.0
  property real informationTextLeftMargin: 5.0

  property real groupTitleLocation: 10.0

  property real preferredWidth: 140.0

  label: Label {
    x: groupTitleLocation
    text: groupTitle
    color: groupTextColor
  }

  RowLayout {

    ColumnLayout {
      Label {
        text: qsTr("Stable duration")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      RowLayout {
        SpinBox {
          id: stableDurationSpinBox
        }
        Label {
          text: qsTr("minutes")
          color: unitTextColor
          font.pointSize: unitFontPointSize
        }
      }
    }
  }
}
