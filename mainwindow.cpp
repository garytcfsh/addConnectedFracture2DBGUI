#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList drivers = QSqlDatabase::drivers();
    dataBase = QSqlDatabase::addDatabase("QMYSQL");
    dataBase.setHostName("127.0.0.1");
    dataBase.setPort(3306);
    dataBase.setUserName("root");
    dataBase.setPassword("modelmuse");
    dataBase.setDatabaseName("fractures");
    bool ok = dataBase.open();

    qDebug()<<ok;

    tableName = "connected_nof1_11";
    fileName = "canister_conn_noF1_11.fab";
    inPath = "E:\\toothpick\\program\\addConnectedFracture2DBGUI\\" + fileName;

    QSqlQuery query(dataBase);
    query.exec("alter table " + tableName + " AUTO_INCREMENT=0");
    numOfFractures();
    readFracFilefast();

    qDebug()<<"finish";
}

MainWindow::~MainWindow()
{
    delete ui;
}

//INSERT INTO `fractures`.`connected` (`fracID`, `attribute`, `xyz1`, `xyz2`, `xyz3`) VALUES ('6', '4', '4', '5', '6');
//UPDATE `fractures`.`connected` SET `attribute` = '4   1   3.3   0.0   9.1', `xyz1` = ' 1 1 2 -3', `xyz2` = ' 2 1 2 -3', `xyz3` = ' 3 1 2 -2', `xyz4` = ' 4 1 2 -2', `vecter` = ' 0 0 -0 -0' WHERE (`fracID` = '1');
//INSERT INTO fractures.connected SET (fracID, attribute, xyz1, xyz2, xyz3)
//VALUES ('3', '4', '1')
//attribut = '4 1 3 0 9', xyz1 = ' 1 1 2 -3', xyz2 = '2 1 2 -3', xyz4 = ' 4 1 2 -2', vecter = '0 0 0 '

void MainWindow::readFracFile()
{
    QString inPath = "E:\\toothpick\\program\\addConnectedFracture2DBGUI\\canister_conn.fab";
    QFile fileIn( inPath);
    fileIn.open( QFile::ReadOnly|QFile::Text);
    QTextStream fileInQTS( &fileIn);
    QStringList in, oneLineQSL, initiative, passive;
    QString oneLine;
    long i=0;

    for (int j=0; j<232; j++)
        fileInQTS.readLine();

    while (!fileInQTS.atEnd())
    {
        oneLine = fileInQTS.readLine();
        oneLineQSL = oneLine.simplified().split(" ");
        attribute = oneLine;
        xyz1 = fileInQTS.readLine();
        xyz2 = fileInQTS.readLine();
        if (oneLineQSL[1] == "4")
        {
            xyz3 = fileInQTS.readLine();
            xyz4 = fileInQTS.readLine();
            xyz5 = "";
        }
        else if (oneLineQSL[1] == "5")
        {
            xyz3 = fileInQTS.readLine();
            xyz4 = fileInQTS.readLine();
            xyz5 = fileInQTS.readLine();
        }
        else
        {
            xyz3 = fileInQTS.readLine();
            xyz4 = "";
            xyz5 = "";
        }

        vecter = fileInQTS.readLine();
        uploadFrac();
        i++;
        if (i%1000 == 0)
            qDebug()<<i;
    }
}

