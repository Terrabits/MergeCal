#ifndef CALIBRATION_H
#define CALIBRATION_H


// Project
#include "FrequencyRange.h"
#include "Thru.h"
#include "PartialCal.h"

// RsaToolbox
#include <Vna.h>

// Qt
#include <QObject>


class Calibration : public QObject
{
    Q_OBJECT
public:
    explicit Calibration(QObject *parent = 0);
    ~Calibration();

    // Initialize
    void setVna(RsaToolbox::Vna *vna);

    void initialize();
    bool isMatchMeasured(uint port);
    void measureMatch(uint port);

    bool isShortMeasured(uint port);
    void measureShort(uint port);

    bool isOffsetShortAMeasured(uint port, uint kitIndex);
    void measureOffsetShortA(uint port, uint kitIndex);

    bool isOffsetShortBMeasured(uint port, uint kitIndex);
    void measureOffsetShortB(uint port, uint kitIndex);

    bool isThruMeasured(uint index);
    uint numberOfThrus() const;
    Thru thru(uint index);
    void measureThru(uint index);

    uint numberOfKits() const;
    RsaToolbox::NameLabel kit(uint index);
    QString offsetAName(uint kitIndex);
    QString offsetBName(uint kitIndex);

signals:

public slots:
    void setPorts(const QVector<uint> &ports);
    void setConnector(const RsaToolbox::Connector &connector);
    void setChannel(uint index);
    void setCalKits(const QVector<FrequencyRange> &kits);

private:
    RsaToolbox::Vna *_vna;

    QVector<uint> _ports;
    RsaToolbox::Connector _connector;
    uint _channel;
    QVector<FrequencyRange> _kits;

    QVector<PartialCal> _partialCals;

};

#endif // CALIBRATION_H
