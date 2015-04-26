#ifndef SETUPPAGE_H
#define SETUPPAGE_H


// Project
#include "FrequencyRange.h"
#include "Calibration.h"

// RsaToolbox
#include <WizardPage.h>
#include <Vna.h>
#include <Connector.h>

// Qt
#include <QWidget>
#include <QLabel>
#include <QStatusBar>


// Project
#include "Calibration.h"

// RsaToolbox


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

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setStatusBar(QStatusBar *statusBar);
    QStatusBar *statusBar();

    void setVna(RsaToolbox::Vna *vna);

public slots:
    void setPorts(const QVector<uint> &ports);
    void setConnector(const RsaToolbox::Connector &connector);
    void setChannel(const uint &channel);
    void setCalKits(const QVector<FrequencyRange> &kits);

signals:
    void calibrationChanged(const Calibration &calibration);

private:
    Ui::SetupPage *ui;

    QLabel *_header;
    QStatusBar *_statusBar;

    RsaToolbox::Vna *_vna;
    QVector<uint> _ports;
    RsaToolbox::Connector _connector;
    uint _channel;
    QVector<FrequencyRange> _kits;

    Calibration _calibration;
};

#endif // SETUPPAGE_H
