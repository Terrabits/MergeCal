#include "FrequencyRange.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;


FrequencyRange::FrequencyRange() :
    _includeStartFrequency(false),
    _startFreq_Hz(-1),
    _includeStopFrequency(false),
    _stopFreq_Hz(-1)
{

}
FrequencyRange::FrequencyRange(DoubleOffsetShortKit kit) :
    _kit(kit),
    _includeStartFrequency(false),
    _startFreq_Hz(-1),
    _includeStopFrequency(false),
    _stopFreq_Hz(-1)

{

}
FrequencyRange::FrequencyRange(const FrequencyRange &other) :
    _kit(other._kit),
    _includeStartFrequency(other._includeStartFrequency),
    _startFreq_Hz(other._startFreq_Hz),
    _includeStopFrequency(other._includeStopFrequency),
    _stopFreq_Hz(other._stopFreq_Hz)
{

}

FrequencyRange::~FrequencyRange()
{

}

void FrequencyRange::setCalKit(DoubleOffsetShortKit kit) {
    _kit = kit;
}
DoubleOffsetShortKit FrequencyRange::calKit() const {
    return _kit;
}

bool FrequencyRange::isStartFrequency() const {
    return _startFreq_Hz != -1;
}
double FrequencyRange::startFrequency_Hz() const {
    return _startFreq_Hz;
}
void FrequencyRange::setStartFrequency(double frequency, RsaToolbox::SiPrefix prefix) {
    _startFreq_Hz = frequency * toDouble(prefix);
}

bool FrequencyRange::isStartFrequencyInclusive() const {
    return _includeStartFrequency;
}
void FrequencyRange::includeStartFrequency(bool isInclusive) {
    _includeStartFrequency = isInclusive;
}

bool FrequencyRange::isStopFrequency() const {
    return _stopFreq_Hz != -1;
}
double FrequencyRange::stopFrequency_Hz() const {
    return _stopFreq_Hz;
}
void FrequencyRange::setStopFrequency(double frequency, SiPrefix prefix) {
    _stopFreq_Hz = frequency * toDouble(prefix);
}

bool FrequencyRange::isStopFrequencyInclusive() const {
    return _includeStopFrequency;
}
void FrequencyRange::includeStopFrequency(bool isInclusive) {
    _includeStopFrequency = isInclusive;
}

void FrequencyRange::clearStartStop() {
    _startFreq_Hz = -1;
    _stopFreq_Hz = -1;
}

void FrequencyRange::operator=(const FrequencyRange &other) {
    _kit = other._kit;
    _includeStartFrequency = other._includeStartFrequency;
    _startFreq_Hz = other._startFreq_Hz;
    _includeStopFrequency = other._includeStopFrequency;
    _stopFreq_Hz = other._stopFreq_Hz;
}
bool FrequencyRange::operator==(const FrequencyRange &other) {
    return _kit == other._kit;
}
