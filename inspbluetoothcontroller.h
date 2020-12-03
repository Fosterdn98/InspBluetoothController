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
public:
    struct bluetoothDevices
    {
        int id;
        QString bluetoothName;
        QBluetoothAddress bluetoothAddress;
        QBluetoothLocalDevice::Pairing bluetoothParingStatus; // 0 = unpaired 1= paired 2= paired and trusted
    };
    explicit InspBluetoothController(QObject *parent = nullptr);
    QVector<bluetoothDevices> *DiscoveredDevices;
    bool IsBluetoothDeviceConnected = false;

signals:
    /** 0= connected or paired
    ///  1= Error: Device Not Found
    ///  2= Error: Could Not Start Service
    ///  3= Error: Device Already Connected
    ///  4= Error: Device Not Paired
    */
    void isDeviceConnected(int connectionStatus);
    void isDevicePaired(int pairedStatus);
    void isDeviceScanningComplete(QVector<bluetoothDevices> btDevices);

public slots:
    void finishedBTDiscovery();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void startBTDiscovery();
    void DisconnectFromDevice(bluetoothDevices btDevice);
    void PairToDevice(QBluetoothAddress btDeviceAddress);
    void ConnectToDevice(QBluetoothAddress btDeviceAddress);
};

#endif // INSPBLUETOOTHCONTROLLER_H
