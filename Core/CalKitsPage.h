#ifndef CALKITSPAGE_H
#define CALKITSPAGE_H


// Project
#include "DoubleOffsetShortKit.h"
#include "AvailableCalKitsModel.h"
#include "FrequencyRange.h"
#include "ChosenCalKitsModel.h"
#include "Calibration.h"

// RsaToolbox
#include <Definitions.h>
#include <Connector.h>
#include <Vna.h>
#include <WizardPage.h>
#include <Keys.h>

// Qt
#include <QWidget>
#include <QLabel>


namespace Ui {
class CalKitsPage;
}

class CalKitsPage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit CalKitsPage(QWidget *parent = 0);
    ~CalKitsPage();

    // Forward
    virtual void initialize();
    virtual bool isReadyForNext();

    // Back
    virtual void backToThis();

    bool isHeader() const;
    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setVna(RsaToolbox::Vna *vna);
    void setKeys(RsaToolbox::Keys *keys);
    void setCalibration(Calibration *calibration);

    RsaToolbox::Connector connectorType() const;

public slots:
    void setPorts(const QVector<uint> &ports);
    void setConnectorType(const RsaToolbox::Connector &type);
    void setChannel(const uint &index);

    virtual void displayError(const QString &message);
    void cancel();

signals:
    void calKitsSelected(const QVector<FrequencyRange> &calKits);

private slots:
    void on_addKit_clicked();
    void on_removeKit_clicked();

private:
    Ui::CalKitsPage *ui;

    QLabel *_header;

    RsaToolbox::Vna *_vna;
    RsaToolbox::Keys *_keys;
    Calibration *_calibration;

    QVector<uint> _ports;
    RsaToolbox::Connector _connectorType;
    uint _channel;
    void updateChannelInfo();

    double _channelStartFreq_Hz;
    double _channelStopFreq_Hz;
    static bool isFrequencyPoint(RsaToolbox::QRowVector &frequencies, bool isStartInclusive, double start, bool isStopInclusive, double stop);

    AvailableCalKitsModel _availableCalKitsModel;
    ChosenCalKitsModel _chosenCalKitsModel;

    void loadKeys();
    void saveKeys();
};

#endif // CALKITSPAGE_H
