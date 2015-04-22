#include "DoubleOffsetShortKit.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;


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
//    _connector = other._connector;

    _isOShort1 = other._isOShort1;
    _isOShort2 = other._isOShort2;
    _isOShort3 = other._isOShort3;

    _minFreq_Hz = other._minFreq_Hz;
    _maxFreq_Hz = other._maxFreq_Hz;
}

DoubleOffsetShortKit::DoubleOffsetShortKit(RsaToolbox::VnaCalKit &calKit) :
    _isValid(false),
    _isOShort1(false),
    _isOShort2(false),
    _isOShort3(false),
    _minFreq_Hz(0),
    _maxFreq_Hz(1.0E12)
{
    _nameLabel = calKit.nameLabel();
//    _connector = calKit.connectorType();
    if (getOffsetShorts(calKit) && getFrequencyRange(calKit)) {
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

void DoubleOffsetShortKit::operator=(const DoubleOffsetShortKit &other) {
    _isValid = other._isValid;

    _nameLabel = other._nameLabel;
//    _connector = other._connector;

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
