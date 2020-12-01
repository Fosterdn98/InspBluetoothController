#include <QCoreApplication>
#include "inspbluetoothcontroller.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InspBluetoothController *inspbc = new InspBluetoothController();
    inspbc->startBTDiscovery();
    inspbc->ConnectToDevice(inspbc->DiscoveredDevices->at(1));
    //inspbc->DisconnectFromDevice("04:FE:A1:86:52:79");

    return a.exec();
}
