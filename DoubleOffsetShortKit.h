#ifndef DOUBLEOFFSETSHORTKIT_H
#define DOUBLEOFFSETSHORTKIT_H


// Project
#include "Ports.h"

// RsaToolbox
#include <Definitions.h>
#include <NameLabel.h>
#include <Connector.h>
#include <Vna.h>
#include <VnaCalKit.h>

// Qt
#include <QDataStream>


class DoubleOffsetShortKit
{
public:
    DoubleOffsetShortKit();
    DoubleOffsetShortKit(const DoubleOffsetShortKit &other);
    DoubleOffsetShortKit(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender vnaGender, QVector<uint> ports);

    bool isValid() const;

    RsaToolbox::NameLabel nameLabel() const;
//    RsaToolbox::Connector connector() const;

    bool isOffsetShort1() const;
    bool isOffsetShort2() const;
    bool isOffsetShort3() const;

    double maximumFrequency_Hz() const;
    double minimumFrequency_Hz() const;

    QString displayNameLabel() const;
    QString displayFrequencyRange() const;

    // Labels
    QString shortLabel(uint port) const;
    QStringList shortLabels() const;
    QString offsetShortALabel(uint port) const;
    QStringList offsetShortALabels() const;
    QString offsetShortBLabel(uint port) const;
    QStringList offsetShortBLabels() const;
    QString thruLabel(const uint &index) const;
    QString thruLabel(uint port1, uint port2) const;
    QStringList thruLabels() const;

    void operator=(const DoubleOffsetShortKit &other);
    bool operator==(const DoubleOffsetShortKit &other);

    void read(QDataStream &stream);
    void write(QDataStream &stream) const;

private:
    bool _isValid;

    RsaToolbox::NameLabel _nameLabel;

    QVector<uint> _ports;
    QVector<QString> _shortLabels;
    QVector<QString> _offsetShort1Labels;
    QVector<QString> _offsetShort2Labels;
    QVector<QString> _offsetShort3Labels;
    mutable ThruValues _thruLabels;

    bool _isOShort1;
    bool _isOShort2;
    bool _isOShort3;

    double _minFreq_Hz;
    double _maxFreq_Hz;

    void getOffsetShortsAndValidate(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender vnaPortGenders, QVector<uint> ports);
    void compareFrequencyRange(const double &min_Hz, const double &max_Hz);
};

QDataStream& operator<<(QDataStream &stream, const DoubleOffsetShortKit &kit);
QDataStream& operator>>(QDataStream &stream, DoubleOffsetShortKit &kit);


#endif // DOUBLEOFFSETSHORTKIT_H
