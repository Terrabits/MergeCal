#include "TestCalibration.h"


// Project
#include "Calibration.h"

// RsaToolbox
#include <VisaBus.h>
using namespace RsaToolbox;

// Qt
#include <QTest>
#include <QSignalSpy>
#include <QVector>
#include <QDebug>


typedef QScopedPointer<QSignalSpy> ScopedSpy;

TestCalibration::TestCalibration()
{
    cycle = 0;
    appName = "Test Calibration";
    appVersion = "0";

    QDir src(SOURCE_DIR);
    src.cd("TestCalibration Results");
    filename = src.filePath("%1 %2 Log.txt");

    connector.setCustomType("Boeing");
    ports << 1 << 2;
    channel = 1;

    lowBandStartFreq_Hz = 0;
    lowBandStopFreq_Hz  = 3.5E9;
    lowBandKit = NameLabel("LowBand", "Boeing");

    highBandStartFreq_Hz = 3.5E9;
    highBandStopFreq_Hz  = 20E9;
    highBandKit = NameLabel("HighBand", "Boeing");

    src.setPath(SOURCE_DIR);
    lowBandFile = src.filePath("LowBand (Boeing).calkit");
    highBandFile = src.filePath("HighBand (Boeing).calkit");
}

TestCalibration::~TestCalibration()
{

}


