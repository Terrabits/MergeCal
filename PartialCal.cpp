#include "PartialCal.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


PartialCal::PartialCal(QObject *parent) :
    QObject(parent),
    _vna(NULL),
    _originalChannel(0),
    _channel(0),
    _sweepTime_ms(0)
{

}
PartialCal::PartialCal(const PartialCal &other) :
    _vna(other._vna),
    _originalChannel(other._originalChannel),
    _ports(other._ports),
    _connector(other._connector),
    _calKit(other._calKit),
    _channel(other._channel),
    _sweepTime_ms(other._sweepTime_ms),
    _cal(other._cal)
{

}

PartialCal::~PartialCal()
{
    deleteChannel();
}

void PartialCal::setVna(RsaToolbox::Vna *vna) {
    _vna = vna;
}
void PartialCal::setOriginalChannel(uint index) {
    _originalChannel = index;
}
void PartialCal::setPorts(const QVector<uint> &ports) {
    _ports = ports;
}
void PartialCal::setConnector(const RsaToolbox::Connector &connector) {
    _connector = connector;
}
void PartialCal::setCalKit(const FrequencyRange &kit) {
    _calKit = kit;
}

void PartialCal::initialize() {
    deleteChannel();
    _vna->channel(_originalChannel).select();
    _channel = _vna->createChannel();

    // Set frequency points
    VnaChannel::SweepType sweepType = _vna->channel(_channel).sweepType();
    if (sweepType == VnaChannel::SweepType::Linear
            || sweepType == VnaChannel::SweepType::Log)
    {
        // Easier of the two options
    }
    else if (sweepType == VnaChannel::SweepType::Segmented)
    {
        // How the funk and wagnalls do I handle this?
    }

    _sweepTime_ms = _vna->channel(_channel).calibrationSweepTime_ms();

    _cal = _vna->channel(_channel).calibrate();
    _cal.setConnectors(_connector);
    _cal.selectKit(_calKit.calKit().nameLabel());

    QString name = "Channel%1Cal";
    name = name.arg(_channel);
    _cal.start(name, VnaCalibrate::CalType::Tosm, _ports);
    _cal.keepRawData();

    foreach (uint port, _ports) {
        qDebug() << "Cal kit: " << _calKit.calKit().displayNameLabel();
        qDebug() << "Match " << port;
        measureMatch(port);
        qDebug() << "Short " << port;
        measureShort(port);
        qDebug() << "Offset Short A " << port;
        measureOffsetShortA(port);
        qDebug() << "Offset short B " << port;
        measureOffsetShortB(port);
    }
    for (int i = 0; i < _ports.size(); i++) {
        for (int j = i+1; j < _ports.size(); j++) {
            qDebug() << "Thru " << _ports[i] << _ports[j];
            measureThru(_ports[i], _ports[j]);
        }
    }
    _cal.apply();
}

void PartialCal::measureMatch(uint port) {
    QString caption = "%1 Match";
    caption = caption.arg(this->_calKit.calKit().nameLabel().displayText());
    emit startingMeasurement(caption, _sweepTime_ms);
    _cal.measureMatch(port);
    emit finishedMeasurement();
}
void PartialCal::measureShort(uint port) {
    QString caption = "%1 Short";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    emit startingMeasurement(caption, _sweepTime_ms);
    _cal.measureShort(port);
    emit finishedMeasurement();
}
void PartialCal::measureOffsetShortA(uint port) {
    QString caption = "%1 Offset Short %2";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (_calKit.calKit().isOffsetShort1())
        caption = caption.arg(1);
    else
        caption = caption.arg(2);
    emit startingMeasurement(caption, _sweepTime_ms);
    if (_calKit.calKit().isOffsetShort1())
        _cal.measureOffsetShort1(port);
    else
        _cal.measureOffsetShort2(port);
    emit finishedMeasurement();
}
void PartialCal::measureOffsetShortB(uint port) {
    QString caption = "%1 Offset Short %2";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (_calKit.calKit().isOffsetShort3())
        caption = caption.arg(3);
    else
        caption = caption.arg(2);
    emit startingMeasurement(caption, _sweepTime_ms);
    if (_calKit.calKit().isOffsetShort3())
        _cal.measureOffsetShort3(port);
    else
        _cal.measureOffsetShort2(port);
    emit finishedMeasurement();
}
void PartialCal::measureThru(uint port1, uint port2) {
    QString caption = "%1 Thru %3 %3";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    caption = caption.arg(port1);
    caption = caption.arg(port2);
    emit startingMeasurement(caption, _sweepTime_ms);
    _cal.measureThru(port1, port2);
    emit finishedMeasurement();
}

ComplexRowVector PartialCal::directivity(uint outputPort, uint inputPort) {
    return _vna->channel(_channel).corrections().directivity(outputPort, inputPort);
}
ComplexRowVector PartialCal::sourceMatch(uint outputPort, uint inputPort) {
    return _vna->channel(_channel).corrections().sourceMatch(outputPort, inputPort);
}
ComplexRowVector PartialCal::reflectionTracking(uint outputPort, uint inputPort) {
    return _vna->channel(_channel).corrections().reflectionTracking(outputPort, inputPort);
}
ComplexRowVector PartialCal::loadMatch(uint outputPort, uint inputPort) {
    return _vna->channel(_channel).corrections().loadMatch(outputPort, inputPort);
}
ComplexRowVector PartialCal::transmissionTracking(uint outputPort, uint inputPort) {
    return _vna->channel(_channel).corrections().transmissionTracking(outputPort, inputPort);
}

void PartialCal::operator=(const PartialCal &other) {
    _vna = other._vna;
    _originalChannel = other._originalChannel;
    _ports = other._ports;
    _connector = other._connector;
    _calKit = other._calKit;
    _channel = other._channel;
    _sweepTime_ms = other._sweepTime_ms;
    _cal = other._cal;
}

void PartialCal::deleteChannel() {
    if (_channel != 0 && _vna->isChannel(_channel)) {
        _vna->deleteChannel(_channel);
        _channel = 0;
        _cal = VnaCalibrate();
    }
}
