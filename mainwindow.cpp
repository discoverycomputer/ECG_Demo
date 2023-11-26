#include "mainwindow.h"
#include "ui_mainwindow.h"
int count;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,count(0.0),
    i(0)
{
    ui->setupUi(this);
    //set up qcustom to plot
    ui->customplot->addGraph();
    ui->customplot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customplot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);

    x.resize(501);
    y.resize(501);


    //config serialPort
    COMPORT = new QSerialPort();
    COMPORT->setPortName("COM5");
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud9600);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT->open(QIODevice::ReadWrite);
    //checking if connection is fine
    if(COMPORT->isOpen())
    {
        qDebug()<<"Serial port is connected completely.";
    }
    else
    {
        qDebug()<<"Serial port is not connected.";
        qDebug() << COMPORT->error();
    }
    connect(COMPORT,SIGNAL(readyRead()),this,SLOT(Read_Data()));
    // setting up qtcustom to plot
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Read_Data()
{
    if(COMPORT->isOpen())
    {
        while(COMPORT->bytesAvailable())
        {
            Data_From_SerialPort +=COMPORT->readAll();
        }
        if (Data_From_SerialPort.at(Data_From_SerialPort.length()-1)== char(10))
        {
            IS_Data_Received = true;
        }
    }
    if(IS_Data_Received == true)
    {
        qDebug() <<"Data from serial port:   " << Data_From_SerialPort;
        ui->textEdit->append(Data_From_SerialPort);
        z = Data_From_SerialPort.toInt();
        qDebug() <<"Data transform:   " << z;
        Data_From_SerialPort = "";
        IS_Data_Received = false;
    }
    qDebug() <<"i = "<<i;
    x[i] = count;
    y[i] = z;
    i++;
    count = count+1.0;
    qDebug() <<"x =  "<<count;
    ui->customplot->graph(0)->setData(x,y);
    ui->customplot->rescaleAxes();
    ui->customplot->replot();
    ui->customplot->update();
    if(i == 20)
    {
        i = 0;
        count = 0;
        //ui->customplot->graph(0)->data()->clear();
        ui->customplot->xAxis->setRange(count,count+10);
        ui->customplot->replot();

    }
}

