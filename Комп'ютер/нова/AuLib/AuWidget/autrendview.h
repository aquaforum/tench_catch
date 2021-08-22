#ifndef AUTRENDVIEW_H
#define AUTRENDVIEW_H

#include <QWidget>
#include <QHash>
#include <QDateTime>

/* Сюди якимось чином треба передати список контролерів та список тегів для кожного контролера
 * Передам це хешем виду
 * QHash<QString,QString> де ключ - назва контролера,  значення - список тегів через кому
 * тільки хтось повинен перевірити що їх не більше восьми в сумі..... ну або зробити полосу прокрути
 *
*/
namespace Ui {
class AuTrendView;
}

class AuBase;
class QCheckBox;
class QRadioButton;
class QPushButton;
class TrendPlot;

class AuTrendView : public QWidget
{
    Q_OBJECT

public:
    explicit AuTrendView(AuBase &base,QStringList &trendFields, QWidget *parent = nullptr);
    explicit AuTrendView(AuBase &base,QStringList &trendFields, QStringList &weidthK, QWidget *parent = nullptr);

    void init(QStringList &trendFields); // сюди винесений конд з конструкторів

    ~AuTrendView();

    void setText(const QString &);

private slots:
    void slotExit();
    void dataChange();
    void colorChange();
    void plotChange();

    void slotHtrData(qint32, QByteArray,int,int);
    void slotCursor(int);
    void setGrid(bool);

    void slotSetMinMax(qint32, const QList<QStringList>&);

signals:
    void finished();

private:
    Ui::AuTrendView *ui;
    AuBase &src;

    QStringList plcS; // список контролерів
    QVector<QStringList> tagS; // список тегів

    TrendPlot *myTrendPlot;

    QVector<QColor> m_Color;
    QVector<QCheckBox*> pv;
    QVector<QRadioButton*> ps;
    QVector<QPushButton*> pc;

    QDateTime timeStart,timeStop,timeCursor; // це для графіків

    QVector<int> iDx;  // ідентифікатори запитів
    QVector<QByteArray> myData; // сюди будуть завантажені дані із бази
    QVector<int> rowS;
    QVector<int> colS;

    qint32 mmIx;

    int BOOL_OFFSET;
    int BOOL_HI;

    QList<QVector<double>> wK;

};

#endif // AUTRENDVIEW_H
