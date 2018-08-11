#ifndef CATALOGUE_H
#define CATALOGUE_H

#include <Qt>
#include <QAbstractItemModel> /* <QAbstractTableModel> */
#include <QTableView>
#include <QTreeView>
#include <QColumnView>

#include "catitemedit.h"

class QAction;

namespace STORE {
    class PosAction;
}

namespace STORE {
namespace Catalogue {

/************************************************************/

//class Model : public QAbstractTableModel {   // for table model
class Model : public QAbstractItemModel {   // for tree model

    Q_OBJECT

private:
    //QList<Item::Data*> Cat; // list of pointers to catalog elements
    mutable int LastTempId; // mutable - secondary var that can change within const func
    Item::List Cat;

protected:
    virtual QVariant dataDisplay(const QModelIndex &I) const;
    virtual QVariant dataTextAlignment(const QModelIndex &I) const;
    virtual QVariant dataForeground(const QModelIndex &I) const;
    virtual QVariant dataBackground(const QModelIndex &I) const;
    virtual QVariant dataFont(const QModelIndex &I) const;
    virtual QVariant dataToolTip(const QModelIndex &I) const;
    //virtual Item::Data *dataDataBlock(const QModelIndex &I) const;

    int tempId() const { return ++LastTempId; }

public:
    Model(QObject *parent = 0);
    virtual ~Model();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &I) const;

    QVariant data(const QModelIndex &I, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    bool delete_all_from_db(Item::Data *D = 0);
    bool delete_all_from_model(Item::Data *D = 0);
    bool save_all_to_db(Item::Data *D = 0);
    bool drop_change_mark(Item::Data *D = 0);
    bool insert_all_to_db(Item::Data *D = 0);
    bool adjust_id_for_new(Item::Data *D = 0);

protected:
    bool delete_all();
    bool save_all();
    bool insert_all();

public slots:
    void editItem(const QModelIndex &I, QWidget *parent = 0);
    void newItem(const QModelIndex &parentI, QWidget *parent = 0);
    void delItem(const QModelIndex &I, QWidget *parent = 0);
    void save();
};

/************************************************************/

class TreeView : public QTreeView {

    Q_OBJECT

public:
    TreeView(QWidget *parent = 0, Model *xModel = 0);
    virtual ~TreeView;
};

/************************************************************/

class ColumnView : public QColumnView {

    Q_OBJECT

protected:
    // on current selected element changed
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

public:
    ColumnView(QWidget *parent = 0, Model *xModel = 0);
    virtual ~ColumnView;

signals:
    void item_selected(QVariant id);
};


/************************************************************/

class TableView : public QTableView {

    Q_OBJECT

private:
    PosAction *actEditItem;
    PosAction *actNewItem;
    PosAction *actDelItem;
    PosAction *actRootItem; // 1 step down
    QAction *actParentRootItem; // 1 step up
    QAction *actSave;

public:
    TableView(QWidget *parent = 0, Model *xModel = 0);
    virtual ~TableView();

private slots:
    void contextMenuRequested(const QPoint &p);
    //void editItem();
    void showChildren(const QModelIndex &I, QWidget *);
    void showParent(void);

/*
signals:
    void mustEditItem(const QModelIndex &I, QWidget *parent);
*/
};

/************************************************************/

} // namespace Catalogue
} // namespace STORE

#endif // CATALOGUE_H
