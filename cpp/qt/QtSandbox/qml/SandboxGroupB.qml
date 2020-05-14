import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

GroupBox {

  property string groupTitle: qsTr("Group B")

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

  ColumnLayout {
    SandboxRange {
      rangeMaximum: 10.0
      rangeStep: 0.1
      id: rangeA
    }
    SandboxRange {
      rangeMaximum: 1.0
      rangeStep: 0.01
      id: rangeB
    }
  }

}
