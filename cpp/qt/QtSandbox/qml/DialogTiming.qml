import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

GroupBox {
  property string groupTitle: qsTr("Timing")

  label: Label {
    x: DialogStyle.groupTitleLocation
    text: groupTitle
    color: DialogStyle.groupTextColor
  }

  RowLayout {

    ColumnLayout {
      Label {
        text: qsTr("Stable duration")
        color: DialogStyle.labelTextColor
        Layout.leftMargin: DialogStyle.labelLeftMargin
        font.pointSize: DialogStyle.labelFontPointSize
      }
      RowLayout {
        SpinBox {
          id: stableDurationSpinBox
        }
        Label {
          text: qsTr("minutes")
          color: DialogStyle.unitTextColor
          font.pointSize: DialogStyle.unitFontPointSize
        }
      }
    }
  }
}
