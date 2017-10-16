#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "vyberherce.h"
#include "vyberrezisera.h"
#include <QSqlQuery>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../MovieDatabase/database.db");
    db.open();
    model = new QSqlTableModel(this);
    model->setTable("film");
    model->select();
    model->setHeaderData(1, Qt::Horizontal, "Název filmu");
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    model_herec = new QSqlTableModel(this);
    model_herec->setTable("herec");
    model_herec->select();
    model_herec->setHeaderData(1, Qt::Horizontal, "Jméno herce");
    ui->tableView_2->setModel(model_herec);
    ui->tableView_2->hideColumn(0);
    model_reziser = new QSqlTableModel(this);
    model_reziser->setTable("reziser");
    model_reziser->select();
    model_reziser->setHeaderData(1, Qt::Horizontal, "Jméno režiséra");
    ui->tableView_3->setModel(model_reziser);
    ui->tableView_3->hideColumn(0);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    aktualni_film = -1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAddFilm_triggered()
{
    Dialog d;
    if(d.exec() == QDialog::Accepted)
    {
        QSqlQuery dotaz;
        dotaz.prepare("INSERT INTO film VALUES(NULL, :nazev)");
        dotaz.bindValue(":nazev", d.getNazev());
        dotaz.exec();
        model->select();
    }
}

void MainWindow::on_pushButton_clicked()
{
    on_actionAddFilm_triggered();
}

void MainWindow::on_actionDeleteFilm_triggered()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    QString sql = "DELETE FROM film WHERE id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sql+= QString::number(model->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    model->select();
}

void MainWindow::on_pushButton_2_clicked()
{
    on_actionDeleteFilm_triggered();
}

void MainWindow::on_pushButton_4_clicked()
{
    QString herec = QInputDialog::getText(this, "Přidat herce", "Zadej jméno herce:");
    QSqlQuery dotaz;
    dotaz.prepare("INSERT INTO herec VALUES(NULL, :jmeno)");
    dotaz.bindValue(":jmeno", herec);
    dotaz.exec();
    model_herec->select();
}

void MainWindow::on_pushButton_5_clicked()
{
    QItemSelectionModel *selection = ui->tableView_2->selectionModel();
    QString sql = "DELETE FROM herec WHERE id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sql+= QString::number(model_herec->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    model_herec->select();
}

void MainWindow::on_pushButton_6_clicked()
{
    VyberHerce v;
    if(v.exec() == QDialog::Accepted)
    {
        int id_herec = v.getHerecId();
        QSqlQuery dotaz;
        dotaz.prepare("INSERT INTO herec_to_film VALUES (NULL, :film, :herec)");
        dotaz.bindValue(":film", aktualni_film);
        dotaz.bindValue(":herec", id_herec);
        dotaz.exec();
    }
    nacti_herce();
}

