#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QtWidgets/QMainWindow>
#include <QGraphicsView>
#include <QPoint>
#include "ui_mathocr.h"

#include "RecognitionResultPane.h"

class ImageView : public QGraphicsView
{
	Q_OBJECT

public:
	ImageView(QWidget *parent = 0);
	~ImageView();

	void showImage(const QString& inImagePath);
	void setSelectModeEnabled(bool inIsEnabled);
	QString getImagePath();
public slots:
	void wheelEvent(QWheelEvent* inEvent);

	void mousePressEvent(QMouseEvent* inEvent);
	void mouseMoveEvent(QMouseEvent* inEvent);
	void mouseReleaseEvent(QMouseEvent* inEvent);
private:
	QRubberBand mRubberBand;
	QPoint mSelectionStart;
	bool mIsSelectMode;
	bool mIsSelection;
	QString mDefaultImagePath;
};

#endif // IMAGE_VIEW_H
