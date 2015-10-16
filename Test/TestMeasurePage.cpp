#include "TestMeasurePage.h"


// Project
#include "Calibration.h"
#include "MeasurePage.h"

// RsaToolbox
#include <VisaBus.h>
using namespace RsaToolbox;

// Qt
#include <QTest>
#include <QSignalSpy>
#include <QVector>
#include <QThread>
#include <QDebug>

typedef QScopedPointer<QSignalSpy> ScopedSpy;


TestMeasurePage::TestMeasurePage(QObject *parent) :
    QObject(parent)
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

TestMeasurePage::~TestMeasurePage()
{

}

void TestMeasurePage::init() {
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
void TestMeasurePage::cleanup() {
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

void TestMeasurePage::createAndDelete() {
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
    QScopedPointer<MeasurePage> measurePage(new MeasurePage);
    measurePage.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}

void TestMeasurePage::setPropertiesAndDelete() {
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

    // Shared
    QScopedPointer<QThread> measureThread(new QThread);
    QScopedPointer<Calibration> calibration(new Calibration);
    calibration->moveToThread(measureThread.data());
    measureThread->start();

    // Page
    QScopedPointer<MeasurePage> measurePage(new MeasurePage);
    measurePage->setVna(&vna);
    measurePage->setCalibration(measureThread.data(), calibration.data());
    measurePage.reset();

    measureThread->quit();
    measureThread->wait();
    measureThread.reset();
    calibration.reset();

    QVERIFY(!vna.isError());
    vna.disconnectLog();
}
