import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

ColumnLayout {
  id: numberControlRowLayout

  property alias numberName: numberNameLabel.text

  property var numberObject: null

  signal numberMinimumChanged(real minimum)
  signal numberMaximumChanged(real minimum)
  signal numberPrecisionChanged(real value)
  signal numberStepSizeChanged(real minimum)

  signal numberChanged(var number)

  property color groupTextColor: "navy"
  property color labelTextColor: "midnightblue"
  property color inputTextColor: "darkslateblue"
  property color inputTextErrorColor: "red"

  property int labelFontPointSize:  8
  property int inputFontPointSize: 10

  property real labelLeftMargin: 2.0
  property real inputTextLeftMargin: 5.0

  property real preferredNumberInputWidth: 60.0

  function setNumberObject(number) {
    if(number) {
      numberControlRowLayout.numberObject = number;
      if(numberControlRowLayout.numberObject.isLimit) {
        setNumberMinimum(numberControlRowLayout.numberObject.minimum);
        setNumberMaximum(numberControlRowLayout.numberObject.maximum);
      }
      setNumberPrecision(numberControlRowLayout.numberObject.precision);
      setNumberStepSize(numberControlRowLayout.numberObject.stepSize);
      resolveUserInterface();
    } else {
      console.log("Setting undefined number for " +
        numberControlRowLayout.numberName);
    }
    return number;
  }

  function setNumberMinimum(minimum) {
    numberMinimumTextInput.text = minimum.toString();
  }
  function setNumberMaximum(maximum) {
    numberMaximumTextInput.text = maximum.toString();
  }
  function setNumberPrecision(precision) {
  }
  function setNumberStepSize(stepSize) {
  }

  DoubleValidator {
    id: doubleValidator
  }

  Label {
    id: numberNameLabel
    color: labelTextColor
    Layout.leftMargin: labelLeftMargin
    font.pointSize: labelFontPointSize
  }
  RowLayout {

    Label {
      id: numberMinimumLabel
      text: qsTr("Minimum: ")
      color: labelTextColor
      Layout.leftMargin: labelLeftMargin
      font.pointSize: labelFontPointSize
    }
    TextInput {
      id: numberMinimumTextInput
      Layout.preferredWidth: preferredNumberInputWidth
      Layout.leftMargin: inputTextLeftMargin
      color: acceptableInput ? inputTextColor : inputTextErrorColor
      font.pointSize: inputFontPointSize
      validator: doubleValidator
      onEditingFinished: {
        if(numberControlRowLayout.numberObject) {
          numberMinimumChanged(numberMinimumTextInput.text);
        }
      }
    }

    Label {
      id: numberMaximumLabel
      text: qsTr("Maximum: ")
      color: labelTextColor
      Layout.leftMargin: labelLeftMargin
      font.pointSize: labelFontPointSize
    }
    TextInput {
      id: numberMaximumTextInput
      Layout.preferredWidth: preferredNumberInputWidth
      Layout.leftMargin: inputTextLeftMargin
      color: acceptableInput ? inputTextColor : inputTextErrorColor
      font.pointSize: inputFontPointSize
      validator: doubleValidator
      onEditingFinished: {
        if(numberControlRowLayout.numberObject) {
          numberMaximumChanged(numberMaximumTextInput.text);
        }
      }
    }

    Label {
      id: numberPrecisionLabel
      text: qsTr("Precision: ")
      color: labelTextColor
      Layout.leftMargin: labelLeftMargin
      font.pointSize: labelFontPointSize
    }

    SpinBox {
      id: numberPrecisionSpinBox
      onValueChanged: {
        if(numberControlRowLayout.numberObject) {
          numberPrecisionChanged(numberPrecisionSpinBox.value);
        }
      }
    }

    Label {
      id: numberStepSizeLabel
      text: qsTr("Step Size: ")
      color: labelTextColor
      Layout.leftMargin: labelLeftMargin
      font.pointSize: labelFontPointSize
    }
    TextInput {
      id: numberStepSizeTextInput
      Layout.preferredWidth: preferredNumberInputWidth
      Layout.leftMargin: inputTextLeftMargin
      color: acceptableInput ? inputTextColor : inputTextErrorColor
      font.pointSize: inputFontPointSize
      validator: doubleValidator
      onEditingFinished: {
        if(numberControlRowLayout.numberObject) {
          numberStepSizeChanged(numberStepSizeTextInput.text);
        }
      }
    }

  }

  function resolveUserInterface() {
    if(numberControlRowLayout.numberObject) {
      numberMinimumLabel.visible = numberControlRowLayout.numberObject.isLimit;
      numberMaximumLabel.visible = numberControlRowLayout.numberObject.isLimit;
      numberMinimumTextInput.visible = numberControlRowLayout.numberObject.isLimit;
      numberMaximumTextInput.visible = numberControlRowLayout.numberObject.isLimit;
    }
  }

}

