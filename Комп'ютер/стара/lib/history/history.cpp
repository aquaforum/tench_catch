#include "history.h"
#include <iodev.h>
#include <trendconstruct.h>
#include "IoNetClient.h"
#include <QUiLoader>

#include <QDebug>
#include <QHBoxLayout>
#include <QList>

#include <QSettings>

#include <QTabWidget>

#include <QPushButton>
#include <QMessageBox>


UHistorySelect::UHistorySelect(QVector<IoNetClient*> &src,struct trendinfo *tp,QWidget *p /*=NULL*/,QString fileName) :
        QDialog(p),
        s(src),
        TrendParam(tp)
{
    initUi(fileName);

}

// цей конструктор залишено для сумісності із старими програма
UHistorySelect::UHistorySelect(IoNetClient  &source,struct trendinfo *tp,QWidget *p,QString fileName) :
    QDialog(p),
    TrendParam(tp)

{
    s << &source ;
    initUi(fileName);
}

void UHistorySelect::initUi(QString fileName)
{
    QUiLoader *puil= new QUiLoader(this);
    QFile file(fileName);
    QWidget *pwgtForm=puil->load(&file); // завантажити файл із мордою

    if(pwgtForm)
    {
        resize(pwgtForm->size()); // підігнати розміри

        QPushButton *p=pwgtForm->findChild<QPushButton*>("Exit");
        connect(p,SIGNAL(clicked()),this,SLOT(reject()));
        // QList<QPushButton*> bnts = pwgtForm->findChildren<QPushButton*> ();
        foreach(QPushButton *p,pwgtForm->findChildren<QPushButton*> ())
        {
            if(p->objectName() != "Exit" && p->objectName().split("_").at(0) != "construct" )
            {
                connect(p,SIGNAL(clicked()),this,SLOT(slotAccept()));
            }

            if(p->objectName().split("_").at(0) == "construct")
                connect(p,SIGNAL(clicked()),this,SLOT(slotConstruct()));
        }


        QHBoxLayout *bxLayout =new QHBoxLayout;
        bxLayout->addWidget(pwgtForm);
        bxLayout->setMargin(0);
        setLayout(bxLayout);
    }
    setWindowTitle(tr("Вибір графіків"));

    QTabWidget *tabW=pwgtForm->findChild<QTabWidget*>("tabW");
    if(tabW)
    {

        QCoreApplication::setApplicationName("history");
        QSettings set;

        tabW->setCurrentIndex(set.value("tabIndex","0").toInt());
        connect(tabW,SIGNAL(currentChanged(int)),this,SLOT(slotStoreTabIndex(int)));

        qDebug() << "uhistory" << tabW->currentIndex();

    }
    else
    {
        qDebug() << "tabW not fount";

    }

    for(int i=0;i<s.size();++i)
    {
        qDebug() << "datasource" << i << s[i]->objectName();

    }

}

UHistorySelect::~UHistorySelect()
{

}

