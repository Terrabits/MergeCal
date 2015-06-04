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
    delete ui;
}

void MeasurePage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/4 Measure.bmp"));

    _progressBar->show();
    buttons()->next()->setText("Apply");
    buttons()->next()->setDisabled(true);

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
    _calibration->applyCorrections();
    wizard()->close();
    return true;
}
bool MeasurePage::isReadyForBack() {
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
    return WizardPage::isReadyForBack();
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
    qDebug() << "Measurement started: " << caption << time_ms;
    _progressBar->start(caption, time_ms);
    wizard()->setDisabled();
}
void MeasurePage::measurementFinished() {
    _progressBar->stop();
    wizard()->setEnabled();
}
void MeasurePage::displayError(const QString &message) {
    qDebug() << "MeasurePage::dispayError";
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
