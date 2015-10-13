#include "testdoubleoffsetshortkit.h"


// Project
#include "DoubleOffsetShortKit.h"


// RsaToolbox
#include <NameLabel.h>
#include <Connector.h>
#include <VnaCalStandard.h>
#include <VisaBus.h>
using namespace RsaToolbox;

// Qt
#include <QVector>
#include <QStringList>
#include <QDebug>


typedef QVector<VnaCalStandard> Standards;
typedef QVector<uint> Ports;

TestDoubleOffsetShortKit::TestDoubleOffsetShortKit() {
    cycle = 0;
    appName = "Test DoubleOffsetShortKit";
    appVersion = "0";

    QDir src(SOURCE_DIR);
    src.cd("Results");
    filename = src.filePath("%1 %2 Log.txt");
}

void TestDoubleOffsetShortKit::init() {
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

    // Specific init
    QVERIFY(vna.testPorts() >= 4);

    vna.clearStatus();
    vna.disconnectLog();
}
void TestDoubleOffsetShortKit::cleanup() {
    vna.disconnectLog();
    log.reset(new Log(filename.arg(cycle++).arg("cleanup"), appName, appVersion));
    log->printHeader();
    vna.useLog(log.data());
    vna.printInfo();

    // Specific cleanup

    vna.clearStatus();
    vna.resetBus();
    vna.disconnectLog();
}

