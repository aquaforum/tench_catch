#include "aulibdefs.h"
#include "aubase.h"
#include <auplc.h>
#include "autag.h"
#include "autrendview.h"
#include "ui_autrendview.h"
#include "datetimedialog.h"
#include "AuTrendPlot.h"

#include <QTimer>

#include <QSettings>
#include <QColor>
#include <QDebug>
#include <QColorDialog>

//#define BOOL_OFFSET 500
//#define BOOL_HI 480

// УВАГА !!!  цей клас перераховує теги із їхньої бази в 0-4000

namespace MyConst
{
    const int tmr[]={300,900,1800,3600,7200,14400,21600,43200};
    // const unsigned int LvCor = 2049136799;
    const Qt::GlobalColor DefColor[8]={Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::white,Qt::darkRed};
}


AuTrendView::AuTrendView(AuBase &base, QStringList &trendFields,QStringList &weidthK, QWidget *parent):
    QWidget(parent),
    ui(new Ui::AuTrendView),
    src(base),
    BOOL_OFFSET(500),
    BOOL_HI(450)
{
    qDebug() << "weidthK" << weidthK;
    wK.clear();

    for(int i=0;i<weidthK.size();++i)
    {
        QVector<double> twK;
        twK.clear();
        QStringList line=weidthK[i].split(',');
        for(int k=0;k<line.size();++k)
            twK<< line[k].toDouble();
        wK << twK;
    }
    init(trendFields);
}

AuTrendView::AuTrendView(AuBase &base, QStringList &trendFields, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuTrendView),
    src(base),
    BOOL_OFFSET(500),
    BOOL_HI(450)
{
    wK.clear();

    for(int i=0;i<trendFields.size();++i)
    {
        QVector<double> twK;
        twK.clear();
        QStringList line=trendFields[i].split(',');
        for(int k=1;k<line.size();++k)
            twK<< 1.0;
        wK << twK;
    }

    init(trendFields);
}

