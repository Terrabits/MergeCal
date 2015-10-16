#ifndef FREQUENCYEDITOR_H
#define FREQUENCYEDITOR_H

#include <QLineEdit>

class FrequencyEditor : public QLineEdit
{
    Q_OBJECT

public:
    FrequencyEditor(QWidget *parent = 0);
    ~FrequencyEditor();

    bool hasPrefix() const;

private slots:
    void checkForPrefix();
};

#endif // FREQUENCYEDITOR_H
