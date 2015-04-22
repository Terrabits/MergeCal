#ifndef AVAILABLECALKITSVIEW_H
#define AVAILABLECALKITSVIEW_H


// RsaToolbox
#include <NameLabel.h>


// Qt
#include <QTableView>


class AvailableCalKitsView : public QTableView
{
    Q_OBJECT
public:
    explicit AvailableCalKitsView(QWidget *parent = 0);

    // Extra
    bool isSelected() const;
    RsaToolbox::NameLabel selectedCalKit() const;

signals:

public slots:

};

#endif // AVAILABLECALKITSVIEW_H