void AuTrendView::init(QStringList &trendFields)
{
    ui->setupUi(this);
    connect(ui->Exit,&QPushButton::clicked,this,&AuTrendView::slotExit);


    myTrendPlot = new TrendPlot(ui->trend);

    QVBoxLayout *vbl=new QVBoxLayout(ui->trend); // менеджер розміщення для об'єкту TrendView
    vbl->setContentsMargins(2,2,2,2);
    vbl->addWidget(myTrendPlot);
    ui->trend->setLayout(vbl);
    connect(myTrendPlot,&TrendPlot::cursorMoved,this,&AuTrendView::slotCursor);
    connect(myTrendPlot,&TrendPlot::_redraw,this,&AuTrendView::dataChange);
    myTrendPlot->setGrid(ui->gridEnable->isChecked());
    connect(ui->gridEnable,QOverload<bool>::of(&QCheckBox::clicked),this,&AuTrendView::setGrid);

    //фарбую кнопки
    QSettings s;
    QStringList fieldHead;

    // розібрати що там робити
    int fieldCount=0;
    foreach (QString line, trendFields) {
        plcS << line.split(',')[0].trimmed(); // витягнути назву контролера
        QStringList tag1;
        for(int i=1;i<line.split(',').size();++i)
            tag1 << line.split(',')[i].trimmed();
        tagS << tag1;
        fieldCount+=tag1.size();
    }
    //qDebug() << plcS;
    //qDebug() << tagS << fieldCount;

    pv << ui->pv_0
    << ui->pv_1
    << ui->pv_2
    << ui->pv_3
    << ui->pv_4
    << ui->pv_5
    << ui->pv_6
    << ui->pv_7;

    ui->ps_0->setChecked(true);
    ps<<ui->ps_0
    << ui->ps_1
    << ui->ps_2
    << ui->ps_3
    << ui->ps_4
    << ui->ps_5
    << ui->ps_6
    << ui->ps_7;

    pc << ui->pc_0
       << ui->pc_1
       << ui->pc_2
       << ui->pc_3
       << ui->pc_4
       << ui->pc_5
       << ui->pc_6
       << ui->pc_7;

    if(fieldCount>8)
    {
        ui->scrollAreaWidgetContents->setGeometry(0,0,730,20*fieldCount);
        BOOL_OFFSET=4000/fieldCount;
        BOOL_HI=BOOL_OFFSET*80/100;

        for(int i=8;i<fieldCount;++i)
        {
            QCheckBox *pvAdd= new QCheckBox(ui->selectedBox);
            pvAdd->setObjectName(QStringLiteral("pv_%1").arg(i));
            //ui->sizePolicy6.setHeightForWidth(pv->sizePolicy().hasHeightForWidth());
            pvAdd->setMinimumSize(QSize(0, 20));
            ui->_3->insertWidget(i,pvAdd);
            pvAdd->setChecked(true);
            pv << pvAdd;

            QRadioButton *psAdd = new QRadioButton(ui->plotSelect);
            psAdd->setObjectName(QStringLiteral("ps_%1").arg(i));
            //sizePolicy6.setHeightForWidth(ps_7->sizePolicy().hasHeightForWidth());
            // ps_7->setSizePolicy(sizePolicy6);
            psAdd->setMinimumSize(QSize(0, 20));
            ui->_2->insertWidget(i,psAdd);
            ps << psAdd;

            QPushButton  *pcAdd = new QPushButton(ui->colorBox);
            pcAdd->setObjectName(QStringLiteral("pc_%1").arg(i));
            pcAdd->setMaximumSize(QSize(16777215, 18));

            ui->_4->insertWidget(i,pcAdd);
            pc << pcAdd;

            // m_Color << m_Color[i-8]; // просто дублювати кольори по кругу

        }

    }

    int j=0;
    for(int i=0;i<plcS.size();++i)
    {
        QString plcName=plcS[i];
        foreach(QString tagName,tagS[i])
        {
            QColor  color = s.value(QStringLiteral("colors/%1/%2")
                               .arg(plcName)
                               .arg(tagName),
                               QColor(MyConst::DefColor[j%8])).value<QColor>(); // цю ініціалізацію треба переробити

            if(color.isValid())
                m_Color << color;
            else
                m_Color << MyConst::DefColor[j%8];

            //qDebug() << plcName << tagName;

            SAFE(fieldHead << (src[plcName][tagName].text().size()==0?tagName:src[plcName][tagName].text()));

            pv[j++]->setChecked(s.value(QStringLiteral("show/%1/%2")
                                  .arg(plcName)
                                  .arg(tagName),true).toBool());
        }

    }




    // добавити масив кольорів до 8
    for(int i=m_Color.size();i<8;++i)
    {
        m_Color << MyConst::DefColor[i];
    }
    //qDebug() << m_Color.size();

    myTrendPlot->setColors(m_Color);

    {
        QPalette pal;
        pal.setColor(QPalette::Highlight,m_Color[0]);
        ui->cursorVal->setPalette(pal);
    }

    myData.fill(QByteArray(),plcS.size());
    colS.fill(0,plcS.size());
    rowS.fill(0,plcS.size());


    // таблиця параметрів внизу вікна
    ui->twAgr->setColumnCount(4);
    ui->twAgr->setRowCount(fieldCount>8?fieldCount:8);
    ui->twAgr->setVerticalHeaderLabels(fieldHead);
    ui->twAgr->horizontalHeader()->hide();
    ui->twAgr->verticalHeader()->setFixedWidth(300);
    ui->twAgr->setSortingEnabled(false);

    for(int i=0;i<fieldCount;++i)
        ui->twAgr->setRowHeight(i,20);
    for(int i=0;i<4;++i)
        ui->twAgr->setColumnWidth(i,75);

    QTableWidgetItem *_item;
    for(int i=0;i<fieldCount;++i)
    {
        for(int j=0;j<4;++j)
        {
            _item= new QTableWidgetItem;
            _item->setText("0.00");
                ui->twAgr->setItem(i,j,_item);
        }
    }



    // кнопки вибору кольору
    QPalette pal;
    int n=0;
    foreach(QPushButton *p,pc)
    {
        pal.setColor(QPalette::Button,m_Color[n]);
        p->setPalette(pal);
        connect(p,&QPushButton::clicked,this,&AuTrendView::colorChange);
        ++n;
    }

    // перемикач вибору поля курсоку
    foreach(QRadioButton *p,ps)
    {
        connect(p,&QRadioButton::clicked,this,&AuTrendView::plotChange);
    }

    // чекбокси вмикання графіку
    foreach(QCheckBox *p,pv)
    {
        connect(p,&QCheckBox::clicked,this,&AuTrendView::dataChange);
    }


    // навігаційні кнопки
    connect(ui->forward,&QPushButton::clicked,this,&AuTrendView::dataChange);
    connect(ui->backward,&QPushButton::clicked,this,&AuTrendView::dataChange);
    connect(ui->forwardHalf,&QPushButton::clicked,this,&AuTrendView::dataChange);
    connect(ui->backwardHalf,&QPushButton::clicked,this,&AuTrendView::dataChange);
    connect(ui->calendarButton,&QPushButton::clicked,this,&AuTrendView::dataChange);
    connect(ui->last,&QPushButton::clicked,this,&AuTrendView::dataChange);
    connect(ui->Interval,QOverload<int>::of(&QComboBox::activated),this,&AuTrendView::dataChange);

    // заблкувати сигнали поки не відпрацює перша послідовність завантаження даних
 /*
    ui->forward->blockSignals(true);
    ui->backward->blockSignals(true);
    ui->forwardHalf->blockSignals(true);
    ui->backwardHalf->blockSignals(true);
    ui->calendarButton->blockSignals(true);
    ui->last->blockSignals(true);
    ui->Interval->blockSignals(true);
*/
    // приймати дані історії
    connect(&src,&AuBase::htrData,this,&AuTrendView::slotHtrData);

    timeStop=QDateTime::currentDateTime();
    // QTimer::singleShot(0,this,&AuTrendView::dataChange);
    // connect(myTrendPlot,&TrendPlot::_redraw,this,&AuTrendView::dataChange);

    connect(&src,&AuBase::sqlData,this,&AuTrendView::slotSetMinMax);
    QString table=trendFields[0].split(',')[0].trimmed();
    mmIx=src.querySql(table,QStringLiteral("SELECT count(*),MIN(Dt),MAX(Dt) FROM %1").arg(table));


}

