

// Testing framework
#include "TestRunner.h"

// Tests
#include "TestDoubleOffsetShortKit.h"
#include "TestPartialCal.h"
#include "TestCalibration.h"

// Qt
#include <QDebug>


int main() {
    TestRunner testRunner;
    testRunner.addTest(new TestDoubleOffsetShortKit);
    testRunner.addTest(new TestPartialCal);
    testRunner.addTest(new TestCalibration);
    qDebug() << "Global result: " << (testRunner.runTests()?"PASS":"FAIL");

    return 0;
}
