

// MergeCalTest
#include "VnaCalKitSettings.h"

// RsaToolbox
#include "Log.h"
#include "Vna.h"
#include "Keys.h"
using namespace RsaToolbox;

// Qt
#include <QString>
#include <QApplication>
#include <QMessageBox>


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

    VnaCalKit kit = vna.calKit(NameLabel("85054D",""));
    qDebug() << "Kit: 85054D (N 50 Ohms)";
    qDebug() << "Has open? " << kit.hasOpen();
    qDebug() << "Has short? " << kit.hasShort();
    qDebug() << "Has match?" << kit.hasMatch();

    qDebug() << "Has offset short 1? " << kit.hasOffsetShort1();
    qDebug() << "Has offset short 2? " << kit.hasOffsetShort2();
    qDebug() << "Has offset short 3? " << kit.hasOffsetShort3();


    kit = vna.calKit(NameLabel("3.5 mm Non Ideal Kit",""));

    qDebug();
    qDebug() << "Kit: 3.5 mm Non Ideal Kit";
    qDebug() << "Has open? " << kit.hasOpen();
    qDebug() << "Has short? " << kit.hasShort();
    qDebug() << "Has match?" << kit.hasMatch();

    qDebug() << "Has offset short 1? " << kit.hasOffsetShort1();
    qDebug() << "Has male offset short 1? " << kit.hasMaleOffsetShort1();
    qDebug() << "Has female offset short 1? " << kit.hasFemaleOffsetShort1();

    qDebug() << "Has offset short 2? " << kit.hasOffsetShort2();
    qDebug() << "Has male offset short 2? " << kit.hasMaleOffsetShort2();
    qDebug() << "Has female offset short 2? " << kit.hasFemaleOffsetShort2();

    qDebug() << "Has offset short 3? " << kit.hasOffsetShort3();
    qDebug() << "Has male offset short 3? " << kit.hasMaleOffsetShort3();
    qDebug() << "Has female offset short 3? " << kit.hasFemaleOffsetShort3();

    vna.preset();
    vna.pause();
    vna.clearStatus();

    

    return 0;
}


bool isNoConnection(Vna &vna) {
    if (vna.isDisconnected()) {
        QString error_message
                = "Instrument not found. Please run this application on a Rohde & Schwarz VNA.";
        QMessageBox::critical(NULL,
                              "My RSA App",
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
                              + "Please use My RSA App with a Rohde & Schwarz instrument");
        QMessageBox::critical(NULL,
                              "My RSA App",
                              error_message);
        vna.print(error_message);
        return true;
    }
    else
        return false;
}

