#include "Ports.h"

// std lib
#include <algorithm>

uint thruCombinations(const Ports &ports) {
    const uint n = ports.size();
    return (n * (n-1))/2;
}

Thru::Thru() {
    port1 = 0;
    port2 = 0;
}
bool Thru::isValid() const {
    if (port1 == 0)
        return false;
    if (port2 == 0)
        return false;
    if (port1 == port2)
        return false;

    return true;
}
bool Thru::isOrderAscending() const {
    return port1 < port2;
}
bool Thru::isOrderDescending() const {
    return port1 > port2;
}

void Thru::orderAscending() {
    if (port1 > port2)
        swap();
}
void Thru::orderDescending() {
    if (port1 < port2)
        swap();
}
void Thru::swap() {
    const uint _port1 = port1;
    port1 = port2;
    port2 = _port1;
}

QString Thru::toString(const QString &delimiter) const {
    QString s = "%1%2%3";
    s = s.arg(port1);
    s = s.arg(delimiter);
    s = s.arg(port2);
    return s;
}

void Thru::read(QDataStream &stream) {
    quint32 i;
    stream >> i;
    port1 = i;
    stream >> i;
    port2 = i;
}
void Thru::write(QDataStream &stream) const {
    stream << quint32(port1);
    stream << quint32(port2);
}

bool operator==(const Thru &left, const Thru &right) {
    if (left.port1 == right.port1 && left.port2 == right.port2)
        return true;
    if (left.port1 == right.port2 && left.port2 == right.port1)
        return true;
    // else
    return false;
}
QDataStream& operator>>(QDataStream &stream, Thru &thru) {
    thru.read(stream);
    return stream;
}
QDataStream& operator<<(QDataStream &stream, const Thru &thru) {
    thru.write(stream);
    return stream;
}

Thrus thrus(const Ports &ports) {
    Ports _ports(ports);
    std::sort(_ports.begin(), _ports.end());
    Thrus thrus;
    const uint size = ports.size();
    for (uint i = 0; i < size; i++) {
        for (uint j = i+1; j < size; j++) {
            Thru thru;
            thru.port1 = ports[i];
            thru.port2 = ports[j];
            thrus << thru;
        }
    }
    return thrus;
}

ThruValues::ThruValues() {

}
ThruValues::ThruValues(const Ports &ports) :
    _ports(ports),
    _thrus(::thrus(ports)),
    _values(_thrus.size())
{

}
ThruValues::ThruValues(const Ports &ports, QVariant value) :
    _ports(ports),
    _thrus(::thrus(ports)),
    _values(_thrus.size(), value)
{

}
ThruValues::ThruValues(const ThruValues &other) :
    _ports(other._ports),
    _thrus(other._thrus),
    _values(other._values)
{

}

bool ThruValues::isEmpty() const {
    return _ports.isEmpty();
}
bool ThruValues::areAllValues(const QVariant &value) const {
    foreach (const QVariant &_value, _values) {
        if (_value != value)
            return false;
    }
    // else
    return true;
}
bool ThruValues::contains(uint port1, uint port2) const {
    if (port1 == 0)
        return false;
    if (port2 == 0)
        return false;
    if (port1 == port2)
        return false;
    if (port1 > port2) {
        const uint _port1 = port1;
        port1 = port2;
        port2 = _port1;
    }

    foreach (Thru thru, _thrus) {
        if (thru.port1 == port1 && thru.port2 == port2)
            return true;
    }

    return false;
}
bool ThruValues::contains(const Thru &thru) const {
    return contains(thru.port1, thru.port2);
}
int ThruValues::indexOf(uint port1, uint port2) const {
    if (port1 == 0)
        return -1;
    if (port2 == 0)
        return -1;
    if (port1 == port2)
        return -1;
    if (port1 > port2) {
        const uint _port1 = port1;
        port1 = port2;
        port2 = _port1;
    }

    for (int i = 0; i < _thrus.size(); i++) {
        if (_thrus[i].port1 == port1 && _thrus[i].port2 == port2)
            return i;
    }

    return -1;
}
int ThruValues::indexOf(const Thru &thru) const {
    return indexOf(thru.port1, thru.port2);
}
int ThruValues::size() const {
    return _thrus.size();
}
void ThruValues::fill(const QVariant &value) {
    _values.fill(value);
}

QVariant &ThruValues::value(const uint &index) {
    return _values[index];
}
QVariant ThruValues::value(const uint &index) const {
    return _values[index];
}
QVariant &ThruValues::value(const uint &port1, const uint &port2) {
    return _values[indexOf(port1, port2)];
}
QVariant ThruValues::value(const uint &port1, const uint &port2) const {
    return _values[indexOf(port1, port2)];
}
Ports ThruValues::ports() const {
    return _ports;
}
Thrus ThruValues::thrus() const {
    return _thrus;
}

void ThruValues::clear() {
    _ports.clear();
    _thrus.clear();
    _values.clear();
}
void ThruValues::resize(const Ports &ports) {
    ThruValues oldValues(*this);

    _ports = ports;
    _thrus = ::thrus(ports);
    _values = QVector<QVariant>(_thrus.size());

    copyValuesFrom(oldValues);
}
void ThruValues::resize(const Ports &ports, QVariant value) {
    ThruValues oldValues(*this);

    _ports = ports;
    _thrus = ::thrus(ports);
    _values = QVector<QVariant>(_thrus.size(), value);

    copyValuesFrom(oldValues);
}
void ThruValues::copyValuesFrom(const ThruValues &other) {
    for (int i = 0; i < other._thrus.size(); i++) {
        const Thru &thru(other._thrus[i]);
        if (contains(thru))
            _values[indexOf(thru)] = other._values[i];
    }
}

void ThruValues::read(QDataStream &stream) {
    QVector<quint32> v;
    stream >> v;
    _ports = v;

    stream >> _thrus;
    stream >> _values;
}
void ThruValues::write(QDataStream &stream) const {
    stream << QVector<quint32>(_ports);
    stream << _thrus;
    stream << _values;
}

void ThruValues::operator=(const ThruValues &other) {
    _ports = other._ports;
    _thrus = other._thrus;
    _values = other._values;
}
QVariant& ThruValues::operator[](const uint &index) {
    return _values[index];
}
QVariant ThruValues::operator[](const uint &index) const {
    return _values[index];
}


QDataStream& operator<<(QDataStream &stream, const ThruValues &values) {
    values.write(stream);
    return stream;
}
QDataStream& operator>>(QDataStream &stream, ThruValues &values) {
    values.read(stream);
    return stream;
}

