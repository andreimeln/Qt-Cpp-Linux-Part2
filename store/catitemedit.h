#ifndef CATITEMEDIT_H
#define CATITEMEDIT_H

#include <QFrame>
#include <QtSql>

#include "ui_catitemframe.h"
#include "dialogtpl.h"

class QSqlQuery;

namespace STORE {
namespace Catalogue {
namespace Item {

/*******************************************************************/

class Data;

class List : public QList<Data*> {

public:
    List() : QList<Data*>() {}
    Data *findPointer(int Id) const;
};

/*******************************************************************/

class Data : public QObject {

    Q_OBJECT

public:
    Data(QObject *parent=0) : QObject(parent), IsLocal(true), pParentItem(0), Deleted(false), Changed(false) {}
    // bool and ptr are built-in types, they get a random value
    // the other types are library types and get default values
    Data(QObject *parent, QSqlQuery &qry);
public:
    // QImage vs. QPixmap pictogram
    QVariant Id;
    QString Code;
    QString Title;
    QDateTime From;
    QDateTime To;
    bool IsLocal;
    QString Comment;
    Data *pParentItem;
    bool Deleted;
    //QList<Data*> Children;
    List Children;
    bool Changed;

public:
    bool isActive() const;
    bool isNew() const;
    bool isSameAs(Data *D) const;
};

/*******************************************************************/

class Frame : public QFrame {

    Q_OBJECT

public:
    Frame(QWidget * parent = 0);
    virtual ~Frame();

private:
    Ui::CatItemFrame ui;
    Data *Block;

public:
    void setDataBlock(Data *D) {Block = D; load(); }


public slots:
    void load();
    bool save();
    void is_good(bool *pOk);

signals:
    void error_message(const QString &);

};

/*******************************************************************/

class Dialog : public CommonDialog {

    Q_OBJECT

private:
    Frame *pFrame;

public:
    Dialog(QWidget *parent = 0);
    virtual ~Dialog();

public:
    void setDataBlock(Data *D) {pFrame->setDataBlock(D);}
};

/*******************************************************************/

} // namespace Item
} // namespace Catalogue
} // namespace STORE

#endif // CATITEMEDIT_H
