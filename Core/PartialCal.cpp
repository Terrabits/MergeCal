#include "PartialCal.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QUuid>
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
    deleteCustomCalKit();
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

void PartialCal::createChannel() {
    _vna->channel(_originalChannel).select();
    _channel = _vna->createChannel();
    VnaChannel channel = _vna->channel(_channel);
    QString trace = _vna->createTrace(_channel);
    uint diagram = _vna->createDiagram();
    _vna->trace(trace).setDiagram(diagram);

    VnaChannel::SweepType sweepType = channel.sweepType();
    if (sweepType == VnaChannel::SweepType::Linear
            || sweepType == VnaChannel::SweepType::Log)
    {
        processLinearLogFrequencies();
    }
    else if (sweepType == VnaChannel::SweepType::Segmented)
    {
        processSegmentedFrequencies();
    }
    else {
        emit error("*Cannot calibrate time or cw sweeps.");
        return;
    }
    _sweepTime_ms = channel.calibrationSweepTime_ms();
}
void PartialCal::createCalKit() {
    // Make temp cal kit, connector
     _tempCalKit.setName(generateUniqueName());
    _vna->calKit(_calKit.calKit().nameLabel()).copy(_tempCalKit);
    _tempConnector = _connector;
    _tempConnector.setCustomType(generateUniqueName());
    _vna->defineCustomConnector(_tempConnector);
    _vna->calKit(_tempCalKit).setConnectorType(_tempConnector);
}
void PartialCal::initialize() {
    _vna->isError();
    _vna->clearStatus();

    _cal = _vna->channel(_channel).calibrate();
    _cal.setConnectors(_tempConnector);
    _cal.selectKit(_tempCalKit);

    QString name = "Channel%1Cal";
    name = name.arg(_channel);
    if (_ports.size() > 1)
        _cal.start(name, VnaCalibrate::CalType::Tosm, _ports);
    else
        _cal.start(name, VnaCalibrate::CalType::Osm, _ports);
    _cal.keepRawData();

    if (_vna->isError()) {
        _vna->clearStatus();
        emit error("*Could not initialize calibration");
        return;
    }
}
void PartialCal::deleteCustomCalKit() {
    deleteCalKit();
    deleteConnector();
}

uint PartialCal::channel() const {
    return _channel;
}

