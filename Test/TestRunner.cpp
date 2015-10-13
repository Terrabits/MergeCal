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

void TestRunner::addTest(QObject *test) {
    test->setParent(this);
    _tests.append(test);
}

bool TestRunner::runTests() {
    int argc =0;
    char * argv[] = {0};
    QCoreApplication app(argc, argv);
    QMetaObject::invokeMethod(this, SLOT(run()),
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
    foreach (QObject * test, _tests) {
        _allTestsPassed |= (QTest::qExec(test) == 0);
    }
}
