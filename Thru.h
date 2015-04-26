#ifndef THRU_H
#define THRU_H


#include <QString>

class Thru
{
public:
    Thru();
    Thru(const Thru &other);
    ~Thru();

    uint port1;
    uint port2;
    bool isMeasured;

    QString displayText() const;
    void operator=(const Thru &other);
};

#endif // THRU_H
