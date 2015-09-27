#include "CalibrationModel.h"


#include <QIcon>
#include <QDebug>


CalibrationModel::CalibrationModel(QObject *parent) :
    QAbstractItemModel(parent),
    NO_PARENT(std::numeric_limits<quintptr>::max())
{
    qDebug() << "NO_PARENT: " << NO_PARENT;
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
    qDebug() << "CalibrationModel::rowCount, parent: " << parent;
    if (_calibration->numberOfPorts() == 0) {
        qDebug() << "  no ports, returning 0";
        return 0;
    }

    if (!parent.isValid()) {
        qDebug() << " no parent, returning " << _calibration->numberOfPorts() + 1;
        return _calibration->numberOfPorts()+1;
    }

    if (parent.internalId() != NO_PARENT) {
        qDebug() << "  parent.internalId != NO_PARENT, returning 0";
        return 0;
    }
    if (parent.column() != 0) {
        qDebug() << " parent.column != 0, returning 0";
        return 0;
    }

    if (uint(parent.row()) < _calibration->numberOfPorts()) {
        qDebug() << "  parent.row < numberOfPorts (single port), returning " << portRows();
        return portRows();
    }
    else {
        qDebug() << "  thru, returning " << _calibration->numberOfThrus();
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
    qDebug() << "CalibrationModel::data " << index << " role: " << role;
    if (index.internalId() == NO_PARENT) {
        // Parent (top) item
        if (uint(index.row()) > _calibration->numberOfPorts()) {
            qDebug() << "  row > numberOfPorts. Returning.";
            return QVariant();
        }
        if (index.column() != 0) {
            qDebug() << "  column != 0. Returning.";
            return QVariant();
        }
        if (uint(index.row()) < _calibration->numberOfPorts()) {
            // Single port
            qDebug() << "  single port standard.";
            uint port = _calibration->port(index.row());
            qDebug() << "  port: " << port;
            if (role == Qt::DisplayRole) {
                qDebug() << "  display role.";
                return QString("Port %1").arg(port);
            }
            else if (_calibration->isPortFullyMeasured(port)) {
                qDebug() << "  decoration role.";
                qDebug() << "  all standards measured.";
                return QIcon(":/images/Images/Measured Single Port.bmp");
            }
            else {
                qDebug() << "  decoration role.";
                qDebug() << "  not all measured.";
                return QIcon(":/images/Images/Unmeasured Single Port.bmp");
            }
        }
        else {
            // Thru
            qDebug() << "  thru";
            if (role == Qt::DisplayRole) {
                qDebug() << "  display role.";
                return "Thrus";
            }
            else if (_calibration->isAllThrusMeasured()) {
                qDebug() << "  All thrus measured";
                return QIcon(":/images/Images/Measured Through.bmp");
            }
            else {
                qDebug() << "  not all measured.";
                return QIcon(":/images/Images/Unmeasured Through.bmp");
            }
        }
    }
    else {
        // Child item
        qDebug() << "  child item (specific measurement)";
        if (index.internalId() < _calibration->numberOfPorts()) {
            // Single Port
            qDebug() << "  single port standard";
            uint port = _calibration->port(index.internalId());
            qDebug() << "  port: " << port;
            if (index.row() == SHORT_ROW) {
                qDebug() << "  short.";
                if (index.column() == NAME_COLUMN) {
                    qDebug() << "  name column";
                    if (role == Qt::DisplayRole) {
                        qDebug() << "  display role.";
                        if (!_calibration->shortLabel(port).isEmpty()) {
                            qDebug() << "  has label.";
                            return QString("Short (%1)").arg(_calibration->shortLabel(port));
                        }
                        else {
                            qDebug() << "  no label";
                            return "Short";
                        }
                    }
                    else if (_calibration->isShortMeasured(port)) {
                        qDebug() << "  decoration role.";
                        qDebug() << "  measured.";
                        return QIcon(":/images/Images/Measured Button.bmp");
                    }
                    else {
                        qDebug() << "  decoration role.";
                        qDebug() << "  unmeasured.";
                        return QIcon(":/images/Images/Unmeasured Button.bmp");
                    }
                }
                else {
                    qDebug() << "  not name column. Returning.";
                    return QVariant();
                }
            }
            else {
                // OffsetShort
                qDebug() << "  offset short";
                uint kitIndex= uint((index.row()-1)/2);
                qDebug() << "  kit index: " << kitIndex;
                bool isOffsetA = ((index.row()-1) % 2) == 0;
                qDebug() << "  is offset A? " << isOffsetA;
                if (index.column() == NAME_COLUMN) {
                    qDebug() << "  name column.";
                    if (isOffsetA) {
                        qDebug() << "  Offset A.";
                        if (role == Qt::DisplayRole) {
                            qDebug() << "  display role.";
                            if (!_calibration->offsetShortALabel(kitIndex, port).isEmpty()) {
                                qDebug() << "  has label";
                                QString s = "%1 (%2)";
                                s = s.arg(_calibration->offsetShortAName(kitIndex));
                                s = s.arg(_calibration->offsetShortALabel(kitIndex, port));
                                qDebug() << "  returning " << s;
                                return s;
                            }
                            else {
                                qDebug() << "  no label";
                                return _calibration->offsetShortAName(kitIndex);
                            }
                        }
                        else if (_calibration->isOffsetShortAMeasured(kitIndex, port)) {
                            qDebug() << "  decoration role, measured";
                            return QIcon(":/images/Images/Measured Button.bmp");
                        }
                        else {
                            qDebug() << "  decoration role, unmeasured.";
                            return QIcon(":/images/Images/Unmeasured Button.bmp");
                        }
                    }
                    else {
                        qDebug() << "  Offset B";
                        if (role == Qt::DisplayRole) {
                            qDebug() << "  display role.";
                            if (!_calibration->offsetShortBLabel(kitIndex, port).isEmpty()) {
                                qDebug() << "  has label.";
                                QString s = "%1 (%2)";
                                s = s.arg(_calibration->offsetShortBName(kitIndex));
                                s = s.arg(_calibration->offsetShortBLabel(kitIndex, port));
                                qDebug() << "  returning " << s;
                                return s;
                            }
                            else {
                                qDebug() << "  no label";
                                return _calibration->offsetShortBName(kitIndex);
                            }
                        }
                        else if (_calibration->isOffsetShortBMeasured(kitIndex, port)) {
                            qDebug() << "  decoration role, measured.";
                            return QIcon(":/images/Images/Measured Button.bmp");
                        }
                        else {
                            qDebug() << "  decoration role, unmeasured.";
                            return QIcon(":/images/Images/Unmeasured Button.bmp");
                        }
                    }
                }
                else {
                    // Kit column
                    qDebug() << "  kit column";
                    return _calibration->kitNameLabel(kitIndex).displayText();
                }
            }
        }
        else {
            // Thru
            qDebug() << "  thru.";
            if (index.column() == NAME_COLUMN) {
                qDebug() << "  name column";
                if (role == Qt::DisplayRole) {
                    qDebug() << " display role.";
                    return "Thru " + _calibration->thru(index.row()).toString();
                }
                else if (_calibration->isThruMeasured(index.row())) {
                    qDebug() << "  decoration role, measured";
                    return QIcon(":/images/Images/Measured Button.bmp");
                }
                else {
                    qDebug() << "  decoration role, unmeasured.";
                    return QIcon(":/images/Images/Unmeasured Button.bmp");
                }
            }
            else {
                qDebug() << "  not name column. Returning.";
                return QVariant();
            }
        }
    }

    qDebug() << "  If this is executed, CalibrationModel::data is not returning a value... :-(";
}

void CalibrationModel::setCalibration(Calibration *calibration) {
    qDebug() << "CalibrationModel::setCalibration";
    _calibration = calibration;

    connect(_calibration, SIGNAL(finishedInitialization()),
            this, SIGNAL(modelReset()));
    connect(_calibration, SIGNAL(measurementStatusUpdated()),
            this, SIGNAL(modelReset()));

    qDebug() << "row count: " << rowCount();
    qDebug() << "column count: " << columnCount();
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
