import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12
import QtQuick.Window 2.14

Window {
  id: qtExperimentalWindow

  title: qtExperimentalParameterPanel.titleText

  visible: true

  /* implicitWidth: 1024 */
  /* implicitHeight: 800 */

  width: 1024
  height: 800

  color: "#404040"

  ExperimentalPanel {
    id: qtExperimentalParameterPanel
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.bottom: parent.bottom
  }

  ExperimentalPanelRight {
    id: qtExperimentalParameterPanelRight
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.bottom: parent.bottom
  }

  ExperimentalChart {
    id: qtExperimentalChart
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    //anchors.right: parent.right
    anchors.right: qtExperimentalParameterPanelRight.left
    anchors.left: qtExperimentalParameterPanel.right
    height: qtExperimentalWindow.height
  }
}
