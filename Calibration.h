#ifndef CALIBRATION_H
#define CALIBRATION_H


// Project
#include "FrequencyRange.h"
#include "Thru.h"
#include "PartialCal.h"

// RsaToolbox
#include <NameLabel.h>
#include <Connector.h>
#include <Vna.h>

// Qt
#include <QObject>
#include <QVector>
#include <QBitArray>
#include <QReadWriteLock>


class Calibration : public QObject
{
    Q_OBJECT
public:
    explicit Calibration(QObject *parent = 0);
    ~Calibration();

    void setVna(RsaToolbox::Vna *vna);

    uint numberOfPorts() const;
    uint port(uint index);
    QVector<uint> ports() const;

    bool isMatchMeasured(uint port);
    bool isShortMeasured(uint port);
    bool isOffsetShortAMeasured(uint kitIndex, uint port);
    bool isOffsetShortBMeasured(uint kitIndex, uint port);

    bool isThruMeasured(uint index);
    uint numberOfThrus() const;
    Thru thru(uint index) const;

    uint numberOfKits() const;
    RsaToolbox::NameLabel kit(uint index) const;
    QString offsetShortAName(uint kitIndex) const;
    QString offsetShortBName(uint kitIndex) const;

signals:
    void startingInitialization();
    void finishedInitialization();

    void startingMeasurement(const QString &caption, uint time_ms);
    void finishedMeasurement();

public slots:
    void setPorts(const QVector<uint> &ports);
    void setConnector(const RsaToolbox::Connector &connector);
    void setChannel(uint index);
    void setCalKits(const QVector<FrequencyRange> &kits);

    void initialize();

    void measureMatch(uint port);
    void measureShort(uint port);
    void measureOffsetShortA(uint kitIndex, uint port);
    void measureOffsetShortB(uint kitIndex, uint port);
    void measureThru(uint index);

    void interrupt();

private:
    RsaToolbox::Vna *_vna;

    QBitArray _isMatchMeasured;
    QBitArray _isShortMeasured;
    QVector<QBitArray> _isOffsetShortAMeasured;
    QVector<QBitArray> _isOffsetShortBMeasured;
    QVector<Thru> _thrus;

    QVector<uint> _ports;
    RsaToolbox::Connector _connector;
    uint _channel;
    QVector<FrequencyRange> _kits;

    QVector<PartialCal> _partialCals;

    bool _interrupt;
    mutable QReadWriteLock _lock;
    bool isInterrupt() const;
    void clearInterrupt();
};

#endif // CALIBRATION_H
