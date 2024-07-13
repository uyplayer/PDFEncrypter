#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <QThread>
#include <QCryptographicHash>
#include <QFileInfo>

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


    // 输出按钮
    ui->outputdirectory->setStyleSheet("QPushButton {"
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
    ui->outputdirectory->setToolTip("Encrypted PDF Files Output Directory");
    ui->outputdirectory->setIcon(QIcon(":/icon/icon/Output.png"));
    ui->outputdirectory->setIconSize(QSize(30, 30));

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


    ui->process_lebel->setStyleSheet("QLabel { border: 0px; padding: 5px; margin: 5px; }");
    ui->process_lebel->setWordWrap(true);
    ui->process_lebel->setAlignment(Qt::AlignTop | Qt::AlignCenter);
}

mainWindow::~mainWindow() {
    delete ui;
    delete model;
    delete msgBox;
}

void mainWindow::on_selectfiles_clicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          tr("Select PDF Files"), QDir::homePath(),
                                                          tr("PDF Files (*.pdf)"));
    if (!fileNames.isEmpty()) {
        model->clear();
        foreach(QString fileName, fileNames) {
            auto *item = new QStandardItem(fileName);
            // if item is not pdf
            if (!fileName.endsWith(".pdf")) {
                msgBox->warning(this, "Warning", fileName + " is not a PDF file");
                model->clear();
                return;
            }
            item->setEditable(false);
            model->appendRow(item);
        }
    }
    if (model->rowCount() == 0) {
        ui->process_lebel->clear();
        ui->process_lebel->setText("No PDF files selected");
    } else {
        ui->process_lebel->clear();
        ui->process_lebel->setText(QString::number(model->rowCount()) + " PDF files Selected");
    }
}

void mainWindow::on_outputdirectory_clicked() {
    ui->outputDirLabel->clear();
    ui->label->hide();

    outputDir = QFileDialog::getExistingDirectory(this,
                                                  tr("Encrypted PDF files output directory"), QDir::homePath());
    if (outputDir.isEmpty()) {
        return;
    }
    ui->outputDirLabel->setText(outputDir);
    ui->label->show();

    ui->process_lebel->clear();
    ui->process_lebel->setText("output directory : " + outputDir + " is selected");
}


void mainWindow::on_encryption_clicked() {
    ui->process_lebel->clear();
    if (ui->uuid->text().isEmpty()) {
        msgBox->warning(this, "Warning", "Please Input User's UUID");
        return;
    }
    if (model->rowCount() == 0) {
        msgBox->warning(this, "Warning", "Please Select PDF Files");
        return;
    }

    if (outputDir.isEmpty()) {
        msgBox->warning(this, "Warning", "Please Select Encrypted PDF files output directory");
        return;
    }

    ui->process_lebel->setText("start to encrypt PDF files");

    ui->selectfiles->setEnabled(false);
    ui->encryption->setEnabled(false);
    ui->outputdirectory->setEnabled(false);
    // 整个窗口禁用
    this->setEnabled(false);
    // start to encrypt pdf files
    for (int i = 0; i < model->rowCount(); i++) {
        QString pdfFile = model->item(i)->text();
        QString fileName = QFileInfo(pdfFile).fileName();
        ui->process_lebel->setText("Encrypting " + fileName + " ...");
        encryptPDFFiles(pdfFile);
    }
    ui->selectfiles->setEnabled(true);
    ui->encryption->setEnabled(true);
    ui->outputdirectory->setEnabled(true);
    this->setEnabled(true);
    msgBox->information(this, "Information", "PDF files encrypted successfully");
}

// xor 加密
void xorEncryptChunk(QByteArray &chunk, const QByteArray &key, int chunkIndex) {
    int keySize = key.size();
    for (int i = 0; i < chunk.size(); ++i) {
        chunk[i] = chunk[i] ^ key[(i + chunkIndex) % keySize];
    }
}

void mainWindow::encryptPDFFiles(const QString &pdfFile) {
    // 拼接 UUID 和 keys
    QString new_uuid = ui->uuid->text() + keys;
    if (new_uuid.isEmpty()) {
        msgBox->warning(this, "Warning", "UUID is empty");
        return;
    }
    QFile file(pdfFile);
    if (!file.open(QIODevice::ReadOnly)) {
        msgBox->warning(this, "Warning", "Cannot open PDF file for reading");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QByteArray key = QCryptographicHash::hash(new_uuid.toUtf8(), QCryptographicHash::Sha256);

    int chunkSize = 1024 * 1024;
    int numChunks = fileData.size() / chunkSize + (fileData.size() % chunkSize == 0 ? 0 : 1);
    std::vector<QByteArray> chunks(numChunks);
    for (int i = 0; i < numChunks; ++i) {
        chunks[i] = fileData.mid(i * chunkSize, chunkSize);
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < numChunks; ++i) {
        threads.emplace_back(xorEncryptChunk, std::ref(chunks[i]), std::ref(key), i * chunkSize);
    }
    for (auto &t: threads) {
        t.join();
    }

    QByteArray encryptedData;
    for (const auto &chunk: chunks) {
        encryptedData.append(chunk);
    }
    std::string new_pdf = outputDir.toStdString() + "/" + QFileInfo(pdfFile).fileName().toStdString();
    QFile newFile(QString::fromStdString(new_pdf));
    if (!newFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        msgBox->warning(this, "Warning", "Cannot open new PDF file for writing");
        return;
    }
    newFile.write(encryptedData);
    newFile.close();
}
