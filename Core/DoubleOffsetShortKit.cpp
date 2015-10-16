#include "DoubleOffsetShortKit.h"


// RsaToolbox
#include <Definitions.h>
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QBitArray>
#include <QDebug>


DoubleOffsetShortKit::DoubleOffsetShortKit() :
    _isValid(false),
    _isOShort1(false),
    _isOShort2(false),
    _isOShort3(false),
    _minFreq_Hz(0),
    _maxFreq_Hz(RsaToolbox::DBL_INF)
{
}

DoubleOffsetShortKit::DoubleOffsetShortKit(const DoubleOffsetShortKit &other) :
    _isValid(other._isValid),
    _nameLabel(other._nameLabel),
    _ports(other._ports),
    _shortLabels(other._shortLabels),
    _offsetShort1Labels(other._offsetShort1Labels),
    _offsetShort2Labels(other._offsetShort2Labels),
    _offsetShort3Labels(other._offsetShort3Labels),
    _thruLabels(other._thruLabels),
    _isOShort1(other._isOShort1),
    _isOShort2(other._isOShort2),
    _isOShort3(other._isOShort3),
    _minFreq_Hz(other._minFreq_Hz),
    _maxFreq_Hz(other._maxFreq_Hz)
{
}

DoubleOffsetShortKit::DoubleOffsetShortKit(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender vnaGender, QVector<uint> ports) :
    _isValid(false),
    _isOShort1(false),
    _isOShort2(false),
    _isOShort3(false),
    _minFreq_Hz(0),
    _maxFreq_Hz(DBL_INF)
{
    _nameLabel = calKit.nameLabel();
    _ports = ports;
    getOffsetShortsAndValidate(calKit, vnaGender);
}

bool DoubleOffsetShortKit::isValid() const {
    return _isValid;
}

NameLabel DoubleOffsetShortKit::nameLabel() const {
    return _nameLabel;
}
//Connector DoubleOffsetShortKit::connector() const {
//    return _connector;
//}

bool DoubleOffsetShortKit::isOffsetShort1() const {
    return _isOShort1;
}
bool DoubleOffsetShortKit::isOffsetShort2() const {
    return _isOShort2;
}
bool DoubleOffsetShortKit::isOffsetShort3() const {
    return _isOShort3;
}

double DoubleOffsetShortKit::maximumFrequency_Hz() const {
    return _maxFreq_Hz;
}
double DoubleOffsetShortKit::minimumFrequency_Hz() const {
    return _minFreq_Hz;
}

QString DoubleOffsetShortKit::displayNameLabel() const {
    return _nameLabel.displayText();
}
QString DoubleOffsetShortKit::displayFrequencyRange() const {
    QString text = "%1 - %2";
    text = text.arg(formatValue(_minFreq_Hz, 3, Units::Hertz));
    text = text.arg(formatValue(_maxFreq_Hz, 3, Units::Hertz));
    return text;
}

QString DoubleOffsetShortKit::shortLabel(uint port) const {
    const int i = _ports.indexOf(port);
    return _shortLabels[i];
}
QStringList DoubleOffsetShortKit::shortLabels() const {
    return _shortLabels.toList();
}
QString DoubleOffsetShortKit::offsetShortALabel(uint port) const {
    const int i = _ports.indexOf(port);
    if (isOffsetShort1())
        return _offsetShort1Labels[i];
    else
        return _offsetShort2Labels[i];
}
QStringList DoubleOffsetShortKit::offsetShortALabels() const {
    if (isOffsetShort1())
        return _offsetShort1Labels.toList();
    else
        return _offsetShort2Labels.toList();
}
QString DoubleOffsetShortKit::offsetShortBLabel(uint port) const {
    const int i = _ports.indexOf(port);
    if (isOffsetShort3())
        return _offsetShort3Labels[i];
    else
        return _offsetShort2Labels[i];
}
QStringList DoubleOffsetShortKit::offsetShortBLabels() const {
    if (isOffsetShort3())
        return _offsetShort3Labels.toList();
    else
        return _offsetShort2Labels.toList();
}
QString DoubleOffsetShortKit::thruLabel(const uint &index) const {
    return _thruLabels.value(index).toString();
}
QString DoubleOffsetShortKit::thruLabel(uint port1, uint port2) const {
    return _thruLabels.value(port1, port2).toString();
}
QStringList DoubleOffsetShortKit::thruLabels() const {
    QStringList list;
    for (int i = 0; i < _thruLabels.size(); i++)
        list << _thruLabels[i].toString();
    return list;
}