AuTrendView::~AuTrendView()
{
    int pvC=0;
    QSettings s;
    for(int i=0;i<plcS.size();++i)
    {
        for(int j=0;j<tagS[i].size();++j)
        {
            if( pv[pvC++]->isChecked())
            {
                s.remove(QStringLiteral("show/%1/%2")
                         .arg(plcS.at(i))
                         .arg(tagS.at(i).at(j)));
            }
            else
            {
                s.setValue(QStringLiteral("show/%1/%2")
                           .arg(plcS[i])
                           .arg(tagS[i][j]),false);
            }
        }

    }


    delete ui;
}

void AuTrendView::slotSetMinMax(qint32 ix,const QList<QStringList>& res)
{
    // qDebug() << "setMinMax " << res;

    if(ix==mmIx && res.size()>0)
    {
        // тут би перевірити що там за дані
        if(res[0][0].toInt()>0) // якщо там є дані
        {
            QDateTime min,max;
            min=QDateTime::fromTime_t(res[0][1].toUInt());
            max=QDateTime::fromTime_t(res[0][2].toUInt());
            ui->db_startDate->setText(min.toString(QStringLiteral("dd/MM/yyyy\nhh:mm:ss")));
            ui->db_stopDate->setText(max.toString(QStringLiteral("dd/MM/yyyy\nhh:mm:ss")));
        }
    }
}

void AuTrendView::setText(const QString &text)
{
    ui->trendHead->setText(text);
}

void AuTrendView::slotExit()
{
    emit finished();
}


