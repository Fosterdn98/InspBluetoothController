#include "inspbluetoothcontroller.h"
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <QProcess>
#include <QDebug>
InspBluetoothController::InspBluetoothController(QObject *parent) : QObject(parent)
{

    localDevice = new QBluetoothLocalDevice(this); ///Creates Instance Of Local Bluetooth Device.
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this); ///Creates Instance Of Local Device Discovery Agent For Scanning
    DiscoveredDevices = new QVector<bluetoothDevices>; ///QVectory Of the bluetooth devices struct to store all scanned devices in
    bluetoothProcess = new QProcess(this); ///Creation of QProccess to be used for bluetooth connection and disconnection
    bluetoothProcess->setProgram("bluetoothctl"); ///binding created qprocess to bluetoothctl command line

    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo))); ///binding discovery agent when device is found to function for found devices
    connect(discoveryAgent, SIGNAL(finished()),this, SLOT(finishedBTDiscovery())); /// binding discovery agent when finished to finished scanning function

    if(localDevice->isValid()) ///checking to make sure the device program is being run on has a bluetooth chip
    {
        localDevice->powerOn(); ///turning on devices bluetooth chip
        localDeviceName = localDevice->name(); ///getting name of local devices bluetooth
        qDebug() << "********** LOCAL BLUETOOTH DEVICE NAME: " << localDeviceName << " **********";
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable); ///setting local devices bluetooth chip to discoverable and host
    }
}
/** This function is used to start the bluetooth discovery agent for scanning of avalible bluetooth devices.
 *
 */
void InspBluetoothController::startBTDiscovery()
{
    discoveryAgent->start();
}
/** This function is called when the bluetooth scanner finds a device and filters the found
 * devices for specific audio devices only.
 *
 * This Function is called on bluetooth scanner found devices. When the scanner find the device it passes
 * to this function a QBluetoothDeviceInfo which has all found info about the device. This info is then ran
 * through a if statment that determins if the device is a Audio device or other. If found device is a Audio device
 * it is then added to the DiscoveredDevices QVector by adding the required infromation to the BluetoothDevices Struct
 *
 * BluetoothDevices Struct has the  following
 * ID: this should be a AutoIncrementing Number
 * Device Name: This is a QString
 * Device Address: This is a QBluetoothAddress
 * Paring Status: This is a QBluetoothLocalDevice::Pairing where as 0= unpaired 1=paired 2=paired and trusted
 */
void InspBluetoothController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if(device.serviceClasses().testFlag(QBluetoothDeviceInfo::AudioService)){
        qDebug() << "Name: " << device.name() << "Class: " << device.serviceClasses();
        DiscoveredDevices->append(bluetoothDevices{AutoIncrementingIDNumber, device.name(), device.address(), localDevice->pairingStatus(device.address())});
        AutoIncrementingIDNumber ++;
    }

}

/** This function is called when the scanner has completed it scanning.
 *
 * This function gets called when the scanner has finished scanning. This
 * function then emits a signal with the devices that it has stored in DiscoveredDevices
 */
void InspBluetoothController::finishedBTDiscovery()
{
    emit isDeviceScanningComplete(*DiscoveredDevices);
    qDebug() << "********** DISPLAYING FOUND DEVICES **********";
    for(int x =0; x < DiscoveredDevices->count(); x ++){
        qDebug() << x << ": " << DiscoveredDevices->at(x).bluetoothName<< " " << DiscoveredDevices->at(x).bluetoothAddress;
    }
}
/** This function is used to pair bluetooth devices.
 * @param btDevice is a BluetoothDevice Struct
 *
 * This function is used to pair bluetooth devices using the command line tools that
 * have been initalized. It takes the counsole out and console errors into QStrings to be read.
 * we run a pair command on the command line via bluetoothProcess->write which will pair the device
 * passed as a paramater. We then pause for a short period of time to make sure the command has run.
 * We read the command line outputs to make sure the device has successfully paired.
 * If device has paired successfully we emmit a signal isDevicePaired with a true flag.
 * If device does not pair successfully we emmit a signal isDevicePaired with a false flag.
 */