void DoubleOffsetShortKit::operator=(const DoubleOffsetShortKit &other) {
    _isValid = other._isValid;

    _nameLabel = other._nameLabel;

    _ports = other._ports;

    _shortLabels = other._shortLabels;
    _offsetShort1Labels = other._offsetShort1Labels;
    _offsetShort2Labels = other._offsetShort2Labels;
    _offsetShort3Labels = other._offsetShort3Labels;
    _thruLabels = other._thruLabels;

    _isOShort1 = other._isOShort1;
    _isOShort2 = other._isOShort2;
    _isOShort3 = other._isOShort3;

    _minFreq_Hz = other._minFreq_Hz;
    _maxFreq_Hz = other._maxFreq_Hz;
}
bool DoubleOffsetShortKit::operator==(const DoubleOffsetShortKit &other) {
    if (_nameLabel != other._nameLabel)
        return false;

//    if (_connector != other._connector)
//        return false;

    if (_isValid != other._isValid)
        return false;

    if (!_isValid)
        return true;

    if (_isOShort1 != other._isOShort1)
        return false;
    if (_isOShort2 != other._isOShort2)
        return false;
    if (_isOShort3 != other._isOShort3)
        return false;

    if (_maxFreq_Hz != other._maxFreq_Hz)
        return false;
    if (_minFreq_Hz != other._minFreq_Hz)
        return false;

    // Else
    return true;
}
void DoubleOffsetShortKit::read(QDataStream &stream) {
    stream >> _isValid;

    stream >> _nameLabel;
    stream >> _ports;

    stream >> _shortLabels;
    stream >> _offsetShort1Labels;
    stream >> _offsetShort2Labels;
    stream >> _offsetShort3Labels;
    stream >> _thruLabels;

    stream >> _isOShort1;
    stream >> _isOShort2;
    stream >> _isOShort3;

    stream >> _minFreq_Hz;
    stream >> _maxFreq_Hz;
}
void DoubleOffsetShortKit::write(QDataStream &stream) const {
    stream << _isValid;

    stream << _nameLabel;
    stream << _ports;

    stream << _shortLabels;
    stream << _offsetShort1Labels;
    stream << _offsetShort2Labels;
    stream << _offsetShort3Labels;
    stream << _thruLabels;

    stream << _isOShort1;
    stream << _isOShort2;
    stream << _isOShort3;

    stream << _minFreq_Hz;
    stream << _maxFreq_Hz;
}

