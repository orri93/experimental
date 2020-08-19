import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

import Pid.Toolkit.Assets 1.0

ColumnLayout {
    RowLayout {
        ColumnLayout {
            Label {
                text: "Serial Port"
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }

            ComboBox {
                id: serialPortComboBox
            }
        }

        ColumnLayout {
            Label {
                text: "Baud Rate"
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }

            ComboBox {
                id: serialBaudComboBox
            }
        }

        ColumnLayout {
            Label {
                text: "Slave ID"
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }

            SpinBox {
                from: 0
                to: 255
            }
        }
    }
    RowLayout {
        ColumnLayout {
            Label {
                text: "Data Bits"
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }

            ComboBox {
                id: dataBitsComboBox
                model: [8, 7]
            }
        }

        ColumnLayout {
            Label {
                text: "Stop Bit"
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }

            ComboBox {
                id: stopBitComboBox
                model: [1, 1.5, 2]
            }
        }

        ColumnLayout {
            Label {
                text: "Slave ID"
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }

            SpinBox {
                from: 0
                to: 255
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/

