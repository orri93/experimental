import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import QtQuick.Extras 1.4

import assets 1.0

ColumnLayout {
  id: qtExperimentalPanelRight

  Layout.fillWidth: true
  Layout.fillHeight: true

  property string titleText: qsTr("Qt Experimental Right")

  Component.onCompleted: {
  }

  Text {
      text: titleText
      color: Style.titleTextColor
      Layout.fillWidth: false
      Layout.leftMargin: Style.titleLeftMargin
      Layout.rightMargin: Style.titleRightMargin
      Layout.topMargin: Style.titleVerticalMargin
      Layout.bottomMargin: Style.titleVerticalMargin
      Layout.alignment: Qt.AlignTop
      font.pointSize: Style.titleFontPointSize
  }

  RealTimeOutput {
  }

  Button {
    text: qsTr("Press Right")
    onClicked: {
      sandboxDialog.open();
    }
    enabled: true
  }

}
