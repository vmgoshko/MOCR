from PyQt4.QtCore import QRectF, QPoint, QRect, QSize
from PyQt4.QtGui import *

__author__ = 'vmgoshko'


class ImageView(QGraphicsView):
    def __init__(self):
        QGraphicsView.__init__(self)
        self.scene = QGraphicsScene(self)
        self.setScene(self.scene)
        self.clear()
        self.selection = False
        self.rubberBand = QRubberBand(QRubberBand.Rectangle, self)
        self.selectionStart = QPoint(0, 0)

    def show_image(self, image_path):
        self.clear()
        pixmap = QPixmap(image_path)
        pixmap_item = QGraphicsPixmapItem(pixmap)
        rect_item = QGraphicsRectItem(QRectF(0, 0, pixmap.width(), pixmap.height()))
        self.scene.addItem(pixmap_item)
        self.scene.addItem(rect_item)

    def clear(self):
        self.scene.clear()
        self.scene.setBackgroundBrush(QBrush(QColor(180, 180, 180)))

    def wheelEvent(self, event):
        self.rubberBand = QRubberBand()
        self.setTransformationAnchor(QGraphicsView.AnchorUnderMouse)
        scale_factor = 1.15

        if event.delta() > 0:
            self.scale(scale_factor, scale_factor)
        else:
            self.scale(1.0 / scale_factor, 1.0 / scale_factor)

    def mousePressEvent(self, event):
        self.rubberBand.setGeometry(QRect(event.pos(), QSize()))
        self.rubberBand.show()

        self.selectionStart = event.pos()
        QGraphicsView.mousePressEvent(self, event)

    def mouseMoveEvent(self, event):
        self.rubberBand.setGeometry(QRect(self.selectionStart, event.pos()).normalized())

        self.viewport().update()
        QGraphicsView.mouseMoveEvent(self, event)

    def mouseReleaseEvent(self, event):
        QGraphicsView.mouseReleaseEvent(self, event)

    def cut_image(self, savepath):
        isHidden = self.rubberBand.isHidden()
        self.rubberBand.hide()

        cut_rect = self.rubberBand.geometry()
        pixmap = QPixmap.grabWidget(self, cut_rect)
        pixmap.save(savepath)

        self.rubberBand.setHidden(isHidden)




