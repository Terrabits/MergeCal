#ifndef CHOSENCALKITSMODEL_H
#define CHOSENCALKITSMODEL_H


// Project
#include "FrequencyRange.h"

// RsaToolbox
#include <NameLabel.h>
#include <Vna.h>

// Qt
#include <QAbstractTableModel>


class ChosenCalKitsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ChosenCalKitsModel();
    ~ChosenCalKitsModel();

    // Sections:
    static const int NAME_COLUMN = 0;
    static const int START_COLUMN = 1;
    static const int STOP_COLUMN = 2;
    static const int COLUMNS = 3;

    // Qt Model
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool isValid() const;
    bool addCalKit(FrequencyRange frequencyRange);
    QVector<FrequencyRange> calKits() const;

public slots:
    void clear();

signals:
    void prefixEntered();
    void error(const QString &message);


private:
    QVector<FrequencyRange> _kits;
    void sortKits();
    void setDefaultFrequencies();
    void resetFrequencies();

};

#endif // CHOSENCALKITSMODEL_H
