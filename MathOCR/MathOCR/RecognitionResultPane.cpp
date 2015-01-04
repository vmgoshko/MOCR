#include "RecognitionResultPane.h"

#include "ImagePreProccessor.h"
#include "NeuralNetworkTools.h"
#include "ComponentsTree.h"
#include "Utils.h"
#include "NeuralNetworkLeafRecognizer.h"
#include "TwoStepsLeafRecognizer.h"
#include "Structurer.h"
#include "SkeletonBuilder.h"
#include "Utils.h"
#include "qcustomplot.h"

#include <opencv\cv.h>

#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>
#include <QListWidgetItem>
#include <QFont>
using namespace std;

RecognitionResultPane::RecognitionResultPane(QWidget *parent/* = 0*/) : QWidget(parent)
{
	ui.setupUi(this);

	/*mProgressDialog = new QProgressDialog(this);
	mProgressDialog->setRange(0, 4);
	mProgressDialog->setValue(0);
	*/
	ui.FullResultLineEdit->setReadOnly(true);

	ui.listWidget->setSpacing(6);
	ui.listWidget->setStyleSheet("QListWidget::item \
									{ \
										border: 1px solid black; \
									}\
								);"
);
	
	connect(ui.listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(fillPossibleCharsLineEdit()));
	connect(ui.listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(showBarChart()));
}

RecognitionResultPane::~RecognitionResultPane()
{

}

void RecognitionResultPane::beginRecognition(const QString& inImagePath)
{
	ImagePreProccessor thePreProcessor;
	TwoStepsLeafRecognizer theRecognizer;
	Structurer theStructurer;
	ComponentsTree theTree;
	std::vector< std::vector< float > > thePosibilities;

	//mProgressDialog->show();
	//mProgressDialog->setLabelText("Preprocessing image");
	cv::Mat theProcessedImage = thePreProcessor.process(inImagePath.toLocal8Bit());
	//mProgressDialog->setValue(1);
	//mProgressDialog->setLabelText("Building tree");
	theTree.build(theProcessedImage);
	//mProgressDialog->setValue(2);
	//mProgressDialog->setLabelText("Recognizing leafs");
	std::vector< std::string > theLeafsStrs = theRecognizer.recognizeLeafs(&theTree, &thePosibilities);
	//mProgressDialog->setValue(3);
	//mProgressDialog->setLabelText("Structuring formula");
	std::string theResult = theStructurer.structure(theProcessedImage, theTree.getLeafs(), theLeafsStrs);
	//mProgressDialog->setValue(4);
	//mProgressDialog->hide();
	
	std::vector< Component > theLeafs = theTree.getLeafs();
	std::vector< BlackObject > theBlackObjects;

	for (int i = 0; i < theLeafs.size(); i++)
	{
		Bound theBound = theLeafs[i].getBound();
		BlackObject theObject;
		theObject.object = submat(theProcessedImage, theBound);
		theObject = bound(&theObject.object, 0);
		theBlackObjects.push_back(theObject);
	}

	int theMaxWidth = 0;
	int theMaxHeight = 0;
	QFont theListItemFont("Times new roman");
	for (int i = 0; i < theBlackObjects.size(); i++)
	{
		QListWidgetItem* theItem = new QListWidgetItem(ui.listWidget);
		std::string theTempFilePath = saveTempFile(&theBlackObjects[i].object);
		QPixmap theImage(theTempFilePath.c_str());
		theItem->setIcon(theImage);
		
		ui.listWidget->addItem(theItem);
		
		if (theImage.height() > theMaxHeight)
			theMaxHeight = theImage.height();

		if (theImage.width() > theMaxWidth)
			theMaxWidth = theImage.width();

		std::vector< char* >* theChars = getOutsVector();
		QStringList theCharsTextList;
		for (int j = 0; j < thePosibilities[i].size(); j++)
		{
			if (thePosibilities[i][j] != 0)
			{
				theCharsTextList << theChars->at(j);
			}
		}

		//theItem->setText(theCharsTextList.join(" "));
		mPossibilitiesMap.insert(theItem, thePosibilities[i]);
		delete theChars;
	}

	ui.listWidget->setIconSize(QSize(theMaxWidth, theMaxHeight));
	ui.FullResultLineEdit->setText(theResult.c_str());

	ui.listWidget->setItemSelected(ui.listWidget->item(0), true);
}

void RecognitionResultPane::fillPossibleCharsLineEdit()
{
	QListWidgetItem* theSelectedItem = ui.listWidget->selectedItems()[0];
	std::vector< float > thePossibleChars = mPossibilitiesMap.value(theSelectedItem);
	std::vector< char* >* theChars = getOutsVector();

	QStringList theCharsTextList;
	for (int i = 0; i < thePossibleChars.size(); i++)
	{
		if (thePossibleChars[i] != 0)
		{
			theCharsTextList << theChars->at(i);
		}
	}

	ui.PossibleCharsLineEdit->setText(theCharsTextList.join(" "));
	delete theChars;
}


void RecognitionResultPane::showBarChart()
{
	QCustomPlot* thePlot = new QCustomPlot(this);
	thePlot->setMinimumHeight(300);
	thePlot->setMinimumWidth(300);
	// create empty bar chart objects:
	QCPBars* theCharBar= new QCPBars(thePlot->xAxis, thePlot->yAxis);
	thePlot->addPlottable(theCharBar);
	
	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	pen.setColor(QColor(150, 222, 0));
	theCharBar->setName("Chars");
	theCharBar->setPen(pen);
	theCharBar->setBrush(QColor(150, 222, 0, 255));

	QListWidgetItem* theSelectedItem = ui.listWidget->selectedItems()[0];
	std::vector< float > thePossibleChars = mPossibilitiesMap.value(theSelectedItem);
	std::vector< char* >* theChars = getOutsVector();

	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;

	for (int i = 0; i < thePossibleChars.size(); i++)
	{
		ticks << i + 1;
		labels << theChars->at(i);
	}

	thePlot->xAxis->setAutoTicks(false);
	thePlot->xAxis->setAutoTickLabels(false);
	thePlot->xAxis->setTickVector(ticks);
	thePlot->xAxis->setTickVectorLabels(labels);
	thePlot->xAxis->setTickLabelRotation(60);
	thePlot->xAxis->setSubTickCount(0);
	thePlot->xAxis->setTickLength(0, 4);
	thePlot->xAxis->grid()->setVisible(true);
	thePlot->xAxis->setRange(0, thePossibleChars.size() + 1);

	// prepare y axis:
	thePlot->yAxis->setRange(0, 1);
	thePlot->yAxis->setPadding(1); // a bit more space to the left border
	thePlot->yAxis->setLabel("Possibility");
	thePlot->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	thePlot->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	thePlot->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<double> theCharData;
	
	for (int i = 0; i < thePossibleChars.size(); i++)
	{
		theCharData << thePossibleChars[i];
	}
	
	theCharBar->setData(ticks, theCharData);

	// setup legend:
	thePlot->legend->setVisible(true);
	thePlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
	thePlot->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	thePlot->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	thePlot->legend->setFont(legendFont);
	thePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

	clearLayout(ui.BarChartLayout);

	ui.BarChartLayout->addWidget(thePlot);

	delete theChars;
}

void RecognitionResultPane::clearLayout(QLayout* inLayout)
{
	QLayoutItem* theItem;
	while (theItem = inLayout->itemAt(0))
	{
		if (theItem->widget())
		{
			delete theItem->widget();
		}
	}
}