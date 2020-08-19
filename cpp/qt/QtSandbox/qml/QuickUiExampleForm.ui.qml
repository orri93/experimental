import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

Item {
  width: 640
  height: 480

  property alias button: button

  Button {
    anchors.centerIn: parent
    id: button
    text: qsTr("Press Me")
  }
}
