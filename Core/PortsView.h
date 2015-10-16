#ifndef PORTSVIEW_H
#define PORTSVIEW_H

#include <QListView>

class PortsView : public QListView
{
    Q_OBJECT
public:
    explicit PortsView(QWidget *parent = 0);

signals:

public slots:
    void toggleKeyPress();

private slots:
    void portClicked(const QModelIndex &index);

};

#endif // PORTSVIEW_H
