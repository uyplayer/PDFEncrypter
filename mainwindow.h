#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>

QT_BEGIN_NAMESPACE

namespace Ui {
    class mainWindow;
}

QT_END_NAMESPACE

class mainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = nullptr);

    mainWindow(const mainWindow &other) = delete;

    mainWindow &operator =(const mainWindow &other) = delete;

    ~mainWindow() override;

private:
    Ui::mainWindow *ui;
    QStandardItemModel *model;
    QMessageBox *msgBox;
    QString outputDir;


private slots:
    void on_selectfiles_clicked();

    void on_encryption_clicked();

protected:
    void closeEvent(QCloseEvent *event) override {
        QWidget::closeEvent(event);
    }
};

#endif // MAINWINDOW_H
