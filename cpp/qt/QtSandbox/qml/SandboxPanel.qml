import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

ColumnLayout {
  Layout.alignment: Qt.AlignTop
  Layout.fillWidth: true
  Layout.fillHeight: true

  property string titleText: qsTr("PID Toolkit")

  property color titleTextColor: "floralwhite"

  property int titleFontPointSize: 18

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


}
