#ifndef PORTSMODEL_H
#define PORTSMODEL_H



// RsaToolbox
#include <General.h>
#include <Vna.h>

// Qt
#include <QBitArray>
#include <QAbstractListModel>


class PortsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PortsModel(QObject *parent = 0);

    // Qt Model
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    // Extended model
    bool toggleIndex(const QModelIndex &index);

    // Vna
    void setVna(RsaToolbox::Vna *vna);
    RsaToolbox::Vna *vna() const;

    QVector<uint> ports() const;

signals:
    void portsChanged(QVector<uint> ports);

public slots:

private:
    bool isVna() const;
    RsaToolbox::Vna *_vna;

    void updatePorts();
    uint _numberOfPorts;
    QBitArray _isSelected;

};

#endif // PORTSMODEL_H
