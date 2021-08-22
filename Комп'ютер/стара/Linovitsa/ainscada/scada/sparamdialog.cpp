#include "sparamdialog.h"
#include "ui_sparamdialog.h"

#include "IoNetClient.h"
#include "iodev.h"

#include <QSettings>
#include <QMessageBox>
#include <QDebug>

#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))
#define APARAT (*(src[3]))
#define CENTRIF (*(src[4]))

#define GASPICH (*(src[5]))

#define ROU (*(src[6]))

SParamDialog::SParamDialog(QVector<IoNetClient*> &source,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SParamDialog),
    src(source),
    bAccess(true)
{
    m_ui->setupUi(this);

    cbList << m_ui->bx_Teg_0 << m_ui->bx_Teg_1 << m_ui->bx_Teg_2 << m_ui->bx_Teg_3 << m_ui->bx_Teg_5 << m_ui->bx_Teg_6;


    for(int i=0;i<6;++i)
    {
        QStringList tn;
        IoNetClient &s=*(src[ i<4?i:i+1 ]);

        int iX=i!=3?0:3;

        foreach(QString tag,s[iX]->getTags().keys())
        {
            if(s[iX]->isScaleChange(tag))
            {
                tn << tag;
                cbList[i]->addItem( s[iX]->getText()[tag] );
            }
        }
        tag_name << tn;
    }



    connect(m_ui->bn_Ok,SIGNAL(clicked()),this,SLOT(myAccept()));
    connect(m_ui->bx_Teg_0,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));
    connect(m_ui->bx_Teg_1,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));
    connect(m_ui->bx_Teg_2,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));
    connect(m_ui->bx_Teg_3,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));
    connect(m_ui->bx_Teg_5,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));
    connect(m_ui->bx_Teg_6,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));

    connect(m_ui->sb_Fs_0,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs_0,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f_0,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));

    connect(m_ui->sb_Fs_1,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs_1,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f_1,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));

    connect(m_ui->sb_Fs_2,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs_2,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f_2,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));

    connect(m_ui->sb_Fs_3,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs_3,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f_3,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));


    connect(m_ui->sb_Fs_5,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs_5,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f_5,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));


    connect(m_ui->sb_Fs_6,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs_6,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f_6,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));

    connect(m_ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(selectPage(int)));

    //connect(m_ui->sb_m,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));

    m_ui->bx_Teg_0->setCurrentIndex(0);

    selectTeg(0);
{
    QCoreApplication::setApplicationName("difuz");
    QSettings set;
// ініціалізація даних у віджетах на сторінці system
    m_ui->le_ioserv_1->setText(set.value("/ioserv/hostname","localhost").toString());
    m_ui->le_dbserv_1->setText(set.value("/db/hostname","localhost").toString());
    m_ui->le_dbname_1->setText(set.value("/db/dbname","test").toString());
    m_ui->le_username_1->setText(set.value("/db/username","scada").toString());
    m_ui->le_passwd_1->setText(set.value("/db/passwd","").toString());
    m_ui->bxcMode_0->setCurrentIndex(set.value("/ioserv/cmode",true).toBool()?1:0);
}

{
        QCoreApplication::setApplicationName("satur");
        QSettings set;
    m_ui->le_ioserv_2->setText(set.value("/ioserv/hostname","localhost").toString());
    m_ui->le_dbserv_2->setText(set.value("/db/hostname","localhost").toString());
    m_ui->le_dbname_2->setText(set.value("/db/dbname","test").toString());
    m_ui->le_username_2->setText(set.value("/db/username","scada").toString());
    m_ui->le_passwd_2->setText(set.value("/db/passwd","").toString());
    m_ui->bxcMode_1->setCurrentIndex(set.value("/ioserv/cmode",true).toBool()?1:0);
}

