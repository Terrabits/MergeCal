#include "Thru.h"

Thru::Thru() :
    outputPort(0),
    inputPort(0),
    isCompleted(false)
{

}

Thru::~Thru()
{

}

QString Thru::displayText() const {
    QString text = "Thru %1 to %2";
    text = text.arg(outputPort);
    text = text.arg(inputPort);
    return text;
}
