#include "Thru.h"

Thru::Thru() :
    port1(0),
    port2(0),
    isMeasured(false)
{

}
Thru::Thru(const Thru &other) :
    port1(other.port1),
    port2(other.port2),
    isMeasured(other.isMeasured)
{

}

Thru::~Thru()
{

}

QString Thru::displayText() const {
    QString text = "Thru %1-%2";
    text = text.arg(port1);
    text = text.arg(port2);
    return text;
}

void Thru::operator=(const Thru &other) {
    port1 = other.port1;
    port2 = other.port2;
    isMeasured = other.isMeasured;
}
