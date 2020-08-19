import QtQuick 2.4

ControlFormatForm {

  property var formatObject: null

  onFormatObjectChanged: {
    if(formatObject) {
      precisionBox.value = formatObject.precision;
    }
  }

  precisionBox.onValueChanged: {
    if(formatObject) {
      formatObject.precision = precisionBox.value;
    }
  }
}