void MainWindow::nacti_herce()
{
    QSqlQuery dotaz;
    dotaz.prepare("SELECT herec.jmeno, herec_to_film.id AS "
                  "id_vazby FROM herec_to_film LEFT JOIN herec"
                  " ON id_herec = herec.id WHERE id_film = :film");
    dotaz.bindValue(":film", aktualni_film);
    dotaz.exec();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    int rows = 0;
    int columns = 2;
    ui->tableWidget->setColumnCount(columns);
    QStringList hlavicky;
    hlavicky << "Id_vazby" << "Jméno herce";
    ui->tableWidget->setHorizontalHeaderLabels(hlavicky);
    ui->tableWidget->hideColumn(0);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    while (dotaz.next()) {
        rows+=1;
        ui->tableWidget->setRowCount(rows);
        QTableWidgetItem *id;
        id = new QTableWidgetItem(dotaz.value("id_vazby").toString());
        ui->tableWidget->setItem(rows-1, 0, id);
        QTableWidgetItem *jmeno;
        jmeno = new QTableWidgetItem(dotaz.value("jmeno").toString());
        ui->tableWidget->setItem(rows-1, 1, jmeno);
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    QList <QTableWidgetItem *> selected = ui->tableWidget->selectedItems();
    if(selected.count() == 0)
    {
        return;
    }
    QString sql = "DELETE FROM herec_to_film WHERE id IN (";
    for(int i=0; i<selected.count();++i)
    {
        sql+= ui->tableWidget->item(selected.at(i)->row(),0)->text();
        if(i+1 != selected.count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    qDebug() << sql;
    nacti_herce();
}

void MainWindow::on_pushButton_3_clicked()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    if(selection->selectedRows().count() == 0)
    {
        QMessageBox::warning(this, "Chyba", "Nevybral jsi žádný film");
        return;
    }
    aktualni_film = model->data(selection->selectedRows()[0]).toInt();
    ui->tabWidget->setCurrentIndex(3);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 3)
    {
        nacti_herce();
        nacti_rezisera();
        if(aktualni_film == -1)
        {
            QMessageBox::warning(this, "Chyba", "Nevybral jsi žádný film");
            ui->tabWidget->setCurrentIndex(0);
            return;
        }
        QSqlQuery dotaz;
        dotaz.prepare("SELECT * FROM film WHERE id=:film");
        dotaz.bindValue(":film", aktualni_film);
        dotaz.exec();
        if(!dotaz.next()) {
            QMessageBox::warning(this, "Chyba", "Nevybral jsi žádný film");
            ui->tabWidget->setCurrentIndex(0);
            return;
        }
        ui->label->setText(dotaz.value("nazev").toString());
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    QString reziser = QInputDialog::getText(this, "Přidat režiséra", "Zadej jméno režiséra:");
    QSqlQuery dotaz;
    dotaz.prepare("INSERT INTO reziser VALUES(NULL, :jmeno)");
    dotaz.bindValue(":jmeno", reziser);
    dotaz.exec();
    model_reziser->select();
}

void MainWindow::on_pushButton_11_clicked()
{
    QItemSelectionModel *selection = ui->tableView_3->selectionModel();
    QString sql = "DELETE FROM reziser WHERE id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sql+= QString::number(model_reziser->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    model_reziser->select();
}

void MainWindow::on_pushButton_8_clicked()
{
    VyberRezisera v;
    if(v.exec() == QDialog::Accepted)
    {
        int id_reziser = v.getReziserId();
        QSqlQuery dotaz;
        dotaz.prepare("INSERT INTO reziser_to_film VALUES (NULL, :film, :reziser)");
        dotaz.bindValue(":film", aktualni_film);
        dotaz.bindValue(":reziser", id_reziser);
        dotaz.exec();
    }
    nacti_rezisera();
}

void MainWindow::on_pushButton_9_clicked()
{
    QList <QTableWidgetItem *> selected = ui->tableWidget_2->selectedItems();
    if(selected.count() == 0)
    {
        return;
    }
    QString sql = "DELETE FROM reziser_to_film WHERE id IN (";
    for(int i=0; i<selected.count();++i)
    {
        sql+= ui->tableWidget_2->item(selected.at(i)->row(),0)->text();
        if(i+1 != selected.count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    qDebug() << sql;
    nacti_rezisera();
}

void MainWindow::nacti_rezisera()
{
    QSqlQuery dotaz;
    dotaz.prepare("SELECT reziser.jmeno, reziser_to_film.id AS "
                  "id_vazby FROM reziser_to_film LEFT JOIN reziser"
                  " ON id_reziser = reziser.id WHERE id_film = :film");
    dotaz.bindValue(":film", aktualni_film);
    dotaz.exec();
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setRowCount(0);
    int rows = 0;
    int columns = 2;
    ui->tableWidget_2->setColumnCount(columns);
    QStringList hlavicky;
    hlavicky << "Id_vazby" << "Jméno režiséra";
    ui->tableWidget_2->setHorizontalHeaderLabels(hlavicky);
    ui->tableWidget_2->hideColumn(0);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    while (dotaz.next()) {
        rows+=1;
        ui->tableWidget_2->setRowCount(rows);
        QTableWidgetItem *id;
        id = new QTableWidgetItem(dotaz.value("id_vazby").toString());
        ui->tableWidget_2->setItem(rows-1, 0, id);
        QTableWidgetItem *jmeno;
        jmeno = new QTableWidgetItem(dotaz.value("jmeno").toString());
        ui->tableWidget_2->setItem(rows-1, 1, jmeno);
    }
}
