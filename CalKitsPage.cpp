#include "CalKitsPage.h"
#include "ui_CalKitsPage.h"


// Project
#include "FrequencyDelegate.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalKitsPage::CalKitsPage(QWidget *parent) :
    WizardPage(parent),
    _header(NULL),
    _vna(NULL),
    _keys(NULL),
    ui(new ::Ui::CalKitsPage)
{
    ui->setupUi(this);

    ui->availableKits->setModel(&_availableCalKitsModel);
    ui->chosenKits->setItemDelegate(new FrequencyDelegate);
    ui->chosenKits->setModel(&_chosenCalKitsModel);

    connect(&_availableCalKitsModel, SIGNAL(modelReset()),
            ui->availableKits, SLOT(resizeColumnsToContents()));
    connect(&_availableCalKitsModel, SIGNAL(modelReset()),
            &_chosenCalKitsModel, SLOT(clear()));
    connect(&_chosenCalKitsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            ui->chosenKits, SLOT(resizeColumnsToContents()));
    connect(&_chosenCalKitsModel, SIGNAL(error(QString)),
            ui->error, SLOT(showMessage(QString)));

    ui->availableKits->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->availableKits->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

CalKitsPage::~CalKitsPage()
{
    delete ui;
}

void CalKitsPage::initialize() {
    breadCrumbs()->hide();
    ui->error->clearMessage();
    if (isHeader())
        _header->setPixmap(QPixmap(":/images/Images/2 Cal Kits.bmp"));
}
bool CalKitsPage::isReadyForNext() {
    if (_chosenCalKitsModel.calKits().isEmpty()) {
        ui->availableKits->selectRow(0);
        ui->addKit->setFocus();
        ui->error->showMessage("*Please choose cal kits");
        return false;
    }
    for (int i = 0; i < _chosenCalKitsModel.calKits().size(); i++) {
        if (!_chosenCalKitsModel.calKits()[i].isStartFrequency()) {
            ui->chosenKits->edit(_chosenCalKitsModel.index(i, ChosenCalKitsModel::START_COLUMN));
            ui->error->showMessage("*Enter a start frequency");
            return false;
        }
        if (!_chosenCalKitsModel.calKits()[i].isStopFrequency()) {
            ui->chosenKits->edit(_chosenCalKitsModel.index(i, ChosenCalKitsModel::STOP_COLUMN));
            ui->error->showMessage("*Enter a stop frequency");
            return false;
        }
        if (_chosenCalKitsModel.calKits()[i].startFrequency_Hz() < _chosenCalKitsModel.calKits()[i].calKit().minimumFrequency_Hz()) {
            ui->chosenKits->edit(_chosenCalKitsModel.index(i, ChosenCalKitsModel::START_COLUMN));
            ui->error->showMessage("*Start frequency is out of the range of the cal kit");
            return false;
        }
        if (_chosenCalKitsModel.calKits()[i].stopFrequency_Hz() > _chosenCalKitsModel.calKits()[i].calKit().maximumFrequency_Hz()) {
            ui->chosenKits->edit(_chosenCalKitsModel.index(i, ChosenCalKitsModel::STOP_COLUMN));
            ui->error->showMessage("*Stop frequency is out of the range of the cal kit");
            return false;
        }
        if (_chosenCalKitsModel.calKits()[i].startFrequency_Hz() >= _chosenCalKitsModel.calKits()[i].stopFrequency_Hz()) {
            ui->chosenKits->selectRow(i);
            ui->chosenKits->setFocus();
            ui->error->showMessage("*Start frequency must be less than stop frequency");
            return false;
        }
        if (i != _chosenCalKitsModel.calKits().size()-1 && _chosenCalKitsModel.calKits()[i].stopFrequency_Hz() != _chosenCalKitsModel.calKits()[i+1].startFrequency_Hz()) {
            ui->chosenKits->selectRow(i);
            ui->chosenKits->setFocus();
            ui->error->showMessage("*Cal kits must cover a continuous frequency range");
            return false;
        }
        if (_chosenCalKitsModel.calKits().first().startFrequency_Hz() > _channelStartFreq_Hz) {
            ui->error->showMessage("*Cal kits do not cover entire frequency range");
            return false;
        }
        if (_chosenCalKitsModel.calKits().last().stopFrequency_Hz() < _channelStopFreq_Hz) {
            ui->error->showMessage("*Cal kits do not cover entire frequency range");
            return false;
        }

        QRowVector frequencies = _vna->channel(_channel).linearSweep().frequencies_Hz();
        if (!isFrequencyPoint(frequencies,
                             _chosenCalKitsModel.calKits()[i].isStartFrequencyInclusive(),
                             _chosenCalKitsModel.calKits()[i].startFrequency_Hz(),
                             _chosenCalKitsModel.calKits()[i].isStopFrequencyInclusive(),
                             _chosenCalKitsModel.calKits()[i].stopFrequency_Hz()))
        {
            ui->chosenKits->selectRow(i);
            ui->chosenKits->setFocus();
            QString msg = "*\'%1\' does not cover any calibration points";
            msg = msg.arg(_chosenCalKitsModel.calKits()[i].calKit().displayNameLabel());
            ui->error->showMessage(msg);
            return false;
        }
    }

    saveKeys();

    emit calKitsSelected(_chosenCalKitsModel.calKits());

    // Set _calibration properties
    _calibration->setVna(_vna);
    _calibration->setPorts(_ports);
    _calibration->setConnector(_connectorType);
    _calibration->setChannel(_channel);
    _calibration->setCalKits(_chosenCalKitsModel.calKits());
//    _calibration->initialize();

    return true;
}

void CalKitsPage::backToThis() {
    breadCrumbs()->hide();
    ui->error->clearMessage();
    if (isHeader())
        _header->setPixmap(QPixmap(":/images/Images/2 Cal Kits.bmp"));
}

bool CalKitsPage::isHeader() const {
    return _header != NULL;
}
void CalKitsPage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *CalKitsPage::headerLabel() {
    return _header;
}

void CalKitsPage::setVna(Vna *vna) {
    if (_vna == vna)
        return;

    _vna = vna;
    _availableCalKitsModel.setVna(_vna);
    ui->availableKits->resizeColumnsToContents();
    ui->availableKits->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->availableKits->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    loadKeys();
}
void CalKitsPage::setKeys(Keys *keys) {
    _keys = keys;
    loadKeys();
}
void CalKitsPage::setCalibration(Calibration *calibration) {
    _calibration = calibration;
}

Connector CalKitsPage::connectorType() const {
    return _connectorType;
}
void CalKitsPage::setPorts(const QVector<uint> &ports) {
    _ports = ports;
    _availableCalKitsModel.setPorts(_ports);
    ui->availableKits->resizeColumnsToContents();
    ui->availableKits->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->availableKits->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void CalKitsPage::setConnectorType(const Connector &type) {
    if (_connectorType == type)
        return;

    _connectorType = type;
    _availableCalKitsModel.setConnectorType(_connectorType);
    ui->availableKits->resizeColumnsToContents();
    ui->availableKits->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->availableKits->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}
void CalKitsPage::setChannel(const uint &index) {
    _channel = index;
    updateChannelInfo();
}

void CalKitsPage::displayError(const QString &message) {
    ui->error->showMessage(message);
}

void CalKitsPage::cancel() {
    wizard()->close();
}

void CalKitsPage::on_addKit_clicked()
{
    QModelIndex index = ui->availableKits->selectionModel()->currentIndex();
    if (!index.isValid())
        return;

    DoubleOffsetShortKit kit = _availableCalKitsModel.calKit(index);
    _chosenCalKitsModel.addCalKit(FrequencyRange(kit));
    ui->chosenKits->resizeColumnsToContents();
    ui->availableKits->resizeColumnsToContents();
}

void CalKitsPage::on_removeKit_clicked()
{
    QModelIndex index = ui->chosenKits->selectionModel()->currentIndex();
    if (!index.isValid())
        return;

    _chosenCalKitsModel.removeRow(index.row());
}

void CalKitsPage::updateChannelInfo() {
    if (_vna == NULL)
        return;

    VnaChannel channel = _vna->channel(_channel);
    QRowVector frequencies = channel.linearSweep().frequencies_Hz();
    _channelStartFreq_Hz = RsaToolbox::min(frequencies);
    _channelStopFreq_Hz = RsaToolbox::max(frequencies);

    QString text = "Calibration range: %1 to %2";
    text = text.arg(formatValue(_channelStartFreq_Hz, 3, Units::Hertz));
    text = text.arg(formatValue(_channelStopFreq_Hz, 3, Units::Hertz));
    ui->channelInfo->setText(text);
}

bool CalKitsPage::isFrequencyPoint(QRowVector &frequencies, bool isStartInclusive, double start, bool isStopInclusive, double stop) {
    while (!frequencies.isEmpty() && frequencies.first() < start)
        frequencies.removeFirst();
    if (!isStartInclusive && !frequencies.isEmpty() && frequencies.first() == start)
        frequencies.removeFirst();
    while (!frequencies.isEmpty() && frequencies.last() > stop)
        frequencies.removeLast();
    if (!isStopInclusive && !frequencies.isEmpty() && frequencies.last() == stop)
        frequencies.removeLast();

    return !frequencies.isEmpty();
}

void CalKitsPage::loadKeys() {
    if (_vna == NULL || _keys == NULL)
        return;

    if (_keys->exists("CalKitsPage_Ports")) {
        QVector<uint> ports;
        _keys->get("CalKitsPage_Ports", ports);
        setPorts(ports);
    }
    if (_keys->exists("CalKitsPage_ConnectorType")) {
        Connector connectorType;
        _keys->get("CalKitsPage_ConnectorType", connectorType);
        setConnectorType(connectorType);
    }
    if (_keys->exists("CalKitsPage_ChosenKits")) {
        QVector<FrequencyRange> calKits;
        _keys->get("CalKitsPage_ChosenKits", calKits);
        foreach (FrequencyRange kit, calKits) {
            if (_availableCalKitsModel.hasCalKit(kit.calKit())) {
                _chosenCalKitsModel.addCalKit(kit);
            }
            else {
            }
        }
        ui->chosenKits->resizeColumnsToContents();
    }
}
void CalKitsPage::saveKeys() {
    if (_vna == NULL || _keys == NULL)
        return;

    _keys->set("CalKitsPage_Ports", _ports);
    _keys->set("CalKitsPage_ConnectorType", _connectorType);
    _keys->set("CalKitsPage_ChosenKits", _chosenCalKitsModel.calKits());
}
