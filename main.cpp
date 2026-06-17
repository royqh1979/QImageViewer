#include "mainwindow.h"
#include "settings.h"

#include <QApplication>
#include <QImageReader>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QImageViewer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    QStringList appLocations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    QDir dataDir{appLocations.first()};
    auto settings = std::make_shared<Settings>(dataDir.filePath("config.ini"));
    pSettings = settings.get();
    settings->load();
    MainWindow w;
    w.show();
    int code = a.exec();
    settings->save();
    return code;
}
