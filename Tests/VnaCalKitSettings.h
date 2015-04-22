#ifndef SETTINGS_H
#define SETTINGS_H


// RsaToolbox
#include "General.h"
#include "GenericBus.h"


// Application settings
const QString APP_NAME = "VnaCalKit Tests";
const QString APP_VERSION = "1.0";
const QString MANUFACTURER_FOLDER = "Rohde-Schwarz";
const QString APP_FOLDER = "VnaCalKit Tests";

// Log, Key
const QDir dataDir(SOURCE_DIR);
const QString LOG_FILENAME = dataDir.filePath("VnaCalKit Tests Log File.txt");
const QString KEY_PATH = dataDir.filePath("Keys");

// Connection settings
const RsaToolbox::ConnectionType CONNECTION_TYPE = RsaToolbox::TCPIP_CONNECTION;
const QString INSTRUMENT_ADDRESS = "127.0.0.1";
const unsigned int TIMEOUT_MS = 1000;


#endif

