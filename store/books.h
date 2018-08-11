#ifndef BOOKS_H
#define BOOKS_H

#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QItemDelegate>

namespace STORE {
namespace Books {

/********************************************************/

class StatusDelegate : public QItemDelegate {

    Q_OBJECT

private:
    QMap<int,QString> fAllStatus;

public:
    StatusDelegate(QObject *parent, const QMap<int,QString> &fAllStatus);
    virtual ~StatusDelegate(void) {}

QWidget *createEditor(QWidget *parent,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &I) const;
void setEditorData(QWidget *editor,
                   const QModelIndex &I) const;
void setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &I) const;

void paint(QPainter *painter,
           const QStyleOptionViewItem &option,
           const QModelIndex &I) const;
};


/********************************************************/

class Model : public QSqlQueryModel {

    Q_OBJECT

private:
    QVariant fAuthor;
    QVariant fTitle;
    QVariant fYear;
    QVariant fCatId;
    void adjust_query();

public:
    Model(QObject *parent=0);
    virtual ~Model();

    int columnCount(const QModelIndex &) const {return 12;}

public slots:
    void cat_item_selected(QVariant Id);
    void apply_filter(QObject *F);
};

/********************************************************/

class Model_EditOnServer : public QSqlTableModel {

    Q_OBJECT

private:
    QVariant fAuthor;
    QVariant fTitle;
    QVariant fYear;
    QVariant fCatId;
    void adjust_query();

    QAction *actDeleteRow;
    QAction *actNewRow;
    QAction *actSaveAll;
    QAction *actRestoreAll;

    // slots are not done private
protected slots:
    void on_delete_row();
    void on_new_row();
    void on_save_all();
    void on_restore_all();

public:
    QMap<int,QString> AllStatus;
    QList<QAction*> AllActions;

public:
    Model_EditOnServer(QObject *parent=0);
    virtual ~Model_EditOnServer();

    QVariant data(const QModelIndex &I, int role) const;
    bool setData(const QModelIndex &I, const QVariant &val, int role);
    int columnCount(const QModelIndex &) const {return 12;}
    Qt::ItemFlags flags(const QModelIndex &I) const;

public slots:
    void cat_item_selected(QVariant Id);
    void apply_filter(QObject *F);
};

/********************************************************/

class View : public QTableView {

    Q_OBJECT

public:
    View(QWidget *parent=0);
    virtual ~View();

};

/********************************************************/

} // namespace Books
} // namespace STORE

#endif // BOOKS_H
