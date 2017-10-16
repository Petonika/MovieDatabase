#ifndef VYBERREZISERA_H
#define VYBERREZISERA_H

#include <QDialog>

namespace Ui {
class VyberRezisera;
}

class VyberRezisera : public QDialog
{
    Q_OBJECT

public:
    explicit VyberRezisera(QWidget *parent = 0);
    ~VyberRezisera();
    int getReziserId();

private:
    Ui::VyberRezisera *ui;
};

#endif // VYBERREZISERA_H
