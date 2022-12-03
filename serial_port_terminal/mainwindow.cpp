#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
QSerialPort serialPort;
QSerialPortInfo info;
QString buffer;
QString carriage_return = "\r\n";
QString delimiter_text;
int delimiter_textSize;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //checkbox
    ui->checkBox->setCheckState(Qt::Checked);

    // Ports
    refresh_ports();

    // Baud Rate Ratios
    QList<qint32> baudRates = info.standardBaudRates();
    QList<QString> stringBaudRates;
    for(int i = 0 ; i < baudRates.size() ; i++){
        stringBaudRates.append(QString::number(baudRates.at(i)));
    }

    ui->comboBox_baud_rate->addItems(stringBaudRates);
    ui->comboBox_baud_rate->setCurrentIndex(12);

    // Data Bits
    ui->comboBox_no_data_bits->addItem("5");
    ui->comboBox_no_data_bits->addItem("6");
    ui->comboBox_no_data_bits->addItem("7");
    ui->comboBox_no_data_bits->addItem("8");
    ui->comboBox_no_data_bits->setCurrentIndex(3);

    // Stop Bits
    ui->comboBox_no_of_stop_bits->addItem("1 Bit");
    ui->comboBox_no_of_stop_bits->addItem("1.5 Bits");
    ui->comboBox_no_of_stop_bits->addItem("2 Bits");

    // Parities
    ui->comboBox_Parity->addItem("No Parity");
    ui->comboBox_Parity->addItem("Even Parity");
    ui->comboBox_Parity->addItem("Odd Parity");
    ui->comboBox_Parity->addItem("Mark Parity");
    ui->comboBox_Parity->addItem("Space Parity");

    //Flow Controls
    ui->comboBox_Flow_control->addItem("No Flow Control");
    ui->comboBox_Flow_control->addItem("Hardware Flow Control");
    ui->comboBox_Flow_control->addItem("Software Flow Control");
}

MainWindow::~MainWindow()
{
    serialPort.close();
    delete ui;
}


void MainWindow::on_connect_button_clicked()
{
    QString portName = ui->comboBox_Port->currentText();
    serialPort.setPortName(portName);

    serialPort.open(QIODevice::ReadWrite);

    if(!serialPort.isOpen()){
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append(">> !!!! Something went Wrong !!!!");
    }
    else {

        QString stringbaudRate = ui->comboBox_baud_rate->currentText();
        int intbaudRate = stringbaudRate.toInt();
        serialPort.setBaudRate(intbaudRate);

        QString dataBits = ui->comboBox_no_data_bits->currentText();
        if(dataBits == "5 Bits") {
           serialPort.setDataBits(QSerialPort::Data5);
        }
        else if((dataBits == "6 Bits")) {
           serialPort.setDataBits(QSerialPort::Data6);
        }
        else if(dataBits == "7 Bits") {
           serialPort.setDataBits(QSerialPort::Data7);
        }
        else if(dataBits == "8 Bits"){
           serialPort.setDataBits(QSerialPort::Data8);
        }

        QString stopBits = ui->comboBox_no_of_stop_bits->currentText();
        if(stopBits == "1 Bit") {
         serialPort.setStopBits(QSerialPort::OneStop);
        }
        else if(stopBits == "1.5 Bits") {
         serialPort.setStopBits(QSerialPort::OneAndHalfStop);
        }
        else if(stopBits == "2 Bits") {
         serialPort.setStopBits(QSerialPort::TwoStop);
        }

        QString parity = ui->comboBox_Parity->currentText();
        if(parity == "No Parity"){
          serialPort.setParity(QSerialPort::NoParity);
        }
        else if(parity == "Even Parity"){
          serialPort.setParity(QSerialPort::EvenParity);
        }
        else if(parity == "Odd Parity"){
          serialPort.setParity(QSerialPort::OddParity);
        }
        else if(parity == "Mark Parity"){
          serialPort.setParity(QSerialPort::MarkParity);
        }
        else if(parity == "Space Parity") {
            serialPort.setParity(QSerialPort::SpaceParity);
        }


        QString flowControl = ui->comboBox_Flow_control->currentText();
        if(flowControl == "No Flow Control") {
          serialPort.setFlowControl(QSerialPort::NoFlowControl);
        }
        else if(flowControl == "Hardware Flow Control") {
          serialPort.setFlowControl(QSerialPort::HardwareControl);
        }
        else if(flowControl == "Software Flow Control") {
          serialPort.setFlowControl(QSerialPort::SoftwareControl);
        }

        if(ui->checkBox->isChecked()){
            delimiter_text = carriage_return;
        }else{
            if(ui->delimiter->text().isEmpty()){
                delimiter_text =carriage_return;
            }else{
                delimiter_text = ui->delimiter->text();
            }
        }
        delimiter_textSize = delimiter_text.size();

        connect(&serialPort,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    }


}


void MainWindow::receiveMessage()
{

    QByteArray dataBA = serialPort.readAll();
    QString data(dataBA);
    buffer.append(data);
    int index = buffer.indexOf(delimiter_text);
    if(index != -1){
       QString message = buffer.mid(0,index);
       ui->textBrowser->setTextColor(Qt::blue); // Receieved message's color is blue.
       ui->textBrowser->append(">> "+message);
       buffer.remove(0,index+delimiter_textSize);
    }

}


void MainWindow::on_disconnect_button_clicked()
{
    serialPort.close();
}



void MainWindow::refresh_ports()
{
    ui->comboBox_Port->clear();
    QList<QSerialPortInfo> ports = info.availablePorts();
    QList<QString> stringPorts;
    for(int i = 0 ; i < ports.size() ; i++){
        stringPorts.append(ports.at(i).portName());
    }
    ui->comboBox_Port->addItems(stringPorts);
}

void MainWindow::on_refresh_ports_button_clicked()
{
    refresh_ports();
}



void MainWindow::on_clear_button_clicked()
{
    ui->textBrowser->clear();
}


void MainWindow::on_send_button_clicked()
{
    if(!serialPort.isOpen()){
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append(">> !!!! Something went Wrong !!!!");
    }else{
        QString message = ui->text_to_send->text();
        ui->textBrowser->setTextColor(Qt::darkGreen); // Color of message to send is green.
        ui->textBrowser->append(">> "+message);
        serialPort.write(message.toUtf8());
    }
}

