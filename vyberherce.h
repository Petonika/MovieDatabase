#ifndef VYBERHERCE_H
#define VYBERHERCE_H

#include <QDialog>

namespace Ui {
class VyberHerce;
}

class VyberHerce : public QDialog
{
    Q_OBJECT

public:
    explicit VyberHerce(QWidget *parent = 0);
    ~VyberHerce();
    int getHerecId();

private:
    Ui::VyberHerce *ui;
};

#endif // VYBERHERCE_H
