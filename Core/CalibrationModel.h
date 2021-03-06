#ifndef CALIBRATIONMODEL_H
#define CALIBRATIONMODEL_H


// Project
#include "Calibration.h"

// Qt
#include <QObject>
#include <QAbstractItemModel>

// C++ std lib
#include <limits>


class CalibrationModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    CalibrationModel(QObject *parent = 0);
    ~CalibrationModel();

    // Port rows
    static const int SHORT_ROW = 0;
    // Offsets A, B for each kit

    // Columns
    static const int NAME_COLUMN = 0;
    static const int KIT_COLUMN = 1;
    static const int COLUMNS = 2;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setCalibration(Calibration *calibration);

public slots:
    void measure(const QModelIndex &index);

private:
    const quintptr NO_PARENT;
    Calibration *_calibration;

    int portRows() const;
};

#endif // CALIBRATIONMODEL_H
