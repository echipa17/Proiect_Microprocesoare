from PySide6.QtWidgets import QMainWindow, QVBoxLayout, QWidget, QGroupBox, QPushButton, QLineEdit, QTextEdit, QApplication
from PySide6.QtGui import QIcon
from PySide6.QtCore import QTimer
import pyqtgraph as pg
import serial
import sys


class MainWindow(QMainWindow):
    promotie = "2023-2024"
    team = ["MISTRIC IOANA", "BANU TEODORA"]

    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")

        self.hour = [0]
        self.temperature = [0]

        self.init_ui()
        self.init_serial()

    def init_ui(self):
        self.layout = QVBoxLayout()

        self.plot_widget = pg.PlotWidget()
        self.temperature_curve = self.plot_widget.plot(self.hour, self.temperature, name="Temperatură")
        self.layout.addWidget(self.plot_widget)

        self.control_group_box = QGroupBox("Controls")
        self.control_layout = QVBoxLayout()
        self.play_button = QPushButton("Play Buzzer")
        self.stop_button = QPushButton("Stop Buzzer")
        
        self.input_field = QLineEdit()
        self.control_layout.addWidget(self.play_button)
        self.control_layout.addWidget(self.stop_button)
        self.control_group_box.setLayout(self.control_layout)
        self.layout.addWidget(self.control_group_box)

        self.debug_text_edit = QTextEdit()
        self.debug_text_edit.setReadOnly(True)
        self.layout.addWidget(self.debug_text_edit)

        central_widget = QWidget()
        central_widget.setLayout(self.layout)
        self.setCentralWidget(central_widget)

        self.play_button.clicked.connect(lambda: self.send_serial_data('1'))
        self.stop_button.clicked.connect(lambda: self.send_serial_data('1'))
        self.input_field.returnPressed.connect(self.send_input)

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_temperature)
        self.timer.start(1000)

    def init_serial(self):
        self.serial_port = serial.Serial(port='COM8', baudrate=9600, timeout=1)

    def update_temperature(self):
        if self.serial_port.in_waiting:
            # data = self.serial_port.readline().decode('utf8').strip()
            data = self.serial_port.readline().decode('utf8', errors='ignore').strip()
            if data.startswith('T:'):
                temp_str = data[2:].strip()
                try:
                    temp = float(temp_str)
                    self.temperature.append(temp)
                    self.hour.append(self.hour[-1] + 1)
                    self.temperature_curve.setData(self.hour, self.temperature)
                    self.debug_text_edit.append(f"Temperatura: {temp}°C")
                except ValueError as e:
                    self.debug_text_edit.append(f"Nu pot procesa datele.")
            else:
                self.debug_text_edit.append(f"{data}")
                pass

    def send_input(self):
        data = self.input_field.text()
        self.input_field.clear()
        self.send_serial_data(data)

    def send_serial_data(self, data):
        try:
            if self.serial_port.is_open:
                self.serial_port.write(data.encode())
        except Exception as e:
            self.debug_text_edit.append(f"Error sending data: {e}")

    def closeEvent(self, event):
        if self.serial_port.is_open:
            self.serial_port.close()
        super().closeEvent(event)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())