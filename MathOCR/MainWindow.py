from RecognitionPane import RecognitionPane
from TrainingPane import TrainingPane

__author__ = 'Admin'

from PyQt4 import QtGui, QtCore


class MainWindow(QtGui.QMainWindow):

    def __init__(self):
        QtGui.QMainWindow.__init__(self)

        self.recognition_pane = RecognitionPane()
        self.training_pane= TrainingPane()
        self.resize(250, 150)
        self.setWindowTitle('MathOCR')

        open_recognition_pane = QtGui.QAction(QtGui.QIcon(), 'Recognition pane', self)
        open_training_pane = QtGui.QAction(QtGui.QIcon(), 'Training pane', self)

        self.statusBar()

        menubar = self.menuBar()
        panes = menubar.addMenu('&Panes')
        panes.addAction(open_recognition_pane)
        panes.addAction(open_training_pane)

        self.openRecognitionPaneTriggered()


    def openRecognitionPaneTriggered(self):
        self.setCentralWidget( self.recognition_pane )

    def openTrainingPaneTriggered(self):
        self.setCentralWidget( self.training_pane )