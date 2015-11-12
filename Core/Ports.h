#ifndef PORTS_H
#define PORTS_H


// RsaToolbox
#include <General.h>
// Qt
#include <QVector>
#include <QVariant>
#include <QDataStream>


typedef QMap<uint, uint> PortMap;

class Thru {
public:

    Thru();

    uint port1;
    uint port2;

    bool isValid() const;
    bool isOrderAscending() const;
    bool isOrderDescending() const;
    void orderAscending();
    void orderDescending();
    void swap();

    QString toString(const QString &delimiter = "-") const;

    void read(QDataStream &stream);
    void write(QDataStream &stream) const;
};
bool operator==(const Thru &left, const Thru &right);
QDataStream& operator>>(QDataStream &stream, Thru &thru);
QDataStream& operator<<(QDataStream &stream, const Thru &thru);
typedef QVector<Thru> Thrus;

class ThruValues {
public:
    ThruValues();
    ThruValues(const RsaToolbox::Ports &ports);
    ThruValues(const RsaToolbox::Ports &ports, QVariant value);
    ThruValues(const ThruValues &other);

    bool isEmpty() const;
    bool areAllValues(const QVariant &value) const;
    bool contains(uint port1, uint port2) const;
    bool contains(const Thru &thru) const;
    int indexOf(uint port1, uint port2) const;
    int indexOf(const Thru &thru) const;
    int size() const;
    void fill(const QVariant &value);
    QVariant &value(const uint &index);
    QVariant value(const uint &index) const;
    QVariant &value(const uint &port1, const uint &port2);
    QVariant value(const uint &port1, const uint &port2) const;

    RsaToolbox::Ports ports() const;
    Thrus thrus() const;

    void clear();
    void resize(const RsaToolbox::Ports &ports);
    void resize(const RsaToolbox::Ports &ports, QVariant value);
    void copyValuesFrom(const ThruValues &other);

    void read(QDataStream &stream);
    void write(QDataStream &stream) const;

    void operator=(const ThruValues &other);
    QVariant &operator[](const uint &index);
    QVariant operator[](const uint &index) const;

private:
    RsaToolbox::Ports _ports;
    Thrus _thrus;
    QVector<QVariant> _values;
};
QDataStream& operator>>(QDataStream &stream, ThruValues &values);
QDataStream& operator<<(QDataStream &stream, const ThruValues &values);

uint thruCombinations(const RsaToolbox::Ports &ports);
Thrus thrus(const RsaToolbox::Ports &ports);

#endif // PORTS_H
