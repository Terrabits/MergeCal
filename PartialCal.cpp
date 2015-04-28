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
    _sweepTime_ms(0),
    _interrupt(false)
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
    _cal(other._cal),
    _interrupt(false)
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
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

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
    qDebug() << "calibration sweep time: " << _sweepTime_ms;
    qDebug() << "sweep time: " << _vna->channel(_channel).sweepTime_ms();

    _cal = _vna->channel(_channel).calibrate();
    _cal.setConnectors(_connector);
    _cal.selectKit(_calKit.calKit().nameLabel());

    QString name = "Channel%1Cal";
    name = name.arg(_channel);
    _cal.start(name, VnaCalibrate::CalType::Tosm, _ports);
    _cal.keepRawData();

    foreach (uint port, _ports) {
        if (isInterrupt()) {
            clearInterrupt();
            return;
        }
        measureMatch(port);
        if (isInterrupt()) {
            clearInterrupt();
            return;
        }
        measureShort(port);
        if (isInterrupt()) {
            clearInterrupt();
            return;
        }
        measureOffsetShortA(port);
        if (isInterrupt()) {
            clearInterrupt();
            return;
        }
        measureOffsetShortB(port);
    }
    for (int i = 0; i < _ports.size(); i++) {
        for (int j = i+1; j < _ports.size(); j++) {
            if (isInterrupt()) {
                clearInterrupt();
                return;
            }
            measureThru(_ports[i], _ports[j]);
        }
    }
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }
    _cal.apply();
}

void PartialCal::measureMatch(uint port) {
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

    QString caption = "%1 Match";
    caption = caption.arg(this->_calKit.calKit().nameLabel().displayText());
    emit startingMeasurement(caption, _sweepTime_ms);
    _cal.measureMatch(port);
    emit finishedMeasurement();
}
void PartialCal::measureShort(uint port) {
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

    QString caption = "%1 Short";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    emit startingMeasurement(caption, _sweepTime_ms);
    _cal.measureShort(port);
    emit finishedMeasurement();
}
void PartialCal::measureOffsetShortA(uint port) {
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

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
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

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
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

    QString caption = "%1 Thru %2 %3";
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

void PartialCal::interrupt() {
    QWriteLocker writeLocker(&_lock);
    _interrupt = true;
}

void PartialCal::deleteChannel() {
    if (_channel != 0 && _vna->isChannel(_channel)) {
        _vna->deleteChannel(_channel);
        _channel = 0;
        _cal = VnaCalibrate();
    }
}

bool PartialCal::isInterrupt() const {
    QReadLocker readLocker(&_lock);
    return _interrupt;
}
void PartialCal::clearInterrupt() {
    QWriteLocker writeLocker(&_lock);
    _interrupt = false;
}
