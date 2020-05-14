import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

Column {
      spacing: 10

      CheckBox {
          text: "Breakfast"
          checked: true
      }
      CheckBox {
          text: "Lunch"
          checked: false
      }
      CheckBox {
          text: "Dinner"
          checked: true
      }

      ColumnLayout {
            id: baseColumn

            Label {
              text: qsTr("Base")
              color: labelTextColor
              Layout.leftMargin: labelLeftMargin
              font.pointSize: labelFontPointSize
            }
            RowLayout {
              SpinBox {
                id: baseInput
                editable: true
                Layout.fillWidth: true
                onValueChanged: {
                  if(blackBoxSettings) {
                    blackBoxSettings.base = value;
                  }
                }
              }
              CheckBox {
                id: baseCheckBox
                checked: true
                onCheckedChanged: {

                }
              }
            }
          }
  }