void AuTrendView::dataChange()
{
         //disconnect(m_tw,SIGNAL(cursorMoved(int)),this,SLOT(setCursor(int)));

        //QString s="";
        //int i ;
    if(sender()!=nullptr)
    {
        QString sndr=sender()->objectName();
        if(sndr=="last")
        {
            timeStop=QDateTime::currentDateTime();
        }
        else if(sndr=="forward")
        {
            timeStop=timeStop.addSecs(MyConst::tmr[ui->Interval->currentIndex()]);
        }
        else if(sndr=="forwardHalf")
        {
            timeStop=timeStop.addSecs(MyConst::tmr[ui->Interval->currentIndex()]/2);
        }
        else if(sndr=="backwardHalf")
        {
            timeStop=timeStop.addSecs(-MyConst::tmr[ui->Interval->currentIndex()]/2);
        }
        else if(sndr=="backward")
        {
            timeStop=timeStop.addSecs(-MyConst::tmr[ui->Interval->currentIndex()]);
        }
        else if(sndr=="calendarButton")
        {
            myDateTimeDialog dt(this,timeStop);
            if(dt.exec()==QDialog::Accepted)
            {
                timeStop.setDate(dt.date->selectedDate());
                timeStop.setTime(QTime(dt.hour->value(),dt.minute->value(),dt.second->value()));
            }
        }

    }
        // початок виконання запиту та малювання графіку
        // QApplication::setOverrideCursor(Qt::WaitCursor);

        timeStart=timeStop.addSecs(-MyConst::tmr[ui->Interval->currentIndex()]);

        ui->startDate->setText(timeStart.toString("hh:mm:ss\ndd:MM:yy"));
        ui->stopDate->setText(timeStop.toString("hh:mm:ss\ndd:MM:yy"));

        // відправити запити на дані історії
        iDx.clear();
        int pvCheck=0;

        // сформувати список тегів.
        for(int i=0;i<plcS.size();++i)
        {
            QStringList qTag=tagS[i];
            for(int j=0;j<qTag.size();++j)
            {
                if(! pv[pvCheck++]->isChecked())
                {
                    qTag[j]="0";
                }
                else // перешкалювання тега в 0-4000
                {
                    SAFE(
                    double tagBase=src[plcS[i]][qTag[j]].baseScale();
                    if(tagBase!=4000.0) // якщо база не рівна 4000.0
                        qTag[j]= QStringLiteral("PRINTF(\"%.0f\",%1*%2)")
                                .arg(qTag[j])
                                .arg(4000.0/tagBase,0,'f');
                    )
                }

            }

            iDx << src.queryHtrData(plcS[i],qTag.join(','),timeStart.toTime_t(),timeStop.toTime_t());

            // qDebug() << "query " << i << iDx << plcS[i] << tagS[i].join(',');
        }

        myTrendPlot->start(MyConst::tmr[ui->Interval->currentIndex()]); // очистити графіки


}

void AuTrendView::colorChange()
{
    int iK=0,iT=0; // індекс контролера і індек тега
    int i=0;
    QColor color;
    if(sender())
    {
        i=sender()->objectName().mid(3,1).toInt();
        color=QColorDialog::getColor(m_Color[i],this);
        // тут треба вирахувати iK та іТ
        try{
            int count=0,x,y;
            for(x=0;x<plcS.size();++x)
            {
                for(y=0;y<tagS[x].size();++y)
                    if(i==count++)
                    {
                        iK=x;
                        iT=y;
                        throw  0;  // нічого далі цикл крутити
                    }
            }

        } catch (...)       {        }
        //qDebug() << "i " << i<< " iK " << iK << " iT " << iT;

        if(color.isValid())
        {
                // зберегти значення кольорів
                QSettings s;
                s.setValue(QStringLiteral("colors/%1/%2")
                                           .arg(plcS[iK])
                                           .arg(tagS[iK][iT]),color);

            QPalette pal;
            m_Color[i]=color;

            pal.setColor(QPalette::Button,m_Color[i]);
            ((QPushButton*)sender())->setPalette(pal);

            QPalette pl;
            pl.setColor(QPalette::Highlight,m_Color[i]);
                ui->cursorVal->setPalette(pl);

                myTrendPlot->setColors(m_Color);
            //dataChange();
            QTimer::singleShot(0,this,SLOT(dataChange()));

        }
    }

}

