#ifndef MEASUREPAGE_H
#define MEASUREPAGE_H


// RsaToolbox
#include <WizardPage.h>

// Qt
#include <QWidget>
#include <QLabel>
#include <QStatusBar>


namespace Ui {
class MeasurePage;
}

class MeasurePage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit MeasurePage(QWidget *parent = 0);
    ~MeasurePage();

    virtual void initialize();

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setStatusBar(QStatusBar *statusBar);
    QStatusBar *statusBar();

private:
    Ui::MeasurePage *ui;

    QLabel *_header;
    QStatusBar *_statusBar;
};

#endif // MEASUREPAGE_H
