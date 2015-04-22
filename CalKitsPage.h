#ifndef CALKITSPAGE_H
#define CALKITSPAGE_H


// Project
#include "DoubleOffsetShortKit.h"
#include "AvailableCalKitsModel.h"


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

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setVna(RsaToolbox::Vna *vna);

    RsaToolbox::Connector connectorType() const;
    void setConnectorType(RsaToolbox::Connector type);

    // Something like this...
    // QVector<CalsAndKits> calibrations() const;

private:
    Ui::CalKitsPage *ui;
    QLabel *_header;

    RsaToolbox::Vna *_vna;
    RsaToolbox::Connector _connectorType;

    AvailableCalKitsModel _availableCalKitsModel;
};

#endif // CALKITSPAGE_H
