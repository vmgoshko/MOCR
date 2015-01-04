#include <QPixmap>
#include <QLabel>
#include <QWidget>

class ObjectItem
{
	QPixmap mBlackObject;
	QPixmap mSkeleton;
	QString mPossibleChars;
};

Q_DECLARE_METATYPE(ObjectItem*);

class ObjectListItemWidget : public QWidget
{
	Q_OBJECT
public:
	ObjectListItemWidget(QWidget* parent = 0);
private:
	QPixmap* mBlackObject;
	QPixmap* mSkeleton;
	QLabel* mPossibleChars;
};