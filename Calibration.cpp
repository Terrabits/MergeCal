#include "Calibration.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


Calibration::Calibration(QObject *parent) :
    QObject(parent),
    _vna(NULL),
    _channel(0),
    _interrupt(false)
{

}

Calibration::~Calibration()
{
    _partialCals.clear();
}

void Calibration::setVna(RsaToolbox::Vna *vna) {
    _vna = vna;
}

void Calibration::initialize() {
    if (isInterrupt()) {
        clearInterrupt();
        return;
    }
    emit startingInitialization();
    _isShortMeasured.fill(false, _ports.size());
    _isOffsetShortAMeasured.fill(QBitArray(_ports.size(), false), _kits.size());
    _isOffsetShortBMeasured.fill(QBitArray(_ports.size(), false), _kits.size());
    _isThruMeasured.resize(_ports);
    _isThruMeasured.fill(false);

    _partialCals.clear();
    foreach (FrequencyRange kit, _kits) {
        PartialCal partial;
        partial.setVna(_vna);
        partial.setOriginalChannel(_channel);
        partial.setPorts(_ports);
        partial.setConnector(_connector);
        partial.setCalKit(kit);
        _partialCals << partial;

        connect(&(_partialCals.last()), SIGNAL(startingMeasurement(QString,uint)),
                this, SIGNAL(startingMeasurement(QString,uint)));
        connect(&(_partialCals.last()), SIGNAL(finishedMeasurement()),
                this, SIGNAL(finishedMeasurement()));

        connect(&(_partialCals.last()), SIGNAL(error(QString)),
                this, SLOT(interrupt()));
        connect(&(_partialCals.last()), SIGNAL(error(QString)),
                this, SIGNAL(error(QString)));

        _partialCals.last().initialize();
        if (isInterrupt()) {
            clearInterrupt();
            return;
        }
    }
    emit finishedInitialization();
}
void Calibration::clearPartialCals() {
    _partialCals.clear();
}

uint Calibration::numberOfPorts() const {
    return _ports.size();
}
uint Calibration::port(uint index) {
    return _ports[index];
}
QVector<uint> Calibration::ports() const {
    return _ports;
}

bool Calibration::isPortFullyMeasured(uint port) const {
    if (!isShortMeasured(port))
        return false;
    for (uint i = 0; i < numberOfKits(); i++) {
        if (!isOffsetShortAMeasured(i, port))
            return false;
        if (!isOffsetShortBMeasured(i, port))
            return false;
    }

    return true;
}

bool Calibration::isShortMeasured(uint port) const {
    return _isShortMeasured[_ports.indexOf(port)];
}
bool Calibration::measureShort(uint port) {
    _vna->isError();
    _vna->clearStatus();

    for (int i = 0; i < _partialCals.size(); i++) {
        if (!_partialCals[i].measureShort(port)) {
            emit error("Error measuring short.\nPlease check your connections and try again.");
            return false;
        }
    }
    _isShortMeasured[_ports.indexOf(port)] = true;
    emit measurementStatusUpdated();
    return true;
}

bool Calibration::isOffsetShortAMeasured(uint kitIndex, uint port) const {
    return _isOffsetShortAMeasured[kitIndex][_ports.indexOf(port)];
}
bool Calibration::measureOffsetShortA(uint kitIndex, uint port) {
    if (!_partialCals[kitIndex].measureOffsetShortA(port)) {
        emit error("Error measuring offset short.\nPlease check your connections and try again.");
        return false;
    }
    _isOffsetShortAMeasured[kitIndex][_ports.indexOf(port)] = true;
    emit measurementStatusUpdated();
    return true;
}

bool Calibration::isOffsetShortBMeasured(uint kitIndex, uint port) const {
    return _isOffsetShortBMeasured[kitIndex][_ports.indexOf(port)];
}
bool Calibration::measureOffsetShortB(uint kitIndex, uint port) {
    if (!_partialCals[kitIndex].measureOffsetShortB(port)) {
        emit error("Error measuring offset short.\nPlease check your connections and try again.");
        return false;
    }

    _isOffsetShortBMeasured[kitIndex][_ports.indexOf(port)] = true;
    emit measurementStatusUpdated();
    return true;
}

bool Calibration::isAllThrusMeasured() const {
    for (uint i = 0; i < numberOfThrus(); i++) {
        if (!isThruMeasured(i))
            return false;
    }

    return true;
}

bool Calibration::isThruMeasured(uint index) const {
    return _isThruMeasured[index].toBool();
}
uint Calibration::numberOfThrus() const {
    return _thrus.size();
}
Thru Calibration::thru(uint index) const {
    return _thrus[index];
}
bool Calibration::measureThru(uint index) {
    for (int i = 0; i < _partialCals.size(); i++) {
        if (!_partialCals[i].measureThru(_thrus[index].port1, _thrus[index].port2)) {
            emit error("Error measuring thru.\nPlease check your connections and try again.");
            return false;
        }
    }

    _isThruMeasured[index] = true;
    emit measurementStatusUpdated();
    return true;
}

