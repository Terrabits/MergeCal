#include "MeasurePage.h"
#include "ui_MeasurePage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QProgressBar>


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

    ui->measureTree->expandAll();
    ui->measureTree->resizeColumnToContents(0);
    ui->measureTree->resizeColumnToContents(1);

    connect(_calibration, SIGNAL(startingMeasurement(QString,uint)),
            this, SLOT(measurementStarted(QString,uint)));
    connect(_calibration, SIGNAL(finishedMeasurement()),
            this, SLOT(measurementFinished()));
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
    _progressBar->start(caption, time_ms);
    wizard()->setDisabled();
}
void MeasurePage::measurementFinished() {
    _progressBar->stop();
    wizard()->setEnabled();
    if (_calibration->allMeasurementsFinished())
        buttons()->next()->setEnabled(true);
}
