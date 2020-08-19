import QtQuick 2.4
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

SpinBox {
  property real realValue: 0.0
  property real realMultiplier: 1000.0
  property int realPrecision: 3
  property real realStepSize: 0.001
  property real realFrom: 0.0
  property real realTo: 100.0

  editable: true

  stepSize: realMultiplier * realStepSize


}
