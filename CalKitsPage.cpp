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
    ui(new ::Ui::CalKitsPage)
{
    ui->setupUi(this);

    ui->availableKits->setModel(&_availableCalKitsModel);
    ui->chosenKits->setItemDelegate(new FrequencyDelegate);
    ui->chosenKits->setModel(&_chosenCalKitsModel);

    connect(&_availableCalKitsModel, SIGNAL(modelReset()),
            &_chosenCalKitsModel, SLOT(clear()));
    connect(&_chosenCalKitsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            ui->chosenKits, SLOT(resizeColumnsToContents()));
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
        if (_chosenCalKitsModel.calKits()[0].startFrequency_Hz() > _channelStartFreq_Hz) {
            ui->error->showMessage("*Cal kits do not cover entire frequency range");
            return false;
        }
        if (_chosenCalKitsModel.calKits().last().stopFrequency_Hz() < _channelStopFreq_Hz) {
            ui->error->showMessage("*Cal kits do not cover entire frequency range");
            return false;
        }
    }

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
}
Connector CalKitsPage::connectorType() const {
    return _connectorType;
}
void CalKitsPage::setPorts(const QVector<uint> &ports) {
    _ports = ports;
    _availableCalKitsModel.setPorts(_ports);
}

void CalKitsPage::setConnectorType(const Connector &type) {
    if (_connectorType == type)
        return;

    _connectorType = type;
    _availableCalKitsModel.setConnectorType(_connectorType);
    ui->availableKits->resizeColumnsToContents();
}
void CalKitsPage::setChannel(uint index) {
    _channel = index;
    updateChannelInfo();
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
