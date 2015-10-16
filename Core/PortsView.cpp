#include "PortsView.h"

#include "PortsModel.h"

#include <QKeyEvent>

PortsView::PortsView(QWidget *parent) :
    QListView(parent)
{
    connect(this, SIGNAL(clicked(QModelIndex)),
            this, SLOT(portClicked(QModelIndex)));
}

void PortsView::toggleKeyPress() {
    if (selectedIndexes().isEmpty())
        return;

    PortsModel *_model = qobject_cast<PortsModel*>(model());
    if (_model == NULL)
        return;

    _model->toggleIndex(selectedIndexes().first());
}

void PortsView::portClicked(const QModelIndex &index) {
    PortsModel *_model = qobject_cast<PortsModel*>(model());
    if (_model != NULL) {
        _model->toggleIndex(index);
    }
}
