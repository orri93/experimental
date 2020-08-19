import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

import assets 1.0

GroupBox {

  property alias integralControl: integralControlFormat

  property string groupTitle: qsTr("Outputs")

  label: Label {
    x: Style.groupTitleLocation
    text: groupTitle
    color: Style.dialogGroupTextColor
  }

  ColumnLayout {
    ControlFormat {
      id: integralControlFormat
      formatName: qsTr("Integral")
    }
  }

}
