import QtQuick 2.4

import qt.sandbox.models 1.0

ControlAccuracyForm {

  property var accuracyObject: null

  restrictionBox.model: backend.restrictionModel
  restrictionBox.textRole: "text"
  restrictionBox.valueRole: "value"

  onAccuracyObjectChanged: {
    if(accuracyObject) {
      restrictionBox.currentIndex =
        backend.restrictionModel.index(accuracyObject.restriction);
      //console.log("Setting restriction index to " + restrictionBox.currentIndex);
      precisionBox.value = accuracyObject.precision;
      minimumInput.text = accuracyObject.range.from.toString();
      maximumInput.text = accuracyObject.range.to.toString();
      resolveUserInterface();
    }
  }

  restrictionBox.onCurrentIndexChanged: {
    if(accuracyObject) {
      //console.log("Current restriction index changed to " +
      // restrictionBox.currentIndex);
      accuracyObject.restriction =
        backend.restrictionModel.restriction(restrictionBox.currentIndex);
      resolveUserInterface();
    }
  }

  precisionBox.onValueChanged: {
    if(accuracyObject) {
      accuracyObject.precision = precisionBox.value;
    }
  }

  minimumInput.onEditingFinished: {
    if(accuracyObject) {
      accuracyObject.range.from = minimumInput.text;
    }
  }

  maximumInput.onEditingFinished: {
    if(accuracyObject) {
      accuracyObject.range.to = maximumInput.text;
    }
  }

  function resolveUserInterface() {
    //console.log("Restriction: " + accuracyObject.restriction);
    //console.log("Both: " + Restriction.Both);
    if(accuracyObject.restriction == Restriction.Both) {
      //console.log("Restriction.Both");
      minimumRow.visible = true;
      maximumRow.visible = true;
    } else if(accuracyObject.restriction == Restriction.Minimum) {
      //console.log("Restriction.Minimum");
      minimumRow.visible = true;
      maximumRow.visible = false;
    } else if(accuracyObject.restriction == Restriction.Maximum) {
      //console.log("Restriction.Maximum");
      minimumRow.visible = false;
      maximumRow.visible = true;
    } else {
      //console.log("Restriction.None");
      minimumRow.visible = false;
      maximumRow.visible = false;
    }
  }
}
