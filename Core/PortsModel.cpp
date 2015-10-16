#include "PortsModel.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QIcon>


PortsModel::PortsModel(QObject *parent) :
    QAbstractListModel(parent),
    _vna(NULL),
    _numberOfPorts(0)
{
}

// Qt Model
int PortsModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid())
        return _numberOfPorts;
    else
        return 0;
}
QVariant PortsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    int i = index.row();
    if (i < 0 || uint(i) >= _numberOfPorts)
        return QVariant();

    switch(role) {
    case Qt::DisplayRole:
        return QString("Port %1").arg(i+1);
    case Qt::DecorationRole:
        if (_isSelected[i])
            return QIcon(":/images/Images/Checked.png");
        else
            return QIcon(":/images/Images/Unchecked.png");
    default:
        return QVariant();
    }
}
bool PortsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;

    int i = index.row();
    if (i < 0 || uint(i) >= _numberOfPorts)
        return false;

    if (role != Qt::EditRole)
        return false;
    if (!value.canConvert<bool>())
        return false;

    if (_isSelected[i] == value.toBool())
        return true;

    _isSelected[i] = value.toBool();
    emit dataChanged(createIndex(i,1),createIndex(i,1));
    emit portsChanged(ports());
    return true;
}

// Extended model
bool PortsModel::toggleIndex(const QModelIndex &index) {
    if (!index.isValid())
        return false;

    int i = index.row();
    if (i < 0 || uint(i) >= _numberOfPorts)
        return false;

    _isSelected.toggleBit(i);
    emit dataChanged(createIndex(i,1),createIndex(i,1));
    emit portsChanged(ports());
    return true;
}

// Vna
void PortsModel::setVna(Vna *vna) {
    if (vna == _vna)
        return;

    beginResetModel();
    _vna = vna;
    updatePorts();
    endResetModel();
}
Vna *PortsModel::vna() const {
    return _vna;
}

QVector<uint> PortsModel::ports() const {
    QVector<uint> result;
    for (uint i = 0; i < _numberOfPorts; i++)
        if (_isSelected[i])
            result << i + 1;
    return result;
}
void PortsModel::setPorts(const QVector<uint> &ports) {
    beginResetModel();
    _isSelected.fill(false);
    foreach (uint port, ports) {
        if (port <= _numberOfPorts)
            _isSelected[port-1] = true;
    }
    endResetModel();
}

// Private
bool PortsModel::isVna() const {
    return _vna != NULL;
}
void PortsModel::updatePorts() {
    if (!isVna())
        return;

    uint numberOfPorts = _vna->testPorts();
    if (numberOfPorts < _numberOfPorts) {
        beginRemoveRows(QModelIndex(), numberOfPorts, _numberOfPorts-1);
        _numberOfPorts = numberOfPorts;
        _isSelected.resize(_numberOfPorts);
        endRemoveRows();
        emit portsChanged(ports());
    }
    else if (numberOfPorts > _numberOfPorts) {
        beginInsertRows(QModelIndex(), _numberOfPorts, numberOfPorts-1);
        _numberOfPorts = numberOfPorts;
        _isSelected.resize(_numberOfPorts);
        endInsertRows();
    }
}
