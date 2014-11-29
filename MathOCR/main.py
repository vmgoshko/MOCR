import sys
from PyQt4.QtGui import *
from MainWindow import MainWindow

if __name__ == "__main__":
    app = QApplication(sys.argv)

    main_widget = MainWindow()
    main_widget.showMaximized()

    sys.exit(app.exec_())