bool Calibration::applyCorrections() {
    // Apply calibration in each channel
    for (int i = 0; i < _partialCals.size(); i++) {
        if (!_partialCals[i].apply()) {
            emit error("Error calculating corrections. Please try again.");
            return false;
        }
    }


    VnaChannel channel = _vna->channel(_channel);
    channel.calibrate().setConnectors(_connector);
    if (_ports.size() > 1)
        channel.calibrate().start("DefaultCal", VnaCalibrate::CalType::Tosm, _ports);
    else
        channel.calibrate().start("DefaultCal", VnaCalibrate::CalType::Osm, _ports);
    channel.calibrate().keepRawData(false);
    channel.corrections().loadDefaultCorrections();

    foreach(const uint &port1, _ports) {
        foreach (const uint &port2, _ports) {
            // Directivity
            ComplexRowVector corrections;
            for (int i = 0; i < _partialCals.size(); i++) {
                ComplexRowVector temp = _partialCals[i].directivity(port1, port2);
                corrections.insert(corrections.end(), temp.begin(), temp.end());
            }
            channel.corrections().setDirectivity(corrections, port1, port2);
            _vna->isError();

            // sourceMatch
            corrections.clear();
            for (int i = 0; i < _partialCals.size(); i++) {
                ComplexRowVector temp = _partialCals[i].sourceMatch(port1, port2);
                corrections.insert(corrections.end(), temp.begin(), temp.end());
            }
            channel.corrections().setSourceMatch(corrections, port1, port2);
            _vna->isError();

            // reflectionTracking
            corrections.clear();
            for (int i = 0; i < _partialCals.size(); i++) {
                ComplexRowVector temp = _partialCals[i].reflectionTracking(port1, port2);
                corrections.insert(corrections.end(), temp.begin(), temp.end());
            }
            channel.corrections().setReflectionTracking(corrections, port1, port2);
            _vna->isError();

            if (port1 != port2) {
                // loadMatch
                corrections.clear();
                for (int i = 0; i < _partialCals.size(); i++) {
                    ComplexRowVector temp = _partialCals[i].loadMatch(port1, port2);
                    corrections.insert(corrections.end(), temp.begin(), temp.end());
                }
                channel.corrections().setLoadMatch(corrections, port1, port2);
                _vna->isError();

                // transmissionTracking
                corrections.clear();
                for (int i = 0; i < _partialCals.size(); i++) {
                    ComplexRowVector temp = _partialCals[i].transmissionTracking(port1, port2);
                    corrections.insert(corrections.end(), temp.begin(), temp.end());
                }
                channel.corrections().setTransmissionTracking(corrections, port1, port2);
                _vna->isError();
            }
        }
    }

    _vna->multiChannelCalibrationOff();
    return true;
}

uint Calibration::numberOfKits() const {
    return _kits.size();
}
NameLabel Calibration::kitNameLabel(uint index) const {
    return _kits[index].calKit().nameLabel();
}
QString Calibration::shortLabel(uint port) const {
    for (int i = 0; i < _kits.size(); i++) {
        if (!_kits[i].calKit().shortLabel(port).isEmpty()) {
            return _kits[i].calKit().shortLabel(port);
        }
    }

    return QString();
}
QString Calibration::offsetShortAName(uint kitIndex) const {
    if (_kits[kitIndex].calKit().isOffsetShort1())
        return "Offset Short 1";
    else
        return "Offset Short 2";
}
QString Calibration::offsetShortALabel(uint kitIndex, uint port) const {
    return _kits[kitIndex].calKit().offsetShortALabel(port);
}
QString Calibration::offsetShortBName(uint kitIndex) const {
    if (_kits[kitIndex].calKit().isOffsetShort3())
        return "Offset Short 3";
    else
        return "Offset Short 2";
}
QString Calibration::offsetShortBLabel(uint kitIndex, uint port) const {
    return _kits[kitIndex].calKit().offsetShortBLabel(port);
}
QString Calibration::thruLabel(uint index) const {
    for (int i = 0; i < _kits.size(); i++) {
        if (!_kits[i].calKit().thruLabel(index).isEmpty())
            return _kits[i].calKit().thruLabel(index);
    }

    return QString();
}

bool Calibration::allMeasurementsFinished() const {
    foreach (uint port, _ports) {
        if (!isPortFullyMeasured(port))
            return false;
    }

    if (!isAllThrusMeasured())
        return false;

    return true;
}

void Calibration::setPorts(const QVector<uint> &ports) {
    _ports = ports;
    _thrus = thrus(_ports);
}
void Calibration::setConnector(const Connector &connector) {
    _connector = connector;
}
void Calibration::setChannel(uint index) {
    _channel = index;
}
void Calibration::setCalKits(const QVector<FrequencyRange> &kits) {
    _kits = kits;
}

void Calibration::interrupt() {
    QWriteLocker writeLocker(&_lock);
    _interrupt = true;
    for (int i = 0; i < _partialCals.size(); i++)
        _partialCals[i].interrupt();
}
bool Calibration::isInterrupt() const {
    QReadLocker readLocker(&_lock);
    return _interrupt;
}
void Calibration::clearInterrupt() {
    QWriteLocker writeLocker(&_lock);
    _interrupt = false;
}
