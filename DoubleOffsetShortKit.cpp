#include "DoubleOffsetShortKit.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


DoubleOffsetShortKit::DoubleOffsetShortKit() :
    _isValid(false),
    _isOShort1(false),
    _isOShort2(false),
    _isOShort3(false),
    _minFreq_Hz(0),
    _maxFreq_Hz(1.0E12)
{
}

DoubleOffsetShortKit::DoubleOffsetShortKit(const DoubleOffsetShortKit &other) {
    _isValid = other._isValid;

    _nameLabel = other._nameLabel;

    _shortLabel = other._shortLabel;
    _offsetShort1Label = other._offsetShort1Label;
    _offsetShort2Label = other._offsetShort2Label;
    _offsetShort3Label = other._offsetShort3Label;
    _thruLabel = other._thruLabel;

    _isOShort1 = other._isOShort1;
    _isOShort2 = other._isOShort2;
    _isOShort3 = other._isOShort3;

    _minFreq_Hz = other._minFreq_Hz;
    _maxFreq_Hz = other._maxFreq_Hz;
}

DoubleOffsetShortKit::DoubleOffsetShortKit(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender gender, bool needThru) :
    _isValid(false),
    _isOShort1(false),
    _isOShort2(false),
    _isOShort3(false),
    _minFreq_Hz(0),
    _maxFreq_Hz(1.0E12)
{
    _nameLabel = calKit.nameLabel();
//    _connector = calKit.connectorType();
    if (getOffsetShortsAndValidate(calKit, gender, needThru)) {
        _isValid = true;
    }
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

QString DoubleOffsetShortKit::shortLabel() const {
    return _shortLabel;
}
QString DoubleOffsetShortKit::offsetShortALabel() const {
    if (isOffsetShort1())
        return _offsetShort1Label;
    else
        return _offsetShort2Label;
}
QString DoubleOffsetShortKit::offsetShortBLabel() const {
    if (isOffsetShort3())
        return _offsetShort3Label;
    else
        return _offsetShort2Label;
}
QString DoubleOffsetShortKit::thruLabel() const {
    return _thruLabel;
}

void DoubleOffsetShortKit::operator=(const DoubleOffsetShortKit &other) {
    _isValid = other._isValid;

    _nameLabel = other._nameLabel;

    _shortLabel = other._shortLabel;
    _offsetShort1Label = other._offsetShort1Label;
    _offsetShort2Label = other._offsetShort2Label;
    _offsetShort3Label = other._offsetShort3Label;
    _thruLabel = other._thruLabel;

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

    stream >> _shortLabel;
    stream >> _offsetShort1Label;
    stream >> _offsetShort2Label;
    stream >> _offsetShort3Label;
    stream >> _thruLabel;

    stream >> _isOShort1;
    stream >> _isOShort2;
    stream >> _isOShort3;

    stream >> _minFreq_Hz;
    stream >> _maxFreq_Hz;
}
void DoubleOffsetShortKit::write(QDataStream &stream) const {
    stream << _isValid;

    stream << _nameLabel;

    stream << _shortLabel;
    stream << _offsetShort1Label;
    stream << _offsetShort2Label;
    stream << _offsetShort3Label;
    stream << _thruLabel;

    stream << _isOShort1;
    stream << _isOShort2;
    stream << _isOShort3;

    stream << _minFreq_Hz;
    stream << _maxFreq_Hz;
}

bool DoubleOffsetShortKit::getOffsetShortsAndValidate(VnaCalKit &calKit, Connector::Gender gender, bool needThru) {
    QVector<VnaCalStandard> standards = calKit.standards();

    bool isShort = false;
    bool isThru = false;
    VnaCalStandard os1, os2, os3;

    if (gender == Connector::Gender::Neutral) {
        foreach (VnaCalStandard s, standards) {
            if (s.isShort()) {
                _shortLabel = s.label();
                isShort = true;
            }
            if (s.isThru()) {
                _thruLabel = s.label();
                isThru = true;
            }
            if (s.isOffsetShort1()) {
                _offsetShort1Label = s.label();
                _isOShort1 = true;
                os1 = s;
            }
            if (s.isOffsetShort2()) {
                _offsetShort2Label = s.label();
                _isOShort2 = true;
                os2 = s;
            }
            if (s.isOffsetShort3()) {
                _offsetShort3Label = s.label();
                _isOShort3 = true;
                os3 = s;
            }
        }
    }
    else if (gender == Connector::Gender::Female) {
        foreach (VnaCalStandard s, standards) {
            if (s.isMaleShort()) {
                _shortLabel = s.label();
                isShort = true;
            }
            if (s.isThruMM()) {
                _thruLabel = s.label();
                isThru = true;
            }
            if (s.isMaleOffsetShort1()) {
                _offsetShort1Label = s.label();
                _isOShort1 = true;
                os1 = s;
            }
            if (s.isMaleOffsetShort2()) {
                _offsetShort2Label = s.label();
                _isOShort2 = true;
                os2 = s;
            }
            if (s.isMaleOffsetShort3()) {
                _offsetShort3Label = s.label();
                _isOShort3 = true;
                os3 = s;
            }
        }
    }
    else {
        foreach (VnaCalStandard s, standards) {
            if (s.isFemaleShort()) {
                _shortLabel = s.label();
                isShort = true;
            }
            if (s.isThruFF()) {
                _thruLabel = s.label();
                isThru = true;
            }
            if (s.isFemaleOffsetShort1()) {
                _offsetShort1Label = s.label();
                _isOShort1 = true;
                os1 = s;
            }
            if (s.isFemaleOffsetShort2()) {
                _offsetShort2Label = s.label();
                _isOShort2 = true;
                os2 = s;
            }
            if (s.isFemaleOffsetShort3()) {
                _offsetShort3Label = s.label();
                _isOShort3 = true;
                os3 = s;
            }
        }
    }

    if (!isShort)
        return false;
    if (needThru && !isThru)
        return false;

    uint numOShorts = 0;
    if (_isOShort1)
        numOShorts++;
    if (_isOShort2)
        numOShorts++;
    if (_isOShort3)
        numOShorts++;

    if (numOShorts != 2)
        return false;

    if (_isOShort1) {
        _minFreq_Hz = os1.minimumFrequency_Hz();
        _maxFreq_Hz = os1.maximumFrequency_Hz();
    }
    else {
        _minFreq_Hz = os2.minimumFrequency_Hz();
        _maxFreq_Hz = os2.maximumFrequency_Hz();
    }

    if (_isOShort3) {
        _minFreq_Hz = std::max(_minFreq_Hz, os3.minimumFrequency_Hz());
        _maxFreq_Hz = std::min(_maxFreq_Hz, os3.maximumFrequency_Hz());
    }
    else {
        _minFreq_Hz = std::max(_minFreq_Hz, os2.minimumFrequency_Hz());
        _maxFreq_Hz = std::min(_maxFreq_Hz, os2.maximumFrequency_Hz());
    }

    return true;
}

QDataStream& operator<<(QDataStream &stream, const DoubleOffsetShortKit &kit) {
    kit.write(stream);
    return stream;
}
QDataStream& operator>>(QDataStream &stream, DoubleOffsetShortKit &kit) {
    kit.read(stream);
    return stream;
}
