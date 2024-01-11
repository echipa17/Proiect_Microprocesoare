from grafic_color import MainWindow
#from grafic_normal import MainWindow
from PySide6.QtWidgets import QApplication
from PySide6.QtGui import QScreen
app = QApplication([])
window = MainWindow()
window.setGeometry(0, 0, 1000, 1000)
screen_geometry = QScreen.availableGeometry(QApplication.primaryScreen())
screenX = (screen_geometry.width() - window.width()) / 2
screenY = (screen_geometry.height() - window.height()) / 2
window.move(screenY, screenY)
window.show()
app.exec()