#include "SetupPage.h"
#include "ui_SetupPage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QProgressBar>


SetupPage::SetupPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::SetupPage)
{
    ui->setupUi(this);
}

SetupPage::~SetupPage()
{
    delete ui;
}

void SetupPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/3 Setting up.bmp"));

    _statusBar->show();

    _calibration.setVna(_vna);
    _calibration.setPorts(_ports);
    _calibration.setConnector(_connector);
    _calibration.setChannel(_channel);
    _calibration.setCalKits(_kits);

    // Connect to status bar?

    _calibration.initialize();
}

void SetupPage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *SetupPage::headerLabel() {
    return _header;
}

void SetupPage::setStatusBar(QStatusBar *statusBar) {
    _statusBar = statusBar;
}
QStatusBar *SetupPage::statusBar() {
    return _statusBar;
}

void SetupPage::setVna(Vna *vna) {
    _vna = vna;
}
void SetupPage::setPorts(const QVector<uint> &ports) {
    _ports = ports;
}
void SetupPage::setConnector(const Connector &connector) {
    _connector = connector;
}
void SetupPage::setChannel(const uint &channel) {
    _channel = channel;
}
void SetupPage::setCalKits(const QVector<FrequencyRange> &kits) {
    _kits = kits;
}


