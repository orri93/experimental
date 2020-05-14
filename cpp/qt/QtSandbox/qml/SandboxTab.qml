import QtQuick 2.14
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Tab {
  width: parent.width
  height: parent.height

  onVisibleChanged: {
    console.log("Tab visible " + visible)
  }

  title: qsTr("Tab 1")
  ColumnLayout {
    SandboxGroupA {
      id: firstGroup
    }
    SandboxGroupB {
      id: secondGroup
    }
  }
}
