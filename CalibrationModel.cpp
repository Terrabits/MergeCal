#include "CalibrationModel.h"


#include <QIcon>
#include <QDebug>


CalibrationModel::CalibrationModel(QObject *parent) :
    QAbstractItemModel(parent),
    NO_PARENT(std::numeric_limits<quintptr>::max())
{
}

CalibrationModel::~CalibrationModel()
{

}

QModelIndex CalibrationModel::index(int row, int column, const QModelIndex &parent) const {
    if (_calibration->numberOfPorts() == 0)
        return QModelIndex();

    if (!parent.isValid()) {
        if (row < 0 || uint(row) > _calibration->numberOfPorts())
            return QModelIndex();
        if (column < 0 || column >= COLUMNS)
            return QModelIndex();

        return createIndex(row, column, NO_PARENT);
    }
    else {
        if (row < 0)
            return QModelIndex();
        if (column < 0 || column >= COLUMNS)
            return QModelIndex();
        if (uint(parent.row()) < _calibration->numberOfPorts()) {
            // Port
            if (row >= portRows())
                return QModelIndex();
            return createIndex(row, column, parent.row());
        }
        else {
            // Thru
            if (uint(row) >= _calibration->numberOfThrus())
                return QModelIndex();
            return createIndex(row, column, parent.row());
        }
    }
}
QModelIndex CalibrationModel::parent(const QModelIndex &child) const {
    if (!child.isValid() || child.internalId() == NO_PARENT)
        return QModelIndex();
    else
        return createIndex(child.internalId(), 0, NO_PARENT);
}
int CalibrationModel::rowCount(const QModelIndex &parent) const {
    if (_calibration->numberOfPorts() == 0) {
        return 0;
    }

    if (!parent.isValid()) {
        return _calibration->numberOfPorts()+1;
    }

    if (parent.internalId() != NO_PARENT) {
        return 0;
    }
    if (parent.column() != 0) {
        return 0;
    }

    if (uint(parent.row()) < _calibration->numberOfPorts()) {
        return portRows();
    }
    else {
        return _calibration->numberOfThrus();
    }
}
int CalibrationModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return COLUMNS;
}
QVariant CalibrationModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole && role != Qt::DecorationRole) {
        return QVariant();
    }

    if (index.row() < 0) {
        return QVariant();
    }
    if (index.column() < 0) {
        return QVariant();
    }
    if (index.column() > COLUMNS) {
        return QVariant();
    }

    // Should be valid...
    if (index.internalId() == NO_PARENT) {
        // Parent (top) item
        if (uint(index.row()) > _calibration->numberOfPorts()) {
            return QVariant();
        }
        if (index.column() != 0) {
            return QVariant();
        }
        if (uint(index.row()) < _calibration->numberOfPorts()) {
            // Single port
            uint port = _calibration->port(index.row());
            if (role == Qt::DisplayRole) {
                return QString("Port %1").arg(port);
            }
            else if (_calibration->isPortFullyMeasured(port)) {
                return QIcon(":/images/Images/Measured Single Port.bmp");
            }
            else {
                return QIcon(":/images/Images/Unmeasured Single Port.bmp");
            }
        }
        else {
            // Thru
            if (role == Qt::DisplayRole) {
                return "Thrus";
            }
            else if (_calibration->isAllThrusMeasured()) {
                return QIcon(":/images/Images/Measured Through.bmp");
            }
            else {
                return QIcon(":/images/Images/Unmeasured Through.bmp");
            }
        }
    }
    else {
        // Child item
        if (index.internalId() < _calibration->numberOfPorts()) {
            // Single Port
            uint port = _calibration->port(index.internalId());
            if (index.row() == SHORT_ROW) {
                if (index.column() == NAME_COLUMN) {
                    if (role == Qt::DisplayRole) {
                        if (!_calibration->shortLabel(port).isEmpty()) {
                            return QString("Short (%1)").arg(_calibration->shortLabel(port));
                        }
                        else {
                            return "Short";
                        }
                    }
                    else if (_calibration->isShortMeasured(port)) {
                        return QIcon(":/images/Images/Measured Button.bmp");
                    }
                    else {
                        return QIcon(":/images/Images/Unmeasured Button.bmp");
                    }
                }
                else {
                    return QVariant();
                }
            }
            else {
                // OffsetShort
                uint kitIndex= uint((index.row()-1)/2);
                bool isOffsetA = ((index.row()-1) % 2) == 0;
                if (index.column() == NAME_COLUMN) {
                    if (isOffsetA) {
                        if (role == Qt::DisplayRole) {
                            if (!_calibration->offsetShortALabel(kitIndex, port).isEmpty()) {
                                QString s = "%1 (%2)";
                                s = s.arg(_calibration->offsetShortAName(kitIndex));
                                s = s.arg(_calibration->offsetShortALabel(kitIndex, port));
                                return s;
                            }
                            else {
                                return _calibration->offsetShortAName(kitIndex);
                            }
                        }
                        else if (_calibration->isOffsetShortAMeasured(kitIndex, port)) {
                            return QIcon(":/images/Images/Measured Button.bmp");
                        }
                        else {
                            return QIcon(":/images/Images/Unmeasured Button.bmp");
                        }
                    }
                    else {
                        if (role == Qt::DisplayRole) {
                            if (!_calibration->offsetShortBLabel(kitIndex, port).isEmpty()) {
                                QString s = "%1 (%2)";
                                s = s.arg(_calibration->offsetShortBName(kitIndex));
                                s = s.arg(_calibration->offsetShortBLabel(kitIndex, port));
                                return s;
                            }
                            else {
                                return _calibration->offsetShortBName(kitIndex);
                            }
                        }
                        else if (_calibration->isOffsetShortBMeasured(kitIndex, port)) {
                            return QIcon(":/images/Images/Measured Button.bmp");
                        }
                        else {
                            return QIcon(":/images/Images/Unmeasured Button.bmp");
                        }
                    }
                }
                else {
                    // Kit column
                    return _calibration->kitNameLabel(kitIndex).displayText();
                }
            }
        }
        else {
            // Thru
            if (index.column() == NAME_COLUMN) {
                if (role == Qt::DisplayRole) {
                    return "Thru " + _calibration->thru(index.row()).toString();
                }
                else if (_calibration->isThruMeasured(index.row())) {
                    return QIcon(":/images/Images/Measured Button.bmp");
                }
                else {
                    return QIcon(":/images/Images/Unmeasured Button.bmp");
                }
            }
            else {
                return QVariant();
            }
        }
    }

    qDebug() << "  If this is executed, CalibrationModel::data is not returning a value... :-(";
}

