#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>
#include <QLibraryInfo>
#include <fstream>


int main(int argc, char *argv[])
{
//    QString executable = argv[0];
//    QString executablePath = executable.mid(0,executable.lastIndexOf("\\"));
//    QString installPathPlugins = QLibraryInfo::path(QLibraryInfo::PluginsPath);
//    QCoreApplication::removeLibraryPath(installPathPlugins);
//    QCoreApplication::addLibraryPath(installPathPlugins);
//    QCoreApplication::addLibraryPath(executablePath);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
