#ifndef CALKITSPAGE_H
#define CALKITSPAGE_H


// Project
#include "DoubleOffsetShortKit.h"
#include "AvailableCalKitsModel.h"
#include "FrequencyRange.h"
#include "ChosenCalKitsModel.h"

// RsaToolbox
#include <Connector.h>
#include <Vna.h>
#include <WizardPage.h>

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

    virtual void initialize();
    virtual void backToThis();

    bool isHeader() const;
    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setVna(RsaToolbox::Vna *vna);

    RsaToolbox::Connector connectorType() const;

    // Something like this...
    // QVector<CalsAndKits> calibrations() const;

public slots:
    void setConnectorType(RsaToolbox::Connector type);

private slots:
    void on_addKit_clicked();

private:
    Ui::CalKitsPage *ui;

    QLabel *_header;

    RsaToolbox::Vna *_vna;
    RsaToolbox::Connector _connectorType;

    AvailableCalKitsModel _availableCalKitsModel;
    ChosenCalKitsModel _chosenCalKitsModel;
};

#endif // CALKITSPAGE_H
