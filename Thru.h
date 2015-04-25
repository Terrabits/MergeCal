#ifndef THRU_H
#define THRU_H


class Thru
{
public:
    Thru();
    ~Thru();

    uint outputPort;
    uint inputPort;
    bool isCompleted;

    QString displayText() const;
};

#endif // THRU_H
