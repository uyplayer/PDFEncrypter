#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("PDF Viewer Manager");
    this->setWindowIcon(QIcon(":/icon/icon/app_books.png"));
    ui->uuid->setPlaceholderText("Please Input User's UUID");
    ui->uuid->setStyleSheet("QLineEdit { border: 1px solid #000; border-radius: 5px; padding: 0 8px; }");

    // 设置 selectfiles 按钮的样式、图标和提示
    ui->selectfiles->setStyleSheet("QPushButton {"
        "   background-color: #cccccc;"
        "   border: 1px solid #000;"
        "   border-radius: 5px;"
        "   padding: 0 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #dddddd;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #aaaaaa;"
        "}");
    ui->selectfiles->setToolTip("Select PDF Files");
    ui->selectfiles->setIcon(QIcon(":/icon/icon/pdf.png"));
    ui->selectfiles->setIconSize(QSize(30, 30));

    // 设置 encryption 按钮的样式、图标和提示
    ui->encryption->setStyleSheet("QPushButton {"
        "   background-color: #cccccc;"
        "   border: 1px solid #000;"
        "   border-radius: 5px;"
        "   padding: 0 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #dddddd;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #aaaaaa;"
        "}");
    ui->encryption->setToolTip("Encrypt PDF Files");
    ui->encryption->setIcon(QIcon(":/icon/icon/encryption.png"));
    ui->encryption->setIconSize(QSize(30, 30));

    // 创建并设置 QStandardItemModel 给 QListView
    model = new QStandardItemModel(this);
    ui->listView->setModel(model);

    // 设置 QListView 的样式
    ui->listView->setStyleSheet("QListView {"
        "    background-color: white;"
        "    border: 1px solid #CCCCCC;"
        "}"
        "QListView::item {"
        "    padding: 5px;"
        "    margin: 2px;"
        "}"
        "QListView::item:selected {"
        "    color: white;"
        "}");
    msgBox = new QMessageBox(this);
    msgBox->setStyleSheet("QMessageBox {"
                             "    background-color: #f0f0f0;"
                             "    border: 2px solid darkkhaki;"
                             "}"
                             "QMessageBox QLabel {"
                             "    color: #333333;"
                             "}"
                             "QMessageBox QPushButton {"
                             "    background-color: #cccccc;"
                             "    border: 1px solid #999999;"
                             "    padding: 5px;"
                             "    margin: 5px;"
                             "}"
                             "QMessageBox QPushButton:hover {"
                             "    background-color: #dddddd;"
                             "}");


    ui->label->setStyleSheet("QLabel { background-color : #cccccc; border: 0px; padding: 2px; }");
    ui->label->hide();

    ui->outputDirLabel->setStyleSheet("QLabel { border: 0px; padding: 5px; margin: 5px; }");
    ui->outputDirLabel->setWordWrap(true);
    ui->outputDirLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);


}

mainWindow::~mainWindow() {
    delete ui;
    delete model;
    delete msgBox;
}

void mainWindow::on_selectfiles_clicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          tr("Select PDF Files"), QDir::homePath(), tr("PDF Files (*.pdf)"));
    if (!fileNames.isEmpty()) {
        model->clear();
        foreach(QString fileName, fileNames) {
            auto *item = new QStandardItem(fileName);
            // if item is not pdf
            if (!fileName.endsWith(".pdf")) {
                msgBox->warning(this, "Warning", fileName + " is not a PDF file");
                model->clear();
                return ;
            }
            item->setEditable(false);
            model->appendRow(item);
        }
    }
}

void mainWindow::on_encryption_clicked() {
    if (ui->uuid->text().isEmpty()) {
        msgBox->warning(this, "Warning", "Please Input User's UUID");
        return;
    }
    if (model->rowCount() == 0) {
        msgBox->warning(this, "Warning", "Please Select PDF Files");
        return;
    }

     outputDir = QFileDialog::getExistingDirectory(this,
        tr("Encrypted PDF files output directory"), QDir::homePath());

    if (outputDir.isEmpty()) {
        msgBox->warning(this, "Warning", "Please Select Encrypted PDF files output directory");
        return;
    }
    std::cout << "User's UUID: " << outputDir.toStdString() << std::endl;

    ui->outputDirLabel->setText(outputDir);
    ui->label->show();


}
