#ifndef TESTPARTIALCAL_H
#define TESTPARTIALCAL_H


// Project
//#include "PartialCal.h"

// RsaToolbox
#include <NameLabel.h>
#include <Connector.h>
#include <Vna.h>
#include <Log.h>

// Qt
#include <QObject>
#include <QString>
#include <QScopedPointer>


typedef QVector<uint> Ports;

class TestPartialCal : public QObject
{
    Q_OBJECT

public:
    TestPartialCal();
    ~TestPartialCal();

private slots:
    void init();
    void cleanup();

    void createAndDelete();
    void setVnaAndDelete();
    void setPortsPageAndDelete();
    void setCalKitPageAndDelete();
    void initializeAndDelete();
    void measureShortAndDelete();
    void applyAndDelete();

private:
    RsaToolbox::Vna vna;
    QScopedPointer<RsaToolbox::Log> log;

    int cycle;
    QString appName;
    QString appVersion;
    QString filename;

    RsaToolbox::Connector connector;
    Ports ports;
    uint channel;

    double lowBandStartFreq_Hz;
    double lowBandStopFreq_Hz;
    RsaToolbox::NameLabel lowBandKit;
    QString lowBandFile;

    double highBandStartFreq_Hz;
    double highBandStopFreq_Hz;
    RsaToolbox::NameLabel highBandKit;
    QString highBandFile;
};


#endif // TESTPARTIALCAL_H
