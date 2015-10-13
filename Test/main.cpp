

#include "TestRunner.h"

// Test classes
//#include "TestClass.h"

#include <QDebug>


int main() {
    TestRunner testRunner;
    testRunner.addTest(new ...()); //your QTest compatible class here

    qDebug() << "Overall result: " << (testRunner.runTests()?"PASS":"FAIL");

    return 0;
}