void UHistorySelect::slotAccept()
{
    QStringList  param = sender()->objectName().split('_');
    int NnetDev=0,NioDev=0; // значення за замовчанням
    QString table="trend"; // таблиця за замовчанням
    if(param.size()>3) // ящоє не всі потя то заповнити значеннями за намовчанням
    {
        nameTrend=param[0];
        table=param[1];
        NnetDev = param[2].toInt();
        NioDev=param[3].toInt();
    }
    else
    {
        nameTrend=sender()->objectName();
    }
    // тут треба завантажити дані в структуру 

    QFile f(QString(":/text/%1").arg(nameTrend));
    QString t;
    QStringList sl,sl2;

  if((s[NnetDev])->objectName()=="")
  {
      QMessageBox::critical(this,tr("Проблема"),tr("Невідома назва NetIoDev\nПотрібно встановити параметер objectName"));
      reject();
  }
  else
  {

    QCoreApplication::setApplicationName((s[NnetDev])->objectName());
    QSettings set;

    TrendParam->host=set.value("/db/hostname","localhost").toString();
    TrendParam->db=set.value("/db/dbname","scada").toString();
    TrendParam->user=set.value("/db/username","scada").toString();
    TrendParam->passwd=set.value("/db/passwd","").toString();

    if(f.open(QIODevice::ReadOnly))
    {
        int i;
        sl.clear();

        for(i=0;!f.atEnd() && i<8;++i) // обмежети зчитування із файла кінцем файла або не більше як 8 рядків
	{
            t=    QString::fromUtf8(f.readLine()).trimmed();
            //qDebug() << t;
            sl2=t.split('\t');

            TrendParam->fields[i]=t=sl2[0];
                        //QString::fromUtf8(f.readLine()).trimmed(); // прочитати назву поля
                if((*s[NnetDev])[NioDev]->getTags().contains(t)) // якщо задане поле знайдено
		{
                    QString plotName=(*s[NnetDev])[NioDev]->getText()[t];
                    if(plotName=="-")
                    {
                        plotName=t;
                    }
                    //sl<< /*s.getText()[t].size() > 0 ? */(*s[NnetDev])[NioDev]->getText()[t] /*: t */; // завантажити назву поля, якщо не знайдено - назву тега
                    sl << plotName;
                    //qDebug() << t << plotName;

                    TrendParam->fScale[i][0]=(*s[NnetDev])[NioDev]->scaleZero(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму
                    TrendParam->fScale[i][1]=(*s[NnetDev])[NioDev]->scaleFull(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму

                     if((*s[NnetDev])[NioDev]->fieldType(t)==1) // якщо дискретний сигнал
                    {
			    // змінити тип поля
                            TrendParam->fields[i]=QString("((%1!=0)*454+%2)").arg(t).arg(i*499);
                            // дискретні шкали
			    TrendParam->fScale[i][0]=0.0-1.1*(double)i;
			    TrendParam->fScale[i][1]=8.8-1.1*(double)i;
			}
		}
		else
                {
                     if(sl2.size()>3) // якщо дані є  в файлі конфігурації тоді зчитати їх звідти
                     {
                         TrendParam->fScale[i][0]=sl2[1].toDouble(); // прочитати що там є
                         TrendParam->fScale[i][1]=sl2[2].toDouble(); //
                         sl << sl2[3];
                     }
                     else
                         --i;
                 } // можливо і поганий варіант яле якщо такого поля не знайдено тоді змінити лічильник циклів
//		{--i;} // можливо і поганий варіант яле якщо такого поля не знайдено тоді змінити лічильник циклів
	}

	TrendParam->numPlot=i; // завантажити кількість графіків
	TrendParam->table=table;
	TrendParam->trend=sender()->objectName(); // Завантажити ім’я тренда
	
	TrendParam->trendHead=qobject_cast<QPushButton*>(sender())->text(); // заголовок тренда - те, що написано на кнопці
	TrendParam->fieldHead = sl;
// #define _TRDEBUG_
#ifdef  _TRDEBUG_
	qDebug() << "1 TrendParam->numPlot=" << TrendParam->numPlot;
	qDebug() << "2 TrendParam->table  =" << TrendParam->table;
	qDebug() << "3 TrendParam->trend  =" << TrendParam->trend; // Завантажити ім’я тренда
	for(i=0;i<TrendParam->numPlot;++i)
	    qDebug() << "4 TrendParam->fields[" << i << "]=" << TrendParam->fields[i];
	
	for(i=0;i<TrendParam->numPlot;++i)
	    qDebug() << "5 TrendParam->fScale["<<i<<"] =" << TrendParam->fScale[i][0] << TrendParam->fScale[i][1];
	    
	qDebug() << "6 TrendParam->trendHead=" << TrendParam->trendHead; // заголовок тренда - те, що написано на кнопці
	qDebug() << "7 TrendParam->fieldHead =" << TrendParam->fieldHead;
#endif

	f.close();
	accept(); // для завершення роботи
    }
    else
	reject(); // якщо не вдалося відкрити відповідний файл
  }

}

void UHistorySelect::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    default:
        break;
    }
}

void UHistorySelect::slotStoreTabIndex(int v)
{
    QCoreApplication::setApplicationName("history");
    QSettings set;
    set.setValue("tabIndex",v);

}


#define __CONSTRUCTOR__
#ifdef __CONSTRUCTOR__
void UHistorySelect::slotConstruct()
{
    int NnetDev=0,NioDev=0; // значення за замовчанням
    QString table="trend"; // таблиця за замовчанням

    QStringList param;
    param=sender()->objectName().split("_");


    if(param.size()>3) // ящоє не всі потя то заповнити значеннями за намовчанням
    {
        nameTrend=param[0];
        table=param[1];
        NnetDev = param[2].toInt();
        NioDev=param[3].toInt();
    }


    TrendConstruct p(*s[NnetDev],this);
    p.exec();
    QStringList list=p.tegList();

    if(list.size())
    {
        int i=0;
        QStringList sl;

        foreach(QString t,list)
        {
            qDebug() << t;
                if((*s[NnetDev])[NioDev]->getTags().contains(t)) // якщо задане поле знайдено
                {
                    TrendParam->fields[i]=t;
                    sl<< /*s.getText()[t].size() > 0 ? */(*s[NnetDev])[NioDev]->getText()[t] /*: t */; // завантажити назву поля, якщо не знайдено - назву тега

                    TrendParam->fScale[i][0]=(*s[NnetDev])[NioDev]->scaleZero(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму
                    TrendParam->fScale[i][1]=(*s[NnetDev])[NioDev]->scaleFull(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму

                     if((*s[NnetDev])[NioDev]->fieldType(t)==1) // якщо дискретний сигнал
                    {
                            // змінити тип поля
                            TrendParam->fields[i]=QString("((%1!=0)*454+%2)").arg(t).arg(i*499);
                            // дискретні шкали
                            TrendParam->fScale[i][0]=0.0-1.1*(double)i;
                            TrendParam->fScale[i][1]=8.8-1.1*(double)i;
                        }
                     ++i;
                }
                else
                {--i;} // можливо і поганий варіант яле якщо такого поля не знайдено тоді змінити лічильник циклів
        }

        TrendParam->numPlot=i; // завантажити кількість графіків
        TrendParam->table=table;
        TrendParam->trend="constract"; // Завантажити ім’я тренда

        TrendParam->trendHead=tr("Конструктор"); // заголовок тренда - те, що написано на кнопці
        TrendParam->fieldHead = sl;

        QCoreApplication::setApplicationName((s[NnetDev])->objectName());
        QSettings set;

        TrendParam->host=set.value("/db/hostname","localhost").toString();
        TrendParam->db=set.value("/db/dbname","scada").toString();
        TrendParam->user=set.value("/db/username","scada").toString();
        TrendParam->passwd=set.value("/db/passwd","").toString();


        accept(); // для завершення роботи

    }
    else
    {
        reject();
    }

}

#endif
