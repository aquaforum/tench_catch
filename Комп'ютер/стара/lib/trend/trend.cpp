#include "trend.h"
#include "ui_trend.h"

#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QColorDialog>
#include <QDebug>

#include "math.h"

#include "datetimedialog.h"

#include <QHeaderView>
#include <QTableWidgetItem>

#include <QMutex>

// Функції-члени класу TrendWindow

#include "historythread.h"

#define TR_HEIGHT 10000

namespace MyConst
{
    const int tmr[]={300,900,1800,3600,7200,14400,21600,43200};
    const unsigned int LvCor = 2049136799;
    const Qt::GlobalColor DefColor[8]={Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::white,Qt::darkRed};
}

TrendWindow::TrendWindow(QWidget *p,struct trendinfo *tri,int nHeight) : QWidget(p)
  ,m_nHeight(nHeight),m_pnDt(0),m_ui(new Ui::Trend),mState(0)
{    

    setObjectName("TrendWindow_1");
    m_ui->setupUi(this);

    //setAttribute( Qt::WA_DeleteOnClose); // ??? це що таке
    connect(m_ui->Exit,SIGNAL(clicked()),this,SLOT(slotExit()));

    m_trinfo=tri;
    setObjectName(m_trinfo->trend);    
    
    QVBoxLayout *vbl=new QVBoxLayout(m_ui->trend); // менеджер розміщення для об'єкту TrendView
    m_tw=new TrendView(m_ui->trend);
    vbl->setContentsMargins(2,2,2,2);
    vbl->addWidget(m_tw);    
    m_ui->trend->setLayout(vbl);
    
    
    m_ui->trendHead->setText(m_trinfo->trendHead);

    // навігаційні кнопки
    connect(m_ui->forward,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->backward,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->forwardHalf,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->backwardHalf,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->calendarButton,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->last,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->Interval,SIGNAL(activated(int)),this,SLOT(dataChange()));

    // заблкувати сигнали поки не відпрацює перша послідовність завантаження даних
    m_ui->forward->blockSignals(true);
    m_ui->backward->blockSignals(true);
    m_ui->forwardHalf->blockSignals(true);
    m_ui->backwardHalf->blockSignals(true);
    m_ui->calendarButton->blockSignals(true);
    m_ui->last->blockSignals(true);
    m_ui->Interval->blockSignals(true);


    // кнопки вибору кольору
    connect(m_ui->pc_0,SIGNAL(clicked()),this,SLOT(colorChange()));
    connect(m_ui->pc_1,SIGNAL(clicked()),this,SLOT(colorChange()));
    connect(m_ui->pc_2,SIGNAL(clicked()),this,SLOT(colorChange()));
    connect(m_ui->pc_3,SIGNAL(clicked()),this,SLOT(colorChange()));
    connect(m_ui->pc_4,SIGNAL(clicked()),this,SLOT(colorChange()));
    connect(m_ui->pc_5,SIGNAL(clicked()),this,SLOT(colorChange()));
    connect(m_ui->pc_6,SIGNAL(clicked()),this,SLOT(colorChange()));
    connect(m_ui->pc_7,SIGNAL(clicked()),this,SLOT(colorChange()));

    // перемикач вибору поля курсоку
    connect(m_ui->ps_0,SIGNAL(clicked()),this,SLOT(plotChange()));
    connect(m_ui->ps_1,SIGNAL(clicked()),this,SLOT(plotChange()));
    connect(m_ui->ps_2,SIGNAL(clicked()),this,SLOT(plotChange()));
    connect(m_ui->ps_3,SIGNAL(clicked()),this,SLOT(plotChange()));
    connect(m_ui->ps_4,SIGNAL(clicked()),this,SLOT(plotChange()));
    connect(m_ui->ps_5,SIGNAL(clicked()),this,SLOT(plotChange()));
    connect(m_ui->ps_6,SIGNAL(clicked()),this,SLOT(plotChange()));
    connect(m_ui->ps_7,SIGNAL(clicked()),this,SLOT(plotChange()));

    // чекбокси вмикання графіку
    connect(m_ui->pv_0,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->pv_1,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->pv_2,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->pv_3,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->pv_4,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->pv_5,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->pv_6,SIGNAL(clicked()),this,SLOT(dataChange()));
    connect(m_ui->pv_7,SIGNAL(clicked()),this,SLOT(dataChange()));


    //connect(m_ui->cursorVal,SIGNAL(valueChanged(int)),m_ui->cursorLCD,SLOT(display(int)));

    QSettings s;

    for(int i=0;i<8;++i)
    {
        QColor  color = s.value(QString("%1/colors/%2")
                           .arg(tri->trend)
                           .arg(tri->fields[i]),
                           QColor(MyConst::DefColor[i])).value<QColor>(); // цю ініціалізацію треба переробити

        if(color.isValid())
            m_Color << color;
        else
            m_Color << MyConst::DefColor[i];
    }

    pv << m_ui->pv_0
    << m_ui->pv_1
    << m_ui->pv_2
    << m_ui->pv_3
    << m_ui->pv_4
    << m_ui->pv_5
    << m_ui->pv_6
    << m_ui->pv_7;


    foreach(QCheckBox *p,pv)
    {
        p->setChecked(s.value(QString("%1/checked/%2")
                              .arg(tri->trend)
                              .arg(tri->fields[p->objectName().right(1).toInt()] ),true).toBool());
    }

    // управління сіткою
    m_ui->gridEnable->setChecked(s.value(QString("%1/grid")
                                         .arg(tri->trend),true).toBool());
    connect(m_ui->gridEnable,SIGNAL(clicked(bool)),this,SLOT(setGrid(bool)));
    // встановити прапор в класі-малювалці
    m_tw->setGrid(m_ui->gridEnable->isChecked());

    //фарбую кнопки
    QPalette pal;
    
    pal.setColor(QPalette::Button,m_Color[0]);
    m_ui->pc_0->setPalette(pal);

    pal.setColor(QPalette::Button,m_Color[1]);
    m_ui->pc_1->setPalette(pal);
    
    pal.setColor(QPalette::Button,m_Color[2]);
    m_ui->pc_2->setPalette(pal);

    pal.setColor(QPalette::Button,m_Color[3]);
    m_ui->pc_3->setPalette(pal);

    pal.setColor(QPalette::Button,m_Color[4]);
    m_ui->pc_4->setPalette(pal);

    pal.setColor(QPalette::Button,m_Color[5]);
    m_ui->pc_5->setPalette(pal);

    pal.setColor(QPalette::Button,m_Color[6]);
    m_ui->pc_6->setPalette(pal);

    pal.setColor(QPalette::Button,m_Color[7]);
    m_ui->pc_7->setPalette(pal);

    QPalette pl;
    pl.setColor(QPalette::Highlight,m_Color[0]);
    m_ui->cursorVal->setPalette(pl);

    m_ui->cursorVal->setValue(0);
    m_ui->cursorVal->setRange(0,m_nHeight);
    
    QPalette pn;     
    pn.setColor(QPalette::Window,QColor((QRgb)0x84A3AB));
    
    m_ui->frame->setAutoFillBackground(true);
    m_ui->frame->setPalette(pn);

    m_ui->navigation->setAutoFillBackground(true);
    m_ui->navigation->setPalette(pn);

    m_ui->view->setAutoFillBackground(true);
    m_ui->view->setPalette(pn);

    m_ui->ps_0->setChecked(true);
    
    ps[0]=m_ui->ps_0;
    ps[1]=m_ui->ps_1;
    ps[2]=m_ui->ps_2;
    ps[3]=m_ui->ps_3;
    ps[4]=m_ui->ps_4;
    ps[5]=m_ui->ps_5;
    ps[6]=m_ui->ps_6;
    ps[7]=m_ui->ps_7;
    
    
    m_tw->setColors(m_Color);

    m_stop=QDateTime::currentDateTime();

    // таблиця параметрів внизу вікна
    m_ui->twAgr->setColumnCount(4);
    m_ui->twAgr->setRowCount(8);
    m_ui->twAgr->setVerticalHeaderLabels(m_trinfo->fieldHead);
    m_ui->twAgr->horizontalHeader()->hide();
    m_ui->twAgr->verticalHeader()->setFixedWidth(300);
    m_ui->twAgr->setSortingEnabled(false);
    for(int i=0;i<8;++i)
        m_ui->twAgr->setRowHeight(i,20);
    for(int i=0;i<4;++i)
        m_ui->twAgr->setColumnWidth(i,75);
    QTableWidgetItem *_item;
    for(int i=0;i<8;++i)
    {
	for(int j=0;j<4;++j)
	{
	    _item= new QTableWidgetItem;
	    _item->setText("0.00");
            m_ui->twAgr->setItem(i,j,_item);
	}
    }
    
    m_sTmpl= "SELECT Dt%1 FROM %2 WHERE Dt BETWEEN %3 AND %4 ORDER BY Dt";// шаблон запиту

	// визначення чісового інтервалу наявних даних
	//if(query.exec(QString("SELECT min(Dt) ,max(Dt) from %1").arg(m_trinfo->table)))
        //tmp.setTime_t(query.value(0).toUInt());
        //m_ui->db_startDate->setText(tmp.toString("hh:mm:ss\ndd:MM:yy"));
	//tmp.setTime_t(query.value(1).toUInt());
        //m_ui->db_stopDate->setText(tmp.toString("hh:mm:ss\ndd:MM:yy"));

     QTimer::singleShot(0,this,SLOT(startHtr()));

     mutex=new QMutex;
    // qDebug() << "TrendWindow init finished";
}

