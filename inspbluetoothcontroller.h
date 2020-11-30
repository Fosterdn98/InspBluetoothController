#ifndef INSPBLUETOOTHCONTROLLER_H
#define INSPBLUETOOTHCONTROLLER_H

#include <QObject>
#include <QVector>

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QProcess>

class InspBluetoothController : public QObject
{
    Q_OBJECT
private:
    QBluetoothLocalDevice *localDevice;
    QString localDeviceName;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QProcess *bluetoothProcess;
public:
    explicit InspBluetoothController(QObject *parent = nullptr);
    QVector<QBluetoothDeviceInfo> *DiscoveredDevices;
    void startBTDiscovery();
    void ConnectToDevice(QString deviceAddress);
    void DisconnectFromDevice(QString deviceAddress);
    bool IsBluetoothDeviceConnected = false;

signals:

public slots:
    void finishedBTDiscovery();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
};

#endif // INSPBLUETOOTHCONTROLLER_H