void AuTrendView::plotChange()
{
    int i= sender()->objectName().mid(3,1).toInt();

    try{
        int count=0;
        //int iK=0,iT=0;
        for(int x=0;x<plcS.size();++x)
        {
            for(int y=0;y<tagS[x].size();++y)
                if(i==count++)
                {
                    //iK=x;
                    //iT=y;
                    throw  0;  // нічого далі цикл крутити
                }
        }

    } catch (...)       {        }

    QPalette pal;
    pal.setColor(QPalette::Highlight,m_Color[i]);
    // ui->cursorVal->setMaximum(src[plcS[iK]][tagS[iK][iT]].baseScale());
    ui->cursorVal->setPalette(pal);

//    cursorVal->setRange(m_trinfo->fScale[i][0],m_trinfo->fScale[i][1]);// недуже точно але....
     slotCursor(-1);
}

void AuTrendView::slotHtrData(qint32 iD, QByteArray baData, int row, int col)
{
    //qDebug() << iD << data.size();
    //qDebug() << sender();
    if(row>0) // якщо є дані - обробити
    {
        if(iDx.contains(iD))
        {
            int i= iDx.indexOf(iD);

            int lineIx=0; // треба розрахувати куди його писати
            for(int j=0;j<i;++j)
                lineIx+=tagS[j].size();
            int colorIx=lineIx;

            // qDebug() << "slotHtrData "<< iDx << " i " << i << " lineIx " << lineIx;

            myData[i]=baData;
            rowS[i]=row;
            colS[i]=col;
            iDx[i]=0;

            //qDebug() << "plc " << plcS[i] << " id "<< iD << " data " << data.size();
            QString &plcName=plcS[i];

            qint32 *htrVal=(qint32*)myData[i].data();
            for(int j=1;j<col;++j) // перебрати по рядках
            {
                double min,avg,max;
                min=avg=max=htrVal[(0*col)+j]; // перше значення для підрахунку
                QString tagName=tagS[i][j-1];


                for(int k=1;k<row;++k)
                {
                    if(src[plcName][tagName].dataType()==tagType::BOOL
                            || src[plcName][tagName].dataType()==tagType::EBOOL
                            || src[plcName][tagName].dataType()==tagType::IX
                            || src[plcName][tagName].dataType()==tagType::QX
                            || src[plcName][tagName].dataType()==tagType::MX )
                    { // якщо булекський тег, замінити його значення
                        htrVal[k*col+j]=BOOL_OFFSET*lineIx+(htrVal[k*col+j]==0?0:BOOL_HI);
                        min=max=avg=0; // для бітових тегів немає змісту рахувати агреговані значення
                    }
                    else
                    {
                        double current=static_cast<double>(htrVal[k*col+j]);
                        avg+=current;
                        if(current<min) min=current;
                        if(current>max) max=current;
                    }
                }

                avg/=static_cast<double>(row); // перерахувати середнє.

                if(src[plcName][tagName].baseScale()!=4000.0) // графіки перешкальовано, треба змінити min,max,avg
                {
                    min*=src[plcName][tagName].baseScale()/4000.0;
                    max*=src[plcName][tagName].baseScale()/4000.0;
                    avg*=src[plcName][tagName].baseScale()/4000.0;
                }

                ui->twAgr->item(lineIx,1)->setText(QString::number(min*src[plcName][tagName].k()+src[plcName][tagName].k(),'f',2));
                ui->twAgr->item(lineIx,2)->setText(QString::number(avg*src[plcName][tagName].k()+src[plcName][tagName].k(),'f',2));
                ui->twAgr->item(lineIx,3)->setText(QString::number(max*src[plcName][tagName].k()+src[plcName][tagName].k(),'f',2));
                ++lineIx;
                // qDebug() << "plc " << plcS[i] << i << " tag " << tagS[i][tagIx] << tagIx << " min " << min << " avg " << avg << " max " << max ;
            }
            myTrendPlot->setData(timeStart.toTime_t(),htrVal,row,col,colorIx,wK[i]);
            slotCursor(-1);
        }
    }
    else
    {
        // тут треба перемалювати графік
        //qDebug() << " No data";
        myTrendPlot->update();
    }

}