void InspBluetoothController::PairToDevice(QBluetoothAddress btDeviceAddress)
{
    qDebug() << "********** ATTEMPTING PAIRING **********";
    QString bluetoothProcess_stdout, bluetoothProcess_stderr;
    bluetoothProcess->start(); ///starting the proccess and attaching to bluetoothctl for later use
    bluetoothProcess->waitForStarted(); ///holding any processes for 20 micro seconds to make sure binding is complete
    bluetoothProcess->write(QString("pair %1\n").arg(btDeviceAddress.toString()).toUtf8());
    bluetoothProcess->waitForFinished(30);
    bluetoothProcess_stdout = bluetoothProcess->readAllStandardOutput();
    bluetoothProcess_stderr = bluetoothProcess->readAllStandardError();
    qDebug() << bluetoothProcess_stdout;
    qDebug() << bluetoothProcess_stderr;
    if(bluetoothProcess_stdout.contains("not available"))
    {
        qDebug() << "Device Can Not Be Found!";
        emit isDevicePaired(1);
        bluetoothProcess->close();
    }
    if(bluetoothProcess_stdout.contains("Paring Successful") || bluetoothProcess_stdout.contains("Already Exists"))
    {
        qDebug() << "Device Has Been Paired Successfully";
        emit isDevicePaired(0);
        bluetoothProcess->close();
        //btDevice.bluetoothParingStatus = QBluetoothLocalDevice::Paired;
    }
}
/** This function is used to Connect bluetooth devices.
 * @param btDevice is a BluetoothDevice Struct
 *
 * This function is used to pair bluetooth devices using the command line tools that
 * have been initalized. It takes the counsole out and console errors into QStrings to be read.
 * we run a connec command on the command line via bluetoothProcess->write which will connect the device
 * passed as a paramater. We then pause for a short period of time to make sure the command has run.
 * We read the command line outputs to make sure the device has successfully connected.
 * If device has connected successfully we emmit a signal isDeviceConnected with a true flag.
 * If device does not connected successfully we emmit a signal isDeviceConnected with a false flag.
 */
void InspBluetoothController::ConnectToDevice(QBluetoothAddress btDeviceAddress)
{
    qDebug() << "********** ATTEMPTING CONNECTION **********";
    QString bluetoothProcess_stdout, bluetoothProcess_stderr;
    bluetoothProcess->start(); ///starting the proccess and attaching to bluetoothctl for later use
    bluetoothProcess->waitForStarted(); ///holding any processes for 20 micro seconds to make sure binding is complete
    bluetoothProcess->write(QString("connect %1 \n").arg(btDeviceAddress.toString()).toUtf8());
    bluetoothProcess->waitForFinished();
    bluetoothProcess_stdout = bluetoothProcess->readAllStandardOutput();
    bluetoothProcess_stderr = bluetoothProcess->readAllStandardError();
    qDebug() << bluetoothProcess_stdout;
    qDebug() << bluetoothProcess_stderr;
    if(bluetoothProcess_stdout.contains("not available"))
    {
        qDebug() << "Device Can Not Be Found";
        emit isDeviceConnected(1);
        bluetoothProcess->close();
    }
    else if(bluetoothProcess_stdout.contains("Connection successful"))
    {
       qDebug() << "Device Has Been Connected";
       emit isDeviceConnected(0);
       bluetoothProcess->close();
    }
}
/** This function is used to disconnect bluetooth devices.
 * @param btDevice is a BluetoothDevice Struct
 *
 * This function is used to disconnect bluetooth devices using the command line tools that
 * have been initalized. It takes the counsole out and console errors into QStrings to be read.
 * we run a disconnect command on the command line via bluetoothProcess->write which will disconnect the device
 * passed as a paramater. We then pause for a short period of time to make sure the command has run.
 * We read the command line outputs to make sure the device has successfully disconnected.
 * If device has disconnected successfully we emmit a signal isDeviceConnected with a false flag.
 */
void InspBluetoothController::DisconnectFromDevice(bluetoothDevices btDevice)
{
    QString bluetoothProcess_stdout, bluetoothProcess_stderr;
    bluetoothProcess->write(QString("disconnect %1\n").arg(btDevice.bluetoothAddress.toString()).toUtf8());
    bluetoothProcess->waitForFinished(30);
    bluetoothProcess_stdout = bluetoothProcess->readAllStandardOutput();
    bluetoothProcess_stderr = bluetoothProcess->readAllStandardError();
    emit isDeviceConnected(false);
}
