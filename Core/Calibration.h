#ifndef CALIBRATION_H
#define CALIBRATION_H


// Project
#include "FrequencyRange.h"
#include "Ports.h"
#include "PartialCal.h"

// RsaToolbox
#include <NameLabel.h>
#include <Connector.h>
#include <Vna.h>
#include <VnaUndo.h>

// Qt
#include <QObject>
#include <QVector>
#include <QList>
#include <QBitArray>
#include <QReadWriteLock>
#include <QHash>


typedef QHash<int, QHash<int, QHash<QString, RsaToolbox::ComplexRowVector>>> CorrectionsHash;

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

    bool isPortFullyMeasured(uint port) const;
    bool isShortMeasured(uint port) const;
    bool isOffsetShortAMeasured(uint kitIndex, uint port) const;
    bool isOffsetShortBMeasured(uint kitIndex, uint port) const;

    bool isAllThrusMeasured() const;
    bool isThruMeasured(uint index) const;
    uint numberOfThrus() const;
    Thru thru(uint index) const;

    uint numberOfKits() const;
    RsaToolbox::NameLabel kitNameLabel(uint index) const;
    QString shortLabel(uint port) const;
    QString offsetShortAName(uint kitIndex) const;
    QString offsetShortALabel(uint kitIndex, uint port) const;
    QString offsetShortBName(uint kitIndex) const;
    QString offsetShortBLabel(uint kitIndex, uint port) const;
    QString thruLabel(uint index) const;

    bool allMeasurementsFinished() const;

signals:
    void startingInitialization();
    void finishedInitialization();

    void startingMeasurement(const QString &caption, uint time_ms);
    void finishedMeasurement();
    void measurementStatusUpdated();

    void error(const QString &message);

public slots:
    void setPorts(const QVector<uint> &ports);
    void setConnector(const RsaToolbox::Connector &connector);
    void setChannel(uint index);
    void setCalKits(const QVector<FrequencyRange> &kits);

    void initialize();
    void reset();

    void interrupt();

    bool measureShort(uint port);
    bool measureOffsetShortA(uint kitIndex, uint port);
    bool measureOffsetShortB(uint kitIndex, uint port);
    bool measureThru(uint index);

    bool apply();

private:
    RsaToolbox::Vna *_vna;

    QBitArray _isShortMeasured;
    QVector<QBitArray> _isOffsetShortAMeasured;
    QVector<QBitArray> _isOffsetShortBMeasured;
    ThruValues _isThruMeasured;

    QVector<uint> _ports;
    Thrus _thrus;
    RsaToolbox::Connector _connector;
    uint _channel;
    QVector<FrequencyRange> _kits;

    RsaToolbox::VnaUndo _undo;
    QList<PartialCal> _partialCals;

    CorrectionsHash getCorrections();
    void applyCorrections(const CorrectionsHash &corrections);

    bool _interrupt;
    mutable QReadWriteLock _lock;
    bool isInterrupt() const;
    void clearInterrupt();
};

#endif // CALIBRATION_H
