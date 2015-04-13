

// MergeCalTest
#include "Settings.h"

// RsaToolbox
#include "Log.h"
#include "Vna.h"
#include "Keys.h"
using namespace RsaToolbox;

// Qt
#include <QString>
#include <QApplication>
#include <QMessageBox>


bool isNoConnection(Vna &vna);
bool isUnknownModel(Vna &vna);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log log(LOG_FILENAME, APP_NAME, APP_VERSION);
    log.printHeader();

    Vna vna(CONNECTION_TYPE, INSTRUMENT_ADDRESS);
    vna.useLog(&log);
    vna.printInfo();

    Keys keys(KEY_PATH);

    if (isNoConnection(vna) || isUnknownModel(vna))
        return 0;


    vna.preset();
    vna.pause();
    vna.clearStatus();

    qDebug() << "Channel 1: Dummy data";
    VnaChannel channel1 = vna.channel(1);

    VnaLinearSweep sweep1 = channel1.linearSweep();
    sweep1.setStart(1, Giga);
    sweep1.setStop(2, Giga);
    sweep1.setSpacing(10, Mega);
    // Points: 101;

    Connector connector(Connector::N_50_OHM_CONNECTOR);
    VnaCalibrate cal1 = channel1.calibrate();
    cal1.setConnector(1, connector);
    cal1.setConnector(2, connector);

    cal1.start("Test",VnaCalibrate::CalType::Tosm, QVector<uint>() << 1 << 2);
    cal1.keepRawData();
    cal1.measureMatch(1);
    cal1.measureOpen(1);
    cal1.measureShort(1);
    cal1.measureMatch(2);
    cal1.measureOpen(2);
    cal1.measureShort(2);
    cal1.measureThru(1,2);
    cal1.apply();
    qDebug() << "is error? " << vna.isError();

    qDebug() << "Channel 2 dummy data";
    vna.createChannel(2);
    VnaChannel channel2 = vna.channel(2);

    VnaLinearSweep sweep2 = channel2.linearSweep();
    sweep2.setStart(2.01, Giga);
    sweep2.setStop(3, Giga);
    sweep2.setSpacing(10, Mega);
    // Points: 100;

    VnaCalibrate cal2 = channel2.calibrate();
    cal2.setConnector(1, connector);
    cal2.setConnector(2, connector);

    cal2.start("Test2",VnaCalibrate::CalType::Tosm, QVector<uint>() << 1 << 2);
    cal2.keepRawData();
    cal2.measureMatch(1);
    cal2.measureOpen(1);
    cal2.measureShort(1);
    cal2.measureMatch(2);
    cal2.measureOpen(2);
    cal2.measureShort(2);
    cal2.measureThru(1,2);
    cal2.apply();
    qDebug() << "is error? " << vna.isError();

    qDebug() << "Channel 1: Measure match on port 1";
    cal1.start("Test1", VnaCalibrate::CalType::Tosm, QVector<uint>() << 1 << 2);
    cal1.keepRawData();
    cal1.measureMatch(1);
    cal1.apply();
    qDebug() << "is error? " << vna.isError();

    qDebug() << "Channel 2: Measure match on port 1";
    cal2.start("Test1", VnaCalibrate::CalType::Tosm, QVector<uint>() << 1 << 2);
    cal2.keepRawData();
    cal2.measureMatch(1);
    cal2.apply();
    qDebug() << "is error? " << vna.isError();

    // Measure rest of the standards
    // ...

    qDebug() << "Channel 3: combine calibrations";
    vna.createChannel(3);
    VnaChannel channel3 = vna.channel(3);

    VnaLinearSweep sweep3 = channel3.linearSweep();
    sweep3.setStart(1, Giga);
    sweep3.setStop(3, Giga);
    sweep3.setSpacing(10, Mega);
    // Points: 201

    VnaCorrections corrections3 = channel3.corrections();
    corrections3.loadDefaultCorrections();
    qDebug() << "Load default corrections? " << !vna.isError();

    ComplexRowVector c1, c2;

    qDebug() << "Port 1,1";
    c1 = channel1.corrections().directivity(1,1);
    c2 = channel2.corrections().directivity(1,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setDirectivity(c1,1,1);
    qDebug() << "Directivity error? " << vna.isError();

    c1 = channel1.corrections().sourceMatch(1,1);
    c2 = channel2.corrections().sourceMatch(1,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setSourceMatch(c1,1,1);
    qDebug() << "Source match error? " << vna.isError();

    c1 = channel1.corrections().reflectionTracking(1,1);
    c2 = channel2.corrections().reflectionTracking(1,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setReflectionTracking(c1,1,1);
    qDebug() << "reflection tracking error? " << vna.isError();

    qDebug() << "Port 1,2";
    c1 = channel1.corrections().directivity(1,2);
    c2 = channel2.corrections().directivity(1,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setDirectivity(c1,1,2);
    qDebug() << "Directivity error? " << vna.isError();

    c1 = channel1.corrections().sourceMatch(1,2);
    c2 = channel2.corrections().sourceMatch(1,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setSourceMatch(c1,1,2);
    qDebug() << "Source match error? " << vna.isError();

    c1 = channel1.corrections().reflectionTracking(1,2);
    c2 = channel2.corrections().reflectionTracking(1,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setReflectionTracking(c1,1,2);
    qDebug() << "reflection tracking error? " << vna.isError();

    c1 = channel1.corrections().loadMatch(1,2);
    c2 = channel2.corrections().loadMatch(1,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setLoadMatch(c1,1,2);
    qDebug() << "Load match error? " << vna.isError();

    c1 = channel1.corrections().transmissionTracking(1,2);
    c2 = channel2.corrections().transmissionTracking(1,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setTransmissionTracking(c1,1,2);
    qDebug() << "Transmission tracking error? " << vna.isError();

    qDebug() << "Port 2,1";
    c1 = channel1.corrections().directivity(2,1);
    c2 = channel2.corrections().directivity(2,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setDirectivity(c1,2,1);
    qDebug() << "Directivity error? " << vna.isError();

    c1 = channel1.corrections().sourceMatch(2,1);
    c2 = channel2.corrections().sourceMatch(2,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setSourceMatch(c1,2,1);
    qDebug() << "Source match error? " << vna.isError();

    c1 = channel1.corrections().reflectionTracking(2,1);
    c2 = channel2.corrections().reflectionTracking(2,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setReflectionTracking(c1,2,1);
    qDebug() << "reflection tracking error? " << vna.isError();

    c1 = channel1.corrections().loadMatch(2,1);
    c2 = channel2.corrections().loadMatch(2,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setLoadMatch(c1,2,1);
    qDebug() << "Load match error? " << vna.isError();

    c1 = channel1.corrections().transmissionTracking(2,1);
    c2 = channel2.corrections().transmissionTracking(2,1);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setTransmissionTracking(c1,2,1);
    qDebug() << "Transmission tracking error? " << vna.isError();

    qDebug() << "Port 2,2";
    c1 = channel1.corrections().directivity(2,2);
    c2 = channel2.corrections().directivity(2,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setDirectivity(c1,2,2);
    qDebug() << "Directivity error? " << vna.isError();

    c1 = channel1.corrections().sourceMatch(2,2);
    c2 = channel2.corrections().sourceMatch(2,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setSourceMatch(c1,2,2);
    qDebug() << "Source match error? " << vna.isError();

    c1 = channel1.corrections().reflectionTracking(2,2);
    c2 = channel2.corrections().reflectionTracking(2,2);
    c1.insert(c1.end(), c2.begin(), c2.end());
    corrections3.setReflectionTracking(c1,2,2);
    qDebug() << "reflection tracking error? " << vna.isError();

    // Display results
    vna.createTrace("Trc2", 2);
    VnaTrace trace2 = vna.trace("Trc2");
    trace2.setNetworkParameter(NetworkParameter::S, 2, 1);

    vna.createDiagram(2);
    trace2.setDiagram(2);

    vna.createTrace("Trc3", 3);
    VnaTrace trace3 = vna.trace("Trc3");
    trace3.setNetworkParameter(NetworkParameter::S, 2, 1);

    vna.createDiagram(3);
    trace3.setDiagram(3);

    return 0;
}


bool isNoConnection(Vna &vna) {
    if (vna.isDisconnected()) {
        QString error_message
                = "Instrument not found. Please run this application on a Rohde & Schwarz VNA.";
        QMessageBox::critical(NULL,
                              "My RSA App",
                              error_message);
        vna.print(error_message);
        return true;
    }
    else
        return false;
}
bool isUnknownModel(Vna &vna) {
    if (vna.properties().isUnknownModel()) {
        QString error_message(QString("VNA not recognized.\n")
                              + "Please use My RSA App with a Rohde & Schwarz instrument");
        QMessageBox::critical(NULL,
                              "My RSA App",
                              error_message);
        vna.print(error_message);
        return true;
    }
    else
        return false;
}

