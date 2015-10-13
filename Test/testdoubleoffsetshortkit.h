#ifndef TESTDOUBLEOFFSETSHORTKIT_H
#define TESTDOUBLEOFFSETSHORTKIT_H


// RsaToolbox
#include <Vna.h>
#include <Log.h>

// Qt
#include <QTest>
#include <QString>
#include <QScopedPointer>


class TestDoubleOffsetShortKit : public QObject
{
    Q_OBJECT

public:
    TestDoubleOffsetShortKit();

private:
    RsaToolbox::Vna vna;
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

#endif // TESTDOUBLEOFFSETSHORTKIT_H

