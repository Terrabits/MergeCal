#include "CalKitsPage.h"
#include "ui_CalKitsPage.h"


// RsaToolbox
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
    ui->chosenKits->setModel(&_chosenCalKitsModel);
}

CalKitsPage::~CalKitsPage()
{
    delete ui;
}

void CalKitsPage::initialize() {
    breadCrumbs()->hide();
    if (isHeader())
        _header->setPixmap(QPixmap(":/images/Images/2 Cal Kits.bmp"));
}
void CalKitsPage::backToThis() {
    breadCrumbs()->hide();
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
void CalKitsPage::setConnectorType(Connector type) {
    if (_connectorType == type)
        return;

    _connectorType = type;
    _availableCalKitsModel.setConnectorType(_connectorType);
    _chosenCalKitsModel.clear();
}

void CalKitsPage::on_addKit_clicked()
{
    QModelIndex index = ui->availableKits->selectionModel()->currentIndex();
    if (!index.isValid())
        return;

    DoubleOffsetShortKit kit = _availableCalKitsModel.calKit(index);
    _chosenCalKitsModel.addCalKit(FrequencyRange(kit));
}
