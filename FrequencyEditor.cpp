#include "FrequencyEditor.h"

#include <QRegExpValidator>

FrequencyEditor::FrequencyEditor(QWidget *parent) :
    QLineEdit(parent)
{
    QRegExp regex("[1-9][0-9]*\\.?[0-9]*[GMK]?", Qt::CaseInsensitive);
    QRegExpValidator *validator = new QRegExpValidator(regex);
    this->setValidator(validator);

    connect(this, SIGNAL(textEdited(QString)),
            this, SLOT(checkForPrefix()));
}

FrequencyEditor::~FrequencyEditor()
{

}

bool FrequencyEditor::hasPrefix() const {
    QRegExp regex("[GMK]",Qt::CaseInsensitive);
    if (text().contains(regex))
        return true;
    else
        return false;
}

void FrequencyEditor::checkForPrefix() {
    if (hasPrefix())
        emit editingFinished();
}