void AuTrendView::slotCursor(int v)
{
    int i,step;
    qint32 pos;
    static int ov=0;
    int line=0;

    if(v==-1) // запам'ятати старе значення V
        v=ov;
    else
        ov=v;


    pos=timeStart.toTime_t()+ MyConst::tmr[ui->Interval->currentIndex()]*v/myTrendPlot->width();

    timeCursor.setTime_t(pos);
    ui->cursorDate->setText(timeCursor.toString("hh:mm:ss\ndd:MM:yy"));
    //qDebug() << "---------------------------------" << plcS << plcS.size()  ;
    for(int k=0;k<plcS.size();++k)
    {
        qint32 *htrVal=(qint32*)myData[k].data();

        int m_nLen = rowS[k]; // myData[k].size(); // тут би треба пройтися по всіх
        //qDebug() << "k " << k << " myData[k].size() " << myData[k].size();
        if(m_nLen > 1)
        {

            step=i=m_nLen/2; // пошук найближчого значення. метод ловлі лева в пуслелі (метод дихотомії)
            do
            {
                step>>=1;
                if(htrVal[i*colS[k]+0]>pos)
                    i-=step;
                else
                    i+=step;
            }while (step>0);
            step=i-1; // зберегти значення
            // qDebug() << "step " << step;

            for(int j=0;j<tagS[k].size();++j)
            {
                if(src[plcS[k]][tagS[k][j]].dataType()==tagType::BOOL ||src[plcS[k]][tagS[k][j]].dataType()==tagType::EBOOL )
                {
                    int val=htrVal[step*colS[k]+j+1]%BOOL_OFFSET!=0?1:0;
                    ui->twAgr->item(line,0)->setText(QString::number(val));
                    if(ps[line]->isChecked())
                    {
                        //qDebug() << ps[j]->objectName() << htrVal[step][j+1];
                        ui->cursorVal->setValue(htrVal[step*colS[k]+j+1]);
                        ui->cursorLCD->display(val);
                    }
                }
                else
                {
                    ui->twAgr->item(line,0)->setText(QString::number(
                                                         (src[plcS[k]][tagS[k][j]].baseScale()!=4000.0?htrVal[step*colS[k]+j+1]*src[plcS[k]][tagS[k][j]].baseScale()/4000.0:htrVal[step*colS[k]+j+1])
                                                     *src[plcS[k]][tagS[k][j]].k()+src[plcS[k]][tagS[k][j]].b(),'f',2));
                    if(ps[line]->isChecked())
                    {
                        //qDebug() << ps[j]->objectName() << htrVal[step][j+1];
                        ui->cursorVal->setValue(htrVal[step*colS[k]+j+1]);
                        ui->cursorLCD->display(QString::number(
                                                   (src[plcS[k]][tagS[k][j]].baseScale()!=4000.0?htrVal[step*colS[k]+j+1]*src[plcS[k]][tagS[k][j]].baseScale()/4000.0:htrVal[step*colS[k]+j+1])
                                               *src[plcS[k]][tagS[k][j]].k()+src[plcS[k]][tagS[k][j]].b(),'f',src[plcS[k]][tagS[k][j]].deciaml()));
                    }
                }
                //qDebug() << "k " << k << " line " << line ;
                ++line;

            }

        }

    }

    //update();

}

void AuTrendView::setGrid(bool v)
{
//    QSettings s;
//    s.setValue(QString("%1/grid")
//               .arg(m_trinfo->trend),v);
    myTrendPlot->setGrid(v);
    //dataChange();
    QTimer::singleShot(0,this,SLOT(dataChange()));

}
