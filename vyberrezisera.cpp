#include "vyberrezisera.h"
#include "ui_vyberrezisera.h"
#include <QSqlQuery>

VyberRezisera::VyberRezisera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VyberRezisera)
{
    ui->setupUi(this);
    QSqlQuery dotaz;
    dotaz.prepare("SELECT * FROM reziser");
    dotaz.exec();
    while(dotaz.next())
    {
        ui->comboBox_reziser->insertItem(0,dotaz.value("jmeno").toString(), dotaz.value("id"));
    }
}

VyberRezisera::~VyberRezisera()
{
    delete ui;
}

int VyberRezisera::getReziserId()
{
    return ui->comboBox_reziser->currentData().toInt();
}
