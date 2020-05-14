import QtQuick 2.14
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12

RowLayout {

  property alias tuningRangeSlider: rangeSlider

  property alias rangeMaximum: rangeSlider.to
  property alias rangeStep: rangeSlider.stepSize

  property color labelTextColor: "midnightblue"
  property color indicatorsTextColor: "indigo"

  property real rangeIndicatorWidth: 40.0
  property real rangeMaximumWidth: 20.0

  DoubleValidator { id: doubleValidator }

  Component.onCompleted: {
    maximumTextInput.text = rangeMaximum.toString();
    firstIndicator.text = rangeSlider.first.value.toString();
    secondIndicator.text = rangeSlider.second.value.toString();
  }

  RowLayout {
    Label {
      color: labelTextColor
      text: qsTr("Maximum: ")
    }
    TextInput {
      id: maximumTextInput
      validator: doubleValidator
      Layout.preferredWidth: rangeMaximumWidth
      onEditingFinished: {
        rangeMaximum = maximumTextInput.text
      }
    }
  }

  RangeSlider {
    id: rangeSlider
    first.onValueChanged: {
      firstIndicator.text = rangeSlider.first.value.toString();
    }
    second.onValueChanged: {
      secondIndicator.text = rangeSlider.second.value.toString();
    }
  }

  RowLayout {
    Label {
      id: firstIndicator
      horizontalAlignment: Text.AlignRight
      Layout.preferredWidth: rangeIndicatorWidth
      Layout.alignment: Qt.AlignRight
      color: indicatorsTextColor
    }
    Label {
      horizontalAlignment: Text.AlignHCenter
      Layout.alignment: Qt.AlignHCenter
      color: labelTextColor
      text: qsTr(" - ")
    }
    Label {
      id: secondIndicator
      horizontalAlignment: Text.AlignLeft
      Layout.preferredWidth: rangeIndicatorWidth
      Layout.alignment: Qt.AlignLeft
      color: indicatorsTextColor
    }
  }
}