// це працює але чи необхідно так скодно все робити ? і ще питання: як його завернути в скінченний автомат?
// є думка зарядити все в чергу ....

void TrendWindow::startHtr()
{
    // запуск потоку роботи із бд

    // connect(this,SIGNAL(destroyed()),htr,SLOT(quit()),Qt::QueuedConnection); // це зруйнує класс

    // qDebug() << "htr alloc";
    htr=new HistoryThread(this,m_trinfo->host,m_trinfo->db,m_trinfo->user,m_trinfo->passwd);
    // qDebug() << "htr created";
    htr->setObjectName("HystoryTrendThread");

    // connect(htr,SIGNAL(started()),this,SLOT(slotStart()));
    QTimer::singleShot(1000,this,SLOT(slotStart()));

//    connect(htr,SIGNAL(pullRows(QStringList)),this,SLOT(processRow(QStringList)),Qt::QueuedConnection); // ,Qt::QueuedConnection
//    connect(htr,SIGNAL(endQuery()),this,SLOT(changeState()),Qt::QueuedConnection);
//    connect(htr,SIGNAL(dbError(QString)),this,SLOT(showErrorText(QString)),Qt::QueuedConnection);
//    connect(this,SIGNAL(execQuery(QString)),htr,SLOT(runQuery(QString)),Qt::QueuedConnection);


    htr->start(QThread::LowestPriority); // запустити із низьким пріоритетом.
    // qDebug() << "htr started";



}

