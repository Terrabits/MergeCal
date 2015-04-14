#include "CalKitsPage.h"
#include "ui_CalKitsPage.h"

CalKitsPage::CalKitsPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::CalKitsPage)
{
    ui->setupUi(this);

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
