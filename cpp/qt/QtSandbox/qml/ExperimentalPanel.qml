import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

ColumnLayout {
  id: qtExperimentalPanel

  Layout.fillWidth: true
  Layout.fillHeight: true

  property string titleText: qsTr("Real Time")

  property color titleTextColor: qsTr("floralwhite")
  property color labelTextColor: qsTr("white")
  property color inputTextColor: qsTr("beige")
  property color informationTextColor: qsTr("burlywood")

  property int titleFontPointSize: 18
  property int labelFontPointSize:  8
  property int inputFontPointSize: 10
  property int informationFontPointSize: 10

  property real titleTopMargin: 10.0
  property real titleVerticalMargin: 10.0
  property real titleLeftMargin: 10.0
  property real labelLeftMargin: 2.0
  property real inputTextLeftMargin: 5.0
  property real informationTextLeftMargin: 5.0

  Component.onCompleted: {
  }

  Text {
      text: titleText
      color: titleTextColor
      Layout.fillWidth: false
      Layout.leftMargin: titleLeftMargin
      Layout.topMargin: titleVerticalMargin
      Layout.bottomMargin: titleVerticalMargin
      Layout.alignment: Qt.AlignTop
      font.pointSize: titleFontPointSize
  }

  SandboxRealSpinBox {

  }

  Button {
    text: qsTr("Press")
    onClicked: {
      sandboxDialog.open();
    }
  }
}
