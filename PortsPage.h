#ifndef PORTSPAGE_H
#define PORTSPAGE_H


// RsaToolbox
#include <WizardPage.h>

// Qt
#include <QWidget>
#include <QLabel>


namespace Ui {
class PortsPage;
}

class PortsPage : public RsaToolbox::WizardPage
{
    Q_OBJECT

public:
    explicit PortsPage(QWidget *parent = 0);
    ~PortsPage();

    virtual void initialize();

    void setHeaderLabel(QLabel *header);
    QLabel *headerLabel();

private:
    Ui::PortsPage *ui;

    QLabel *_header;
};

#endif // PORTSPAGE_H
