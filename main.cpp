#include <QCoreApplication>
#include <iostream>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello PDFGlimpseManager !" << std::endl;
    std::cout << "Project dir "<< PROJECT_DIR   << std::endl;

    QApplication app(argc, argv);
    mainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
