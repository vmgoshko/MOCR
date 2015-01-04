#include "ImageView.h"

#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsDropShadowEffect>
#include <QSettings>
#include <QDir>
#include "SettingsKeys.h"

ImageView::ImageView(QWidget* parent /* = 0 */) : QGraphicsView(parent), mRubberBand(QRubberBand::Rectangle, this)
{
	QGraphicsScene* theScene = new QGraphicsScene(this);
	theScene->setBackgroundBrush(QBrush(QColor(180, 180, 180)));
	setScene(theScene);
	setSelectModeEnabled(false);
	mIsSelection = false;
}

ImageView::~ImageView()
{

}

void ImageView::showImage(const QString& inImagePath)
{
	mDefaultImagePath = inImagePath;
	scene()->clear();
	QPixmap theImage(mDefaultImagePath);
	QGraphicsPixmapItem* theImageGraphicsItem = new QGraphicsPixmapItem(theImage);
	theImageGraphicsItem->setFlag(QGraphicsItem::ItemIsMovable);

	scene()->addItem(theImageGraphicsItem);
}


void ImageView::wheelEvent(QWheelEvent* inEvent)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	float scaleFactor = 1.5f;

	if (inEvent->delta() > 0)
		scale(scaleFactor, scaleFactor);
	else
		scale(1 / scaleFactor, 1 / scaleFactor);

}

void ImageView::setSelectModeEnabled(bool inIsEnabled)
{
	mIsSelectMode = inIsEnabled;
	QList< QGraphicsItem* > theItems = scene()->items();
	
	for (int i = 0; i < theItems.size(); i++)
	{
		QGraphicsItem::GraphicsItemFlags theFlags = theItems.at(i)->flags();
		theFlags ^= QGraphicsItem::ItemIsMovable;
		theItems.at(i)->setFlags(theFlags);
	}

	if (!mIsSelectMode)
		mRubberBand.hide();
}


QString ImageView::getImagePath()
{
	if (!mIsSelectMode)
	{
		return mDefaultImagePath;
	}
	else
	{
		static int theTempPathCounter = 0;
		bool theIsRubberBandHidden = mRubberBand.isHidden();
		mRubberBand.hide();

		QRect theCutRect = mRubberBand.geometry();
		QPixmap thePixmap = QPixmap::grabWidget(this, theCutRect);
		QString thePath = QSettings().value(SettingsKey::TEMP_PATH_KEY, SettingsKey::TEMP_PATH_DEFAULT).toString();
		QDir theDir;
		theDir.mkpath(thePath);
		
		thePath += QString("/cut_image_");
		thePath += QString::number(theTempPathCounter);
		thePath += QString(".jpg");
		
		thePixmap.save(thePath);
		++theTempPathCounter;
		mRubberBand.setHidden(theIsRubberBandHidden);
		return thePath;
	}
}

void ImageView::mousePressEvent(QMouseEvent* inEvent)
{
	if (mIsSelectMode)
	{
		mRubberBand.hide();
		mRubberBand.setGeometry(QRect(inEvent->pos(), QSize()));
		mRubberBand.show();

		mSelectionStart = inEvent->pos();

		mIsSelection = true;
	}
	
	QGraphicsView::mousePressEvent(inEvent);
}

void ImageView::mouseMoveEvent(QMouseEvent* inEvent)
{
	if (mIsSelectMode && mIsSelection)
	{
		mRubberBand.setGeometry(QRect(mSelectionStart, inEvent->pos()).normalized());
	}

	QGraphicsView::mouseMoveEvent(inEvent);
}

void ImageView::mouseReleaseEvent(QMouseEvent* inEvent)
{
	if( mIsSelectMode )
		mIsSelection = false;
	QGraphicsView::mouseReleaseEvent(inEvent);
}