void TrendWindow::slotStart()
{
    emit execQuery(QString("SELECT min(Dt) ,max(Dt) from %1").arg(m_trinfo->table));
}


TrendWindow::~TrendWindow()
{
    qDebug() << "TrendWindow end";

    QSettings s;

    foreach(QCheckBox *p,pv)
    {
        QString f=m_trinfo->fields[p->objectName().right(1).toInt()];

        if( p->isChecked())
        {
            s.remove(QString("%1/checked/%2")
                     .arg(m_trinfo->trend)
                     .arg(f));
        }
        else
        {
            s.setValue(QString("%1/checked/%2")
                       .arg(m_trinfo->trend)
                       .arg(f),false);
        }
    }

    delete m_ui;

    htr->quit();
    if(!htr->wait(5000))
        htr->terminate();

    //htr->deleteLater();  // це треба перевірити валгріндом, тут може бути протікання....
    delete htr;

    delete mutex;

}

void TrendWindow::slotExit()
{
    emit finished();
}

void TrendWindow::dataChange()
{
//     qDebug() << "start" << sender();

//    if(mState==1)
//        return ;
    if (mutex->tryLock())
    {
        disconnect(m_tw,SIGNAL(cursorMoved(int)),this,SLOT(setCursor(int)));

        QString s="";
        int i ;

        if(sender()!=NULL) // обробка зміни часу від навігаційних кнопок
        {
            QString sndr=sender()->objectName();
            if(sndr=="last")
            {
                m_stop=QDateTime::currentDateTime();
            }
            else if(sndr=="forward")
            {
                m_stop=m_stop.addSecs(MyConst::tmr[m_ui->Interval->currentIndex()]);
            }
            else if(sndr=="forwardHalf")
            {
                m_stop=m_stop.addSecs(MyConst::tmr[m_ui->Interval->currentIndex()]/2);
            }
            else if(sndr=="backwardHalf")
            {
                m_stop=m_stop.addSecs(-MyConst::tmr[m_ui->Interval->currentIndex()]/2);
            }
            else if(sndr=="backward")
            {
                m_stop=m_stop.addSecs(-MyConst::tmr[m_ui->Interval->currentIndex()]);
            }
            else if(sndr=="calendarButton")
            {
                myDateTimeDialog dt(this,m_stop);
                if(dt.exec()==QDialog::Accepted)
                {
                    m_stop.setDate(dt.date->selectedDate());
                    m_stop.setTime(QTime(dt.hour->value(),dt.minute->value(),dt.second->value()));
                }
            }
        }

        // початок виконання запиту та малювання графіку
        // QApplication::setOverrideCursor(Qt::WaitCursor);

        m_start=m_stop.addSecs(-MyConst::tmr[m_ui->Interval->currentIndex()]);

        m_ui->startDate->setText(m_start.toString("hh:mm:ss\ndd:MM:yy"));
        m_ui->stopDate->setText(m_stop.toString("hh:mm:ss\ndd:MM:yy"));

        for(i=0;i<m_trinfo->numPlot;++i)  //визначення полів запиту. ті які не відображаються == -1 щоб бути поза зоною відображення
        {
            s+=",";
            s+=pv[i]->checkState()==Qt::Checked?m_trinfo->fields[i]:"-1";
        }
        //qDebug() << "FL s=" << s << "m_trinfo->numPlot = " << m_trinfo->numPlot;

        // фінальна побудова запиту
        sQuery=m_sTmpl.arg(s).arg(m_trinfo->table).arg(m_start.toTime_t()).arg(m_stop.toTime_t());

        // робимо все асинхронно !!!!
        //QTimer::singleShot(0,this,SLOT(sendQuery()));

        mState=1;
        m_pnDt.clear();

        // qDebug() << "Query :" << query.lastQuery() << " size:"  << query.size() ;
        // ініціалізація об'єкта малювання графіку
         m_tw->start(m_stop.toTime_t()-m_start.toTime_t(),m_trinfo->numPlot,m_nHeight);
            // виймання даних
         m_nLen=0;

         QApplication::setOverrideCursor(Qt::WaitCursor);

         m_ui->forward->blockSignals(true);
         m_ui->backward->blockSignals(true);
         m_ui->forwardHalf->blockSignals(true);
         m_ui->backwardHalf->blockSignals(true);
         m_ui->calendarButton->blockSignals(true);
         m_ui->last->blockSignals(true);
         m_ui->Interval->blockSignals(true);

         qDebug() << "execQuery" << sQuery;

         emit execQuery(sQuery);

    }
    else
    {
        qDebug() << "Mutex locked";
    }

 }

