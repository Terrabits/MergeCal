#include "MeasurePage.h"
#include "ui_MeasurePage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QProgressBar>


MeasurePage::MeasurePage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::MeasurePage)
{
    ui->setupUi(this);
}

MeasurePage::~MeasurePage()
{
    delete ui;
}

void MeasurePage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/4 Measure.bmp"));

    _statusBar->show();
    buttons()->next()->setText("Apply");
    buttons()->next()->setDisabled(true);
}

void MeasurePage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *MeasurePage::headerLabel() {
    return _header;
}

void MeasurePage::setStatusBar(QStatusBar *statusBar) {
    _statusBar = statusBar;
}
QStatusBar *MeasurePage::statusBar() {
    return _statusBar;
}
