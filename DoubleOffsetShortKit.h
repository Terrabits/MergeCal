#ifndef DOUBLEOFFSETSHORTKIT_H
#define DOUBLEOFFSETSHORTKIT_H


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
    DoubleOffsetShortKit(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender gender, bool needThru);

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
    QString shortLabel() const;
    QString offsetShortALabel() const;
    QString offsetShortBLabel() const;
    QString thruLabel() const;

    void operator=(const DoubleOffsetShortKit &other);
    bool operator==(const DoubleOffsetShortKit &other);

    void read(QDataStream &stream);
    void write(QDataStream &stream) const;

private:
    bool _isValid;

    RsaToolbox::NameLabel _nameLabel;

    QString _shortLabel;
    QString _offsetShort1Label;
    QString _offsetShort2Label;
    QString _offsetShort3Label;
    QString _thruLabel;

    bool _isOShort1;
    bool _isOShort2;
    bool _isOShort3;

    double _minFreq_Hz;
    double _maxFreq_Hz;

    bool getOffsetShortsAndValidate(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender gender, bool needThru);
};

QDataStream& operator<<(QDataStream &stream, const DoubleOffsetShortKit &kit);
QDataStream& operator>>(QDataStream &stream, DoubleOffsetShortKit &kit);


#endif // DOUBLEOFFSETSHORTKIT_H
