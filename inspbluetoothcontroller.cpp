#include "inspbluetoothcontroller.h"
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <QProcess>
#include <QDebug>
InspBluetoothController::InspBluetoothController(QObject *parent) : QObject(parent)
{

    localDevice = new QBluetoothLocalDevice(this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    DiscoveredDevices = new QVector<QBluetoothDeviceInfo>;
    bluetoothProcess = new QProcess(this);
    bluetoothProcess->setProgram("bluetoothctl");
    bluetoothProcess->start();
    bluetoothProcess->waitForStarted(30);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()),this, SLOT(finishedBTDiscovery()));

    if(localDevice->isValid())
    {
        localDevice->powerOn();
        localDeviceName = localDevice->name();
        qDebug() << "********** LOCAL BLUETOOTH DEVICE NAME: " << localDeviceName << " **********";
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }
}

void InspBluetoothController::startBTDiscovery()
{
    discoveryAgent->start();
}
void InspBluetoothController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if(device.AudioService){
        DiscoveredDevices->append(device);
    }

}
void InspBluetoothController::finishedBTDiscovery()
{
    qDebug() << "********** DISPLAYING FOUND DEVICES **********";
    for(int x =0; x < DiscoveredDevices->count(); x ++){
        qDebug() << x << ": " << DiscoveredDevices->at(x).name() << " " << DiscoveredDevices->at(x).address();
    }
}

void InspBluetoothController::ConnectToDevice(QString deviceAddress)
{
    qDebug() << "********** ATTEMPTING PAIR AND CONNECTION **********";
    QString bluetoothProcess_stdout, bluetoothProcess_stderr;
    bluetoothProcess->write(QString("pair %1\n").arg(deviceAddress).toUtf8());
    bluetoothProcess->waitForFinished(30);
    bluetoothProcess_stdout = bluetoothProcess->readAllStandardOutput();
    bluetoothProcess_stderr = bluetoothProcess->readAllStandardError();
    if(bluetoothProcess_stdout.contains("not available"))
    {
        qDebug() << "Device Can Not Be Found!";
    }
    if(bluetoothProcess_stdout.contains("Paring Successful") || bluetoothProcess_stdout.contains("AlreadyExists"))
    {
        qDebug() << "Device Has Been Paired Successfully";
        bluetoothProcess->write(QString("connect %1\n").arg(deviceAddress).toUtf8());
        bluetoothProcess->waitForFinished(30);
        bluetoothProcess_stdout = bluetoothProcess->readAllStandardOutput();
        bluetoothProcess_stderr = bluetoothProcess->readAllStandardError();
        if(bluetoothProcess_stdout.contains("not available"))
        {
            qDebug() << "Device Can Not Be Found";
            IsBluetoothDeviceConnected = false;
        }
        if(bluetoothProcess_stdout.contains("Connection Successful"))
        {
           qDebug() << "Device Has Been Connected";
           IsBluetoothDeviceConnected = true;
        }
    }
}
void InspBluetoothController::DisconnectFromDevice(QString deviceAddress)
{
    QString bluetoothProcess_stdout, bluetoothProcess_stderr;
    bluetoothProcess->write(QString("disconnect %1\n").arg(deviceAddress).toUtf8());
    bluetoothProcess->waitForFinished(30);
    bluetoothProcess_stdout = bluetoothProcess->readAllStandardOutput();
    bluetoothProcess_stderr = bluetoothProcess->readAllStandardError();
}
