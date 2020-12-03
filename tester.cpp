#include "tester.h"
#include <QDebug>
tester::tester(QObject *parent) : QObject(parent)
{
    inspbc = new InspBluetoothController();
    connect(inspbc, SIGNAL(isDeviceScanningComplete(QVector<bluetoothDevices>)), this, SLOT(scanoverwith(QVector<bluetoothDevices>)));
    connect(inspbc, SIGNAL(isDevicePaired(int)), this, SLOT(devicepaired(int)));
    connect(inspbc, SIGNAL(isDeviceConnected(int)), this, SLOT(deviceconnected(int)));
    emit inspbc->startBTDiscovery();
}
void tester::scanoverwith(QVector<bluetoothDevices> btdevice)
{
    //emit inspbc->ConnectToDevice(QBluetoothAddress("04:FE:A1:86:52:79"));
}
void tester::deviceconnected(int connected)
{
    if(connected == 0)
    {
        qDebug() <<"Device Connectecd";
    }
    if(connected == 1)
    {
        qDebug() << "Device Not Connected";
    }
}
void tester::devicepaired(int paired)
{
    if(paired == 0)
    {
        qDebug() <<"Device paired";
    }
    if(paired == 1)
    {
        qDebug() << "Device Not paired";
    }
}
