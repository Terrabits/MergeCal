#include "PortsPage.h"
#include "ui_PortsPage.h"


// RsaToolbox
using namespace RsaToolbox;


PortsPage::PortsPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::PortsPage)
{
    ui->setupUi(this);


}

PortsPage::~PortsPage()
{
    delete ui;
}


void PortsPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/1 Ports.bmp"));
}

void PortsPage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *PortsPage::headerLabel() {
    return _header;
}
