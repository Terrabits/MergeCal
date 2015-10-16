#include "PortsPage.h"
#include "ui_PortsPage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QKeyEvent>
#include <QDebug>


PortsPage::PortsPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::PortsPage),
    _header(NULL),
    _vna(NULL),
    _keys(NULL)
{
    ui->setupUi(this);

    ui->ports->setModel(&_portsModel);
}

PortsPage::~PortsPage()
{
    delete ui;
}


void PortsPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/1 Ports.bmp"));
    ui->error->clearMessage();
    disconnect(buttons()->cancel(), SIGNAL(clicked()),
            wizard(), SLOT(restart()));
    connect(buttons()->cancel(), SIGNAL(clicked()),
            wizard(), SLOT(close()));
}
void PortsPage::backToThis() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/1 Ports.bmp"));
    ui->error->clearMessage();
}

bool PortsPage::isReadyForNext() {
    if (_portsModel.ports().isEmpty()) {
        ui->error->showMessage("*No ports selected");
        ui->ports->setFocus();
        ui->ports->setCurrentIndex(_portsModel.index(0));
        return false;
    }

    VnaChannel::SweepType sweepType = _vna->channel(channel()).sweepType();
    if (sweepType == VnaChannel::SweepType::Power
            || sweepType == VnaChannel::SweepType::Cw
            || sweepType == VnaChannel::SweepType::Time)
    {
        ui->channel->setFocus();
        ui->error->showMessage("*Channel must perform a frequency sweep");
        return false;
    }

    saveKeys();
    emit portsSelected(_portsModel.ports());
    emit connectorSelected(connector());
    emit channelSelected(channel());
    return true;
}

void PortsPage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *PortsPage::headerLabel() {
    return _header;
}

void PortsPage::setVna(Vna *vna) {
    _vna = vna;
    _portsModel.setVna(vna);
    updateConnectors();
    updateGender();
    updateChannels();
    loadKeys();
}
Vna *PortsPage::vna() const {
    return _vna;
}

void PortsPage::setKeys(Keys *keys) {
    _keys = keys;
    loadKeys();
}

QVector<uint> PortsPage::ports() const {
    return _portsModel.ports();
}
Connector PortsPage::connector() const {
    const int typeIndex = ui->connectorType->currentIndex();
    Connector connector(_connectors[typeIndex]);

    const int genderIndex = ui->connectorGender->currentIndex();
    const int genderCount = ui->connectorGender->count();
    if (genderCount == 1)
        connector.setGender(Connector::Gender::Neutral);
    else if (genderIndex == 0)
        connector.setGender(Connector::Gender::Male);
    else
        connector.setGender(Connector::Gender::Female);

    return connector;
}
uint PortsPage::channel() const {
    int i = ui->channel->currentIndex();
    return _channels[i];
}

// Private
void PortsPage::updateConnectors() {
    ui->connectorType->clear();
    _connectors = _vna->connectorTypes();
    foreach (const Connector &connector, _connectors)
        ui->connectorType->addItem(connector.displayType());
    ui->connectorType->setCurrentIndex(0);
}
void PortsPage::updateGender() {
    if (_connectors.isEmpty())
        return;

    Connector connector = _connectors[ui->connectorType->currentIndex()];
    if (connector.isGenderNeutral() && ui->connectorGender->count() == 2) {
        ui->connectorGender->clear();
        ui->connectorGender->addItem("Neutral", Connector::Gender::Neutral);
    }
    else if (!connector.isGenderNeutral() && ui->connectorGender->count() == 1) {
        ui->connectorGender->clear();
        ui->connectorGender->addItem("Male", Connector::Gender::Male);
        ui->connectorGender->addItem("Female", Connector::Gender::Female);
    }
}

void PortsPage::updateChannels() {
    ui->channel->clear();
    _channels = _vna->channels();
    foreach (const uint &channel, _channels)
        ui->channel->addItem(QVariant(channel).toString());
    ui->channel->setCurrentIndex(0);
}

void PortsPage::on_connectorType_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateGender();
}

void PortsPage::loadKeys() {
    if (_vna == NULL || _keys == NULL)
        return;

    if (_keys->exists("PortsPage_Ports")) {
        QVector<uint> ports;
        _keys->get("PortsPage_Ports", ports);
        _portsModel.setPorts(ports);
    }
    if (_keys->exists("PortsPage_ConnectorType")) {
        Connector connector;
        _keys->get("PortsPage_ConnectorType", connector);
        int connectorIndex = _connectors.indexOf(connector);
        if (connectorIndex != -1) {
            ui->connectorType->setCurrentIndex(connectorIndex);
            updateGender(); // Necessary?
            if (_keys->exists("PortsPage_ConnectorGender")) {
                int genderIndex;
                _keys->get("PortsPage_ConnectorGender", genderIndex);
                if (genderIndex < ui->connectorGender->count())
                    ui->connectorGender->setCurrentIndex(genderIndex);
            }
        }
    }
    if (_keys->exists("PortsPage_Channel")) {
        uint channel;
        _keys->get("PortsPage_Channel", channel);
        int index = _channels.indexOf(channel);
        if (index != -1)
            ui->channel->setCurrentIndex(index);
    }
}
void PortsPage::saveKeys() {
    if (_vna == NULL || _keys == NULL)
        return;

    _keys->set("PortsPage_Ports", ports());

    const int typeIndex = ui->connectorType->currentIndex();
    Connector connector(_connectors[typeIndex]);
    _keys->set("PortsPage_ConnectorType", connector);

    _keys->set("PortsPage_ConnectorGender", ui->connectorGender->currentIndex());

    _keys->set("PortsPage_Channel", channel());
}
