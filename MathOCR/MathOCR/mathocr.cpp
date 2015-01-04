#include "mathocr.h"

#include <QHBoxLayout>
#include <QSettings>
#include <QFileDialog>
#include <QFileInfo>
#include <QWindow>
#include "SettingsKeys.h"
#include "ImageView.h"


MathOCR::MathOCR(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// Creating menu
	QMenuBar* theMenuBar = ui.menuBar;
	QMenu* theFileMenu = new QMenu("File", theMenuBar);
	QAction* theOpenFileAction = new QAction("&Open", theFileMenu);
	theMenuBar->addMenu(theFileMenu);
	theFileMenu->addAction(theOpenFileAction);
	
	//Filling out toolbar
	QToolBar* theToolBar = ui.mainToolBar;
	QIcon theOCRActionIcon(":/Resources/images/OCR_action_icon.png");
	
	mOCRAction = new QAction("Select OCR region", theToolBar);
	mOCRAction->setEnabled(false);
	mOCRAction->setIcon(theOCRActionIcon);
	mOCRAction->setCheckable(true);
	
	QIcon theRecognizeActionIcon(":/Resources/images/Recognize.png");

	mRecognizeAction = new QAction("Recognize", theToolBar);
	mRecognizeAction->setEnabled(false);
	mRecognizeAction->setIcon(theRecognizeActionIcon);

	theToolBar->addAction(mOCRAction);
	theToolBar->addAction(mRecognizeAction);

	//Creating layouts
	mImageView = new ImageView(this);
	setCentralWidget(mImageView);

	connect(theOpenFileAction, SIGNAL(triggered()), this, SLOT(openFileClicked()));
	connect(mOCRAction, SIGNAL(triggered()), this, SLOT(OCRActionClicked()));
	connect(mRecognizeAction, SIGNAL(triggered()), this, SLOT(RecognizeClicked()));
}

MathOCR::~MathOCR()
{

}

void MathOCR::openFileClicked()
{
	QString theStartDirectory = QSettings().value(SettingsKey::OPEN_FILE_DIALOG_PATH_KEY, SettingsKey::OPEN_FILE_DIALOG_DEFAULT).toString();
	if (theStartDirectory.isEmpty())
		theStartDirectory = "D:\\";

	QString theOpenFileName = QFileDialog::getOpenFileName(this, "Open file", theStartDirectory);

	if (!theOpenFileName.isEmpty())
		QSettings().setValue(SettingsKey::OPEN_FILE_DIALOG_PATH_KEY, QFileInfo(theOpenFileName).dir().absolutePath());

	mImageView->showImage(theOpenFileName);

	mOCRAction->setEnabled(true);
	mRecognizeAction->setEnabled(true);
}

void MathOCR::OCRActionClicked()
{
	mImageView->setSelectModeEnabled(mOCRAction->isChecked());
}

void MathOCR::RecognizeClicked()
{
	QString theImagePath = mImageView->getImagePath();

	RecognitionResultPane* thePane = new RecognitionResultPane(NULL);
	thePane->showMaximized();
	thePane->beginRecognition(theImagePath);
}
