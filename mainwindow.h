#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void uploadFrac();
    void readFracFile();
    void uploadFracfast( QStringList);
    void readFracFilefast();
    void numOfFractures();

private:
    Ui::MainWindow *ui;
    QSqlDatabase dataBase;
    QString fracID, attribute, xyz1, xyz2, xyz3, xyz4, xyz5, vecter;
    QString inPath;
    long fracNum;
    QString tableName, fileName;
};
#endif // MAINWINDOW_H
