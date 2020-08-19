import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

import assets 1.0

ColumnLayout {

    property alias accuracyName: accuracyNameLabel.text
    property alias restrictionBox: restrictionComboBox
    property alias precisionBox: precisionSpinBox
    property alias minimumRow: minimumRowLayout
    property alias maximumRow: maximumRowLayout
    property alias minimumInput: minimumTextInput
    property alias maximumInput: maximumTextInput

    Label {
        id: accuracyNameLabel
        color: Style.dialogLabelTextColor
        Layout.leftMargin: Style.labelLeftMargin
        font.pointSize: Style.labelFontPointSize
    }

    RowLayout {
        Label {
            text: qsTr("Restriction:")
            color: Style.dialogLabelTextColor
            Layout.leftMargin: Style.labelLeftMargin
            font.pointSize: Style.labelFontPointSize
        }
        ComboBox {
            id: restrictionComboBox
        }

        Label {
            text: qsTr("Precision:")
            color: Style.dialogLabelTextColor
            Layout.leftMargin: Style.labelLeftMargin
            font.pointSize: Style.labelFontPointSize
        }

        SpinBox {
            id: precisionSpinBox
        }

        DoubleValidator {
            id: doubleValidator
        }

        RowLayout {
            id: minimumRowLayout
            visible: false
            Label {
                text: qsTr("Minimum:")
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }
            TextInput {
                id: minimumTextInput
                validator: doubleValidator
                Layout.preferredWidth: Style.rangeMaximumWidth
            }
        }

        RowLayout {
            id: maximumRowLayout
            visible: false
            Label {
                text: qsTr("Maximum:")
                color: Style.dialogLabelTextColor
                Layout.leftMargin: Style.labelLeftMargin
                font.pointSize: Style.labelFontPointSize
            }
            TextInput {
                id: maximumTextInput
                validator: doubleValidator
                Layout.preferredWidth: Style.rangeMaximumWidth
            }
        }
    }
}
