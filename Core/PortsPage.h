#ifndef PORTSPAGE_H
#define PORTSPAGE_H


// Project
#include "PortsModel.h"

// RsaToolbox
#include <Connector.h>
#include <Vna.h>
#include <WizardPage.h>
#include <Keys.h>

// Qt
#include <QWidget>
#include <QLabel>


namespace Ui {
class PortsPage;
}

class PortsPage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit PortsPage(QWidget *parent = 0);
    ~PortsPage();

    virtual void initialize();
    virtual void backToThis();
    virtual bool isReadyForNext();

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setVna(RsaToolbox::Vna *vna);
    RsaToolbox::Vna *vna() const;

    void setKeys(RsaToolbox::Keys *keys);

    QVector<uint> ports() const;
    RsaToolbox::Connector connector() const;
    uint channel() const;

signals:
    void portsSelected(const QVector<uint> &ports);
    void connectorSelected(const RsaToolbox::Connector &type);
    void channelSelected(const uint &index);

private slots:
    void on_connectorType_currentIndexChanged(int index);

private:
    Ui::PortsPage *ui;
    QLabel *_header;

    RsaToolbox::Vna *_vna;
    PortsModel _portsModel;

    RsaToolbox::Keys *_keys;

    void updateConnectors();
    QVector<RsaToolbox::Connector> _connectors;

    void updateGender();

    void updateChannels();
    QVector<uint> _channels;

    void loadKeys();
    void saveKeys();

};

#endif // PORTSPAGE_H
