from PyQt4.QtCore import QSize
from PyQt4.QtGui import *

from ImageView import ImageView

class RecognitionPane(QWidget):
    def __init__(self):
        QWidget.__init__(self)

        # Define items
        self.view = ImageView()
        self.load_image_button = QPushButton("Load image")
        self.recognize_button = QPushButton("Recognize")
        self.spacer = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        # Define layouts
        self.buttons_layout = QVBoxLayout()
        self.main_layout = QHBoxLayout()

        # Add top level items
        self.main_layout.addWidget(self.view)
        self.main_layout.addLayout(self.buttons_layout)

        # Add buttons
        self.buttons_layout.addWidget(self.load_image_button)
        self.buttons_layout.addWidget(self.recognize_button)
        self.buttons_layout.addItem(self.spacer)

        self.setMinimumSize(QSize(500, 500))
        self.setLayout(self.main_layout)

        self.load_image_button.clicked.connect(self.load_image_clicked)
        self.recognize_button.clicked.connect(self.recognize_clicked)

    def load_image_clicked(self):
        # create choose file dialog
        dlg = QFileDialog(self)
        dlg.setWindowTitle('Choose image')
        dlg.setViewMode(QFileDialog.Detail)
        dlg.setNameFilters([self.tr('BMP image (*.bmp)'), self.tr('JPEG image (*.jpeg)')])

        # exec choose file dialog
        dlg.exec()
        files = dlg.selectedFiles()

        self.view.show_image(files[0])

    def recognize_clicked(self):
        filename = QFileDialog.getSaveFileName(self, "Save cutted image", "", "BMP image (*.bmp)")
        self.view.cut_image(filename)
