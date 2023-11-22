#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <Qstring>
#include <qvector.h>
#include <QDateTime>
class QLabel;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void Read_Data();

private:
    Ui::MainWindow *ui;
    QSerialPort* COMPORT;
    QString Data_From_SerialPort;
    bool IS_Data_Received = false;
    int z;
    QLabel *label;
    double count;
    int i;
    QVector<double> x, y;
};
#endif // MAINWINDOW_H
