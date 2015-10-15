#include "TestPartialCal.h"


// Project
#include "PartialCal.h"

// RsaToolbox
#include <VisaBus.h>
using namespace RsaToolbox;

// Qt
#include <QTest>
#include <QSignalSpy>
#include <QDebug>


typedef QScopedPointer<QSignalSpy> ScopedSpy;

TestPartialCal::TestPartialCal() {
    cycle = 0;
    appName = "Test PartialCal";
    appVersion = "0";

    QDir src(SOURCE_DIR);
    src.cd("TestPartialCal Results");
    filename = src.filePath("%1 %2 Log.txt");

    connector.setCustomType("Boeing");
    ports << 1 << 2;
    channel = 1;

    lowBandStartFreq_Hz = 9.0E3;
    lowBandStopFreq_Hz  = 3.5E9;
    lowBandKit = NameLabel("LowBand", "Boeing");

    highBandStartFreq_Hz = 3.5E9;
    highBandStopFreq_Hz  = 8.5E9;
    highBandKit = NameLabel("HighBand", "Boeing");

    src.setPath(SOURCE_DIR);
    lowBandFile = src.filePath("LowBand (Boeing).calkit");
    highBandFile = src.filePath("HighBand (Boeing).calkit");
}
TestPartialCal::~TestPartialCal() {
    //
}

void TestPartialCal::init() {
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
void TestPartialCal::cleanup() {
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

void TestPartialCal::createAndDelete() {
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

    QScopedPointer<PartialCal> partialCal(new PartialCal);
    partialCal.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestPartialCal::setVnaAndDelete() {
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

    // Set Vna only
    QScopedPointer<PartialCal> partialCal(new PartialCal);
    partialCal->setVna(&vna);
    partialCal.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestPartialCal::setPortsPageAndDelete() {
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

    // Finished ports page
    QScopedPointer<PartialCal> partialCal(new PartialCal);
    partialCal->setVna(&vna);
    partialCal->setOriginalChannel(channel);
    partialCal->setPorts(ports);
    partialCal->setConnector(connector);
    partialCal.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestPartialCal::setCalKitPageAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("setCalKitPageAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();

    // Finished CalKitPage, LowBand
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    FrequencyRange range;
    range.setCalKit(kit);
    range.setStartFrequency(lowBandStartFreq_Hz);
    range.setStopFrequency(lowBandStopFreq_Hz);

    QScopedPointer<PartialCal> partialCal(new PartialCal);
    partialCal->setVna(&vna);
    partialCal->setOriginalChannel(1);
    partialCal->setPorts(QVector<uint>() << 1 << 2);
    partialCal->setConnector(Connector());
    partialCal->setCalKit(range);
    partialCal.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestPartialCal::initializeAndDelete() {
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

    // Initialize, low band kit
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    FrequencyRange range;
    range.setCalKit(kit);
    range.setStartFrequency(lowBandStartFreq_Hz);
    range.setStopFrequency(lowBandStopFreq_Hz);

    QScopedPointer<PartialCal> partialCal(new PartialCal);
    partialCal->setVna(&vna);
    partialCal->setOriginalChannel(1);
    partialCal->setPorts(QVector<uint>() << 1 << 2);
    partialCal->setConnector(Connector());
    partialCal->setCalKit(range);

    ScopedSpy error(new QSignalSpy(partialCal.data(), SIGNAL(error(QString))));
    ScopedSpy starting(new QSignalSpy(partialCal.data(), SIGNAL(startingMeasurement(QString,uint))));
    ScopedSpy finished(new QSignalSpy(partialCal.data(), SIGNAL(finishedMeasurement())));
    partialCal->createChannel();
    partialCal->createCalKit();
    partialCal->initialize();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(starting->count(), int(0));
    QCOMPARE(finished->count(), int(0));
    partialCal.reset();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(starting->count(), int(0));
    QCOMPARE(finished->count(), int(0));
    error.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestPartialCal::measureShortAndDelete() {
    log.reset(new Log(filename.arg(cycle++).arg("measureShortAndDelete"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();

    // Measure short, port 1, lowBand
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    FrequencyRange range;
    range.setCalKit(kit);
    range.setStartFrequency(lowBandStartFreq_Hz);
    range.setStopFrequency(lowBandStopFreq_Hz);

    QScopedPointer<PartialCal> partialCal(new PartialCal);
    partialCal->setVna(&vna);
    partialCal->setOriginalChannel(1);
    partialCal->setPorts(QVector<uint>() << 1 << 2);
    partialCal->setConnector(Connector());
    partialCal->setCalKit(range);

    ScopedSpy error(new QSignalSpy(partialCal.data(), SIGNAL(error(QString))));
    ScopedSpy starting(new QSignalSpy(partialCal.data(), SIGNAL(startingMeasurement(QString,uint))));
    ScopedSpy finished(new QSignalSpy(partialCal.data(), SIGNAL(finishedMeasurement())));
    partialCal->createChannel();
    partialCal->createCalKit();
    partialCal->initialize();
    partialCal->measureShort(1);
    QCOMPARE(error->count(), int(0));
    QCOMPARE(starting->count(), int(1));
    QCOMPARE(finished->count(), int(1));
    QVERIFY(!vna.isError());
    partialCal.reset();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(starting->count(), int(1));
    QCOMPARE(finished->count(), int(1));
    QVERIFY(!vna.isError());

    error.reset();
    starting.reset();
    finished.reset();

    vna.preset();
    vna.pause();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
void TestPartialCal::applyAndDelete() {
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

    // Perform all measurements
    DoubleOffsetShortKit kit(vna.calKit(lowBandKit), Connector::Gender::Male, ports);
    FrequencyRange range;
    range.setCalKit(kit);
    range.setStartFrequency(lowBandStartFreq_Hz);
    range.setStopFrequency(lowBandStopFreq_Hz);

    QScopedPointer<PartialCal> partialCal(new PartialCal);
    partialCal->setVna(&vna);
    partialCal->setOriginalChannel(1);
    partialCal->setPorts(QVector<uint>() << 1 << 2);
    partialCal->setConnector(Connector());
    partialCal->setCalKit(range);

    ScopedSpy error(new QSignalSpy(partialCal.data(), SIGNAL(error(QString))));
    ScopedSpy starting(new QSignalSpy(partialCal.data(), SIGNAL(startingMeasurement(QString,uint))));
    ScopedSpy finished(new QSignalSpy(partialCal.data(), SIGNAL(finishedMeasurement())));
    partialCal->createChannel();
    partialCal->createCalKit();
    partialCal->initialize();
    partialCal->measureShort(1);
    partialCal->measureShort(2);
    partialCal->measureOffsetShortA(1);
    partialCal->measureOffsetShortA(2);
    partialCal->measureOffsetShortB(1);
    partialCal->measureOffsetShortB(2);
    partialCal->measureThru(1,2);
    partialCal->apply();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(starting->count(), int(7));
    QCOMPARE(finished->count(), int(7));
    QVERIFY(!vna.isError());
    partialCal.reset();
    QCOMPARE(error->count(), int(0));
    QCOMPARE(starting->count(), int(7));
    QCOMPARE(finished->count(), int(7));
    QVERIFY(!vna.isError());

    error.reset();
    starting.reset();
    finished.reset();

    vna.preset();
    vna.pause();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}

