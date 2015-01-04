#ifndef RECOGNITION_RESULT_PANE_H
#define RECOGNITION_RESULT_PANE_H

#include <QtWidgets/QMainWindow>
#include <QGraphicsView>
#include <QProgressDialog>
#include "Defs.h"
#include "ui_RecognitionResultPane.h"

class ImageView;
class QAction;
class QProgressDialog;

class RecognitionResultPane : public QWidget
{
	Q_OBJECT

public:
	RecognitionResultPane(QWidget *parent = 0);
	~RecognitionResultPane();

	void beginRecognition(const QString& inImagePath);
private slots:
	void fillPossibleCharsLineEdit();
	void showBarChart();
private:
	void clearLayout(QLayout* inLayout);
private:
	Ui::RecognitionResultPane ui;
	QProgressDialog* mProgressDialog;
	QMap< QListWidgetItem*, std::vector< float > > mPossibilitiesMap;
};

#endif // MATHOCR_H
