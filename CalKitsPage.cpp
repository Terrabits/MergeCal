#include "CalKitsPage.h"
#include "ui_CalKitsPage.h"


// RsaToolbox
using namespace RsaToolbox;

CalKitsPage::CalKitsPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::CalKitsPage)
{
    ui->setupUi(this);

    ui->availableKits->setModel(&_availableCalKitsModel);

}

CalKitsPage::~CalKitsPage()
{
    delete ui;
}

void CalKitsPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/2 Cal Kits.bmp"));
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
}
