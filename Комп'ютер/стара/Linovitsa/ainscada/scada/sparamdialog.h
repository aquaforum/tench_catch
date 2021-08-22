#ifndef SPARAMDIALOG_H
#define SPARAMDIALOG_H

#include <QDialog>

class IoNetClient;
class QComboBox;

namespace Ui {
    class SParamDialog;
}

class SParamDialog : public QDialog {
    Q_OBJECT
public:
    SParamDialog(QVector<IoNetClient*> &source,QWidget *parent = 0);
    ~SParamDialog();

private slots:
    void myAccept();
    void selectTeg(int v);
    void selectPage(int v);
    void slotSave();
    void slotOpen();

    void slotSet(QString);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SParamDialog *m_ui;
    QVector<IoNetClient*> &src;

    QVector<QStringList> tag_name; // список тегів, які треба налаштовувати
    QVector<QComboBox*> cbList;
    bool bAccess;
};

#endif // SPARAMDIALOG_H
