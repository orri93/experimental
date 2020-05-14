import QtQuick 2.1
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

ColumnLayout {
  property string sandboxTitle: qsTr("Real Spin Box")

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

  GroupBox {

    title: sandboxTitle + " Settings"

    GridLayout {
      columns: 2
      rows: 2

      ColumnLayout {
        Layout.column: 0
        Layout.row: 0
        Label {
          text: qsTr("Value")
        }

        RowLayout {
          Slider {
            to: 100.0
            stepSize: 1.0
            onValueChanged: {
              realValueText.text = value.toString();
              controlRealSpinBox.realValue = value;
            }
          }
          Text {
            id: realValueText
            text: qsTr("0")
          }
        }
      }

      ColumnLayout {
        Layout.column: 1
        Layout.row: 0
        Label {
          text: qsTr("To")
        }

        RowLayout {
          Slider {
            to: 300.0
            stepSize: 10.0
            onValueChanged: {
              realToText.text = value.toString();
              controlRealSpinBox.realTo = value;
            }
          }
          Text {
            id: realToText
            text: qsTr("0")
          }
        }
      }

      ColumnLayout {
        Layout.column: 1
        Layout.row: 1
        Label {
          text: qsTr("Step Size")
        }

        RowLayout {
          Slider {
            to: 0.1
            stepSize: 0.001
            onValueChanged: {
              realStepSizeText.text = value.toString();
              controlRealSpinBox.realStepSize = value;
            }
          }
          Text {
            id: realStepSizeText
            text: qsTr("0")
          }

        }
      }

      ColumnLayout {
        Layout.column: 0
        Layout.row: 1
        Label {
          text: qsTr("Decimals")
        }

        RowLayout {
          Slider {
            to: 4
            stepSize: 1
            onValueChanged: {
              realDecimalsText.text = value.toString();
              controlRealSpinBox.realDecimals = value;
            }
          }
          Text {
            id: realDecimalsText
            text: qsTr("0")
          }

        }
      }

    }
  }

  GroupBox {

    title: sandboxTitle

    ColumnLayout {
      Label {
        text: qsTr("Real Spin Box")
        color: labelTextColor
        Layout.leftMargin: labelLeftMargin
        font.pointSize: labelFontPointSize
      }
      RowLayout {
        ControlRealSpinBox {
          id: controlRealSpinBox
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

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
