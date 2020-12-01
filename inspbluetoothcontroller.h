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
    int AutoIncrementingIDNumber = 0;
    struct bluetoothDevices
    {
        int id;
        QString bluetoothName;
        QBluetoothAddress bluetoothAddress;
        QBluetoothLocalDevice::Pairing bluetoothParingStatus; // 0 = unpaired 1= paired 2= paired and trusted
    };
public:
    explicit InspBluetoothController(QObject *parent = nullptr);
    QVector<bluetoothDevices> *DiscoveredDevices;
    bool IsBluetoothDeviceConnected = false;

signals:
    void isDeviceConnected(bool ConnectionStatus);
    void isDevicePaired(bool PairedStatus);
    void isDeviceScanningComplete(QVector<bluetoothDevices> btDevices);

public slots:
    void finishedBTDiscovery();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void startBTDiscovery();
    void DisconnectFromDevice(bluetoothDevices btDevice);
    void PairToDevice(bluetoothDevices btDevice);
    void ConnectToDevice(bluetoothDevices btDevice);
};

#endif // INSPBLUETOOTHCONTROLLER_H
