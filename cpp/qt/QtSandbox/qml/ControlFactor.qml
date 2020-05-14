import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

RowLayout {

  property string name: qsTr("Name")

  property color labelTextColor: "white"
  property color inputTextColor: "beige"
  property color inputTextErrorColor: "red"

  property int labelFontPointSize:  8
  property int inputFontPointSize: 10

  property real labelLeftMargin: 2.0
  property real inputTextLeftMargin: 5.0

  property real preferredNumberInputWidth: 60.0

  DoubleValidator {
    id: doubleValidator
  }

  ColumnLayout {
    Label {
      text: name
      color: labelTextColor
      Layout.leftMargin: labelLeftMargin
      font.pointSize: labelFontPointSize
    }
    RowLayout {
      CheckBox {
        text: qsTr("Factor:")
      }
      TextInput {
        Layout.preferredWidth: preferredNumberInputWidth
        Layout.leftMargin: inputTextLeftMargin
        color: acceptableInput ? inputTextColor : inputTextErrorColor
        font.pointSize: inputFontPointSize
        validator: floatValidator
      }
      ComboBox {
        id: factorOperation
        model: ["Add", "Subtract"]
      }
      CheckBox {
        text: qsTr("Reverse:")
      }
      TextInput {
        Layout.preferredWidth: preferredNumberInputWidth
        Layout.leftMargin: inputTextLeftMargin
        color: acceptableInput ? inputTextColor : inputTextErrorColor
        font.pointSize: inputFontPointSize
        validator: floatValidator
      }
    }
  }
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