void TrendWindow::colorChange()
{
    int i=0;
    QColor color;
    if(sender())
    {
	i=sender()->objectName().mid(3,1).toInt();	
        color=QColorDialog::getColor(m_Color[i],this);
	if(color.isValid())
	{
            // зберегти значення кольорів
            QSettings s;
            s.setValue(QString("%1/colors/%2")
                                       .arg(m_trinfo->trend)
                                       .arg(m_trinfo->fields[i]),color);

	    QPalette pal;
	    m_Color[i]=color;

	    pal.setColor(QPalette::Button,m_Color[i]);
	    ((QPushButton*)sender())->setPalette(pal);

	    QPalette pl;
	    pl.setColor(QPalette::Highlight,m_Color[i]);
            m_ui->cursorVal->setPalette(pl);
	
            m_tw->setColors(m_Color);
	    //dataChange();
        QTimer::singleShot(0,this,SLOT(dataChange()));

	}
    }
}

void TrendWindow::plotChange()
{
    // переприв'язка курсора до нової кривої
    int i= sender()->objectName().mid(3,1).toInt();
    QPalette pal;
    pal.setColor(QPalette::Highlight,m_Color[i]);
    m_ui->cursorVal->setPalette(pal);
//    cursorVal->setRange(m_trinfo->fScale[i][0],m_trinfo->fScale[i][1]);// недуже точно але....
    setCursor(-1);    
    
}

