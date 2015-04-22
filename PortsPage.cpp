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
}
bool PortsPage::isReadyForNext() {
    if (_model.ports().isEmpty()) {
        ui->error->showMessage("*No ports selected");
        ui->ports->setFocus();
        ui->ports->setCurrentIndex(_model.index(0));
        return false;
    }
    // submit ports
    // submit connector
    // submit gender
    // submit channel
    // if successful, return true;
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
