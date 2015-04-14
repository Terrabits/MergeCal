#ifndef CALKITSPAGE_H
#define CALKITSPAGE_H


// RsaToolbox
#include <WizardPage.h>

// Qt
#include <QWidget>
#include <QLabel>


namespace Ui {
class CalKitsPage;
}

class CalKitsPage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit CalKitsPage(QWidget *parent = 0);
    ~CalKitsPage();

    virtual void initialize();

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

private:
    Ui::CalKitsPage *ui;

    QLabel *_header;
};

#endif // CALKITSPAGE_H
