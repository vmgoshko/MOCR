#ifndef MATHOCR_H
#define MATHOCR_H

#include <QtWidgets/QMainWindow>
#include "ui_mathocr.h"

class ImageView;
class QAction;

class MathOCR : public QMainWindow
{
	Q_OBJECT

public:
	MathOCR(QWidget *parent = 0);
	~MathOCR();

private slots:
	void openFileClicked();
	void OCRActionClicked();
	void RecognizeClicked();
private:
	Ui::MathOCRClass ui;
	ImageView* mImageView;
	QAction* mOCRAction;
	QAction* mRecognizeAction;
};

#endif // MATHOCR_H
