import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

import assets 1.0

ColumnLayout {
  id: qtExperimentalPanel

  Layout.fillWidth: true
  Layout.fillHeight: true

  property string titleText: qsTr("Real Time")

  Component.onCompleted: {
  }

  Text {
      text: titleText
      color: Style.titleTextColor
      Layout.fillWidth: false
      Layout.leftMargin: Style.titleLeftMargin
      Layout.topMargin: Style.titleVerticalMargin
      Layout.bottomMargin: Style.titleVerticalMargin
      Layout.alignment: Qt.AlignTop
      font.pointSize: Style.titleFontPointSize
  }

  /* SandboxRealSpinBox {} */
  ControlRealSpinBox {
    id: setpointControlRealSpinBox
    accuracyObject: backend.configuration.ui.setpoint
    realValue: backend.setpoint

    onRealValueChanged: {
      backend.setpoint = setpointControlRealSpinBox.realValue;
    }
  }

  Connections {
    target: backend.configuration
    onUiChanged: {
      console.log("UI Changed");
      setpointControlRealSpinBox.refresh();
    }
  }


  UiDialog { id: uiDialog }

  Button {
    text: qsTr("Press")
    onClicked: {
      uiDialog.open();
    }
  }
}