void TestCalibration::init() {
    log.reset(new Log(filename.arg(cycle++).arg("init"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();

    if (!vna.isCalKit(lowBandKit))
        vna.importCalKit(lowBandFile);
    if (!vna.isCalKit(highBandKit))
        vna.importCalKit(highBandFile);

    vna.clearStatus();
    vna.disconnectLog();
}
void TestCalibration::cleanup() {
    vna.disconnectLog();
    log.reset(new Log(filename.arg(cycle++).arg("cleanup"), appName, appVersion));
    log->printHeader();
    vna.useLog(log.data());
    vna.printInfo();

    if (vna.isCalKit(lowBandKit))
        vna.deleteCalKit(lowBandKit);
    if (vna.isCalKit(highBandKit))
        vna.deleteCalKit(highBandKit);
    if (vna.isConnectorType(connector))
        vna.deleteConnector(connector);

    vna.clearStatus();
    vna.preset();
    vna.pause();
    vna.resetBus();
    vna.disconnectLog();
}

void TestCalibration::createAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("createAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();
    vna.pause();

    // Default values
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}

void TestCalibration::setVnaAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("setVnaAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();
    vna.pause();

    // Vna only
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration->setVna(&vna);
    calibration.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestCalibration::setPortsPageAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("setPortsPageAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();
    vna.pause();

    // Finished PortsPage?
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration->setVna(&vna);
    calibration->setChannel(channel);
    calibration->setPorts(ports);
    calibration->setConnector(connector);
    calibration.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestCalibration::setCalKitsAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("setCalKitsAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();
    vna.pause();

    // Finished CalKitsPage?
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration->setVna(&vna);
    calibration->setChannel(channel);
    calibration->setPorts(ports);
    calibration->setConnector(connector);

    // Low band kit
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    QVector<FrequencyRange> kits;
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(lowBandStartFreq_Hz);
    kits.last().setStopFrequency(lowBandStopFreq_Hz);

    // High band kit
    kit = DoubleOffsetShortKit(vna.calKit(highBandKit), Connector::Gender::Male, ports);
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(highBandStartFreq_Hz);
    kits.last().setStopFrequency(highBandStopFreq_Hz);

    calibration->setCalKits(kits);
    calibration.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestCalibration::initializeAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("initializeAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();
    vna.pause();

    // Initialize measurement
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration->setVna(&vna);
    calibration->setChannel(channel);
    calibration->setPorts(ports);
    calibration->setConnector(connector);

    // Kits
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    QVector<FrequencyRange> kits;
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(lowBandStartFreq_Hz);
    kits.last().setStopFrequency(lowBandStopFreq_Hz);
    kit = DoubleOffsetShortKit(vna.calKit(highBandKit), Connector::Gender::Male, ports);
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(highBandStartFreq_Hz);
    kits.last().setStopFrequency(highBandStopFreq_Hz);

    calibration->setCalKits(kits);

    ScopedSpy error(new QSignalSpy(calibration.data(), SIGNAL(error(QString))));
    ScopedSpy startingInit(new QSignalSpy(calibration.data(), SIGNAL(startingInitialization())));
    ScopedSpy finishedInit(new QSignalSpy(calibration.data(), SIGNAL(finishedInitialization())));
    ScopedSpy startingMeas(new QSignalSpy(calibration.data(), SIGNAL(startingMeasurement(QString,uint))));
    ScopedSpy finishedMeas(new QSignalSpy(calibration.data(), SIGNAL(finishedMeasurement())));
    ScopedSpy statusUpdate(new QSignalSpy(calibration.data(), SIGNAL(measurementStatusUpdated())));
    calibration->initialize();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(0));
    QCOMPARE(finishedMeas->count(), int(0));
    QCOMPARE(statusUpdate->count(), int(0));
    QVERIFY(!vna.isError());
    calibration.reset();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(0));
    QCOMPARE(finishedMeas->count(), int(0));
    QCOMPARE(statusUpdate->count(), int(0));

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestCalibration::measureAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("measureAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();
    vna.pause();

    //
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration->setVna(&vna);
    calibration->setChannel(channel);
    calibration->setPorts(ports);
    calibration->setConnector(connector);

    // Kits
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    QVector<FrequencyRange> kits;
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(lowBandStartFreq_Hz);
    kits.last().setStopFrequency(lowBandStopFreq_Hz);
    kit = DoubleOffsetShortKit(vna.calKit(highBandKit), Connector::Gender::Male, ports);
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(highBandStartFreq_Hz);
    kits.last().setStopFrequency(highBandStopFreq_Hz);

    calibration->setCalKits(kits);

    ScopedSpy error(new QSignalSpy(calibration.data(), SIGNAL(error(QString))));
    ScopedSpy startingInit(new QSignalSpy(calibration.data(), SIGNAL(startingInitialization())));
    ScopedSpy finishedInit(new QSignalSpy(calibration.data(), SIGNAL(finishedInitialization())));
    ScopedSpy startingMeas(new QSignalSpy(calibration.data(), SIGNAL(startingMeasurement(QString,uint))));
    ScopedSpy finishedMeas(new QSignalSpy(calibration.data(), SIGNAL(finishedMeasurement())));
    ScopedSpy statusUpdate(new QSignalSpy(calibration.data(), SIGNAL(measurementStatusUpdated())));

    calibration->initialize();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(0));
    QCOMPARE(finishedMeas->count(), int(0));
    QCOMPARE(statusUpdate->count(), int(0));
    QVERIFY(!vna.isError());

    calibration->measureShort(1);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(2));
    QCOMPARE(finishedMeas->count(), int(2));
    QCOMPARE(statusUpdate->count(), int(1));

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestCalibration::applyAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("applyAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();
    vna.pause();

    // Apply measurement test
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration->setVna(&vna);
    calibration->setChannel(channel);
    calibration->setPorts(ports);
    calibration->setConnector(connector);

    // Kits
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    QVector<FrequencyRange> kits;
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(lowBandStartFreq_Hz);
    kits.last().setStopFrequency(lowBandStopFreq_Hz);
    kit = DoubleOffsetShortKit(vna.calKit(highBandKit), Connector::Gender::Male, ports);
    kits << FrequencyRange(kit);
    kits.last().setStartFrequency(highBandStartFreq_Hz);
    kits.last().setStopFrequency(highBandStopFreq_Hz);

    calibration->setCalKits(kits);

    ScopedSpy error(new QSignalSpy(calibration.data(), SIGNAL(error(QString))));
    ScopedSpy startingInit(new QSignalSpy(calibration.data(), SIGNAL(startingInitialization())));
    ScopedSpy finishedInit(new QSignalSpy(calibration.data(), SIGNAL(finishedInitialization())));
    ScopedSpy startingMeas(new QSignalSpy(calibration.data(), SIGNAL(startingMeasurement(QString,uint))));
    ScopedSpy finishedMeas(new QSignalSpy(calibration.data(), SIGNAL(finishedMeasurement())));
    ScopedSpy statusUpdate(new QSignalSpy(calibration.data(), SIGNAL(measurementStatusUpdated())));

    calibration->initialize();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(0));
    QCOMPARE(finishedMeas->count(), int(0));
    QCOMPARE(statusUpdate->count(), int(0));
    QVERIFY(!vna.isError());

    calibration->measureShort(1);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(2));
    QCOMPARE(finishedMeas->count(), int(2));
    QCOMPARE(statusUpdate->count(), int(1));
    QVERIFY(!vna.isError());

    calibration->measureShort(2);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(4));
    QCOMPARE(finishedMeas->count(), int(4));
    QCOMPARE(statusUpdate->count(), int(2));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortA(0, 1);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(5));
    QCOMPARE(finishedMeas->count(), int(5));
    QCOMPARE(statusUpdate->count(), int(3));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortA(0, 2);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(6));
    QCOMPARE(finishedMeas->count(), int(6));
    QCOMPARE(statusUpdate->count(), int(4));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortB(0, 1);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(7));
    QCOMPARE(finishedMeas->count(), int(7));
    QCOMPARE(statusUpdate->count(), int(5));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortB(0, 2);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(8));
    QCOMPARE(finishedMeas->count(), int(8));
    QCOMPARE(statusUpdate->count(), int(6));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortA(1, 1);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(9));
    QCOMPARE(finishedMeas->count(), int(9));
    QCOMPARE(statusUpdate->count(), int(7));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortA(1, 2);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(10));
    QCOMPARE(finishedMeas->count(), int(10));
    QCOMPARE(statusUpdate->count(), int(8));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortB(1, 1);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(11));
    QCOMPARE(finishedMeas->count(), int(11));
    QCOMPARE(statusUpdate->count(), int(9));
    QVERIFY(!vna.isError());

    calibration->measureOffsetShortB(1, 2);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(12));
    QCOMPARE(finishedMeas->count(), int(12));
    QCOMPARE(statusUpdate->count(), int(10));
    QVERIFY(!vna.isError());

    calibration->measureThru(0);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(14));
    QCOMPARE(finishedMeas->count(), int(14));
    QCOMPARE(statusUpdate->count(), int(11));
    QVERIFY(!vna.isError());

    calibration->apply();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(startingInit->count(), int(1));
    QCOMPARE(finishedInit->count(), int(1));
    QCOMPARE(startingMeas->count(), int(14));
    QCOMPARE(finishedMeas->count(), int(14));
    QCOMPARE(statusUpdate->count(), int(11));

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