void CalibrationModel::setCalibration(Calibration *calibration) {
    _calibration = calibration;

    connect(_calibration, SIGNAL(finishedInitialization()),
            this, SIGNAL(modelReset()));
    connect(_calibration, SIGNAL(measurementStatusUpdated()),
            this, SIGNAL(modelReset()));
}

void CalibrationModel::measure(const QModelIndex &index) {
    if (!index.isValid())
        return;
    if (index.internalId() == NO_PARENT)
        return;
    if (index.column() != 0)
        return;

    if (index.internalId() < _calibration->numberOfPorts()) {
        // Single port
        uint port = _calibration->port(index.internalId());
        if (index.row() == SHORT_ROW) {
            QMetaObject::invokeMethod(_calibration,
                                      "measureShort",
                                      Qt::QueuedConnection,
                                      Q_ARG(uint, port));
        }
        else {
            uint kitIndex= uint((index.row()-1)/2);
            bool isOffsetA = ((index.row()-1) % 2) == 0;
            if (isOffsetA)
                QMetaObject::invokeMethod(_calibration,
                                          "measureOffsetShortA",
                                          Qt::QueuedConnection,
                                          Q_ARG(uint, kitIndex),
                                          Q_ARG(uint, port));
            else
                QMetaObject::invokeMethod(_calibration,
                                          "measureOffsetShortB",
                                          Qt::QueuedConnection,
                                          Q_ARG(uint, kitIndex),
                                          Q_ARG(uint, port));
        }
    }
    else {
        // Thru
        uint thruIndex = index.row();
        QMetaObject::invokeMethod(_calibration,
                                  "measureThru",
                                  Qt::QueuedConnection,
                                  Q_ARG(uint, thruIndex));
    }
}

int CalibrationModel::portRows() const {
    return 1 + 2 * _calibration->numberOfKits();
}
