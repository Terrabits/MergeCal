#include "SetupPage.h"
#include "ui_SetupPage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QProgressBar>
#include <QDebug>
#include <QMetaObject>


SetupPage::SetupPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::SetupPage),
    _isInitializing(false)
{
    ui->setupUi(this);

}

SetupPage::~SetupPage()
{
//    if (_isInitializing) {
//        _calibration->interrupt();
//        _measureThread->quit();
//        _measureThread->wait();
//        _vna->closeActiveSet();
//        _vna->openSet(_setName);
//        _vna->deleteSet(_setName);
//    }

    if (_vna != NULL)
        _vna->isError();

    delete ui;
}

void SetupPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/3 Setting up.bmp"));

    _calibration->setVna(_vna);
    _calibration->setPorts(_ports);
    _calibration->setConnector(_connector);
    _calibration->setChannel(_channel);
    _calibration->setCalKits(_kits);
    wizard()->setDisabled();
}
bool SetupPage::skip() {
    qDebug() << "SetupPage::skip";
    _isInitializing = true;
    _setName = _vna->activeSet();
    _vna->saveActiveSet(_setName);

    connect(_calibration, SIGNAL(startingMeasurement(QString,uint)),
            this, SLOT(measurementStarted(QString,uint)));
    connect(_calibration, SIGNAL(finishedMeasurement()),
            this, SLOT(measurementFinished()));
    connect(_calibration, SIGNAL(finishedInitialization()),
            this, SLOT(initializationFinished()));
    connect(_calibration, SIGNAL(error(QString)),
               this, SLOT(initializationError(QString)));

    _vna->settings().displayOff();
    _vna->settings().errorDisplayOff();
    QMetaObject::invokeMethod(_calibration,
                              "initialize",
                              Qt::QueuedConnection);

    return false;
}
bool SetupPage::isReadyForBack() {
    qDebug() << "SetupPage::isReadyForBack " << !_isInitializing;
    return !_isInitializing;
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
    qDebug() << "SetupPage::initializationFinished";
    if (!_isInitializing)
        return;

    _isInitializing = false;
    _vna->settings().displayOn();
    _vna->settings().errorDisplayOn();
    _vna->deleteSet(_setName);

    disconnect(_calibration, SIGNAL(startingMeasurement(QString,uint)),
            this, SLOT(measurementStarted(QString,uint)));
    disconnect(_calibration, SIGNAL(finishedMeasurement()),
            this, SLOT(measurementFinished()));
    disconnect(_calibration, SIGNAL(finishedInitialization()),
            this, SLOT(initializationFinished()));
    disconnect(_calibration, SIGNAL(error(QString)),
               this, SLOT(initializationError(QString)));

    qDebug() << "SetupPage::initializationFinished - enable wizard, next";
    wizard()->setEnabled();
    wizard()->next();
    qDebug() << "/SetupPage::initializationFinished";
}

void SetupPage::initializationError(const QString &message) {
    qDebug() << "SetupPage init error " << QObject::sender();
    _isInitializing = false;

    disconnect(_calibration, SIGNAL(startingMeasurement(QString,uint)),
            this, SLOT(measurementStarted(QString,uint)));
    disconnect(_calibration, SIGNAL(finishedMeasurement()),
            this, SLOT(measurementFinished()));
    disconnect(_calibration, SIGNAL(finishedInitialization()),
            this, SLOT(initializationFinished()));
    disconnect(_calibration, SIGNAL(error(QString)),
               this, SLOT(initializationError(QString)));

    _vna->isError();
    _vna->clearStatus();
    _vna->closeActiveSet();
    _vna->openSet(_setName);
    _vna->deleteSet(_setName);

    _vna->settings().errorDisplayOn();

    wizard()->setEnabled();
    wizard()->back();
    emit setupAborted(message);
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
void SetupPage::setCalibration(QThread *measureThread, Calibration *calibration) {
    _measureThread = measureThread;
    _calibration = calibration;
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


