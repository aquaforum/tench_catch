#include <aulibdefs.h>

#include <aubase.h>
#include "auhistory.h"
#include <QUiLoader>
#include <QFile>

#include <QPushButton>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QDir>
#include <QCoreApplication>

#include <QDebug>
#include <QRegExp>

#include <QSettings>

AuHistory::AuHistory(AuBase &base, QWidget *parent, QString uiHistory,QString fileTrendLists) : QDialog(parent)
  ,src(base)
  ,trendLists(fileTrendLists)
{
    QUiLoader *ui=new QUiLoader(this);
    QFile uiFile(uiHistory);
    QWidget *uiWidget=ui->load(&uiFile);

    resize(uiWidget->size());

    QPushButton *p=uiWidget->findChild<QPushButton*>("bnExit");
    connect(p,&QPushButton::clicked,this,&AuHistory::reject);

    // QList<QPushButton*> bnts = pwgtForm->findChildren<QPushButton*> ();
    foreach(QPushButton *p,uiWidget->findChildren<QPushButton*> ())
    {
        if(p->objectName() != "bnExit" && p->objectName().split("_").at(0) != "construct" )
        {
            connect(p,SIGNAL(clicked()),this,SLOT(slotAccept()));
        }

        if(p->objectName().split("_").at(0) == "construct")
            connect(p,SIGNAL(clicked()),this,SLOT(slotConstruct()));
    }
    setWindowTitle(tr("Вибір графіків"));


    QHBoxLayout *bxLayout =new QHBoxLayout;
    bxLayout->addWidget(uiWidget);
    bxLayout->setMargin(0);
    setLayout(bxLayout);

    QTabWidget *tabW=uiWidget->findChild<QTabWidget*>("tabW");
    if(tabW)
    {
        QSettings set;
        tabW->setCurrentIndex(set.value("tabIndex","0").toInt());
        connect(tabW,&QTabWidget::currentChanged,this,&AuHistory::slotStoreTabIndex);
    }

}

AuHistory::~AuHistory()
{

}

void AuHistory::slotAccept()
{
    myTrendFields.clear();
    // першим ділом треба знайти останній файл
    /*
     * ім'я файлу зберігається в форматі trends_yyyyMMddhhmmss.list
     * yyyy - рік
     * MM - місяць
     * dd - дата
     * hh - година
     * mm - хвилина
     * ss - секунда
     * відповідно чим новише файл тим це значення більше,
     * нас цікавить самий новий файл
     */
    QDir confDir(QDir::homePath().append(QStringLiteral("/.config/")).append(qApp->organizationName())); // тут вони зберігаються, треба це якось адаптувати для віндовс... мабуть....
    ulong lastData=0;
    QString lastName;
    QStringList  filter;
    filter <<  "trends_*.list";
    foreach(QString fName,confDir.entryList(filter)) // шукаємо самий новий файл
    {
        QString strDate=fName.split('.')[0];
        if(strDate.size()>0)
        {
            ulong curData=strDate.split('_')[1].toULong();
            if(curData>lastData)
            {
                lastData=curData;
                lastName=fName;
            }
        }
    }

    // qDebug() << lastData << lastName << confDir.path().append('/').append(lastName) ;
    loadData(lastName.size()>0?confDir.path().append('/').append(lastName):trendLists);
    if(myTrendFields.size()==0) // а якщо там нічого не знайшли
        loadData(trendLists); // спробувати завантажити з ресурсів

    myText=qobject_cast<QPushButton*>(sender())->text();

    if(myTrendFields.size()==0) // якщо і після цього нічого немає
        reject();
    else // інакше
        accept();
}

void AuHistory::slotConstruct()
{
    accept();
}


// функція завантажує дані з файла якщо вони там є
void AuHistory::loadData(QString fileName)
{
    QFile fTrendsList(fileName);
    if(fTrendsList.open(QIODevice::ReadOnly)) // для повного щастя треба перевірити чи існує файл ко
    {
        while(!fTrendsList.atEnd()) // перебрати всі рядки
        {
            QString line=fTrendsList.readLine().trimmed(); // прочитати і обрізати по кінці
            if(line.size()>0) // якщо там щось є
            {
                QStringList record=line.split('='); // розділити на ключ=значення
                if(record.size()>1) // якщо є мінімум два варіанти
                {
                    if(record[0].trimmed()==sender()->objectName()) //  якщо ключ співпав з назвою кнопки
                        myTrendFields << record[1].trimmed(); // зберегти
                }
            }
        }

        fTrendsList.close();
    }
    else
        qDebug() << "not file open " << fileName;
}

void AuHistory::slotStoreTabIndex(int v)
{
    QSettings set;
    set.setValue("tabIndex",v);
}
