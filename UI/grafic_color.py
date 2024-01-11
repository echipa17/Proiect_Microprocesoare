from PySide6.QtWidgets import QMainWindow, QVBoxLayout, QWidget, QGroupBox, QPushButton, QLineEdit, QTextEdit, QApplication, QLabel
from PySide6.QtGui import QIcon, QFont
from PySide6.QtCore import QTimer
import pyqtgraph as pg
import serial
import sys
import numpy as np
from pyqtgraph import mkPen


class MainWindow(QMainWindow):
    promotie = "2023-2024"
    team = ["Ioana Mistric", "Teodora Banu"]

    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))

        self.hour = [0]
        self.temperature = [0]

        self.init_ui()
        self.init_serial()
        self.line_segments = []  # Inițializare listă pentru segmentele de linie

    def init_ui(self):
        self.layout = QVBoxLayout()
       
        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member,1)
        team_box_layout.addWidget(second_member,1)
        team_box.setLayout(team_box_layout)

        self.layout.addWidget(team_box)

        self.plot_widget = pg.PlotWidget()
        self.temperature_curve = self.plot_widget.plot(self.hour, self.temperature, name="Temperatură")
        self.layout.addWidget(self.plot_widget)

        self.control_group_box = QGroupBox("Controls")
        self.control_layout = QVBoxLayout()
        self.play_button = QPushButton("Play Buzzer")
        self.stop_button = QPushButton("Stop Buzzer")
        self.secventa_button = QPushButton("Schimba secventa ledurilor")
        
        self.input_field = QLineEdit()
        self.control_layout.addWidget(self.play_button)
        self.control_layout.addWidget(self.stop_button)
        self.control_layout.addWidget(self.secventa_button)
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
        self.secventa_button.clicked.connect(lambda: self.send_serial_data('2'))
        self.input_field.returnPressed.connect(self.send_input)

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_temperature)
        self.timer.start(1000)

    def init_serial(self):
        self.serial_port = serial.Serial(port='COM8', baudrate=9600, timeout=1)

    def get_color_for_temperature(self, temp):
        if temp < 20:
            return 'g'  # Verde
        elif 20 <= temp < 30:
            return 'y'  # Galben
        else:
            return 'r'  # Roșu

    def update_temperature(self):
        if self.serial_port.in_waiting:
            data = self.serial_port.readline().decode('utf8').strip()

            if data.startswith('T:'):
                temp_str = data[2:].strip()
                try:
                    temp = float(temp_str)
                    self.temperature.append(temp)
                    self.hour.append(self.hour[-1] + 1)

                    color = self.get_color_for_temperature(temp)
                    # Crearea unui nou segment și adăugarea lui la grafic
                    new_segment = self.plot_widget.plot(
                        self.hour[-2:], self.temperature[-2:], pen=mkPen(color, width=2))
                    self.line_segments.append(new_segment)

                    # O logică pentru a limita numărul de segmente stocate (opțional)
                    if len(self.line_segments) > 50:  # Exemplu: limitează la 50 de segmente
                        old_segment = self.line_segments.pop(0)
                        self.plot_widget.removeItem(old_segment)

                    self.debug_text_edit.append(f"Temperatura: {temp}°C")
                except ValueError as e:
                    self.debug_text_edit.append(f"Nu pot procesa datele.")
            else:
                self.debug_text_edit.append(f"{data}")
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
