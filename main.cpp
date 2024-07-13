#include <QCoreApplication>
#include <iostream>
#include <QApplication>
#include "mainwindow.h"
#include "spdlog/spdlog.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    mainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