{
        QCoreApplication::setApplicationName("viparka");
        QSettings set;
    m_ui->le_ioserv_3->setText(set.value("/ioserv/hostname","localhost").toString());
    m_ui->le_dbserv_3->setText(set.value("/db/hostname","localhost").toString());
    m_ui->le_dbname_3->setText(set.value("/db/dbname","test").toString());
    m_ui->le_username_3->setText(set.value("/db/username","scada").toString());
    m_ui->le_passwd_3->setText(set.value("/db/passwd","").toString());
    m_ui->bxcMode_2->setCurrentIndex(VIPARKA.cmode()?1:0);
}

    {
            QCoreApplication::setApplicationName("aparat");
            QSettings set;
        m_ui->le_ioserv_4->setText(set.value("/ioserv/hostname","localhost").toString());
        m_ui->le_dbserv_4->setText(set.value("/db/hostname","localhost").toString());
        m_ui->le_dbname_4->setText(set.value("/db/dbname","test").toString());
        m_ui->le_username_4->setText(set.value("/db/username","scada").toString());
        m_ui->le_passwd_4->setText(set.value("/db/passwd","").toString());
        m_ui->bxcMode_3->setCurrentIndex(APARAT.cmode()?1:0);
    }

    {
            QCoreApplication::setApplicationName("centrif");
            QSettings set;
        m_ui->le_ioserv_5->setText(set.value("/ioserv/hostname","localhost").toString());
        m_ui->le_dbserv_5->setText(set.value("/db/hostname","localhost").toString());
        m_ui->le_dbname_5->setText(set.value("/db/dbname","test").toString());
        m_ui->le_username_5->setText(set.value("/db/username","scada").toString());
        m_ui->le_passwd_5->setText(set.value("/db/passwd","").toString());
        m_ui->bxcMode_4->setCurrentIndex(CENTRIF.cmode()?1:0);
    }

    {
            QCoreApplication::setApplicationName("gaspich");
            QSettings set;
        m_ui->le_ioserv_6->setText(set.value("/ioserv/hostname","localhost").toString());
        m_ui->le_dbserv_6->setText(set.value("/db/hostname","localhost").toString());
        m_ui->le_dbname_6->setText(set.value("/db/dbname","test").toString());
        m_ui->le_username_6->setText(set.value("/db/username","scada").toString());
        m_ui->le_passwd_6->setText(set.value("/db/passwd","").toString());
        m_ui->bxcMode_5->setCurrentIndex(GASPICH.cmode()?1:0);
    }

    {
            QCoreApplication::setApplicationName("rou");
            QSettings set;
        m_ui->le_ioserv_7->setText(set.value("/ioserv/hostname","localhost").toString());
        m_ui->le_dbserv_7->setText(set.value("/db/hostname","localhost").toString());
        m_ui->le_dbname_7->setText(set.value("/db/dbname","test").toString());
        m_ui->le_username_7->setText(set.value("/db/username","scada").toString());
        m_ui->le_passwd_7->setText(set.value("/db/passwd","").toString());
        m_ui->bxcMode_6->setCurrentIndex(ROU.cmode()?1:0);
    }

#ifndef WIN32
    // перевірка наявності файлу розмежування доступу
    QFile fAccess("/usr/etc/ac.list");
    qDebug() << "Access control enabled";
    if(fAccess.exists())
    {
        bAccess=false;
        QDir dlist("/dev/disk/by-uuid");
        QStringList list=dlist.entryList();
        list.removeAt(0); // видалити два перших елементи
        list.removeAt(0);
        fAccess.open(QIODevice::ReadOnly);
        for(;!fAccess.atEnd();)
        {
            QString key=fAccess.readLine().trimmed();
            if(list.contains(key)) bAccess=true;
        }
        if(bAccess)
        {
            QMessageBox::information(this,tr("Повідомлення"),tr("Ключ доступу знайдено.\nЗміну параметрів дозволено"));
        }

    }
#endif

    connect(m_ui->bnOpen,SIGNAL(clicked()),this,SLOT(slotOpen()));
    connect(m_ui->bnSave,SIGNAL(clicked()),this,SLOT(slotSave()));

}

SParamDialog::~SParamDialog()
{
    delete m_ui;
}

void SParamDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void SParamDialog::myAccept()
{

    if(bAccess)
    {
        DIFUZ[0]->sendValue("Save",qint16(-1));
        DIFUZ[0]->sendValue("Run",qint16(0));

        SATUR[0]->sendValue("Save",qint16(-1));
        SATUR[0]->sendValue("Run",qint16(0));

        VIPARKA[0]->sendValue("Save",qint16(-1));
        VIPARKA[0]->sendValue("Run",qint16(0));

        ROU[0]->sendValue("Save",qint16(-1));
        ROU[0]->sendValue("Run",qint16(0));

    {
        QCoreApplication::setApplicationName("difuz");
        QSettings set;
        set.setValue("/ioserv/hostname",m_ui->le_ioserv_1->text());
        set.setValue("/db/hostname",m_ui->le_dbserv_1->text());
        set.setValue("/db/dbname",m_ui->le_dbname_1->text());
        set.setValue("/db/username",m_ui->le_username_1->text());
        set.setValue("/db/passwd",m_ui->le_passwd_1->text());
        set.setValue("/ioserv/cmode",m_ui->bxcMode_0->currentIndex()==1);
        DIFUZ.setCmode(m_ui->bxcMode_0->currentIndex()==1);

    }

    {
        QCoreApplication::setApplicationName("satur");
        QSettings set;
        set.setValue("/ioserv/hostname",m_ui->le_ioserv_2->text());
        set.setValue("/db/hostname",m_ui->le_dbserv_2->text());
        set.setValue("/db/dbname",m_ui->le_dbname_2->text());
        set.setValue("/db/username",m_ui->le_username_2->text());
        set.setValue("/db/passwd",m_ui->le_passwd_2->text());
        set.setValue("/ioserv/cmode",m_ui->bxcMode_1->currentIndex()==1);
        SATUR.setCmode(m_ui->bxcMode_1->currentIndex()==1);

    }
    {
        QCoreApplication::setApplicationName("viparka");
        QSettings set;
        set.setValue("/ioserv/hostname",m_ui->le_ioserv_3->text());
        set.setValue("/db/hostname",m_ui->le_dbserv_3->text());
        set.setValue("/db/dbname",m_ui->le_dbname_3->text());
        set.setValue("/db/username",m_ui->le_username_3->text());
        set.setValue("/db/passwd",m_ui->le_passwd_3->text());
        set.setValue("/ioserv/cmode",m_ui->bxcMode_2->currentIndex()==1);
        VIPARKA.setCmode(m_ui->bxcMode_2->currentIndex()==1);

    }
        {
            QCoreApplication::setApplicationName("aparat");
            QSettings set;
            set.setValue("/ioserv/hostname",m_ui->le_ioserv_4->text());
            set.setValue("/db/hostname",m_ui->le_dbserv_4->text());
            set.setValue("/db/dbname",m_ui->le_dbname_4->text());
            set.setValue("/db/username",m_ui->le_username_4->text());
            set.setValue("/db/passwd",m_ui->le_passwd_4->text());
            set.setValue("/ioserv/cmode",m_ui->bxcMode_3->currentIndex()==1);
            APARAT.setCmode(m_ui->bxcMode_3->currentIndex()==1);

        }
        {
            QCoreApplication::setApplicationName("centrif");
            QSettings set;
            set.setValue("/ioserv/hostname",m_ui->le_ioserv_5->text());
            set.setValue("/db/hostname",m_ui->le_dbserv_5->text());
            set.setValue("/db/dbname",m_ui->le_dbname_5->text());
            set.setValue("/db/username",m_ui->le_username_5->text());
            set.setValue("/db/passwd",m_ui->le_passwd_5->text());
            set.setValue("/ioserv/cmode",m_ui->bxcMode_4->currentIndex()==1);
            CENTRIF.setCmode(m_ui->bxcMode_4->currentIndex()==1);

        }
        {
            QCoreApplication::setApplicationName("gaspich");
            QSettings set;
            set.setValue("/ioserv/hostname",m_ui->le_ioserv_6->text());
            set.setValue("/db/hostname",m_ui->le_dbserv_6->text());
            set.setValue("/db/dbname",m_ui->le_dbname_6->text());
            set.setValue("/db/username",m_ui->le_username_6->text());
            set.setValue("/db/passwd",m_ui->le_passwd_6->text());
            set.setValue("/ioserv/cmode",m_ui->bxcMode_5->currentIndex()==1);
            GASPICH.setCmode(m_ui->bxcMode_5->currentIndex()==1);

        }

        {
            QCoreApplication::setApplicationName("rou");
            QSettings set;
            set.setValue("/ioserv/hostname",m_ui->le_ioserv_7->text());
            set.setValue("/db/hostname",m_ui->le_dbserv_7->text());
            set.setValue("/db/dbname",m_ui->le_dbname_7->text());
            set.setValue("/db/username",m_ui->le_username_7->text());
            set.setValue("/db/passwd",m_ui->le_passwd_7->text());
            set.setValue("/ioserv/cmode",m_ui->bxcMode_6->currentIndex()==1);
            ROU.setCmode(m_ui->bxcMode_6->currentIndex()==1);

        }}
    else
    {
        QMessageBox::information(this,tr("Попередження"),tr("Ключ доступу не був зайдений\nВсі зміни проігноговано!"));
    }

    accept();
}