void TrendWindow::setCursor(int v)
{
        int i,step;
        unsigned int pos;
        static int ov=0;

        if(v==-1)
            v=ov;
        else
            ov=v;


        pos=m_start.toTime_t()+ MyConst::tmr[m_ui->Interval->currentIndex()]*v/m_tw->width();

        m_cursor.setTime_t(pos);
        m_ui->cursorDate->setText(m_cursor.toString("hh:mm:ss\ndd:MM:yy"));

        if(m_nLen > 1)
        {
            step=i=m_nLen/2; // пошук найближчого значення. метод ловлі лева в пуслелі (метод дихотомії)
            do
            {
                step>>=1;
                if(m_pnDt[i]>pos)
                    i-=step;
                else
                    i+=step;
            }while (step>0);
            step=i; // зберегти значення

    //	QSqlQuery qry(dbs);
            QString s;

            for(i=0;i<m_trinfo->numPlot;++i)
            {
                s+=",";
                s+=pv[i]->checkState()==Qt::Checked?m_trinfo->fields[i]:"-1";
            }

            if(mutex->tryLock())
            {
                mState=3;
                emit execQuery(QString("SELECT Dt%1 from %2 WHERE Dt= %3").arg(s).arg(m_trinfo->table).arg(m_pnDt[step]));
            }
            else
            {
                qDebug() << "setCursor: Mutex locked";
            }


        }



}


void TrendWindow::setColors(QVector<QColor> &colors)
{
    QPalette pal;

    QVector<QPushButton*> pb;
    pb
            << m_ui->pc_0
            << m_ui->pc_1
            << m_ui->pc_2
            << m_ui->pc_3
            << m_ui->pc_4
            << m_ui->pc_5
            << m_ui->pc_6
            << m_ui->pc_7 ;

    for(int i=0;i<colors.size();++i)
    {
        m_Color[i] = colors[i]; // цю ініціалізацію треба переробити
        pal.setColor(QPalette::Button,m_Color[i]);
        pb[i]->setPalette(pal);
    }

    pal.setColor(QPalette::Highlight,m_Color[0]);
    m_ui->cursorVal->setPalette(pal);

    m_tw->setColors(m_Color);
    //dataChange();
    //QTimer::singleShot(0,this,SLOT(dataChange()));

}

void TrendWindow::setGrid(bool v)
{
    QSettings s;
    s.setValue(QString("%1/grid")
               .arg(m_trinfo->trend),v);
    m_tw->setGrid(v);
    //dataChange();
    QTimer::singleShot(0,this,SLOT(dataChange()));

}

void TrendWindow::processRow(QStringList row) // це отримує дані
{
    //qDebug() << sender()->objectName() << "mState="<<mState;
    double val;

    switch(mState)
    {
        case 0: // це буде потрібне для визначення дати початку і кінця
            {
                QDateTime tmp;

                tmp.setTime_t(row[0].toUInt());
                m_ui->db_startDate->setText(tmp.toString("hh:mm:ss\ndd:MM:yy"));

                tmp.setTime_t(row[1].toUInt());
                m_ui->db_stopDate->setText(tmp.toString("hh:mm:ss\ndd:MM:yy"));

            }
            break;

        case 1: // виймати дані і запихати їх на графік
            m_nLen++;
            //qDebug() << "row size" << row.size() << row;

            m_pnDt << row[0].toUInt(); // зберегти масив точок
            {
                QVector<int> v(m_trinfo->numPlot);

                for(int i=0;i<m_trinfo->numPlot;++i)
                    v[i]=row[i+1].toDouble();
                m_tw->setData(row[0].toUInt()-m_start.toTime_t(),v);
             }

            break;
        case 2: // це буде запит на виймання середніх значень.
            for(int i=0;i<m_trinfo->numPlot;++i)
            {
                double min,avg,max;

                min=row[i*3+1].toDouble() *(m_trinfo->fScale[i][1]-m_trinfo->fScale[i][0])/m_nHeight + m_trinfo->fScale[i][0];
                m_ui->twAgr->item(i,1)->setText(QString("%1").arg(min,6,'f',2));

                avg=row[i*3+2].toDouble() *(m_trinfo->fScale[i][1]-m_trinfo->fScale[i][0])/m_nHeight + m_trinfo->fScale[i][0];
                m_ui->twAgr->item(i,2)->setText(QString("%1").arg(avg,6,'f',2));

                max=row[i*3+3].toDouble() *(m_trinfo->fScale[i][1]-m_trinfo->fScale[i][0])/m_nHeight + m_trinfo->fScale[i][0];
                m_ui->twAgr->item(i,3)->setText(QString("%1").arg(max,6,'f',2));
            }
            connect(m_tw,SIGNAL(cursorMoved(int)),this,SLOT(setCursor(int)));
        break;

        case 3:
            for(int i=0;i<m_trinfo->numPlot;++i)
            {
                //qDebug() << i+1 << qry.value(i+1).toDouble() << m_trinfo->fScale[i][1] << m_trinfo->fScale[i][0] << m_nHeight;

                val=(row[i+1].toDouble()*	(m_trinfo->fScale[i][1]-m_trinfo->fScale[i][0]))/(double)m_nHeight + m_trinfo->fScale[i][0];
                m_ui->twAgr->item(i,0)->setText(QString("%1").arg(val,6,'f',2));
                if(ps[i]->isChecked())
                {
                    m_ui->cursorVal->setValue(row[i+1].toInt());
                    m_ui->cursorLCD->display(val); //display(fabs(val)<1.5?ceil(val):val);

                }
            }

        default:
            break;


    }
   // qDebug() << "value" << row;
}

