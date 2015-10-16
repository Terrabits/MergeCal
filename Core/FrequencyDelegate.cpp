#include "FrequencyDelegate.h"


#include "FrequencyEditor.h"
#include "ChosenCalKitsModel.h"

#include <QDebug>


FrequencyDelegate::FrequencyDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

FrequencyDelegate::~FrequencyDelegate()
{

}

QWidget *FrequencyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);
    Q_UNUSED(option);
    FrequencyEditor *editor = new FrequencyEditor(parent);
    connect(editor, SIGNAL(editingFinished()),
            this, SLOT(commitAndCloseEditor()));
    return editor;
}
void FrequencyDelegate::commitAndCloseEditor() {
    FrequencyEditor *editor = qobject_cast<FrequencyEditor*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
void FrequencyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    Q_UNUSED(editor);
    Q_UNUSED(index);
    // Do nothing?
}
void FrequencyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    FrequencyEditor *_editor = qobject_cast<FrequencyEditor*>(editor);
    ChosenCalKitsModel *_model = qobject_cast<ChosenCalKitsModel*>(model);

    if (_editor->hasAcceptableInput() && !_editor->text().isEmpty()) {
        QString text = _editor->text();
        char prefix = '\0';
        if (_editor->hasPrefix()) {
            prefix = text.at(text.size()-1).toUpper().toLatin1();
            text.chop(1);
        }
        double value = text.toDouble();
        switch (prefix) {
        case 'G':
            value *= 1.0E9;
            break;
        case 'M':
            value *= 1.0E6;
            break;
        case 'K':
            value *= 1.0E3;
            break;
        default:
            break;
        }
        _model->setData(index, value);
    }
}
