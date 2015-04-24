#include "FrequencyRange.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;


FrequencyRange::FrequencyRange() :
    _startFreq_Hz(-1),
    _stopFreq_Hz(-1)
{

}
FrequencyRange::FrequencyRange(DoubleOffsetShortKit kit) :
    _kit(kit),
    _startFreq_Hz(-1),
    _stopFreq_Hz(-1)

{

}
FrequencyRange::FrequencyRange(const FrequencyRange &other) :
    _kit(other._kit),
    _startFreq_Hz(other._startFreq_Hz),
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

bool FrequencyRange::isStopFrequency() const {
    return _stopFreq_Hz != -1;
}
double FrequencyRange::stopFrequency_Hz() const {
    return _stopFreq_Hz;
}
void FrequencyRange::setStopFrequency(double frequency, SiPrefix prefix) {
    _stopFreq_Hz = frequency * toDouble(prefix);
}

void FrequencyRange::clearStartStop() {
    _startFreq_Hz = -1;
    _stopFreq_Hz = -1;
}

void FrequencyRange::operator=(const FrequencyRange &other) {
    _kit = other._kit;
    _startFreq_Hz = other._startFreq_Hz;
    _stopFreq_Hz = other._stopFreq_Hz;
}
bool FrequencyRange::operator==(const FrequencyRange &other) {
    return _kit == other._kit;
}
