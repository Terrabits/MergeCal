#ifndef MEASUREPAGE_H
#define MEASUREPAGE_H


// Project
#include "TimedProgressBar.h"
#include "Calibration.h"
#include "CalibrationModel.h"

// RsaToolbox
#include <WizardPage.h>
#include <Vna.h>

// Qt
#include <QWidget>
#include <QLabel>
#include <QStatusBar>


namespace Ui {
class MeasurePage;
}

class MeasurePage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit MeasurePage(QWidget *parent = 0);
    ~MeasurePage();

    virtual void initialize();
    virtual bool isReadyForNext();
    virtual bool isReadyForBack();

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setProgressBar(TimedProgressBar *progressBar);
    TimedProgressBar *progressBar();

    void setVna(RsaToolbox::Vna *vna);
    void setCalibration(QThread *measureThread, Calibration *calibration);

public slots:
    void measurementStarted(const QString &caption, uint time_ms);
    void measurementFinished();

private slots:
    void formatTree();

private:
    Ui::MeasurePage *ui;

    QLabel *_header;
    TimedProgressBar *_progressBar;

    RsaToolbox::Vna *_vna;
    QThread *_measureThread;
    Calibration *_calibration;
    CalibrationModel _model;

    bool _isMeasuring;
};

#endif // MEASUREPAGE_H
