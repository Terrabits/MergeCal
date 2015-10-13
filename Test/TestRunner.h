#ifndef TESTRUNNER_H
#define TESTRUNNER_H


// Modified from Stack Overflow:
// http://stackoverflow.com/questions/1524390/what-unit-testing-framework-should-i-use-for-qt
// Specifically, from answer:
// http://stackoverflow.com/a/12683094/1390788
//   by mlvjr


// Qt
#include <QList>
#include <QTimer>
#include <QCoreApplication>
#include <QtTest>


class TestRunner: public QObject
{
    Q_OBJECT

public:
    TestRunner(QObject *parent);

    void addTest(QObject * test);
    bool runTests();

private slots:
    void run();

private:
    QList<QObject*> _tests;
    int _globalResult;

    void doRunTests();
};

#endif // TESTRUNNER_H
