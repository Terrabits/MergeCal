#include "AvailableCalKitsModel.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


AvailableCalKitsModel::AvailableCalKitsModel(QObject *parent) :
    QAbstractTableModel(parent),
    _vna(NULL),
    _needThru(false)
{
}

QVariant AvailableCalKitsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case 0:
        return "Cal Kit";
    case 1:
        return "Range";
    default:
        return QVariant();
    }
}
int AvailableCalKitsModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return _kits.size();
}
int AvailableCalKitsModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return COLUMNS;
}

QVariant AvailableCalKitsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    int column = index.column();
    if (column < 0 || uint(column) >= COLUMNS)
        return QVariant();

    int row = index.row();
    if (row < 0 || row >= _kits.size())
        return QVariant();

    switch (column) {
    case NAME_COLUMN:
        return _kits[row].displayNameLabel();
    case RANGE_COLUMN:
        return _kits[row].displayFrequencyRange();
    default:
        return QVariant();
    }
}

void AvailableCalKitsModel::setVna(Vna *vna) {
    if (_vna == vna)
        return;

    _vna = vna;
    initializeKits();
}
void AvailableCalKitsModel::setPorts(const QVector<uint> &ports) {
    if (_ports == ports)
        return;

    _ports = ports;
    bool needThru = _ports.size() > 1;
    if (needThru != _needThru) {
        _needThru = needThru;
        initializeKits();
    }
}

void AvailableCalKitsModel::setConnectorType(const Connector &type) {
    if (_connectorType == type)
        return;

    _connectorType = type;
    initializeKits();
}

bool AvailableCalKitsModel::hasCalKit(const DoubleOffsetShortKit &kit) const {
    return _kits.contains(kit);
}
DoubleOffsetShortKit AvailableCalKitsModel::calKit(const QModelIndex &index) const {
    if (!index.isValid())
        return DoubleOffsetShortKit();

    int row = index.row();
    if (row < 0 || row >= _kits.size())
        return DoubleOffsetShortKit();

    return _kits[row];
}

bool AvailableCalKitsModel::isVna() const {
    return _vna != NULL;
}
bool AvailableCalKitsModel::isPorts() const {
    return !_ports.isEmpty();
}
bool AvailableCalKitsModel::isConnectorType() const {
    return _connectorType.type() != Connector::Type::UNKNOWN_CONNECTOR;
}
void AvailableCalKitsModel::initializeKits() {
    if (!isVna())
        return;
    if (!isConnectorType())
        return;
    if (!isPorts())
        return;

    QVector<DoubleOffsetShortKit> kits;
    kits.clear();
    QVector<NameLabel> nameLabels = _vna->calKits(_connectorType);
    foreach (NameLabel nl, nameLabels) {
        DoubleOffsetShortKit kit(_vna->calKit(nl), _connectorType.gender(), _needThru);
        if (kit.isValid())
            kits << kit;
    }
    sort(kits);

    if (_kits != kits) {
        beginResetModel();
        _kits = kits;
        endResetModel();
    }
}
void AvailableCalKitsModel::sort(QVector<DoubleOffsetShortKit> kits) {
    // Bubble sort!
    for (int i = 0; i < kits.size()-1; i++) {
        for (int j = 0; j < kits.size()-1; j++) {
            if (kits[j].minimumFrequency_Hz() > kits[j+1].minimumFrequency_Hz()) {
                // swap
                DoubleOffsetShortKit kit_j = kits[j];
                kits[j] = kits[j+1];
                kits[j+1] = kit_j;
            }
        }
    }
}
