import QtQuick 2.0
import QtCharts 2.1

ChartView {
  id: qtExperimentalChart

  animationOptions: ChartView.NoAnimation
  theme: ChartView.ChartThemeDark

  property bool openGl: true
  property bool openGlSupported: true

  onOpenGlChanged: {
    if (openGlSupported) {
      series("output").useOpenGL = openGl
      series("temperature").useOpenGL = openGl;
      series("setpoint").useOpenGL = openGl;
    }
  }

  Component.onCompleted: {
    if (!series("output").useOpenGL) {
      openGlSupported = false
      openGl = false
    }
  }

  ValueAxis {
    id: axisY
    min: 0
    max: 300
  }

  ValueAxis {
    id: axisX
    min: 0
    max: 1024
  }

  LineSeries {
    id: lineSeriesOutput
    name: "output"
    axisX: axisX
    axisY: axisY
    useOpenGL: qtExperimentalChart.openGl
  }
  LineSeries {
    id: lineSeriesTemperature
    name: "temperature"
    axisX: axisX
    axisY: axisY
    useOpenGL: qtExperimentalChart.openGl
  }
  LineSeries {
    id: lineSeriesSetpoint
    name: "setpoint"
    axisX: axisX
    axisY: axisY
    useOpenGL: qtExperimentalChart.openGl
  }

  Connections {
    target: backend
    onIntervalChanged: {
      refreshTimer.interval = backend.interval;
    }
  }

  Connections {
    target: backend
    onIsConnectedChanged: {
      refreshTimer.running = backend.isConnected;
    }
  }

  Timer {
    id: refreshTimer
    interval: 1000
    running: false
    repeat: true
    onTriggered: {
      axisX.max = backend.update(
        pidChart.series("output"),
        pidChart.series("temperature"),
        pidChart.series("setpoint"));
    }
  }
}
