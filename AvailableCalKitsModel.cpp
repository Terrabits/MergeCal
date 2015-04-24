#include "AvailableCalKitsModel.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


AvailableCalKitsModel::AvailableCalKitsModel(QObject *parent) :
    QAbstractTableModel(parent),
    _vna(NULL)
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
    if (isVna() && isConnectorType())
        initializeKits();
}
void AvailableCalKitsModel::setConnectorType(Connector type) {
    if (_connectorType == type)
        return;

    _connectorType = type;
    if (isVna() && isConnectorType())
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
bool AvailableCalKitsModel::isConnectorType() const {
    return _connectorType.type() != Connector::Type::UNKNOWN_CONNECTOR;
}
void AvailableCalKitsModel::initializeKits() {
    beginResetModel();
    _kits.clear();
    QVector<NameLabel> nameLabels = _vna->calKits(_connectorType);
    foreach (NameLabel nl, nameLabels) {
        DoubleOffsetShortKit kit(_vna->calKit(nl), _connectorType.gender());
        if (kit.isValid())
            _kits << kit;
    }
    endResetModel();
}
