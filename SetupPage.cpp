#include "SetupPage.h"
#include "ui_SetupPage.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QProgressBar>


SetupPage::SetupPage(QWidget *parent) :
    WizardPage(parent),
    ui(new ::Ui::SetupPage)
{
    ui->setupUi(this);
}

SetupPage::~SetupPage()
{
    delete ui;
}

void SetupPage::initialize() {
    breadCrumbs()->hide();
    _header->setPixmap(QPixmap(":/images/Images/3 Setting up.bmp"));

    _statusBar->show();
//    QProgressBar *progressBar = new QProgressBar;
//    progressBar->setMaximum(100);
//    progressBar->setValue(50);
//    progressBar->setTextVisible(false);
//    _statusBar->addWidget(progressBar, 1);
}

void SetupPage::setHeaderLabel(QLabel *header) {
    _header = header;
}
QLabel *SetupPage::headerLabel() {
    return _header;
}

void SetupPage::setStatusBar(QStatusBar *statusBar) {
    _statusBar = statusBar;
}
QStatusBar *SetupPage::statusBar() {
    return _statusBar;
}
