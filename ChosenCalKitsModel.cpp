#include "ChosenCalKitsModel.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QRegExp>
#include <QDebug>


ChosenCalKitsModel::ChosenCalKitsModel()
{

}

ChosenCalKitsModel::~ChosenCalKitsModel()
{

}

QVariant ChosenCalKitsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case 0:
        return "Cal Kit";
    case 1:
        return "Start";
    case 2:
        return "Stop";
    default:
        return QVariant();
    }
}
Qt::ItemFlags ChosenCalKitsModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return QAbstractTableModel::flags(index);

    if (index.row() >= 0
            && index.row() < _kits.size()
            && (index.column() == START_COLUMN || index.column() == STOP_COLUMN))
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    else
        return QAbstractTableModel::flags(index);
}
int ChosenCalKitsModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return _kits.size();
}
int ChosenCalKitsModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return COLUMNS;
}
QVariant ChosenCalKitsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    int column = index.column();
    if (column < 0 || uint(column) >= COLUMNS)
        return QVariant();

    int row = index.row();
    if (row < 0 || row >= _kits.size())
        return QVariant();

    switch (column) {
    case NAME_COLUMN:
        return _kits[row].calKit().displayNameLabel();
    case START_COLUMN:
        if (role == Qt::DisplayRole)
            return formatValue(_kits[row].startFrequency_Hz(), 3, Units::Hertz);
        else
            return _kits[row].startFrequency_Hz();
    case STOP_COLUMN:
        if (role == Qt::DisplayRole)
            return formatValue(_kits[row].stopFrequency_Hz(), 3, Units::Hertz);
        else
            return _kits[row].stopFrequency_Hz();
    default:
        return QVariant();
    }
}
bool ChosenCalKitsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    int column = index.column();
    if (column != START_COLUMN && column != STOP_COLUMN)
        return false;

    int row = index.row();
    if (row < 0 || row >= _kits.size())
        return false;

    if (!value.canConvert<double>())
        return false;

    // set data
    double _value = value.toDouble();
    if (_value < _kits[row].calKit().minimumFrequency_Hz() || _value > _kits[row].calKit().maximumFrequency_Hz()) {
        QString msg = "*Frequency out of range of cal kit";
        emit error(msg);
        return false;
    }

    QModelIndex topLeft = index;
    QModelIndex bottomRight = index;
    if (column == START_COLUMN) {
        _kits[row].setStartFrequency(_value);
        if (row != 0 && _value <= _kits[row-1].calKit().maximumFrequency_Hz()) {
            _kits[row-1].setStopFrequency(_value);
            topLeft = this->index(row-1, STOP_COLUMN);
        }
    }
    else { // column == STOP_COLUMN
        _kits[row].setStopFrequency(_value);
        if (row != _kits.size()-1 && _value >= _kits[row+1].calKit().minimumFrequency_Hz()) {
            _kits[row+1].setStartFrequency(_value);
            bottomRight = this->index(row+1, START_COLUMN);
        }
    }
    emit dataChanged(topLeft, bottomRight);
    return true;
}
bool ChosenCalKitsModel::removeRow(int row, const QModelIndex &parent) {
    if (parent.isValid())
        return false;

    if (row < 0 || row >= _kits.size())
        return false;

    beginRemoveRows(parent, row, row);
    _kits.remove(row);
    resetFrequencies();
    setDefaultFrequencies();
    endRemoveRows();
    return true;
}

bool ChosenCalKitsModel::isValid() const {
    return false;
}
bool ChosenCalKitsModel::addCalKit(FrequencyRange frequencyRange) {
    if (!_kits.contains(frequencyRange)) {
        beginResetModel();
        _kits << frequencyRange;
        resetFrequencies();
        // sortKits();
        setDefaultFrequencies(); // includes sort
        qDebug() << "Adding kit: " << frequencyRange.calKit().displayNameLabel();
        qDebug() << "chosen kits: " << _kits.size();
        endResetModel();
        return true;
    }
    else {
        return false;
    }
}
QVector<FrequencyRange> ChosenCalKitsModel::calKits() const {
    return _kits;
}

void ChosenCalKitsModel::clear() {
    beginResetModel();
    _kits.clear();
    endResetModel();
}

void ChosenCalKitsModel::sortKits() {
    // Bubble sort :-)
    for (int i = 0; i < _kits.size() - 1; i++) {
        for (int j = 0; j < _kits.size()-1; j++) {
            if (_kits[j].calKit().minimumFrequency_Hz() > _kits[j+1].calKit().minimumFrequency_Hz())
            {
                FrequencyRange kit_j = _kits[j];
                _kits[j] = _kits[j+1];
                _kits[j+1] = kit_j;
            }
            else if (_kits[j].calKit().minimumFrequency_Hz() == _kits[j+1].calKit().minimumFrequency_Hz()
                     && _kits[j].calKit().maximumFrequency_Hz() > _kits[j+1].calKit().maximumFrequency_Hz())
            {
                FrequencyRange kit_j = _kits[j];
                _kits[j] = _kits[j+1];
                _kits[j+1] = kit_j;
            }
        }
    }
}
void ChosenCalKitsModel::setDefaultFrequencies() {
    sortKits();
    for (int i = 0; i < _kits.size(); i++) {
        if (!_kits[i].isStartFrequency()) {
            // Set start
            _kits[i].setStartFrequency(_kits[i].calKit().minimumFrequency_Hz());
        }
        if (!_kits[i].isStopFrequency()) {
            // Set stop
            if (i == _kits.size() - 1) {
                // Last kit; set to max
                _kits[i].setStopFrequency(_kits[i].calKit().maximumFrequency_Hz());
            }
            else {
                if (_kits[i].calKit().maximumFrequency_Hz() > _kits[i+1].calKit().minimumFrequency_Hz()) {
                    double average = (_kits[i].calKit().maximumFrequency_Hz() + _kits[i+1].calKit().minimumFrequency_Hz())/2.0;
                    if (average < _kits[i+1].calKit().maximumFrequency_Hz()) {
                        _kits[i].setStopFrequency(average);
                        _kits[i+1].setStartFrequency(_kits[i].stopFrequency_Hz());
                    }
                    else {
                        _kits[i].setStopFrequency(_kits[i].calKit().maximumFrequency_Hz());
                    }
                }
                else {
                    _kits[i].setStopFrequency(_kits[i].calKit().maximumFrequency_Hz());
                }
            }
        }
    }
}
void ChosenCalKitsModel::resetFrequencies() {
    for (int i = 0; i < _kits.size(); i++)
        _kits[i].clearStartStop();
}


