

#include "DoubleOffsetShortKit.h"

#include <NameLabel.h>
#include <Connector.h>
#include <Vna.h>
#include <VnaCalStandard.h>
#include <Log.h>
#include <VisaBus.h>
using namespace RsaToolbox;
typedef QVector<VnaCalStandard> Standards;

#include <QTest>
#include <QString>
#include <QVector>
#include <QDebug>


class TestDoubleOffsetShortKit : public QObject
{
    Q_OBJECT

public:
    TestDoubleOffsetShortKit();

private:
    Vna vna;
    QScopedPointer<Log> log;

    int cycle;
    QString appName;
    QString appVersion;
    QString filename;

private slots:
    void init();
    void cleanup();

    void test1_data();
    void test1();
};

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
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();

    // Specific init

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
    // NameLabel | Connector type | Shorts | OffsetShort1s | OffsetShort2s | OffsetShorts3 | Thrus | isValid
    QTest::addColumn<NameLabel>("nameLabel");
    QTest::addColumn<Connector::Type>("connectorType");
    QTest::addColumn<Standards>("standards");
    QTest::addColumn<bool>("isValid");

    Connector connector;
    Standards standards;

    // Cannot create an empty cal kit!
//    QTest::newRow("Empty Kit") << NameLabel("EmptyKit", "label")
//                               << Connector::Type::mm_3_5_CONNECTOR
//                               << standards
//                               << false;

    connector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    standards << VnaCalStandard(VnaStandardType::SHORT_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT1_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT2_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT3_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::THRU_STANDARD_TYPE, connector, connector);
    QTest::newRow("Three Offset Shorts") << NameLabel("Three Offset Shorts", "")
                                         << Connector::Type::mm_3_5_CONNECTOR
                                         << standards
                                         << false;

    connector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    standards << VnaCalStandard(VnaStandardType::SHORT_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT1_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT2_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::THRU_STANDARD_TYPE, connector, connector);
    QTest::newRow("Offsets 1 and 2") << NameLabel("Offsets 1 and 2", "")
                                         << Connector::Type::mm_3_5_CONNECTOR
                                         << standards
                                         << true;

    connector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    standards << VnaCalStandard(VnaStandardType::SHORT_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT1_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT3_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::THRU_STANDARD_TYPE, connector, connector);
    QTest::newRow("Offsets 1 and 3") << NameLabel("Offsets 1 and 3", "")
                                         << Connector::Type::mm_3_5_CONNECTOR
                                         << standards
                                         << true;

    connector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    standards << VnaCalStandard(VnaStandardType::SHORT_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT2_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT3_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::THRU_STANDARD_TYPE, connector, connector);
    QTest::newRow("Offsets 2 and 3") << NameLabel("Offsets 2 and 3", "")
                                         << Connector::Type::mm_3_5_CONNECTOR
                                         << standards
                                         << true;

    connector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Female);
    standards.clear();
    standards << VnaCalStandard(VnaStandardType::SHORT_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT1_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::OFFSET_SHORT2_STANDARD_TYPE, connector);
    standards << VnaCalStandard(VnaStandardType::THRU_STANDARD_TYPE, connector, connector);
    QTest::newRow("Wrong Gender") << NameLabel("Wrong Gender", "")
                                         << Connector::Type::mm_3_5_CONNECTOR
                                         << standards
                                         << false;


    connector = Connector(Connector::Type::mm_3_5_CONNECTOR, Connector::Gender::Male);
    standards.clear();
    VnaCalStandard s;
    s.setType(VnaStandardType::SHORT_STANDARD_TYPE);
    s.setConnector(connector);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    s.setType(VnaStandardType::OFFSET_SHORT1_STANDARD_TYPE);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    s.setType(VnaStandardType::OFFSET_SHORT2_STANDARD_TYPE);
    s.setPort(1);
    standards << s;
    s.setPort(2);
    standards << s;
    s.setType(VnaStandardType::THRU_STANDARD_TYPE);
    s.setConnectors(connector, connector);
    s.setPorts(1, 2);
    standards << s;
    QTest::newRow("Port Specific - two ports") << NameLabel("Port Specific", "")
                                         << Connector::Type::mm_3_5_CONNECTOR
                                         << standards
                                         << true;

}
void TestDoubleOffsetShortKit::test1() {
    log.reset(new Log(filename.arg(cycle++).arg("test"), appName, appVersion));
    log->printHeader();
    vna.resetBus(new VisaBus(TCPIP_CONNECTION, "127.0.0.1", 1000));
    vna.useLog(log.data());
    vna.printInfo();
    vna.preset();

    QFETCH(NameLabel, nameLabel);
    QFETCH(Connector::Type, connectorType);
    QFETCH(Standards, standards);
    QFETCH(bool, isValid);

    NameLabel name("New Cal Kit", "");

    if (vna.isCalKit(name))
        vna.deleteCalKit(name);

    VnaCalKit calKit = vna.calKit(name);
    foreach(VnaCalStandard s, standards) {
        calKit.addStandard(s);
    }

    DoubleOffsetShortKit doubleOffsetShortKit(calKit, Connector::Gender::Female, true);
    QCOMPARE(doubleOffsetShortKit.isValid(), isValid);
    QVERIFY(!vna.isError());
    vna.disconnectLog();
}

QTEST_MAIN(TestDoubleOffsetShortKit)
#include "TestDoubleOffsetShortKit.moc"
