#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAddFilm_triggered();

    void on_pushButton_clicked();

    void on_actionDeleteFilm_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void nacti_herce();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void nacti_rezisera();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlTableModel *model_herec;
    QSqlTableModel *model_reziser;
    int aktualni_film;
};

#endif // MAINWINDOW_H
