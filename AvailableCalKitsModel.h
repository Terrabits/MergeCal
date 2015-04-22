#ifndef AVAILABLECALKITSMODEL_H
#define AVAILABLECALKITSMODEL_H



// Project
#include "DoubleOffsetShortKit.h"

// RsaToolbox
#include <NameLabel.h>
#include <Vna.h>

// Qt
#include <QAbstractTableModel>


class AvailableCalKitsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AvailableCalKitsModel(QObject *parent = 0);


    // Sections:
    static const int NAME_COLUMN = 0;
    static const int RANGE_COLUMN = 1;
    static const int COLUMNS = 2;

    // Qt Model
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    // Extra
    void setVna(RsaToolbox::Vna *vna);
    void setConnectorType(RsaToolbox::Connector type);

    bool hasCalKit(const DoubleOffsetShortKit &kit) const;
    DoubleOffsetShortKit calKit(const QModelIndex &index) const;

signals:

public slots:

private:
    bool isVna() const;
    RsaToolbox::Vna *_vna;

    bool isConnectorType() const;
    RsaToolbox::Connector _connectorType;

    void initializeKits();
    QVector<DoubleOffsetShortKit> _kits;

};

#endif // AVAILABLECALKITSMODEL_H
