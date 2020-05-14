import QtQuick 2.14
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

TabView {
  width: parent.width
  height: parent.height

  SandboxTab {
    id: sandboxTab
  }

  Tab {
    title: qsTr("Tab 2")
  }

  Tab {
    title: qsTr("Tab 3")
  }
}