void TestDoubleOffsetShortKit::test1_data() {
    QTest::addColumn<NameLabel>("nameLabel");
    QTest::addColumn<Connector>("vnaConnector");
    QTest::addColumn<Ports>("ports");
    QTest::addColumn<Standards>("standards");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<bool>("isOffsetShort1");
    QTest::addColumn<bool>("isOffsetShort2");
    QTest::addColumn<bool>("isOffsetShort3");
    QTest::addColumn<double>("maxFrequency_Hz");
    QTest::addColumn<double>("minFrequency_Hz");
    QTest::addColumn<QStringList>("shortLabels");
    QTest::addColumn<QStringList>("offsetShortALabels");
    QTest::addColumn<QStringList>("offsetShortBLabels");
    QTest::addColumn<QStringList>("thruLabels");


    Connector vnaConnector;
    Ports ports;
    Connector kitConnector;
    Standards standards;
    QStringList shortLabels;
    QStringList offsetShortALabels;
    QStringList offsetShortBLabels;
    QStringList thruLabels;

    // Cannot create an empty cal kit!

    // Too many offset shorts (gendered)
    vnaConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    standards << VnaCalStandard(VnaCalStandard::Type::Short, kitConnector);
    shortLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort1, kitConnector);
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort2, kitConnector);
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort3, kitConnector);
    offsetShortALabels << "" << "";
    offsetShortBLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::Thru, kitConnector, kitConnector);
    thruLabels << "";
    QTest::newRow("Too many Offset Shorts") << NameLabel("Three Offset Shorts", "")
                                         << vnaConnector
                                         << ports
                                         << standards
                                         << false // isValid
                                         << true // isOffsetShort1
                                         << true // isOffsetShort2
                                         << true // isOffsetShort3
                                         << 1.0E12 // maxFrequency_Hz
                                         << 0.0 // minFrequency_Hz
                                         << shortLabels // Shorts
                                         << offsetShortALabels // Offset Short As
                                         << offsetShortBLabels // Offset Short Bs
                                         << thruLabels; // Thrus

    // Pass: Offset shorts 1 and 2 (gendered)
    vnaConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    standards << VnaCalStandard(VnaCalStandard::Type::Short, kitConnector);
    shortLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort1, kitConnector);
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort2, kitConnector);
    offsetShortALabels << "" << "";
    offsetShortBLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::Thru, kitConnector, kitConnector);
    thruLabels << "";
    QTest::newRow("Offsets 1 and 2") << NameLabel("Offsets 1 and 2", "")
                                     << vnaConnector
                                     << ports
                                     << standards
                                     << true // isValid
                                     << true // isOffsetShort1
                                     << true // isOffsetShort2
                                     << false // isOffsetShort3
                                     << 1.0E12 // maxFrequency_Hz
                                     << 0.0 // minFrequency_Hz
                                     << shortLabels // Shorts
                                     << offsetShortALabels // Offset Short As
                                     << offsetShortBLabels // Offset Short Bs
                                     << thruLabels; // Thrus

    // Pass: Offset short 1 and 3 (gendered)
    vnaConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    standards << VnaCalStandard(VnaCalStandard::Type::Short, kitConnector);
    shortLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort1, kitConnector);
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort3, kitConnector);
    offsetShortALabels << "" << "";
    offsetShortBLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::Thru, kitConnector, kitConnector);
    thruLabels << "";
    QTest::newRow("Offsets 1 and 3") << NameLabel("Offsets 1 and 3", "")
                                     << vnaConnector
                                     << ports
                                     << standards
                                     << true // isValid
                                     << true // isOffsetShort1
                                     << false // isOffsetShort2
                                     << true // isOffsetShort3
                                     << 1.0E12 // maxFrequency_Hz
                                     << 0.0 // minFrequency_Hz
                                     << shortLabels // Shorts
                                     << offsetShortALabels // Offset Short As
                                     << offsetShortBLabels // Offset Short Bs
                                     << thruLabels; // Thrus

    // Pass: Offset short 2 and 3 (gendered)
    vnaConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    standards << VnaCalStandard(VnaCalStandard::Type::Short, kitConnector);
    shortLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort2, kitConnector);
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort3, kitConnector);
    offsetShortALabels << "" << "";
    offsetShortBLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::Thru, kitConnector, kitConnector);
    thruLabels << "";
    QTest::newRow("Offsets 2 and 3") << NameLabel("Offsets 2 and 3", "")
                                     << vnaConnector
                                     << ports
                                     << standards
                                     << true // isValid
                                     << false // isOffsetShort1
                                     << true // isOffsetShort2
                                     << true // isOffsetShort3
                                     << 1.0E12 // maxFrequency_Hz
                                     << 0.0 // minFrequency_Hz
                                     << shortLabels // Shorts
                                     << offsetShortALabels // Offset Short As
                                     << offsetShortBLabels // Offset Short Bs
                                     << thruLabels; // Thrus

    // Fail: Wrong gender
    vnaConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    standards << VnaCalStandard(VnaCalStandard::Type::Short, kitConnector);
    shortLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort1, kitConnector);
    standards << VnaCalStandard(VnaCalStandard::Type::OffsetShort2, kitConnector);
    offsetShortALabels << "" << "";
    offsetShortBLabels << "" << "";
    standards << VnaCalStandard(VnaCalStandard::Type::Thru, kitConnector, kitConnector);
    thruLabels << "";
    QTest::newRow("Wrong Gender") << NameLabel("Wrong Gender", "")
                                  << vnaConnector
                                  << ports
                                  << standards
                                  << false // isValid
                                  << false // isOffsetShort1
                                  << false // isOffsetShort2
                                  << false // isOffsetShort3
                                  << 1.0E12 // maxFrequency_Hz
                                  << 0.0 // minFrequency_Hz
                                  << shortLabels // Shorts
                                  << offsetShortALabels // Offset Short As
                                  << offsetShortBLabels // Offset Short Bs
                                  << thruLabels; // Thrus

    // Pass: All port-specific standards (Ports 1, 2)
    vnaConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    VnaCalStandard s;
    s.setType(VnaCalStandard::Type::Short);
    s.setConnector(kitConnector);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    shortLabels << "" << "";
    s.setType(VnaCalStandard::Type::OffsetShort1);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    offsetShortALabels << "" << "";
    s.setType(VnaCalStandard::Type::OffsetShort2);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    offsetShortBLabels << "" << "";
    s.setType(VnaCalStandard::Type::Thru);
    s.setConnectors(kitConnector, kitConnector);
    s.setPorts(1, 2);
    standards << s;
    thruLabels << "";
    QTest::newRow("Port Specific (2 port)") << NameLabel("Port Specific", "")
                                               << vnaConnector
                                               << ports
                                               << standards
                                               << true // isValid
                                               << true // isOffsetShort1
                                               << true // isOffsetShort2
                                               << false // isOffsetShort3
                                               << 1.0E12 // maxFrequency_Hz
                                               << 0.0 // minFrequency_Hz
                                               << shortLabels // Shorts
                                               << offsetShortALabels // Offset Short As
                                               << offsetShortBLabels // Offset Short Bs
                                               << thruLabels; // Thrus

    // Fail: Wrong specific ports
    vnaConnector = Connector(Connector::Type::mm_7_CONNECTOR, Connector::Gender::Neutral);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_7_CONNECTOR, Connector::Gender::Neutral);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    s.clear();
    s.setType(VnaCalStandard::Type::Short);
    s.setConnector(kitConnector);
    standards << s;
    shortLabels << "" << "";
    s.setType(VnaCalStandard::Type::OffsetShort1);
    s.setPort(2);
    standards << s;
    s.setPort(3);
    standards << s;
    offsetShortALabels << "" << "";
    s.setType(VnaCalStandard::Type::OffsetShort2);
    s.setPort(2);
    standards << s;
    s.setPort(3);
    standards << s;
    offsetShortBLabels << "" << "";
    s.setType(VnaCalStandard::Type::Thru);
    s.setConnectors(kitConnector, kitConnector);
    standards << s;
    thruLabels << "";
    QTest::newRow("Wrong specific ports") << NameLabel("Port Specific", "")
                                               << vnaConnector
                                               << ports
                                               << standards
                                               << false // isValid
                                               << false // isOffsetShort1
                                               << false // isOffsetShort2
                                               << false // isOffsetShort3
                                               << 1.0E12 // maxFrequency_Hz
                                               << 0.0 // minFrequency_Hz
                                               << shortLabels // Shorts
                                               << offsetShortALabels // Offset Short As
                                               << offsetShortBLabels // Offset Short Bs
                                               << thruLabels; // Thrus

    // Pass: Short and Thru gendered (neutral), port-specific offset shorts 1, 2
    vnaConnector = Connector(Connector::Type::mm_7_CONNECTOR, Connector::Gender::Neutral);
    ports.clear();
    ports << 1 << 2;
    kitConnector = Connector(Connector::Type::mm_7_CONNECTOR, Connector::Gender::Neutral);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    s.clear();
    s.setType(VnaCalStandard::Type::Short);
    s.setConnector(kitConnector);
    standards << s;
    shortLabels << "" << "";
    s.setType(VnaCalStandard::Type::OffsetShort1);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    offsetShortALabels << "" << "";
    s.setType(VnaCalStandard::Type::OffsetShort2);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    offsetShortBLabels << "" << "";
    s.setType(VnaCalStandard::Type::Thru);
    s.setConnectors(kitConnector, kitConnector);
    standards << s;
    thruLabels << "";
    QTest::newRow("Port Specific offsets only") << NameLabel("Port Specific", "")
                                               << vnaConnector
                                               << ports
                                               << standards
                                               << true // isValid
                                               << true // isOffsetShort1
                                               << true // isOffsetShort2
                                               << false // isOffsetShort3
                                               << 1.0E12 // maxFrequency_Hz
                                               << 0.0 // minFrequency_Hz
                                               << shortLabels // Shorts
                                               << offsetShortALabels // Offset Short As
                                               << offsetShortBLabels // Offset Short Bs
                                               << thruLabels; // Thrus

    // Pass: 4-port, port specific, specific labels
    vnaConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    ports.clear();
    ports << 1 << 2 << 3 << 4;
    kitConnector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    standards.clear();
    shortLabels.clear();
    offsetShortALabels.clear();
    offsetShortBLabels.clear();
    thruLabels.clear();
    s.clear();
    s.setType(VnaCalStandard::Type::Short);
    s.setConnector(kitConnector);
    s.setPort(1);
    s.setLabel("Short P1");
    standards << s;
    s.setPort(2);
    s.setLabel("Short P2");
    standards << s;
    s.setPort(3);
    s.setLabel("Short P3");
    standards << s;
    s.setPort(4);
    s.setLabel("Short P4");
    standards << s;
    shortLabels << "Short P1" << "Short P2" << "Short P3" << "Short P4";
    s.setType(VnaCalStandard::Type::OffsetShort1);
    s.setPort(1);
    s.setLabel("OS1 P1");
    standards << s;
    s.setPort(2);
    s.setLabel("OS1 P2");
    standards << s;
    s.setPort(3);
    s.setLabel("OS1 P3");
    standards << s;
    s.setPort(4);
    s.setLabel("OS1 P4");
    standards << s;
    offsetShortALabels << "OS1 P1" << "OS1 P2" << "OS1 P3" << "OS1 P4";
    s.setType(VnaCalStandard::Type::OffsetShort2);
    s.setPort(1);
    s.setLabel("OS2 P1");
    standards << s;
    s.setPort(2);
    s.setLabel("OS2 P2");
    standards << s;
    s.setPort(3);
    s.setLabel("OS2 P3");
    standards << s;
    s.setPort(4);
    s.setLabel("OS2 P4");
    standards << s;
    offsetShortBLabels << "OS2 P1" << "OS2 P2" << "OS2 P3" << "OS2 P4";
    s.setType(VnaCalStandard::Type::Thru);
    s.setConnectors(kitConnector, kitConnector);
    s.setPorts(1,2);
    s.setLabel("Thru 1-2");
    standards << s;
    s.setPorts(1,3);
    s.setLabel("Thru 1-3");
    standards << s;
    s.setPorts(1,4);
    s.setLabel("Thru 1-4");
    standards << s;
    s.setPorts(2,3);
    s.setLabel("Thru 2-3");
    standards << s;
    s.setPorts(2,4);
    s.setLabel("Thru 2-4");
    standards << s;
    s.setPorts(3,4);
    s.setLabel("Thru 3-4");
    standards << s;
    thruLabels << "Thru 1-2" << "Thru 1-3" << "Thru 1-4" << "Thru 2-3" << "Thru 2-4" << "Thru 3-4";
    QTest::newRow("Port specific with labels") << NameLabel("Port Specific", "")
                                               << vnaConnector
                                               << ports
                                               << standards
                                               << true // isValid
                                               << true // isOffsetShort1
                                               << true // isOffsetShort2
                                               << false // isOffsetShort3
                                               << 1.0E12 // maxFrequency_Hz
                                               << 0.0 // minFrequency_Hz
                                               << shortLabels // Shorts
                                               << offsetShortALabels // Offset Short As
                                               << offsetShortBLabels // Offset Short Bs
                                               << thruLabels; // Thrus
}
void TestDoubleOffsetShortKit::test1() {
    log.reset(new Log(filename.arg(cycle++).arg("test"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    QVERIFY(vna.isConnected());
    if (vna.isConnected()) {
        QVERIFY(!vna.idString().isEmpty());
    }
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();

    QFETCH(NameLabel, nameLabel);
    QFETCH(Connector, vnaConnector);
    QFETCH(Ports, ports);
    QFETCH(Standards, standards);
    QFETCH(bool, isValid);
    QFETCH(bool, isOffsetShort1);
    QFETCH(bool, isOffsetShort2);
    QFETCH(bool, isOffsetShort3);
    QFETCH(double, minFrequency_Hz);
    QFETCH(double, maxFrequency_Hz);
    QFETCH(QStringList, shortLabels);
    QFETCH(QStringList, offsetShortALabels);
    QFETCH(QStringList, offsetShortBLabels);
    QFETCH(QStringList, thruLabels);

    NameLabel name("New Cal Kit", "");

    if (vna.isCalKit(name))
        vna.deleteCalKit(name);

    VnaCalKit calKit = vna.calKit(name);
    foreach(VnaCalStandard s, standards) {
        calKit.addStandard(s);
    }

    QVERIFY(vna.isCalKit(name));
    DoubleOffsetShortKit doubleOffsetShortKit(calKit, vnaConnector.gender(), ports);
    QCOMPARE(doubleOffsetShortKit.isValid(), isValid);

    if (isValid) {
        QCOMPARE(doubleOffsetShortKit.isOffsetShort1(), isOffsetShort1);
        QCOMPARE(doubleOffsetShortKit.isOffsetShort2(), isOffsetShort2);
        QCOMPARE(doubleOffsetShortKit.isOffsetShort3(), isOffsetShort3);
        QCOMPARE(doubleOffsetShortKit.maximumFrequency_Hz(), maxFrequency_Hz);
        QCOMPARE(doubleOffsetShortKit.minimumFrequency_Hz(), minFrequency_Hz);
        QCOMPARE(doubleOffsetShortKit.shortLabels(), shortLabels);
        QCOMPARE(doubleOffsetShortKit.offsetShortALabels(), offsetShortALabels);
        QCOMPARE(doubleOffsetShortKit.offsetShortBLabels(), offsetShortBLabels);
        QCOMPARE(doubleOffsetShortKit.thruLabels(), thruLabels);
    }

//    vna.deleteCalKit(name);
//    QVERIFY(vna.isNotCalKit(name));
    QVERIFY(!vna.isError());
    vna.disconnectLog();
}

