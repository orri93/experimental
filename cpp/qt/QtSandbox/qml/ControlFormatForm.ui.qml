import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

import assets 1.0

ColumnLayout {

  property alias formatName: formatNameLabel.text
  property alias precisionBox: precisionSpinBox

  Label {
    id: formatNameLabel
    color: Style.dialogLabelTextColor
    Layout.leftMargin: Style.labelLeftMargin
    font.pointSize: Style.labelFontPointSize
  }

  RowLayout {
    Label {
      text: qsTr("Precision:")
      color: Style.dialogLabelTextColor
      Layout.leftMargin: Style.labelLeftMargin
      font.pointSize: Style.labelFontPointSize
    }

    SpinBox {
      id: precisionSpinBox
    }
  }
}
