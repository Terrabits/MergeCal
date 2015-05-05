#ifndef SETTINGS_H
#define SETTINGS_H


// RsaToolbox
#include "General.h"
#include "GenericBus.h"


// Application settings
const QString APP_NAME = "R&S Merge Cal";
const QString APP_VERSION = "1.0";
const QString MANUFACTURER_FOLDER = "Rohde-Schwarz";
const QString APP_FOLDER = "Merge Cal";
const QString APP_DESCRIPTION = "Calibrate using offset short pairs from multiple cal kits";
const QString CONTACT_INFO = "<html><head/><body><p>Nick Lalic<br/>VNA Software Developer<br/>Cupertino, CA USA<br/>+1 424 200 2846<br/>nick.lalic@rsa.rohde-schwarz.com<br/><a href=\"http://vna.rs-us.net\"><span style=\"text-decoration: underline; color:#0000ff;\">http://vna.rs-us.net</span></a></p></body></html>";

// Log, Key
#ifdef DEBUG_MODE
const QDir dataDir(SOURCE_DIR);
const QString LOG_FILENAME = dataDir.filePath("DEBUG_LOGFILE.txt");
const QString KEY_PATH = dataDir.filePath("Keys");
#else
const QDir dataDir(RsaToolbox::GetAppDataPath(MANUFACTURER_FOLDER, APP_FOLDER));
const QString LOG_FILENAME = dataDir.filePath("R&S Merge Cal Log.txt");
const QString KEY_PATH = dataDir.filePath("Settings");
#endif

// Connection settings
const RsaToolbox::ConnectionType CONNECTION_TYPE = RsaToolbox::TCPIP_CONNECTION;
const QString INSTRUMENT_ADDRESS = "127.0.0.1";
const unsigned int TIMEOUT_MS = 1000;

// Keys
const QString EXAMPLE_KEY = "EXAMPLE"; // Place your keys here


#endif

