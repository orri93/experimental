#
# Optimization Experiment 1
#

import sys

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
matplotlib.use('QtAgg')
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

from PyQt6.QtCore import  QTimer
from PyQt6.QtWidgets import (
  QApplication,
  QHBoxLayout,
  QPushButton,
  QComboBox,
  QSpinBox,
  QLabel,
  QVBoxLayout,
  QWidget,
)

import optmodels1
import optmplcanvas1
import optchart1
import optalg1

model_files = optmodels1.get_model_files()

class Window(QWidget):
  def __init__(self):
    super().__init__()

    self.current_model = None
    self.current_model_key = None
    self.current_model_data = None
    self.current_model_data_min_index = None

    self.optimization_chart = optchart1.Chart()

    self.optimization_algorithm = optalg1.Algorithm()

    self.timer = QTimer()
    # self.timer.setSingleShot(True)
    self.timer.timeout.connect(self.timer_timeout)

    # Define our layout.
    vbox = QVBoxLayout()

    self.sc = optmplcanvas1.MplCanvas(self, width=5, height=4, dpi=100)
    self.optimization_chart.handover_canvas(self.sc)

    # Create toolbar, passing canvas as first parament, parent (self, the MainWindow) as second.
    toolbar = NavigationToolbar(self.sc, self)

    main_layout = QVBoxLayout()
    main_layout.addWidget(toolbar)
    main_layout.addWidget(self.sc)

    # Create a combo box for the model files.
    self.model_file_combo = QComboBox()
    self.model_file_combo.addItems(model_files)
    self.model_file_combo.setCurrentIndex(-1)
    self.model_file_combo.currentIndexChanged.connect(self.model_file_combo_changed)

    self.model_combo = QComboBox()
    self.model_combo.currentIndexChanged.connect(self.model_combo_changed)

    self.start_button = QPushButton("Start")
    self.start_button.clicked.connect(self.start_button_clicked)
    self.start_button.setEnabled(False)

    self.stop_button = QPushButton("Stop")
    self.stop_button.clicked.connect(self.stop_button_clicked)
    self.stop_button.setEnabled(False)

    self.stop_when_min_than_spinbox = QSpinBox()
    self.stop_when_min_than_spinbox.setRange(0, 100)
    self.stop_when_min_than_spinbox.setValue(0)
    self.stop_when_min_than_spinbox.setSingleStep(1)

    self.interval_spinbox = QSpinBox()
    self.interval_spinbox.setRange(100, 5000)
    self.interval_spinbox.setValue(500)
    self.interval_spinbox.setSingleStep(100)

    self.seed_spinbox = QSpinBox()
    self.seed_spinbox.setRange(0, 1000)
    self.seed_spinbox.setValue(1)
    self.seed_spinbox.setSingleStep(1)

    self.min_step_ratio_spinbox = QSpinBox()
    self.min_step_ratio_spinbox.setRange(1, 100)
    self.min_step_ratio_spinbox.setValue(10)
    self.min_step_ratio_spinbox.setSingleStep(1)

    self.max_step_ratio_spinbox = QSpinBox()
    self.max_step_ratio_spinbox.setRange(1, 100)
    self.max_step_ratio_spinbox.setValue(30)
    self.max_step_ratio_spinbox.setSingleStep(1)

    self.random_configuration_combo = QComboBox()
    self.random_configuration_combo.addItems(optalg1.RandomOptions)
    self.random_configuration_combo.setCurrentIndex(0)
    self.random_configuration_combo.currentIndexChanged.connect(self.random_configuration_combo_changed)

    self.random_mu_spinbox = QSpinBox()
    self.random_mu_spinbox.setRange(0, 10000)
    self.random_mu_spinbox.setValue(0)
    self.random_mu_spinbox.setSingleStep(10)
    self.random_mu_spinbox.setEnabled(False)

    self.random_sigma_spinbox = QSpinBox()
    self.random_sigma_spinbox.setRange(0, 10000)
    self.random_sigma_spinbox.setValue(100)
    self.random_sigma_spinbox.setSingleStep(10)
    self.random_sigma_spinbox.setEnabled(False)

    vbox.addWidget(QLabel("Model File"))
    vbox.addWidget(self.model_file_combo)
    vbox.addWidget(QLabel("Model"))
    vbox.addWidget(self.model_combo)
    vbox.addWidget(QLabel("Controls"))
    vbox.addWidget(self.start_button)
    vbox.addWidget(self.stop_button)
    vbox.addWidget(QLabel("Stop when lower than"))
    vbox.addWidget(self.stop_when_min_than_spinbox)
    vbox.addWidget(QLabel("Interval (ms)"))
    vbox.addWidget(self.interval_spinbox)
    vbox.addWidget(QLabel("Seed"))
    vbox.addWidget(self.seed_spinbox)
    vbox.addWidget(QLabel("Min Step Ratio"))
    vbox.addWidget(self.min_step_ratio_spinbox)
    vbox.addWidget(QLabel("Max Step Ratio"))
    vbox.addWidget(self.max_step_ratio_spinbox)
    vbox.addWidget(QLabel("Random Option"))
    vbox.addWidget(self.random_configuration_combo)
    vbox.addWidget(QLabel("Random Mu (ppt)"))
    vbox.addWidget(self.random_mu_spinbox)
    vbox.addWidget(QLabel("Random Sigma (ppt)"))
    vbox.addWidget(self.random_sigma_spinbox)
    vbox.addStretch()

    alg_vbox = QVBoxLayout()
    self.alg_status_label = QLabel("Algorithm Status: Idle")
    self.alg_abs_min_label = QLabel("Absolute Minimum: -")
    self.alg_current_min_label = QLabel("Current Minimum: -")
    self.alg_step_count = QLabel("Step Count: -")

    alg_vbox.addWidget(self.alg_status_label)
    alg_vbox.addWidget(self.alg_abs_min_label)
    alg_vbox.addWidget(self.alg_current_min_label)
    alg_vbox.addWidget(self.alg_step_count)
    alg_vbox.addStretch()

    hbox = QHBoxLayout(self)
    hbox.addLayout(vbox)
    hbox.addLayout(main_layout)
    hbox.addLayout(alg_vbox)

    self.setLayout(hbox)

  def model_file_combo_changed(self, index):
    print(f"Model file changed to index {index}.")
    model_file = model_files[index]
    print(f"Model file changed to {model_file}.")
    self.current_model = optmodels1.load_model(model_file)
    self.model_keys = optmodels1.get_model_keys(self.current_model)
    self.model_combo.currentIndexChanged.disconnect(self.model_combo_changed)
    self.model_combo.clear()
    self.model_combo.addItems(self.model_keys)
    self.model_combo.setCurrentIndex(-1)
    self.model_combo.currentIndexChanged.connect(self.model_combo_changed)

  def model_combo_changed(self, index):
    print(f"Model changed to index {index}.")
    self.current_model_key = self.model_keys[index]
    print(f"Model changed to {self.current_model_key}.")
    self.current_model_data = optmodels1.get_model_data(self.current_model, self.current_model_key)
    self.current_model_data_min_index = np.unravel_index(np.argmin(self.current_model_data, axis=None), self.current_model_data.shape)
    val_at_min_index = self.current_model_data[self.current_model_data_min_index]
    val_at_min_index_f = "{:.2f}".format(val_at_min_index)
    self.alg_abs_min_label.setText(f"Absolute Minimum: {val_at_min_index_f}")
    self.optimization_chart.clear()
    self.optimization_chart.set_data(self.current_model_data)
    self.optimization_chart.set_minimum_index(self.current_model_data_min_index)
    self.optimization_chart.plot()
    self.start_button.setEnabled(True)

  def start_button_clicked(self):
    print("Start button clicked.")
    self.model_file_combo.setEnabled(False)
    self.model_combo.setEnabled(False)
    self.start_button.setEnabled(False)
    self.interval_spinbox.setEnabled(False)
    self.seed_spinbox.setEnabled(False)
    self.min_step_ratio_spinbox.setEnabled(False)
    self.max_step_ratio_spinbox.setEnabled(False)
    self.stop_button.setEnabled(True)
    self.alg_status_label.setText("Algorithm Status: Running")
    self.optimization_algorithm.seed(self.seed_spinbox.value())
    self.optimization_algorithm.set_landscape(self.current_model_data)
    self.optimization_algorithm.set_min_step_ratio(self.min_step_ratio_spinbox.value() / 100)
    self.optimization_algorithm.set_max_step_ratio(self.max_step_ratio_spinbox.value() / 100)
    self.optimization_algorithm.set_normal_mu(self.random_mu_spinbox.value() / 1000)
    self.optimization_algorithm.set_normal_sigma(self.random_sigma_spinbox.value() / 1000)
    first_point = self.optimization_algorithm.first()
    print(f"First point: ({first_point.x}, {first_point.y})")
    self.optimization_chart.set_current_point(first_point)
    self.optimization_chart.plot()
    current_minimum = self.optimization_algorithm.get_current_minimum()
    current_minimum_f = "{:.2f}".format(current_minimum)
    self.alg_current_min_label.setText(f"Current Minimum: {current_minimum_f}")
    self.timer.start(self.interval_spinbox.value())

  def stop_button_clicked(self):
    print("Stop button clicked.")
    self.stop()

  def random_configuration_combo_changed(self, index):
    print(f"Random Configuration changed to index {index}.")
    self.optimization_algorithm.set_random_option(index)
    if (index > 0):
      self.random_mu_spinbox.setEnabled(True)
      self.random_sigma_spinbox.setEnabled(True)
    else:
      self.random_mu_spinbox.setEnabled(False)
      self.random_sigma_spinbox.setEnabled(False)

  def timer_timeout(self):
    print("Timer timeout.")
    step_count = self.optimization_algorithm.get_step_count()
    self.alg_step_count.setText(f"Step Count: {step_count}")
    if (self.optimization_algorithm.state == 'Step'):
      next_point = self.optimization_algorithm.next()
      self.optimization_chart.set_next_point(next_point)
      self.optimization_chart.plot()
    elif (self.optimization_algorithm.state == 'Evaluate'):
      isbetter = self.optimization_algorithm.evaluate()
      if (isbetter):
        current_minimum = self.optimization_algorithm.get_current_minimum()
        current_minimum_f = "{:.2f}".format(current_minimum)
        self.alg_current_min_label.setText(f"Current Minimum: {current_minimum_f}")
        if (current_minimum <= self.stop_when_min_than_spinbox.value()):
          self.stop()
      self.optimization_chart.clear()
      self.optimization_chart.set_current_point(self.optimization_algorithm.current_point)
      self.optimization_chart.plot()

  def stop(self):
    self.timer.stop()
    self.optimization_chart.clear()
    self.optimization_chart.plot()
    self.alg_status_label.setText("Algorithm Status: Idle")
    self.stop_button.setEnabled(False)
    self.model_file_combo.setEnabled(True)
    self.model_combo.setEnabled(True)
    self.start_button.setEnabled(True)
    self.interval_spinbox.setEnabled(True)
    self.seed_spinbox.setEnabled(True)
    self.min_step_ratio_spinbox.setEnabled(True)
    self.max_step_ratio_spinbox.setEnabled(True)


app = QApplication(sys.argv)
w = Window()
w.show()

app.exec()
