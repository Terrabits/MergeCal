#include "PartialCal.h"

PartialCal::PartialCal(QObject *parent) :
    QObject(parent),
    _vna(NULL),
    _originalChannel(0),
    _channel(0)
{

}

PartialCal::~PartialCal()
{

}

void PartialCal::setVna(RsaToolbox::Vna *vna) {
    _vna = vna;
}
void PartialCal::setOriginalChannel(uint index) {
    _originalChannel = index;
}
void PartialCal::setPorts(const QVector<uint> &ports) {
    _ports = ports;
}
void PartialCal::setConnector(const RsaToolbox::Connector &connector) {
    _connector = connector;
}
void PartialCal::setCalKit(const FrequencyRange &kit) {
    _calKit = kit;
}

void PartialCal::initialize() {
    _vna->channel(_originalChannel).select();
}
