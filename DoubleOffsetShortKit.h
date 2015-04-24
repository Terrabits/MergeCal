#ifndef DOUBLEOFFSETSHORTKIT_H
#define DOUBLEOFFSETSHORTKIT_H


// RsaToolbox
#include <Definitions.h>
#include <NameLabel.h>
#include <Connector.h>
#include <Vna.h>
#include <VnaCalKit.h>


class DoubleOffsetShortKit
{
public:
    DoubleOffsetShortKit();
    DoubleOffsetShortKit(const DoubleOffsetShortKit &other);
    DoubleOffsetShortKit(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender gender);

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

    void operator=(const DoubleOffsetShortKit &other);
    bool operator==(const DoubleOffsetShortKit &other);

private:
    bool _isValid;

    RsaToolbox::NameLabel _nameLabel;
//    RsaToolbox::Connector _connector;

    bool _isOShort1;
    bool _isOShort2;
    bool _isOShort3;

    double _minFreq_Hz;
    double _maxFreq_Hz;

    bool getOffsetShorts(RsaToolbox::VnaCalKit &calKit, RsaToolbox::Connector::Gender gender);
};

#endif // DOUBLEOFFSETSHORTKIT_H