void SParamDialog::slotSet(QString v )
{
    QLocale loc;

    if(bAccess)
    {
        int i= m_ui->tabWidget->currentIndex();
        IoNetClient &s=*(src[i<4?i:i+1]);

        int iX=sender()->objectName().right(1).toInt()!=4?0:3;

        QString name= sender()->objectName().left(sender()->objectName().size()-2);

        //qDebug() << "-----------------i" << i << "name" << name ;

        if(name=="sb_Zs")
            s[iX]->setScaleZero(tag_name[i][cbList[i]->currentIndex()],loc.toDouble(v));
        else if(name=="sb_Fs")
        {
            s[iX]->setScaleFull(tag_name[i][cbList[i]->currentIndex()],loc.toDouble(v));
            /*
            QMessageBox::warning(this,tr("setScaleFull"),QString("s[%1]->setScaleFull(%2,%3)")
                                 .arg(s.objectName())
                                 .arg(tag_name[i][cbList[i]->currentIndex()])
                                 .arg(loc.toDouble(v),0,'f')
                                 );*/
        }
        else if(name=="sb_f")
            s[iX]->sendValue(QString("f_V_%1").arg(tag_name[i][cbList[i]->currentIndex()].right(2)),qint16(v.toInt()));

    }

}



void SParamDialog::selectTeg(int v)
{

    int i= m_ui->tabWidget->currentIndex();

    qDebug() << " -------------------------------- selectTag" << i;

    if(i<6)
    {
    IoNetClient &s=*(src[i]);

    if(i==0)
    {
        if(v<m_ui->bx_Teg_0->count())
        {
            m_ui->sb_Zs_0->blockSignals(true);
            m_ui->sb_Zs_0->setValue(s[0]->scaleZero(tag_name[i][v]));
            m_ui->sb_Zs_0->blockSignals(false);

            m_ui->sb_Fs_0->blockSignals(true);
            m_ui->sb_Fs_0->setValue(s[0]->scaleFull(tag_name[i][v]));
            m_ui->sb_Fs_0->blockSignals(false);

            m_ui->sb_f_0->blockSignals(true);
            m_ui->sb_f_0->setValue(s[0]->getValue16(QString("f_V_%1").arg(tag_name[i][v].right(2))));
            m_ui->sb_f_0->blockSignals(false);

        }
    }
    else if( i==1)
    {
        if(v<m_ui->bx_Teg_1->count())
        {
            m_ui->sb_Zs_1->blockSignals(true);
            m_ui->sb_Zs_1->setValue(s[0]->scaleZero(tag_name[i][v]));
            m_ui->sb_Zs_1->blockSignals(false);

            m_ui->sb_Fs_1->blockSignals(true);
            m_ui->sb_Fs_1->setValue(s[0]->scaleFull(tag_name[i][v]));
            m_ui->sb_Fs_1->blockSignals(false);

            m_ui->sb_f_1->blockSignals(true);
            m_ui->sb_f_1->setValue(s[0]->getValue16(QString("f_V_%1").arg(tag_name[i][v].right(2))));
            m_ui->sb_f_1->blockSignals(false);
        }
    }
    else if( i==2)
    {
        if(v<m_ui->bx_Teg_2->count())
        {
            m_ui->sb_Zs_2->blockSignals(true);
            m_ui->sb_Zs_2->setValue(s[0]->scaleZero(tag_name[i][v]));
            m_ui->sb_Zs_2->blockSignals(false);

            m_ui->sb_Fs_2->blockSignals(true);
            m_ui->sb_Fs_2->setValue(s[0]->scaleFull(tag_name[i][v]));
            m_ui->sb_Fs_2->blockSignals(false);

            m_ui->sb_f_2->blockSignals(true);
            m_ui->sb_f_2->setValue(s[0]->getValue16(QString("f_V_%1").arg(tag_name[i][v].right(2))));
            m_ui->sb_f_2->blockSignals(false);

        }
    }

    else if( i==3) // PVS
    {
        if(v<m_ui->bx_Teg_3->count())
        {
            m_ui->sb_Zs_3->blockSignals(true);
            m_ui->sb_Zs_3->setValue(s[3]->scaleZero(tag_name[i][v]));
            m_ui->sb_Zs_3->blockSignals(false);

            m_ui->sb_Fs_3->blockSignals(true);
            m_ui->sb_Fs_3->setValue(s[3]->scaleFull(tag_name[i][v]));
            m_ui->sb_Fs_3->blockSignals(false);

            m_ui->sb_f_3->blockSignals(true);
            m_ui->sb_f_3->setValue(s[3]->getValue16(QString("f_V_%1").arg(tag_name[i][v].right(2))));
            m_ui->sb_f_3->blockSignals(false);

        }
    }
    else if( i==4)
    {

        if(v<m_ui->bx_Teg_5->count())
        {
            m_ui->sb_Zs_5->blockSignals(true);
            m_ui->sb_Zs_5->setValue(GASPICH[0]->scaleZero(tag_name[i][v]));
            m_ui->sb_Zs_5->blockSignals(false);

            m_ui->sb_Fs_5->blockSignals(true);
            m_ui->sb_Fs_5->setValue(GASPICH[0]->scaleFull(tag_name[i][v]));
            m_ui->sb_Fs_5->blockSignals(false);

            m_ui->sb_f_5->blockSignals(true);
            m_ui->sb_f_5->setValue(GASPICH[0]->getValue16(QString("fV%1").arg(tag_name[i][v].right(2))));
            m_ui->sb_f_5->blockSignals(false);

        }
    }

    else if( i==5)
    {

        if(v<m_ui->bx_Teg_6->count())
        {
            m_ui->sb_Zs_6->blockSignals(true);
            m_ui->sb_Zs_6->setValue(ROU[0]->scaleZero(tag_name[i][v]));
            m_ui->sb_Zs_6->blockSignals(false);

            m_ui->sb_Fs_6->blockSignals(true);
            m_ui->sb_Fs_6->setValue(ROU[0]->scaleFull(tag_name[i][v]));
            m_ui->sb_Fs_6->blockSignals(false);

            m_ui->sb_f_6->blockSignals(true);
            m_ui->sb_f_6->setValue(ROU[0]->getValue16(QString("fV00%1").arg(tag_name[i][v].right(2))));
            m_ui->sb_f_6->blockSignals(false);

        }
    }

    //m_ui->sb_m->setValue((double)s.getValue16(QString("m_%1").arg(tag_name[v]))/4000.0*(s.scaleFull(tag_name[v])-s.scaleZero(tag_name[v]))+s.scaleZero(tag_name[v]));
    }
}