void DoubleOffsetShortKit::getOffsetShortsAndValidate(VnaCalKit &calKit, Connector::Gender vnaPortGenders) {
    if (_ports.isEmpty()) {
        _isValid = false;
        return;
    }
    QVector<VnaCalStandard> standards = calKit.standards();
    if (standards.isEmpty()) {
        _isValid = false;
        return;
    }

    QBitArray isShort(_ports.size(), false);
    _shortLabels.clear();
    _shortLabels.resize(_ports.size());

    QBitArray isOffsetShort1(_ports.size(), false);
    _offsetShort1Labels.clear();
    _offsetShort1Labels.resize(_ports.size());

    QBitArray isOffsetShort2(_ports.size(), false);
    _offsetShort2Labels.clear();
    _offsetShort2Labels.resize(_ports.size());

    QBitArray isOffsetShort3(_ports.size(), false);
    _offsetShort3Labels.clear();
    _offsetShort3Labels.resize(_ports.size());

    const bool needThru = _ports.size() > 1;
    ThruValues isThru(_ports, false);
    _thruLabels.clear();
    _thruLabels.resize(_ports);

    foreach(VnaCalStandard s, standards) {
        if (s.isSinglePort()) {
            if (s.isPortSpecific()) {
                if (_ports.contains(s.port())) {
                    const int i = _ports.indexOf(s.port());
                    if (s.isShort()) {
                        isShort[i] = true;
                        _shortLabels[i] = s.label();
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isOffsetShort1()) {
                        isOffsetShort1[i] = true;
                        _offsetShort1Labels[i] = s.label();
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isOffsetShort2()) {
                        isOffsetShort2[i] = true;
                        _offsetShort2Labels[i] = s.label();
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isOffsetShort3()) {
                        isOffsetShort3[i] = true;
                        _offsetShort3Labels[i] = s.label();
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                }
            }
            else {
                if (vnaPortGenders == Connector::Gender::Neutral) {
                    if (s.isShort()) {
                        isShort.fill(true);
                        _shortLabels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isOffsetShort1()) {
                        isOffsetShort1.fill(true);
                        _offsetShort1Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isOffsetShort2()) {
                        isOffsetShort2.fill(true);
                        _offsetShort2Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isOffsetShort3()) {
                        isOffsetShort3.fill(true);
                        _offsetShort3Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                }
                else if (vnaPortGenders == Connector::Gender::Male) {
                    // Need female port
                    if (s.isFemaleShort()) {
                        isShort.fill(true);
                        _shortLabels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isFemaleOffsetShort1()) {
                        isOffsetShort1.fill(true);
                        _offsetShort1Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isFemaleOffsetShort2()) {
                        isOffsetShort2.fill(true);
                        _offsetShort2Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isFemaleOffsetShort3()) {
                        isOffsetShort3.fill(true);
                        _offsetShort3Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                }
                else {
                    // Need male port
                    if (s.isMaleShort()) {
                        isShort.fill(true);
                        _shortLabels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isMaleOffsetShort1()) {
                        isOffsetShort1.fill(true);
                        _offsetShort1Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isMaleOffsetShort2()) {
                        isOffsetShort2.fill(true);
                        _offsetShort2Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                    else if (s.isMaleOffsetShort3()) {
                        isOffsetShort3.fill(true);
                        _offsetShort3Labels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                }
            }
        }
        else if (needThru) {
            if (s.isPortSpecific()) {
                if (isThru.contains(s.port1(), s.port2())) {
                    isThru.value(s.port1(), s.port2()) = true;
                    _thruLabels.value(s.port1(), s.port2()) = s.label();
                    compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                }
            }
            else {
                if (vnaPortGenders == Connector::Gender::Neutral) {
                    // Gender neutral connectors
                    if (s.isThru()) {
                        isThru.fill(true);
                        _thruLabels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                }
                else if (vnaPortGenders == Connector::Gender::Male) {
                    // Need female ports
                    if (s.isThruFF()) {
                        isThru.fill(true);
                        _thruLabels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                }
                else {
                    // Need male ports
                    if (s.isThruMM()) {
                        isThru.fill(true);
                        _thruLabels.fill(s.label());
                        compareFrequencyRange(s.minimumFrequency_Hz(), s.maximumFrequency_Hz());
                    }
                }
            }
        }
    }

    // Shorts?
    if (isShort.count(false) != 0) {
        _isValid = false;
        return;
    }

    // Offset shorts?
    _isOShort1 = isOffsetShort1.count(false) == 0;
    _isOShort2 = isOffsetShort2.count(false) == 0;
    _isOShort3 = isOffsetShort3.count(false) == 0;
    uint numOShorts = 0;
    if (_isOShort1)
        numOShorts++;
    if (_isOShort2)
        numOShorts++;
    if (_isOShort3)
        numOShorts++;
    if (numOShorts != 2) {
        _isValid = false;
        return;
    }

    // Thrus?
    if (needThru && !isThru.areAllValues(true)) {
        _isValid = false;
        return;
    }

    // Valid frequency ranges?
    if (_minFreq_Hz >= _maxFreq_Hz) {
        _isValid = false;
        return;
    }

    // Valid
    _isValid = true;
}
void DoubleOffsetShortKit::compareFrequencyRange(const double &min_Hz, const double &max_Hz) {
    if (_minFreq_Hz < min_Hz)
        _minFreq_Hz = min_Hz;
    if (_maxFreq_Hz > max_Hz)
        _maxFreq_Hz = max_Hz;
}

QDataStream& operator<<(QDataStream &stream, const DoubleOffsetShortKit &kit) {
    kit.write(stream);
    return stream;
}
QDataStream& operator>>(QDataStream &stream, DoubleOffsetShortKit &kit) {
    kit.read(stream);
    return stream;
}
