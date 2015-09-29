#ifndef PARTIALCAL_H
#define PARTIALCAL_H


// Project
#include "FrequencyRange.h"

// RsaToolbox
#include <Vna.h>
#include <Connector.h>

// Qt
#include <QObject>
#include <QReadWriteLock>


class PartialCal : public QObject
{
    Q_OBJECT
public:
    explicit PartialCal(QObject *parent = 0);
    PartialCal(const PartialCal &other);
    ~PartialCal();

    void setVna(RsaToolbox::Vna *vna);
    void setOriginalChannel(uint index);
    void setPorts(const QVector<uint> &ports);
    void setConnector(const RsaToolbox::Connector &connector);
    void setCalKit(const FrequencyRange &kit);

    void initialize();
    void resetCalibration();

    uint channel() const;

    bool measureShort(uint port);
    bool measureOffsetShortA(uint port);
    bool measureOffsetShortB(uint port);
    bool measureThru(uint port1, uint port2);
    bool apply();

    RsaToolbox::ComplexRowVector directivity(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector sourceMatch(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector reflectionTracking(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector loadMatch(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector transmissionTracking(uint outputPort, uint inputPort);

    void operator=(const PartialCal &other);

signals:
    void startingMeasurement(const QString &caption, uint time_ms);
    void finishedMeasurement();

    void error(const QString &message);

public slots:
    void interrupt();

private:
    RsaToolbox::Vna *_vna;
    uint _originalChannel;

    QVector<uint> _ports;
    RsaToolbox::Connector _connector;
    FrequencyRange _calKit;

    uint _channel;
    RsaToolbox::Connector _tempConnector;
    RsaToolbox::NameLabel _tempCalKit;
    void deleteChannel();
    void deleteCalKit();
    void deleteConnector();
    static QString generateUniqueName();

    uint _sweepTime_ms;
    RsaToolbox::VnaCalibrate _cal;

    bool _interrupt;
    mutable QReadWriteLock _lock;
    bool isInterrupt() const;
    void clearInterrupt();

    // For initialize()
    bool processLinearLogFrequencies();
    bool processSegmentedFrequencies();
    static bool frequencyList(RsaToolbox::QRowVector &frequencies, bool isStartInclusive, double start, bool isStopInclusive, double stop);

    // (no apply cal after measure)
    void _measureShort(uint port);
    void _measureOffsetShortA(uint port);
    void _measureOffsetShortB(uint port);
    void _measureThru(uint port1, uint port2);

};


#endif // PARTIALCAL_H
