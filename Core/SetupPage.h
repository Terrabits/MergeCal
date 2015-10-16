#ifndef SETUPPAGE_H
#define SETUPPAGE_H


// Project
#include "FrequencyRange.h"
#include "Calibration.h"
#include "TimedProgressBar.h"

// RsaToolbox
#include <WizardPage.h>
#include <Vna.h>
#include <Connector.h>

// Qt
#include <QWidget>
#include <QLabel>
#include <QStatusBar>
#include <QThread>
#include <QProgressBar>
#include <QTimer>


namespace Ui {
class SetupPage;
}

class SetupPage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit SetupPage(QWidget *parent = 0);
    ~SetupPage();

    virtual void initialize();
    virtual bool skip();
    virtual bool isReadyForBack();
    virtual bool skipBackwards() const;

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setProgressBar(TimedProgressBar *progressBar);
    TimedProgressBar *progressBar();

    void setVna(RsaToolbox::Vna *vna);
    void setCalibration(QThread *measureThread, Calibration *calibration);

signals:
    void setupAborted(const QString &message);

public slots:
    void setPorts(const QVector<uint> &ports);
    void setConnector(const RsaToolbox::Connector &connector);
    void setChannel(const uint &channel);
    void setCalKits(const QVector<FrequencyRange> &kits);

    void measurementStarted(const QString &caption, uint time_ms);
    void measurementFinished();
    void initializationFinished();

private slots:
    void initializationError(const QString &message);

private:
    Ui::SetupPage *ui;

    QLabel *_header;
    TimedProgressBar *_progressBar;

    RsaToolbox::Vna *_vna;
    QVector<uint> _ports;
    RsaToolbox::Connector _connector;
    uint _channel;
    QVector<FrequencyRange> _kits;

    QThread *_measureThread;
    Calibration *_calibration;

    bool _isInitializing;
    QString _setName;
};

#endif // SETUPPAGE_H
