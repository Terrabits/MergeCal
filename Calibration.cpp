#include "Calibration.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


Calibration::Calibration(QObject *parent) :
    QObject(parent),
    _vna(NULL),
    _channel(0)
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
    qDebug() << "Calibration::initialize";
    _isMatchMeasured.fill(false, _ports.size());
    _isShortMeasured.fill(false, _ports.size());
    _isOffsetShortAMeasured.fill(QBitArray(_ports.size(), false), _kits.size());
    _isOffsetShortBMeasured.fill(QBitArray(_ports.size(), false), _kits.size());
    _thrus.clear();
    for (int i = 0; i < _ports.size(); i++) {
        for (int j = i+1; j < _ports.size(); j++) {
            Thru thru;
            thru.port1 = _ports[i];
            thru.port2 = _ports[j];
            _thrus << thru;
        }
    }

    _partialCals.clear();
    foreach (FrequencyRange kit, _kits) {
        PartialCal partial;
        partial.setVna(_vna);
        partial.setOriginalChannel(_channel);
        partial.setPorts(_ports);
        partial.setConnector(_connector);
        partial.setCalKit(kit);
        partial.initialize();
        _partialCals << partial;
    }
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

bool Calibration::isMatchMeasured(uint port) {
    return _isMatchMeasured[_ports.indexOf(port)];
}
void Calibration::measureMatch(uint port) {
    for (int i = 0; i < _partialCals.size(); i++) {
        _partialCals[i].measureMatch(port);
    }
    _isMatchMeasured[_ports.indexOf(port)] = true;
}

bool Calibration::isShortMeasured(uint port) {
    return _isShortMeasured[_ports.indexOf(port)];
}
void Calibration::measureShort(uint port) {
    for (int i = 0; i < _partialCals.size(); i++) {
        _partialCals[i].measureShort(port);
    }
    _isShortMeasured[_ports.indexOf(port)] = true;
}

bool Calibration::isOffsetShortAMeasured(uint kitIndex, uint port) {
    return _isOffsetShortAMeasured[kitIndex][_ports.indexOf(port)];
}
void Calibration::measureOffsetShortA(uint kitIndex, uint port) {
    _partialCals[kitIndex].measureOffsetShortA(port);
    _isOffsetShortAMeasured[kitIndex][_ports.indexOf(port)] = true;
}

bool Calibration::isOffsetShortBMeasured(uint kitIndex, uint port) {
    return _isOffsetShortBMeasured[kitIndex][_ports.indexOf(port)];
}
void Calibration::measureOffsetShortB(uint kitIndex, uint port) {
    _partialCals[kitIndex].measureOffsetShortB(port);
    _isOffsetShortBMeasured[kitIndex][_ports.indexOf(port)] = true;
}

bool Calibration::isThruMeasured(uint index) {
    return _thrus[index].isMeasured;
}
uint Calibration::numberOfThrus() const {
    return _thrus.size();
}
Thru Calibration::thru(uint index) const {
    return _thrus[index];
}
void Calibration::measureThru(uint index) {
    for (int i = 0; i < _partialCals.size(); i++) {
        _partialCals[i].measureThru(_thrus[index].port1, _thrus[index].port2);
    }
    _thrus[index].isMeasured = true;
}

uint Calibration::numberOfKits() const {
    return _kits.size();
}
NameLabel Calibration::kit(uint index) const {
    return _kits[index].calKit().nameLabel();
}
QString Calibration::offsetShortAName(uint kitIndex) const {
    if (_kits[kitIndex].calKit().isOffsetShort1())
        return "Offset Short 1";
    else
        return "Offset Short 2";
}
QString Calibration::offsetShortBName(uint kitIndex) const {
    if (_kits[kitIndex].calKit().isOffsetShort3())
        return "Offset Short 3";
    else
        return "Offset Short 2";
}

void Calibration::setPorts(const QVector<uint> &ports) {
    _ports = ports;
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
