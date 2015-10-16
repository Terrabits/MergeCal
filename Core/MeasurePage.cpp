#include "MeasurePage.h"
#include "ui_MeasurePage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QProgressBar>
#include <QMessageBox>
#include <QDebug>


MeasurePage::MeasurePage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::MeasurePage),
    _header(NULL),
    _progressBar(NULL),
    _vna(NULL),
    _measureThread(NULL),
    _calibration(NULL),
    _isMeasuring(false)
{
    ui->setupUi(this);
    connect(ui->measureTree, SIGNAL(clicked(QModelIndex)),
            &_model, SLOT(measure(QModelIndex)));
}

MeasurePage::~MeasurePage()
{
    qDebug() << "~MeasurePage";

    // This code causes the application to crash
    // for some reason. The original intent was
    // to get a chance to reset the VNA in the event
    // that a crash occurs in the middle of calibration
    // (ironically). Instead it just causes a crash!
    // Consider deleting.
//    if (_calibration) {
//        _calibration->reset();
//    }
//    if (_vna) {
//        _vna->isError();
//        _vna->clearStatus();
//    }
    delete ui;
}

void MeasurePage::initialize() {
    qDebug() << "MeasurePage::initialize";
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/4 Measure.bmp"));
    _progressBar->show();
    buttons()->next()->setText("Apply");
    buttons()->next()->setDisabled(true);

    _calibration->initialize();

    formatTree();

    connect(_calibration, SIGNAL(startingMeasurement(QString,uint)),
            this, SLOT(measurementStarted(QString,uint)));
    connect(_calibration, SIGNAL(finishedMeasurement()),
            this, SLOT(measurementFinished()));
    connect(_calibration, SIGNAL(error(QString)),
               this, SLOT(displayError(QString)));
    connect(_calibration, SIGNAL(measurementStatusUpdated()),
            this, SLOT(formatTree()));
    connect(_calibration, SIGNAL(measurementStatusUpdated()),
            this, SLOT(updateApplyButton()));
}
bool MeasurePage::isReadyForNext() {
    qDebug() << "MeasurePage::isReadyForNext";
    if (_calibration->apply()) {
        _isMeasuring = false;
        wizard()->close();
        return true;
    }
    else {
        return false;
    }
}
bool MeasurePage::isReadyForBack() {
    qDebug() << "MeasurePage::isReadyForBack()";
    if (!_isMeasuring)
        return true;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Cancel calibration?",
                                  "Do you really want to leave the current calibration?\n"
                                  "All results will be lost. Continue?");
    return reply == QMessageBox::Yes;
}
void MeasurePage::back() {
    qDebug() << "MeasurePage::back";
    _isMeasuring = false;
    if (!_calibration)
        return;
    disconnect(_calibration, SIGNAL(startingMeasurement(QString,uint)),
            this, SLOT(measurementStarted(QString,uint)));
    disconnect(_calibration, SIGNAL(finishedMeasurement()),
            this, SLOT(measurementFinished()));
    disconnect(_calibration, SIGNAL(error(QString)),
               this, SLOT(displayError(QString)));
    disconnect(_calibration, SIGNAL(measurementStatusUpdated()),
            this, SLOT(formatTree()));
    disconnect(_calibration, SIGNAL(measurementStatusUpdated()),
            this, SLOT(updateApplyButton()));
    _calibration->reset();
    _vna->isError();
    _vna->clearStatus();
}

void MeasurePage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *MeasurePage::headerLabel() {
    return _header;
}

void MeasurePage::setProgressBar(TimedProgressBar *progressBar) {
    _progressBar = progressBar;
}
TimedProgressBar *MeasurePage::progressBar() {
    return _progressBar;
}

void MeasurePage::setVna(Vna *vna) {
    _vna = vna;
}
void MeasurePage::setCalibration(QThread *measureThread, Calibration *calibration) {
    _measureThread = measureThread;
    _calibration = calibration;
     _model.setCalibration(_calibration);
     ui->measureTree->setModel(&_model);
}

void MeasurePage::measurementStarted(const QString &caption, uint time_ms) {
    _isMeasuring = true;
    _progressBar->start(caption, time_ms);
    wizard()->setDisabled();
}
void MeasurePage::measurementFinished() {
    _progressBar->stop();
    wizard()->setEnabled();
}
void MeasurePage::displayError(const QString &message) {
    QMessageBox::critical(this,
                          "Measurement error",
                          message);
}

void MeasurePage::formatTree() {
    ui->measureTree->expandAll();
    ui->measureTree->resizeColumnToContents(0);
    ui->measureTree->resizeColumnToContents(1);
}
void MeasurePage::updateApplyButton() {
    if (_calibration->allMeasurementsFinished())
        buttons()->next()->setEnabled(true);
}
