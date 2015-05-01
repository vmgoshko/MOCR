#include "mathocr.h"
#include <QtWidgets/QApplication>
#include <QSettings>
#include "SettingsKeys.h"
#include "Defs.h"

Configuration gConfiguration;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("BSU");
	QCoreApplication::setApplicationName("MathOCR");

	QSettings().setValue(SettingsKey::TEMP_PATH_KEY, SettingsKey::TEMP_PATH_DEFAULT);
	gConfiguration = { "../../libmocr/nn_skeleton_vector", LeafRecognizerType::TwoStepsRecognizer, NNToolsType::Skeleton, SkeletonType::Vector };

	MathOCR w;
	w.showMaximized();
	return a.exec();
}
