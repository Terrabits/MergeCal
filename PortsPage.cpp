#include "PortsPage.h"
#include "ui_PortsPage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QKeyEvent>
#include <QDebug>


PortsPage::PortsPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::PortsPage)
{
    ui->setupUi(this);

    ui->ports->setModel(&_model);
}

PortsPage::~PortsPage()
{
    delete ui;
}


void PortsPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/1 Ports.bmp"));
    ui->error->clearMessage();
    connect(buttons()->cancel(), SIGNAL(clicked()),
            wizard(), SLOT(close()));
}
void PortsPage::backToThis() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/1 Ports.bmp"));
    ui->error->clearMessage();
}

bool PortsPage::isReadyForNext() {
    if (_model.ports().isEmpty()) {
        ui->error->showMessage("*No ports selected");
        ui->ports->setFocus();
        ui->ports->setCurrentIndex(_model.index(0));
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

    emit portsSelected(_model.ports());
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
    _model.setVna(vna);
    updateConnectors();
    updateGender();
    updateChannels();
}
Vna *PortsPage::vna() const {
    return _vna;
}

QVector<uint> PortsPage::ports() const {
    return _model.ports();
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
