#include "mathocr.h"
#include <QtWidgets/QApplication>
#include <QSettings>
#include "SettingsKeys.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("BSU");
	QCoreApplication::setApplicationName("MathOCR");

	QSettings().setValue(SettingsKey::TEMP_PATH_KEY, SettingsKey::TEMP_PATH_DEFAULT);

	MathOCR w;
	w.showMaximized();
	return a.exec();
}
