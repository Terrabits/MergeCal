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
    if (column < 1 || uint(column) >= COLUMNS)
        return false;

    int row = index.row();
    if (row < 0 || row >= _kits.size())
        return false;

    // set data
    Q_UNUSED(value);
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
            if (_kits[j].calKit().minimumFrequency_Hz() > _kits[j+1].calKit().minimumFrequency_Hz()) {
                FrequencyRange temp = _kits[j];
                _kits[j] = _kits[j+1];
                _kits[j+1] = temp;
            }
        }
    }
}
void ChosenCalKitsModel::setDefaultFrequencies() {
    sortKits();
    for (int i = 0; i < _kits.size(); i++) {
        if (!_kits[i].isStartFrequency()) {
            if (i == 0)
                _kits[i].setStartFrequency(_kits[i].calKit().minimumFrequency_Hz());
            else if (_kits[i-1].isStopFrequency())
                _kits[i].setStartFrequency(_kits[i-1].stopFrequency_Hz());
            else {
                if (_kits[i].calKit().minimumFrequency_Hz() < _kits[i-1].calKit().maximumFrequency_Hz())
                    _kits[i].setStartFrequency((_kits[i].calKit().minimumFrequency_Hz() + _kits[i-1].calKit().maximumFrequency_Hz())/2.0);
                else
                    _kits[i].setStartFrequency(_kits[i].calKit().minimumFrequency_Hz());
            }
        }
        if (!_kits[i].isStopFrequency()) {
            if (i == _kits.size() - 1)
                _kits[i].setStopFrequency(_kits[i].calKit().maximumFrequency_Hz());
            else if (_kits[i+1].isStartFrequency())
                _kits[i].setStopFrequency(_kits[i+1].startFrequency_Hz());
            else {
                if (_kits[i].calKit().maximumFrequency_Hz() > _kits[i+1].calKit().minimumFrequency_Hz())
                    _kits[i].setStopFrequency((_kits[i].calKit().maximumFrequency_Hz() + _kits[i+1].calKit().minimumFrequency_Hz())/2.0);
                else
                    _kits[i].setStopFrequency(_kits[i].calKit().maximumFrequency_Hz());
            }
        }
    }
}
void ChosenCalKitsModel::resetFrequencies() {
    foreach (FrequencyRange r, _kits) {
        r.clearStartStop();
    }
}


