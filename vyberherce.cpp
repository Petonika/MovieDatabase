#include "vyberherce.h"
#include "ui_vyberherce.h"
#include <QSqlQuery>

VyberHerce::VyberHerce(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VyberHerce)
{
    ui->setupUi(this);
    QSqlQuery dotaz;
        dotaz.prepare("SELECT * FROM herec");
        dotaz.exec();
        while(dotaz.next())
        {
            ui->comboBox->insertItem(0,dotaz.value("jmeno").toString(), dotaz.value("id"));
        }
}

VyberHerce::~VyberHerce()
{
    delete ui;
}

int VyberHerce::getHerecId()
{
    return ui->comboBox->currentData().toInt();
}
