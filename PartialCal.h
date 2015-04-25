#ifndef PARTIALCAL_H
#define PARTIALCAL_H


// Project
#include "FrequencyRange.h"

// RsaToolbox
#include <Vna.h>
#include <Connector.h>

// Qt
#include <QObject>


class PartialCal : public QObject
{
    Q_OBJECT
public:
    explicit PartialCal(QObject *parent = 0);
    ~PartialCal();

    void setVna(RsaToolbox::Vna *vna);
    void setOriginalChannel(uint index);
    void setPorts(const QVector<uint> &ports);
    void setConnector(const RsaToolbox::Connector &connector);
    void setCalKit(const FrequencyRange &kit);

    void initialize();

    void measureMatch(uint port);
    void measureShort(uint port);
    void measureOffsetShortA(uint port);
    void measureOffsetShortB(uint port);

    void measureThru(uint outputPort, uint inputPort);

    RsaToolbox::ComplexRowVector directivity(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector sourceMatch(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector reflectionTracking(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector loadMatch(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector transmissionTracking(uint outputPort, uint inputPort);

signals:
    void startingMeasurement(const QString &caption, uint time_ms);
    void finishedMeasurement();

public slots:

private:
    RsaToolbox::Vna *_vna;
    uint _originalChannel;

    QVector<uint> _ports;
    RsaToolbox::Connector _connector;
    uint _channel;
    FrequencyRange _calKit;

};


#endif // PARTIALCAL_H
