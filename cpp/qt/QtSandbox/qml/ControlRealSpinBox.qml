import QtQuick 2.1
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

SpinBox {
  id: realSpinBox

  property var numberObject: null

  property real realValue: 0.0
  property real multiplier: 1000.0
  property int realDecimals: 3

  editable: true

  function setRealUiNumber(number) {
    if(number) {
      numberObject = number;
      setRealDecimals(numberObject.precision);
      setRealFrom(numberObject.minimum);
      setRealTo(numberObject.maximum);
      setRealStepSize(numberObject.stepSize);
      console.log("Set Real UI Number");
      console.log("  Precision: " + numberObject.precision);
      console.log("  From: " + numberObject.minimum);
      console.log("  To: " + numberObject.maximum);
      console.log("  Step Szie: " + numberObject.stepSize);
      console.log("  Multiplier: " + realSpinBox.multiplier);
    }
  }

  function setRealValue(value) {
    realSpinBox.value = realSpinBox.multiplier * value;
  }

  function setRealDecimals(decimals) {
    realSpinBox.realDecimals = decimals;
    realSpinBox.multiplier = Math.pow(10.0, realSpinBox.realDecimals);
  }

  function setRealFrom(from) {
    realSpinBox.from = realSpinBox.multiplier * from;
  }

  function setRealTo(to) {
    realSpinBox.to = realSpinBox.multiplier * to;
  }

  function setRealStepSize(stepSize) {
    realSpinBox.stepSize = realSpinBox.multiplier * stepSize;
  }

  onValueChanged: {
    if(realSpinBox.numberObject) {
      realSpinBox.realValue = value / realSpinBox.multiplier;
    }
  }

  validator: DoubleValidator {
      bottom: Math.min(realSpinBox.from, realSpinBox.to)
      top:  Math.max(realSpinBox.from, realSpinBox.to)
  }

  textFromValue: function(value, locale) {
    return Number(value / realSpinBox.multiplier).toLocaleString(
      locale, 'f', realSpinBox.realDecimals)
  }

  valueFromText: function(text, locale) {
    return realSpinBox.multiplier * Number.fromLocaleString(locale, text)
  }
}