void SParamDialog::selectPage(int v)
{
    if(v<6)
    {
        cbList[v]->setCurrentIndex(0);
        selectTeg(0);
    }
}

void SParamDialog::slotSave()
{
    int iX= m_ui->tabWidget_2->currentIndex();
    QStringList names;
    names << "difuz"
                      << "satur"
                      << "vipar"
                      << "gaspich"
                      << "aparat"
                      << "centrif"
                      << "rou";

    QString fileName=QString("%1/%2-%3.ini").arg(QDir::homePath())
            .arg(names[iX])
            .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));

    QSettings saveSet(fileName,QSettings::NativeFormat);

    IoNetClient &s=*(src[iX]);

    foreach(QString tag,s[0]->getTags().keys())
    {
        switch(s[0]->fieldType(tag)) // тип даних
            {
                default:
                case 0: // Integer
                case 1: // Bool
                    saveSet.setValue(tag,s[0]->getValue16(tag));
                    break;
                case 2: // Real
                    saveSet.setValue(tag,s[0]->getValueFloat(tag));
                    break;
                case 3: // Timer
                case 4: // Long
                    saveSet.setValue(tag,s[0]->getValue32(tag));
                    break;
            }

    }


    QMessageBox::information(this,tr("Збереження параметрів"),QString(tr("Параметри збережено у файл\n%1")).arg(fileName));


}

void SParamDialog::slotOpen()
{
    /*
    QString filter=QString("Ini (ap%1*.ini)").arg(windowTitle().split(' ').at(1));
    QString fileName=QFileDialog::getOpenFileName(

                this,tr("Виберіть файл"),QDir::homePath(),filter);
    if(QFile::exists(fileName))
    {
        QSettings openSet(fileName,QSettings::NativeFormat);

        foreach (QDoubleSpinBox *p, findChildren<QDoubleSpinBox*>()) {
            //saveSet.setValue(p->objectName(),p->value());
            p->setValue(openSet.value(p->objectName(),-1.0).toDouble());
        }

        foreach (QSpinBox *p, findChildren<QSpinBox*>()) {
            p->setValue(openSet.value(p->objectName(),-1).toInt());
        }

        foreach (QCheckBox *p, findChildren<QCheckBox*>()) {
            p->setChecked(openSet.value(p->objectName(),false).toBool());
        }
        ui->sensorMode->setCurrentIndex(openSet.value("SensorMode",0).toInt());

        QMessageBox::information(this,tr("Збереження параметрів"),QString(tr("Параметри завантажено з файлу\n%1")).arg(fileName));

    }
*/

}


