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
    void encryptPDFFiles(const QString &pdfFile);

private:
    Ui::mainWindow *ui;
    QStandardItemModel *model;
    QMessageBox *msgBox;
    QString outputDir;
    QString keys = "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAKlqprKhrrP5jfOFt6QAd/wv54EY+EOx+AYws4qDbbKQNj80rDNB1+1MJq+/H1rK53hJZr64SpympFc878EzzWXaSWMo96L43yJAa++dXVehrlsYthQr3LrmLx4mOhMnkeVpZzfdg9GQF5KbH+pzmtY+oKG1mu1H92VWiejRYQfzAgMBAAECgYACdVEIi+MMwiIGqytGIYdtoVE6PBl5ZIlLBJxYgZgpTeofOx7uLpBb8ROGcwq77h75E3FnW1RoAq52m5jL67/TWQAPNDY/29WIRonuLgygALBodtrqRfKt7qK+eHpjqGHmmKb8gHZszs1roed+jNRmFNNv4DmI2c8f9DNTQwfdvQJBALLfAETxkT4HmwACQtbFplsm/XmLzENieyvCoe1JlQQgTQ1fbZ/35jkMiYJ6tdbFuNJ2N+q1PPK2JPSPUlj7nM0CQQDyd//fjsTNB7wxK/4z9Cybvk2VDbX2orwvwtvNnS/M89vH1he/ezRw3KPWX/7xFRY9iiV1AkDA8VZk8Diq0ze/AkEAlD7WTk2Xfr127S7yjmYJVgqekZoXlNGOG9qXsb4GTwEmWyr4BFjE6gtF/GoPQw1j/KoV9eR3uwDS6A3QxgbjEQJBAOcPDaoHzmkoJuGg78Soul3MUvhosW+QEs9c3MOrOCMSe+2KiuTxTr8Cs6qVnkg1dRU98YX4mKaFFXRpK0c2stcCQH8usBqkVmRUYSQirIurzVDv9funv6dwCEWVGANXAGWlYb75UrKUtFE0l1N3jt5zIQ3Pek8QV+fdX+JT6HX5pTQ=";


private slots:
    void on_selectfiles_clicked();
    void on_encryption_clicked();
    void on_outputdirectory_clicked();

protected:
    void closeEvent(QCloseEvent *event) override {
        QWidget::closeEvent(event);
    }
};

#endif // MAINWINDOW_H
