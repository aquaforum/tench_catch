/*
 * Формат файла історії буде дуже простим
 * назваКнопки=контролер,список,параметрів
 * одна назва кнопки може дублюватися для для різних контролерів.
 * пусті рядки ігноруються.
 * */

#ifndef AUHISTORY_H
#define AUHISTORY_H

#include <QWidget>
#include <QDialog>
class AuBase;

class AuHistory : public QDialog
{
    Q_OBJECT
public:
    AuHistory(AuBase &base, QWidget *parent=nullptr, QString uiHistory=QStringLiteral(":/history.ui"),QString fileTrendLists=QStringLiteral(":/trends.list"));
    ~AuHistory();

    QStringList &trendFields() { return myTrendFields;}
    QString text() {return myText;}
private slots:
    void slotAccept();
    void slotConstruct();
    void slotStoreTabIndex(int);

private:
    AuBase &src;
    QString trendLists;

    QStringList myTrendFields;

    void loadData(QString);
    QString myText;
};

#endif // AUHISTORY_H