void TrendWindow::changeState()     // це викликається в кінці обробки запиту;
{
    qDebug() << "changeState" << mState;

    switch(mState)
    {
    case 0:

        // сигнал від перегрядача трендів, запит наперемалювання
        connect(m_tw,SIGNAL(_redraw()),this,SLOT(dataChange()));
        QTimer::singleShot(0,this,SLOT(dataChange()));

        break;

    case 1:
        m_tw->draw();
        // а після цього треба заватажити таблицю
        {
            QString s;
            for(int i=0;i<m_trinfo->numPlot;++i)
            {
                s+=pv[i]->checkState()==Qt::Checked?",min("+m_trinfo->fields[i]+"),avg("+m_trinfo->fields[i]+"),max("+m_trinfo->fields[i]+")" :",-1,-1,-1";
            }
            s=QString("SELECT COUNT(Dt)%1 FROM %2 WHERE Dt BETWEEN %3 AND %4").arg(s).arg(m_trinfo->table).arg(m_start.toTime_t()).arg(m_stop.toTime_t());

            emit execQuery(s);
            mState=2;
        }
        break;

    case 2:

        QApplication::setOverrideCursor(Qt::ArrowCursor);
        QTimer::singleShot(10,this,SLOT(setCursor()));
        qDebug() << "Mutex unlock";
        mutex->unlock();

        // це перенесено сюди для того щоб при першому вийманні даних не засмикували програму кнопками навігації
        m_ui->forward->blockSignals(false);
        m_ui->backward->blockSignals(false);
        m_ui->forwardHalf->blockSignals(false);
        m_ui->backwardHalf->blockSignals(false);
        m_ui->calendarButton->blockSignals(false);
        m_ui->last->blockSignals(false);
        m_ui->Interval->blockSignals(false);

        mState=0; // тут будемо дописувати .....

        break;

    default:

        qDebug() << "Mutex unlock";
        mutex->unlock();


        mState=0; // тут будемо дописувати .....

        break;

    }

}

void TrendWindow::showErrorText(QString v)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    m_ui->forward->blockSignals(false);
    m_ui->backward->blockSignals(false);
    m_ui->forwardHalf->blockSignals(false);
    m_ui->backwardHalf->blockSignals(false);
    m_ui->calendarButton->blockSignals(false);
    m_ui->last->blockSignals(false);
    m_ui->Interval->blockSignals(false);

    QMessageBox::critical(this,tr("Error"),v);
}


//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------

// Функції-члени класу TrendView
TrendView::TrendView(QWidget *p) : QWidget(p),
    m_nH(4000)
{
    setObjectName("TrendView_1");
    m_px=new QPixmap(2000,2000);
    m_px->fill(QColor(Qt::black));


    m_paint.begin(m_px);

    
    repaint();
    m_bStart = false;
    m_nPlot=0; // на всяк пожарний 
    m_pColor.fill(QColor(Qt::black),8);

}

