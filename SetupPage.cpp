#include "SetupPage.h"
#include "ui_SetupPage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QProgressBar>
#include <QDebug>


SetupPage::SetupPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::SetupPage),
    _isInitializing(false)
{
    ui->setupUi(this);

    _measureThread.start();
    _calibration.moveToThread(&_measureThread);
}

SetupPage::~SetupPage()
{
    if (_isInitializing) {
        _calibration.interrupt();
        _measureThread.quit();
        _measureThread.wait();
        _vna->closeActiveSet();
        _vna->openSet(_setName);
        _vna->deleteSet(_setName);
    }
    else {
        _measureThread.quit();
        _measureThread.wait();
    }

    if (_vna != NULL)
        _vna->isError();

    delete ui;
}

void SetupPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/3 Setting up.bmp"));

    _calibration.setVna(_vna);
    _calibration.setPorts(_ports);
    _calibration.setConnector(_connector);
    _calibration.setChannel(_channel);
    _calibration.setCalKits(_kits);
    buttons()->next()->setDisabled(true);
    buttons()->previous()->setDisabled(true);
}
bool SetupPage::skip() {
    connect(&_calibration, SIGNAL(startingMeasurement(QString,uint)),
            this, SLOT(measurementStarted(QString,uint)));
    connect(&_calibration, SIGNAL(finishedMeasurement()),
            this, SLOT(measurementFinished()));
    connect(&_calibration, SIGNAL(finishedInitialization()),
            this, SLOT(initializationFinished()));

    _isInitializing = true;
    _setName = _vna->activeSet();
    _vna->saveActiveSet(_setName);
    QMetaObject::invokeMethod(&_calibration,
                              "initialize",
                              Qt::QueuedConnection);

    return false;
}
bool SetupPage::skipBackwards() const {
    return true;
}


void SetupPage::measurementStarted(const QString &caption, uint time_ms) {
    qDebug() << "Measurement started: " << caption << time_ms;
    _progressBar->start(caption, time_ms);
}
void SetupPage::measurementFinished() {
    _progressBar->stop();
}

void SetupPage::initializationFinished() {
    _isInitializing = false;
    _vna->deleteSet(_setName);
    buttons()->next()->setEnabled(true);
    buttons()->previous()->setEnabled(true);
    wizard()->next();
}

void SetupPage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *SetupPage::headerLabel() {
    return _header;
}

void SetupPage::setProgressBar(TimedProgressBar *statusBar) {
    _progressBar = statusBar;
}
TimedProgressBar *SetupPage::progressBar() {
    return _progressBar;
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


