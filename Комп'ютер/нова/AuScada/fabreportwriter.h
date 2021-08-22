#ifndef FABREPORTWRITER_H
#define FABREPORTWRITER_H

#include <QObject>
#include <QVector>

class AuBase;

class FabReportWriter: public QObject
{
    Q_OBJECT
public:
    FabReportWriter(AuBase &base, QObject *parent=nullptr);
    ~FabReportWriter();

private slots:
    //void exit();
    void sqlResult(qint32,const QList<QStringList>&);
    void slotSendQry();
    void writeReport();

    void slotClock();

signals:
    void finished();

private:
    AuBase &src;
    int n;

    // тут треба описати всі змінні які будуть містити дані для збереження в базу даних звітів
    QVector<double> v; // в цей масив будемо записувати результати.

    qint32 stateIx; // іднекс поточного запиту
    QVector<qint32> qryIx; // всі запити відправляємо пачкою.

    // в цю структурі збиратимутся дані перед записом в таблицю звітів
    union {
    // дістаємо з баз даних сумарні значення  з 7.45 по 7.45
    // дифузія
        struct {
            double SGstr; // стружка по стрічкових вагах
            double SVvoda; // витрата води
            double SVds; // видтрада дифузійного соку

            //для розрахунку співвідношення стружка-відкачка треба з бази даних дузії дістати сумарну відкачку див. соку за період з 7.00  до 7.00
            double S7Vds;

            //сатурація
            double SVvm; // сумарна витрата вапняного молока

            //випарка
            double SVsvp; // витрата соку на випарку
            double SVsfv; // витрата сиропу з випарки

            //РОУ
            double GRou; // витрапари пари на роу
            double GT1; // витрата пари на Т1
            double GT2; // витрата пари на Т2

            //газова, таблиця звітів
            double SGcl; // витрата вугілля на газову
            double SGst; // витрата каменю на газову

            //фільтр-преси, таблиця звітів
            // кількість циклів фільтр-пресів
            double KFn;

            //контролер випарки
            double SVvfd; // витрата води на дискові фільтри
            double SVvtv; // витрата води на випарку
            double SVvva; // витрата води на вакуум-апараи

            //контролер дифузії
            // почасова витрата стружки із бункерних ваг
            //зовнішня програма дістає html файл із сервера, а ця його парсить і записує в звіт.

            // старий параметр
            // SGutf // витрата утфелю, зараз не дійсний оскільки центрифуги не працюють

        } V ;
        double value[16];
    } reportData;

    union
    {
        struct
        {
            //із сайта ізодрому
            // почасова витрата газу
            double H11, H00, H01, H02, H03, H04, H05, H06, H07, H08, H09, H10;

        } V;
        double value[12];
    } gasRep;

    union
    {
        struct
        {
            double W11, W00, W01, W02, W03, W04, W05, W06, W07, W08, W09, W10;

        } V;
        double value[12];
    } massRep;

    time_t start,stop;
    time_t dstart,dstop;

    QStringList tagsList; // пачка SQL запитів до розних баз даних
    int state;


};

#endif // FABREPORT_H
