#include "mainwindow.h"

#include <QApplication>

#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load Centralized Stylesheet
    QFile styleFile(":/resources/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        a.setStyleSheet(style);
        styleFile.close();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
