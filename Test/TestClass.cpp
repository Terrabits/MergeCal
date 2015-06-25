

#include <QTest>
#include <QString>


class TestClass : public QObject
{
    Q_OBJECT

private slots:
    void test1_data();
    void test1();
};

void TestClass::test1_data() {
    QTest::addColumn<QString>("column1");
    QTest::addColumn<QString>("column2");

    QTest::newRow("row1") << "row 1, column 1" << "row 1, column 2";
    QTest::newRow("row2") << "row 2, column 1" << "row 2, column 2";
}
void TestClass::test1() {
    QFETCH(QString, column1);
    QFETCH(QString, column2);

    QCOMPARE(column1, column2);
}

QTEST_MAIN(TestClass)
#include "TestClass.moc"
