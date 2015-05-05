#ifndef FREQUENCYRANGE_H
#define FREQUENCYRANGE_H


// Project
#include "DoubleOffsetShortKit.h"

// RsaToolbox
#include <General.h>

// Qt
#include <QDataStream>

class FrequencyRange
{
public:
    FrequencyRange();
    FrequencyRange(DoubleOffsetShortKit kit);
    FrequencyRange(const FrequencyRange &other);
    ~FrequencyRange();

    void setCalKit(DoubleOffsetShortKit kit);
    DoubleOffsetShortKit calKit() const;

    bool isStartFrequency() const;
    double startFrequency_Hz() const;
    void setStartFrequency(double frequency, RsaToolbox::SiPrefix prefix = RsaToolbox::SiPrefix::None);

    bool isStartFrequencyInclusive() const;
    void includeStartFrequency(bool isInclusive = true);

    bool isStopFrequency() const;
    double stopFrequency_Hz() const;
    void setStopFrequency(double frequency, RsaToolbox::SiPrefix prefix = RsaToolbox::SiPrefix::None);

    bool isStopFrequencyInclusive() const;
    void includeStopFrequency(bool isInclusive = false);

    void clearStartStop();

    void read(QDataStream &stream);
    void write(QDataStream &stream) const;

    void operator=(const FrequencyRange &other);
    bool operator==(const FrequencyRange &other);

private:
    DoubleOffsetShortKit _kit;

    bool _includeStartFrequency;
    double _startFreq_Hz;

    bool _includeStopFrequency;
    double _stopFreq_Hz;
};

QDataStream& operator<<(QDataStream &stream, const FrequencyRange &kit);
QDataStream& operator>>(QDataStream &stream, FrequencyRange &kit);

#endif // FREQUENCYRANGE_H