TrendView::~TrendView()
{
    m_paint.end();
    delete m_px;
}

void TrendView::start(int nLen,int numPlot,int nHeight /*=4000*/) // тут створ.ється новий oб'єкт m_px
{

    if(m_paint.isActive())
        m_paint.end();    


    if(m_px)
        delete m_px; // знищити старий малюнок

    m_px = new QPixmap(size());  // створити новий малюнок
    if(m_px)
    {

	m_paint.begin(m_px);
#if QT_VERSION_MAJOR>4
    m_paint.setRenderHint(QPainter::Qt4CompatiblePainting	,true);
#endif
    m_paint.scale((double)m_px->width()/(double)nLen,(double)m_px->height()/(double)nHeight);
    //m_pen.setWidthF((double)nLen/(double)m_px->width()+(double)nHeight/(double)m_px->height());


        m_px->fill(Qt::black);     // замалювати фон

        // намалювати сітку якщо увімкнута
        if(m_grid)
        {
            QPen m_tPen;
            m_tPen.setColor(Qt::darkGray);
            m_tPen.setWidth(0);

            QVector<qreal> dashes;
            dashes << 1.0 << 2.0
                   << 1.0 << 2.0
                   << 1.0 << 2.0
                   << 1.0 << 5.0 ;

            m_tPen.setDashPattern(dashes);

            m_paint.setPen(m_tPen);
            int nStep=nHeight/4;
            for(int i=1;i<4;++i)
            {
                m_paint.drawLine(0,nStep*i,nLen,nStep*i);
                // qDebug() << i <<nHeight/i;
            }

            nStep=nLen/4;
            for(int i=1;i<4;++i)
            {
                m_paint.drawLine(nStep*i,0,nStep*i,nHeight);
                // qDebug() << i <<nHeight/i;
            }


        }
        m_nPlot=numPlot;
        m_bStart=true;
    
        m_nH=nHeight; // зберегти висоту графіку
    }
    else
    {
	QMessageBox::critical(this,tr("Помилка"),tr("Не вдалося виділити пам\'ять під об\'єкт QPixmap"));
	close();
    }
}

void TrendView::setData(double nX,QVector<int>& Data) // цей слот приймає дані для малювання.
{
    int i;

    if(m_bStart) // якщо рисуємо першу точку
    {
	m_bStart= false	;
    }
    else
    {
	for(i=0;i<m_nPlot;++i)
	{
	    m_pen.setColor(m_pColor[i]);
	    m_paint.setPen(m_pen);
	    m_paint.drawLine(m_nX,m_pfData[i],nX,m_nH-Data[i]); // намалювати поточний відрізок	    
	}
    }
    // зберегти стару точну
        for(i=0;i<m_nPlot;++i)
	{
		m_pfData[i]=m_nH-Data[i];
        }
        m_nX=nX;    
}

void TrendView::draw() // власне по цій команді об'єкт m_px відображається на екрані
{
    repaint(); // а можливо це непотрібно ?
}

void TrendView::setColors(QVector<QColor> &pColor)
{
    int i;
    for(i=0;i<pColor.size();++i)
    {
	m_pColor[i]=pColor[i];	
    }

}

void TrendView::paintEvent(QPaintEvent *) // малюю вищенаведену картину на екрані.
{
    QPainter pnt(this);
    QPen p(QColor(Qt::yellow));
    
    pnt.drawPixmap(0,0,*m_px);

    pnt.setPen(p);
    pnt.drawLine(m_nCursorPos,0,m_nCursorPos,height()); // намалювати курсор
    
}

/* схоче що це поки-що непортрібно, управлінням розміром об'єкту керує менеджер розміщення */
void TrendView::resizeEvent(QResizeEvent *e)
{    
//    resize(e->size());
    if(e->size()!=e->oldSize()) // якщо розміри змінились, сказати що треба перемалюватися
        emit _redraw();
} 

void TrendView::mouseMoveEvent(QMouseEvent *e)
{    
    if (e->buttons()&Qt::LeftButton)
    {
        m_nCursorPos=e->x();
	emit cursorMoved(m_nCursorPos);
        repaint();
    }
}

void TrendView::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        m_nCursorPos=e->x();
	emit cursorMoved(m_nCursorPos);
        repaint();
    }    
}	

