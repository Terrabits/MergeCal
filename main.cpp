

// MergeCal
#include "Settings.h"
#include "PortsPage.h"
#include "CalKitsPage.h"
#include "SetupPage.h"
#include "MeasurePage.h"

// RsaToolbox
#include <Log.h>
#include <Vna.h>
#include <Keys.h>
#include <Wizard.h>
using namespace RsaToolbox;

// Qt
#include <QString>
#include <QApplication>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDesktopWidget>
#include <QStatusBar>


bool isNoConnection(Vna &vna);
bool isUnknownModel(Vna &vna);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log log(LOG_FILENAME, APP_NAME, APP_VERSION);
    log.printHeader();

    Vna vna(CONNECTION_TYPE, INSTRUMENT_ADDRESS);
    vna.useLog(&log);
    vna.printInfo();

    Keys keys(KEY_PATH);

    if (isNoConnection(vna) || isUnknownModel(vna))
        return 0;

    Wizard wizard;
    wizard.setWindowTitle(APP_NAME);
    wizard.setGeometry(0,0,625,500);
    wizard.move(QApplication::desktop()->screen()->rect().center() - wizard.rect().center());
    wizard.setWindowFlags(wizard.windowFlags() | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(wizard.layout());
    QLabel *label = new QLabel;
    layout->insertWidget(1, label);

    QStatusBar *statusBar = new QStatusBar;
    layout->insertWidget(-1, statusBar);
    statusBar->setSizeGripEnabled(false);
    statusBar->hide();

    PortsPage *portsPage = new PortsPage;
    portsPage->setName("Ports");
    portsPage->setNextIndex(1);
    portsPage->setHeaderLabel(label);
    portsPage->setVna(&vna);
    wizard.addPage(portsPage);

    CalKitsPage *calKitsPage = new CalKitsPage;
    calKitsPage->setName("Cal Kits");
    calKitsPage->setHeaderLabel(label);
    calKitsPage->setNextIndex(2);
    calKitsPage->setVna(&vna);
    QObject::connect(portsPage, SIGNAL(connectorSelected(RsaToolbox::Connector)),
                     calKitsPage, SLOT(setConnectorType(RsaToolbox::Connector)));
    wizard.addPage(calKitsPage);

    SetupPage *setupPage = new SetupPage;
    setupPage->setName("Setup");
    setupPage->setHeaderLabel(label);
    setupPage->setStatusBar(statusBar);
    setupPage->setNextIndex(3);
    wizard.addPage(setupPage);

    MeasurePage *measurePage = new MeasurePage;
    measurePage->setName("Measure");
    measurePage->setHeaderLabel(label);
    measurePage->setStatusBar(statusBar);
    wizard.addPage(measurePage);

    wizard.show();


    return a.exec();
}


bool isNoConnection(Vna &vna) {
    if (vna.isDisconnected()) {
        QString error_message
                = "Instrument not found. Please run this application on a Rohde & Schwarz VNA.";
        QMessageBox::critical(NULL,
                              "R&S Merge Cal",
                              error_message);
        vna.print(error_message);
        return true;
    }
    else
        return false;
}
bool isUnknownModel(Vna &vna) {
    if (vna.properties().isUnknownModel()) {
        QString error_message(QString("VNA not recognized.\n")
                              + "Please use R&S Merge Cal with a Rohde & Schwarz instrument");
        QMessageBox::critical(NULL,
                              "R&S Merge Cal",
                              error_message);
        vna.print(error_message);
        return true;
    }
    else
        return false;
}

