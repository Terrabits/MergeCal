#ifndef MEASUREPAGE_H
#define MEASUREPAGE_H


// Project
#include "TimedProgressBar.h"

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

    void setProgressBar(TimedProgressBar *progressBar);
    TimedProgressBar *progressBar();

private:
    Ui::MeasurePage *ui;

    QLabel *_header;
    TimedProgressBar *_progressBar;
};

#endif // MEASUREPAGE_H
