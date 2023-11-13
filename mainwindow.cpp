#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set up qcustom to plot
    ui->customplot->addGraph();
    ui->customplot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customplot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customplot->xAxis->setLabel("X-axis");
    ui->customplot->yAxis->setLabel("Y-axis");
    ui->customplot->xAxis->setRange(0,100);
    ui->customplot->yAxis->setRange(0,100);
    ui->customplot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->customplot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    QVector<double> x(251), y(251);
    for(int i=4;i<251;i++)
    {
        x[i] = i;
        y[i] = i;
    }
    ui->customplot->graph(0)->setData(x, y);



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
        qDebug()<<"Serial port is connected.";
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
}

