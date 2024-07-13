#include <QCoreApplication>
#include <iostream>
#include <QApplication>
#include "mainwindow.h"
#include "spdlog/spdlog.h"



int main(int argc, char* argv[])
{
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    QApplication app(argc, argv);
    mainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