void MainWindow::uploadFrac()
{
    QSqlQuery query(dataBase);
    QString insert, insert1, insert2;
    insert1 = "INSERT INTO fractures.connected (attribute, xyz1, xyz2, xyz3, xyz4, xyz5, vecter) ";
    insert2 = QString("VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
            .arg(attribute)
            .arg(xyz1)
            .arg(xyz2)
            .arg(xyz3)
            .arg(xyz4)
            .arg(xyz5)
            .arg(vecter);
    insert = insert1 + insert2;
    query.exec(insert);
}

void MainWindow::numOfFractures()
{
    QFile fileIn( inPath);
    fileIn.open( QFile::ReadOnly|QFile::Text);
    QTextStream fileInQTS( &fileIn);
    QString oneLine;
    QStringList oneLineQSL;

    while (!(fileInQTS.readLine() == "BEGIN FRACTURE"))
        ;

    fracNum = 0;
    bool end = false;
    while (!end)
    {
        for (int j=0; j<1000; j++)
        {
            oneLine = fileInQTS.readLine();
            oneLineQSL = oneLine.simplified().split(" ");
            if (oneLineQSL[0] != "END")
            {
                fileInQTS.readLine();
                fileInQTS.readLine();
                if (oneLineQSL[1] == "4")
                {
                    fileInQTS.readLine();
                    fileInQTS.readLine();
                }
                else if (oneLineQSL[1] == "5")
                {
                    fileInQTS.readLine();
                    fileInQTS.readLine();
                    fileInQTS.readLine();
                }
                else
                {
                    fileInQTS.readLine();
                }
                fileInQTS.readLine();
                oneLineQSL.clear();
                fracNum++;
            }
            else
            {
                j = 1000;
                end = true;
            }

        }
    }
    fileIn.close();
}


void MainWindow::readFracFilefast()
{
    QFile fileIn( inPath);
    fileIn.open( QFile::ReadOnly|QFile::Text);
    QTextStream fileInQTS( &fileIn);
    QStringList in, oneLineQSL, initiative, passive, pluralLineQSL;
    QString oneLine;
    long i=0;

    while (!(fileInQTS.readLine() == "BEGIN FRACTURE"))
        ;

    long ii=0;
    bool end = false;
    while (!end)
    {
        pluralLineQSL.clear();
        for (int j=0; j<1000; j++)
        {
            oneLine = fileInQTS.readLine();
            oneLineQSL = oneLine.simplified().split(" ");
            if (oneLineQSL[0] != "END")
            {
                pluralLineQSL.append(oneLine);//atribute
                pluralLineQSL.append(fileInQTS.readLine());//xyz1
                pluralLineQSL.append(fileInQTS.readLine());//xyz2 = fileInQTS.readLine();
                if (oneLineQSL[1] == "4")
                {
                    pluralLineQSL.append(fileInQTS.readLine());//xyz3 = fileInQTS.readLine();
                    pluralLineQSL.append(fileInQTS.readLine());//xyz4 = fileInQTS.readLine();
                    pluralLineQSL.append("");//xyz5 = "";
                }
                else if (oneLineQSL[1] == "5")
                {
                    pluralLineQSL.append(fileInQTS.readLine());//xyz3 = fileInQTS.readLine();
                    pluralLineQSL.append(fileInQTS.readLine());//xyz4 = fileInQTS.readLine();
                    pluralLineQSL.append(fileInQTS.readLine());//xyz5 = fileInQTS.readLine();
                }
                else
                {
                    pluralLineQSL.append(fileInQTS.readLine());//xyz3 = fileInQTS.readLine();
                    pluralLineQSL.append("");//xyz4 = "";
                    pluralLineQSL.append("");//xyz5 = "";
                }

                pluralLineQSL.append(fileInQTS.readLine());//vecter = fileInQTS.readLine();
                oneLineQSL.clear();
                i++;
            }
            else
            {
                j = 1000;
                end = true;
            }

        }
        uploadFracfast(pluralLineQSL);
        ii = ii + 1000;
        qDebug()<<((float)ii/(float)fracNum*100)<<"%";
    }
}

void MainWindow::uploadFracfast( QStringList pluralLineQSL)
{

    QSqlQuery query(dataBase);
    QString insert, insert1, insert2;
    QStringList insertQSL;
    insert1 = "INSERT INTO fractures." + tableName + " (attribute, xyz1, xyz2, xyz3, xyz4, xyz5, vecter) VALUES ";
    insertQSL.append( QString("('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
            .arg(pluralLineQSL[0])//.arg(attribute)
            .arg(pluralLineQSL[1])//.arg(xyz1)
            .arg(pluralLineQSL[2])//.arg(xyz2)
            .arg(pluralLineQSL[3])//.arg(xyz3)
            .arg(pluralLineQSL[4])//.arg(xyz4)
            .arg(pluralLineQSL[5])//.arg(xyz5)
            .arg(pluralLineQSL[6]));//.arg(vecter));
    for (int i=7; i<pluralLineQSL.count(); i=i+7)
    {
        insertQSL.append( QString(", ('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
                .arg(pluralLineQSL[i])//.arg(attribute)
                .arg(pluralLineQSL[i+1])//.arg(xyz1)
                .arg(pluralLineQSL[i+2])//.arg(xyz2)
                .arg(pluralLineQSL[i+3])//.arg(xyz3)
                .arg(pluralLineQSL[i+4])//.arg(xyz4)
                .arg(pluralLineQSL[i+5])//.arg(xyz5)
                .arg(pluralLineQSL[i+6]));//.arg(vecter))
    }
    for (int i=0; i<pluralLineQSL.count()/7; i++)
    {
        insert = insert + insertQSL[i];
    }
    insert = insert1 + insert;
    query.exec(insert);
    insertQSL.clear();
}
