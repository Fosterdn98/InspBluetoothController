#ifndef TESTER_H
#define TESTER_H

#include <QObject>
#include <QVector>
#include "inspbluetoothcontroller.h"
class tester : public QObject
{
    Q_OBJECT
public:
    explicit tester(QObject *parent = nullptr);
    struct bluetoothDevices
    {
        int id;
        QString bluetoothName;
        QBluetoothAddress bluetoothAddress;
        QBluetoothLocalDevice::Pairing bluetoothParingStatus; // 0 = unpaired 1= paired 2= paired and trusted
    };

    InspBluetoothController *inspbc;
signals:

public slots:
    void scanoverwith(QVector<bluetoothDevices> btdevice);
    void devicepaired(int paired);
    void deviceconnected(int connected);

};

#endif // TESTER_H
