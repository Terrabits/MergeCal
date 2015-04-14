#ifndef SETUPPAGE_H
#define SETUPPAGE_H


// RsaToolbox
#include <WizardPage.h>

// Qt
#include <QWidget>
#include <QLabel>
#include <QStatusBar>


namespace Ui {
class SetupPage;
}

class SetupPage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit SetupPage(QWidget *parent = 0);
    ~SetupPage();

    virtual void initialize();

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

    void setStatusBar(QStatusBar *statusBar);
    QStatusBar *statusBar();

private:
    Ui::SetupPage *ui;

    QLabel *_header;
    QStatusBar *_statusBar;
};

#endif // SETUPPAGE_H