bool PartialCal::measureShort(uint port) {
    if (isInterrupt()) {
        clearInterrupt();
        return false;
    }

    _vna->isError();
    _vna->clearStatus();

    QString caption = "%1 Short";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (!_calKit.calKit().shortLabel(port).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().shortLabel(port).trimmed());
    caption += QString(" Port %1").arg(port);
    emit startingMeasurement(caption, _sweepTime_ms);

    _cal.measureShort(port);
    emit finishedMeasurement();

    if (_vna->isError()) {
        _vna->clearStatus();
        return false;
    }
    else {
        return true;
    }
}
bool PartialCal::measureOffsetShortA(uint port) {
    if (isInterrupt()) {
        clearInterrupt();
        return false;
    }

    _vna->isError();
    _vna->clearStatus();

    QString caption = "%1 Offset Short %2";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (_calKit.calKit().isOffsetShort1())
        caption = caption.arg(1);
    else
        caption = caption.arg(2);
    if (!_calKit.calKit().offsetShortALabel(port).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().offsetShortALabel(port).trimmed());
    caption += QString(" Port %1").arg(port);
    emit startingMeasurement(caption, _sweepTime_ms);


    if (_calKit.calKit().isOffsetShort1())
        _cal.measureOffsetShort1(port);
    else
        _cal.measureOffsetShort2(port);
    emit finishedMeasurement();

    if (_vna->isError()) {
        _vna->clearStatus();
        return false;
    }
    else {
        return true;
    }
}
bool PartialCal::measureOffsetShortB(uint port) {
    if (isInterrupt()) {
        clearInterrupt();
        return false;
    }

    _vna->isError();
    _vna->clearStatus();

    QString caption = "%1 Offset Short %2";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (_calKit.calKit().isOffsetShort3())
        caption = caption.arg(3);
    else
        caption = caption.arg(2);
    if (!_calKit.calKit().offsetShortBLabel(port).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().offsetShortBLabel(port).trimmed());
    caption += QString(" Port %1").arg(port);
    emit startingMeasurement(caption, _sweepTime_ms);

    if (_calKit.calKit().isOffsetShort3())
        _cal.measureOffsetShort3(port);
    else
        _cal.measureOffsetShort2(port);
    emit finishedMeasurement();

    if (_vna->isError()) {
        _vna->clearStatus();
        return false;
    }
    else {
        return true;
    }
}
bool PartialCal::measureThru(uint port1, uint port2) {
    if (isInterrupt()) {
        clearInterrupt();
        return false;
    }

    _vna->isError();
    _vna->clearStatus();

    QString caption = "%1 Thru";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (!_calKit.calKit().thruLabel(port1, port2).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().thruLabel(port1, port2).trimmed());
    caption += " Port %1-%2";
    caption = caption.arg(port1);
    caption = caption.arg(port2);
    emit startingMeasurement(caption, _sweepTime_ms);

    _cal.measureThru(port1, port2);
    emit finishedMeasurement();

    if (_vna->isError()) {
        _vna->clearStatus();
        return false;
    }
    else {
        return true;
    }
}
bool PartialCal::apply() {
    _vna->isError();
    _vna->clearStatus();
    _cal.apply();
    if (_vna->isError()) {
        _vna->clearStatus();
        return false;
    }
    else {
        return true;
    }
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

void PartialCal::deleteCalKit() {
    if (!_tempCalKit.isEmpty()) {
        _vna->deleteCalKit(_tempCalKit);
        _tempCalKit.clear();
    }
}
void PartialCal::deleteConnector() {
    if (!_tempConnector.isType(Connector::Type::UNKNOWN_CONNECTOR)) {
        _vna->deleteConnector(_tempConnector);
        _tempConnector = Connector();
    }
}
QString PartialCal::generateUniqueName() {
    return QUuid::createUuid().toString().remove('{').remove('}').remove('-');
}

bool PartialCal::isInterrupt() const {
    QReadLocker readLocker(&_lock);
    return _interrupt;
}
void PartialCal::clearInterrupt() {
    QWriteLocker writeLocker(&_lock);
    _interrupt = false;
}

bool PartialCal::processLinearLogFrequencies() {
    VnaChannel channel = _vna->channel(_channel);
    QRowVector frequencies = channel.linearSweep().frequencies_Hz();
    if (!frequencyList(frequencies,
                       _calKit.isStartFrequencyInclusive(), _calKit.startFrequency_Hz(),
                       _calKit.isStopFrequencyInclusive(), _calKit.stopFrequency_Hz()))
    {
        // No frequency points!
        QString msg = "*Cal Kit \'%1\' covers none of the cal frequencies.";
        msg = msg.arg(_calKit.calKit().displayNameLabel());
        emit error(msg);
        return false;
    }
    if (frequencies.size() == 1) {
        channel.linearSweep().setPoints(1);
        channel.linearSweep().setStart(frequencies.first());
        channel.linearSweep().setStop(frequencies.last());
        return true;
    }
    else {
        channel.linearSweep().setStart(frequencies.first());
        channel.linearSweep().setStop(frequencies.last());
        channel.linearSweep().setPoints(frequencies.size());
        return true;
    }
}
bool PartialCal::processSegmentedFrequencies() {
    VnaChannel channel = _vna->channel(_channel);
    uint segments = channel.segmentedSweep().segments();
    for (uint i = 1; i <= segments; i++) {
        VnaSweepSegment segment = channel.segmentedSweep().segment(i);
        QRowVector frequencies = linearSpacing(segment.start_Hz(), segment.stop_Hz(), segment.points());
        if (!frequencyList(frequencies,
                           _calKit.isStartFrequencyInclusive(), _calKit.startFrequency_Hz(),
                           _calKit.isStopFrequencyInclusive(), _calKit.stopFrequency_Hz()))
        {
            if (segments == 1)
                return false;

            channel.segmentedSweep().deleteSegment(i);
            i--;
            segments--;
        }
        else if (frequencies.size() == 1) {
            segment.setSingleFrequency(frequencies.first());
        }
        else {
            segment.setStart(frequencies.first());
            segment.setStop(frequencies.last());
            segment.setPoints(frequencies.size());
        }
    }

    return true;
}
bool PartialCal::frequencyList(QRowVector &frequencies, bool isStartInclusive, double start, bool isStopInclusive, double stop) {
    while (!frequencies.isEmpty() && frequencies.first() < start)
        frequencies.removeFirst();
    if (!isStartInclusive && !frequencies.isEmpty() && frequencies.first() == start)
        frequencies.removeFirst();
    while (!frequencies.isEmpty() && frequencies.last() > stop)
        frequencies.removeLast();
    if (!isStopInclusive && !frequencies.isEmpty() && frequencies.last() == stop)
        frequencies.removeLast();

    return !frequencies.isEmpty();
}

void PartialCal::_measureShort(uint port) {
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

    QString caption = "%1 Short";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (!_calKit.calKit().shortLabel(port).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().shortLabel(port).trimmed());
    caption += QString(" Port %1").arg(port);
    emit startingMeasurement(caption, _sweepTime_ms);

    _cal.measureShort(port);
    _vna->isError();
    emit finishedMeasurement();
}
void PartialCal::_measureOffsetShortA(uint port) {
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
    if (!_calKit.calKit().offsetShortALabel(port).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().offsetShortALabel(port).trimmed());
    caption += QString(" Port %1").arg(port);
    emit startingMeasurement(caption, _sweepTime_ms);

    if (_calKit.calKit().isOffsetShort1())
        _cal.measureOffsetShort1(port);
    else
        _cal.measureOffsetShort2(port);
    _vna->isError();
    emit finishedMeasurement();
}
void PartialCal::_measureOffsetShortB(uint port) {
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
    if (!_calKit.calKit().offsetShortBLabel(port).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().offsetShortBLabel(port).trimmed());
    caption += QString(" Port %1").arg(port);
    emit startingMeasurement(caption, _sweepTime_ms);

    if (_calKit.calKit().isOffsetShort3())
        _cal.measureOffsetShort3(port);
    else
        _cal.measureOffsetShort2(port);
    _vna->isError();
    emit finishedMeasurement();
}
void PartialCal::_measureThru(uint port1, uint port2) {
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }

    QString caption = "%1 Thru";
    caption = caption.arg(_calKit.calKit().nameLabel().displayText());
    if (!_calKit.calKit().thruLabel(port1, port2).trimmed().isEmpty())
        caption += QString(" (%1)").arg(_calKit.calKit().thruLabel(port1, port2).trimmed());
    caption += " Port %1-%2";
    caption = caption.arg(port1);
    caption = caption.arg(port2);
    emit startingMeasurement(caption, _sweepTime_ms);

    _cal.measureThru(port1, port2);
    _vna->isError();
    emit finishedMeasurement();
}
