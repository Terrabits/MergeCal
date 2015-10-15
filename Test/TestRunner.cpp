#include "TestRunner.h"


// Qt
#include <QTimer>


TestRunner::TestRunner(QObject *parent) :
    QObject(parent),
    _allTestsPassed(true)
{

}

TestRunner::~TestRunner()
{

}

void TestRunner::addTest(QObject *test, const QStringList &args) {
    test->setParent(this);
    _tests.append(test);
    _args.append(args);
}

bool TestRunner::runTests() {
    int argc =0;
    char * argv[] = {0};
    QCoreApplication app(argc, argv);
    QMetaObject::invokeMethod(this, "run",
                              Qt::QueuedConnection);
//    QTimer::singleShot(0, this, SLOT(run()) );
    app.exec();
    return _allTestsPassed;
}

void TestRunner::run() {
    doRunTests();
    QCoreApplication::instance()->quit();
}

void TestRunner::doRunTests() {
    _allTestsPassed = true;
    for (int i = 0; i < _tests.size(); i++) {
        int result = QTest::qExec(_tests[i], _args[i]);
        _allTestsPassed &= (result == 0);
    }
}
