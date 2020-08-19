pragma Singleton
import QtQuick 2.12
import QtCharts 2.1

Item {

  readonly property color dialogColor: "#404040"
  readonly property color titleTextColor: "floralwhite"
  readonly property color labelTextColor: "white"
  readonly property color inputTextColor: "beige"
  readonly property color informationTextColor: "burlywood"
  readonly property color errorTextColor: "red"
  readonly property color groupTextColor: "gray"
  readonly property color unitTextColor: "cornsilk"

  readonly property color dialogGroupTextColor: "navy"
  readonly property color dialogLabelTextColor: "midnightblue"
  readonly property color dialogInputTextColor: "darkslateblue"
  readonly property color dialogIndicatorsTextColor: "indigo"

  readonly property int chartTheme: ChartView.ChartThemeDark

  readonly property int titleFontPointSize: 18
  readonly property int labelFontPointSize:  8
  readonly property int inputFontPointSize: 10
  readonly property int informationFontPointSize: 10

  readonly property double titleTopMargin: 10.0
  readonly property double titleVerticalMargin: 10.0
  readonly property double titleLeftMargin: 10.0
  readonly property double titleRightMargin: 10.0
  readonly property double labelLeftMargin: 2.0
  readonly property double inputTextLeftMargin: 5.0
  readonly property double informationTextLeftMargin: 5.0

  readonly property real groupTitleLocation: 10.0

  readonly property real rangeIndicatorWidth: 40.0
  readonly property real rangeMaximumWidth: 20.0
}
