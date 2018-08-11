#include "books.h"

#include <QtSql>
#include <QtDebug>
#include <QHeaderView>
#include <QAction>
#include <QComboBox>
#include <QPainter>

namespace STORE {
namespace Books {

//anonymous namespace, to prevent using this function in other files
namespace {
void REPORT_ERROR(QSqlQuery &QUERY) {
    qDebug() << QUERY.executedQuery().toUtf8().data(); \
    qCritical() << QUERY.lastError().databaseText().toUtf8().data(); \
}
} // namespace

/********************************************************/

StatusDelegate::StatusDelegate(QObject *parent, const QMap<int,QString> &fAllStatus) {

}

// for non-standart editing of model
// not necessary to re-define, the default function would do as well
QWidget *StatusDelegate::createEditor(QWidget *parent,
                  const QStyleOptionViewItem &option,
                  const QModelIndex &I) const {

    QComboBox *CB = new QComboBox(parent);
    QMapIterator<int,QString> K(fAllStatus);
    CB->addItem(QString(), QVariant()); // blank status
    while (K.hasNext()) {
        K.next();
        CB->addItem(K.value(), K->key());
    }
    return CB;

}

// needed to be re-defined
void StatusDelegate::setEditorData(QWidget *editor,
               const QModelIndex &I) const {

    // editor is ComboBox
    QComboBox *CB = qobject_cast<QComboBox*>(editor);
    if (!CB)
        return;

    QVariant IdStatus = I.data(Qt::EditRole);
    if (! IdStatus.isValid()) {
        CB->setCurrentIndex(0);
        return;
    }

    for (int k = 1; k < CB->count(); k++) {
        if (CB->itemData(k) == IdStatus) {
            CB->setCurrentIndex(k);
            break;
        }
    }

}
void StatusDelegate::setModelData(QWidget *editor,
              QAbstractItemModel *model,
              const QModelIndex &I) const {

    QComboBox *CB = qobject_cast<QComboBox*>(editor);
    if (!CB)
        return;

    model->setData(I,CB->currentData(), Qt::EditRole);
}

// zashtrixovat
void StatusDelegate::paint(QPainter *painter,
           const QStyleOptionViewItem &option,
           const QModelIndex &I) const {

    QItemDelegate::paint(painter, option, I);
    if (I.data(Qt::EditRole) != -2)
        return;
    painter->setBrush(Brush(QColor("black"), Qt::DiagCrossPattern));
    painter->setPen(Qt::NoPen);
    painter->drawRect(option.rect);
}

/********************************************************/

Model::Model(QObject *parent)
    : QSqlQueryModel(parent) {

}

void Model::cat_item_selected(QVariant Id) {
/*
        QSqlQuery qry;
        qry.prepare(
            "select \n"
            "   b.iid,\n"
            "   b.rid_catalogue,\n"
            "   b.author,\n"
            "   b.title,\n"
            "   b.eyear,\n"
            "   b.location,\n"
            "   b.publisher,\n"
            "   b.pages,\n"
            "   b.annote,\n"
            "   b.rid_status,\n"
            "   s.title,\n"
            "   b.acomment\n"
            "from books b;\n"
            "left outer join status s\n"
            "   on b.rid_status = s.iid\n"
            "where b.rid_catalogue = :CID;\n"
        );
        qry.bindValue(":CID", Id);
        if (! qry.exec()) {
            //qCritical() << qry.lastError().databaseText().toUtf8().data();
        }
        setQuery(qry);
        */
    fCatId = Id;
    adjust_query();

}

void Model::apply_filter(QObject *F) {

    fAuthor = F->property("author");
    fTitle = F->property("title");
    fYear = F->property("year");
    adjust_query();
}

Model::~Model() {

}

void Model::adjust_query() {

    QString QueryText =
            "select \n"
            "   b.iid,\n"
            "   b.rid_catalogue,\n"
            "   b.author,\n"
            "   b.title,\n"
            "   b.eyear,\n"
            "   b.location,\n"
            "   b.publisher,\n"
            "   b.pages,\n"
            "   b.annote,\n"
            "   b.rid_status,\n"
            "   s.title,\n"
            "   b.acomment\n"
            "from books b;\n"
            "left outer join status s\n"
            "   on b.rid_status = s.iid\n"
            "where b.rid_catalogue = :CID\n";
    if (fAuthor.isValid())
        QueryText += "     and b.author ~ :AUTHOR \n";
    if (fTitle.isValid())
        QueryText += "     and b.title ~ :TITLE \n";
    if (fYear.isValid())
        QueryText += "     and (b.eyear - :YEAR) \n";
    QueryText += ";\n";
    QSqlQuery qry;
    qry.prepare(QueryText);
    qry.bindValue(":CID", fCatId);
    if (fAuthor.isValid())
        qry.BindValue(":AUTHOR", "^"+fAuthor.toString());
    if (fTitle.isValid())
        qry.BindValue(":TITLE", "^"+fTitle());
    if (fYear.isValid())
        qry.BindValue(":YEAR", "^"+fYear());
    if (! qry.exec()) {
        qDebug() << qry.executedQuery();
        qCritical() << qry.lastError().databaseText().toUtf8().data();
    }
    setQuery(qry);

}

/********************************************************/

Model_EditOnServer::Model_EditOnServer(QObject *parent)
    : QSqlTableModel(parent) {

    setEditStrategy(OnFieldChange);

    // created in M, not V
    {
     QAction *A = actDeleteRow = new QAction(this);
     A->setText(tr("Delete"));
     connect(A,SIGNAL(triggered()), this, SLOT(on_delete_row()));
     AllActions << A;
    }{
     QAction *A = actNewRow = new QAction(this);
     A->setText(tr("New"));
     connect(A,SIGNAL(triggered()), this, SLOT(on_new_row()));
     AllActions << A;
    }{
     QAction *A = actSaveAll = new QAction(this);
     A->setText(tr("Save"));
     connect(A,SIGNAL(triggered()), this, SLOT(on_save_all()));
     AllActions << A;
    }{
     QAction *A = actRestoreAll = new QAction(this);
     A->setText(tr("Restore"));
     connect(A,SIGNAL(triggered()), this, SLOT(on_restore_all()));
     AllActions << A;
    }

    {
     QSqlQuery QUERY;
     QUERY.prepare("select iid, title from status;");
     bool OK = QUERY.exec();
     if (!OK) {
         REPORT_ERROR(QUERY);
         return
     }
     while (QUERY.next()) {
         Int Id = QUERY.value("iid").toInt();
         AllStatus[Id] = QUERY.value("title").toString();
     }
    }

}

void Model_EditOnServer::cat_item_selected(QVariant Id) {

    fCatId = Id;
    qDebug() << "cat_item_selected" << fCatId;
    adjust_query();

}

void Model_EditOnServer::apply_filter(QObject *F) {

    fAuthor = F->property("author");
    fTitle = F->property("title");
    fYear = F->property("year");
    qDebug() << "apply_filter" << fAuthor << fTitle << fYear;
    adjust_query();

}

Model_EditOnServer::~Model_EditOnServer() {

}

void Model_EditOnServer::on_delete_row() {
    qDebug << "on_delete_row";
}

void Model_EditOnServer::on_new_row() {
    //insertRecord(0, record());
    insertRow(0);
    setData(index(0,1), fCatId, Qt::EditRole);
    qDebug << "on_new_row";
}

void Model_EditOnServer::on_save_all() {
    qDebug << "on_save_all";
}

void Model_EditOnServer::on_restore_all() {
    qDebug << "on_restore_all";
}

void Model_EditOnServer::adjust_query() {

    setTable(QString ()); // switch off the table

    { // here we kill the temporary table in case it has existed
      QSqlQuery Drop;
      //Drop.prepare("drop table if exists my_books;");
      bool OK = Drop.exec("drop table if exists my_books;");
      //if (!Drop.exec()) {
      if (!OK) {
          REPORT_ERROR(DROP);
          return;
      }
    }{
      QSqlQuery CREATE;
      bool OK = CREATE.exec(
      "create temporary table my_books(\n"
      "   iid bigint primary key,\n"  // SIC no 'not null'
      "   rid_catalogue bigint,\n"
      "   author text,\n"
      "   title text,\n"
      "   eyear int,\n"
      "   location text,\n"
      "   publisher text,\n"
      "   pages int,\n"
      "   annote text,\n"
      "   rid_status bigint,\n"
      "   status_title text,\n"
      "   acomment text\n"
      "); \n"
      );
      if (! OK) {
          REPORT_ERROR(CREATE);
          return;
      }
    } {

       QString QueryText =
            //"create temporary table my_books as \n"
            "insert into my_books\n"
            "   iid,\n"
            "   rid_catalogue,\n"
            "   author,\n"
            "   title,\n"
            "   eyear,\n"
            "   location,\n"
            "   publisher,\n"
            "   pages,\n"
            "   annote,\n"
            "   rid_status,\n"
            "   status_title,\n"
            "   acomment\n"
            ")                  "
            "select \n"
            "   b.iid,\n"
            "   b.rid_catalogue,\n"
            "   b.author,\n"
            "   b.title,\n"
            "   b.eyear,\n"
            "   b.location,\n"
            "   b.publisher,\n"
            "   b.pages,\n"
            "   b.annote,\n"
            "   b.rid_status,\n"
            //"   s.title as status_title,\n"
            "   s.title,\n"
            "   b.acomment\n"
            "from books b\n"
            "left outer join status s\n"
            "   on b.rid_status = s.iid\n"
            "where b.rid_catalogue = :CID\n"
               ;
    if (fAuthor.isValid())
        QueryText += "     and b.author ~ :AUTHOR \n";
    if (fTitle.isValid())
        QueryText += "     and b.title ~ :TITLE \n";
    if (fYear.isValid())
        QueryText += "     and (b.eyear - :YEAR) \n";
    QueryText += ";\n";

    QSqlQuery qry;
    qry.prepare(QueryText);
    qry.bindValue(":CID", fCatId);
    if (fAuthor.isValid())
        qry.BindValue(":AUTHOR", "^"+fAuthor.toString());
    if (fTitle.isValid())
        qry.BindValue(":TITLE", "^"+fTitle());
    if (fYear.isValid())
        qry.BindValue(":YEAR", "^"+fYear());
    if (! qry.exec()) {
        REPORT_ERROR(qry);
        return;
    }
    //setQuery(qry);
    } // block ended

    setTable("my_books");
    if (! select()) {
        qCritical() << "Error selecting";
    } else {
        qDebug() << "Selected successfully";
    }

}

QVariant Model_EditOnServer::data(const QModelIndex &I, int role) const {

    if (! I.isValid())
        return QSqlTableModel::data(I,role);
    if (role != Qt::EditRole)
        return QSqlTableModel::data(I,role);
    if (I.column != 10)
        return QSqlTableModel::data(I,role);
    //QSqlRecord R = record(I.row());
    //qDebug() << R.isEmpty();
    //if (role == Qt::EditRole && I.column() = 10)
    //    return record(I.row()).value("rid_status");
    //else
    //    return QSqlTableModel::data(I,role);
    return QSqlTableModel::data(index(I.row(),9), role);

}

bool Model_EditOnServer::setData(const QModelIndex &I, const QVariant &val, int role) {

    if (! I.isValid())
        return QSqlTableModel::setData(I,val,role);
    if (role != Qt::EditRole)
        return QSqlTableModel::setData(I,val,role);
    if (I.column != 10)
        return QSqlTableModel::setData(I,val,role);
    //QVariant V = val.toInt();
    //bool Result = QSqlTableModel::setData(index(I.row(),9),val,role); // ID of the status
    bool Result = true;
    if (val.isValid()) {
        bool OK;
        int status_id = val.toInt(&OK);
        if (!OK) {
            qWarning() << "Invalid status" << val;
            return false;
        } else if (! AllStatus.contains(status_id)) {
            qWarning() << "Invalid status" << val;
            return false;
        }
        Result |= QSqlTableModel::setData(index(I.row(),9), val, role);
        Result |= QSqlTableModel::setData(I,AllStatus[status_id], role);
    } else {
        Result |= QSqlTableModel::setData(index(I.row(),9), QVariant(), role);
        Result |= QSqlTableModel::setData(I,QString(),role);
    }
    //qDebug() << val;
    //return QSqlTableModel::data(index(I.row(),9), role);
    return Result;

}

Qt::ItemFlags Model_EditOnServer::flags(const QModelIndex &I) const {

    // NoItemFlags; - grey, ItemIsEnabled - enabled, no edit
    Qt::ItemFlags Result = Qt::ItemIsEnabled || Qt::ItemIsSelectable;
    //if (I.column() != 0 && I.column() != 1 && I.column() != 9 && I.column() != 10)
    //if (I.column() != 0 && I.column() != 9 && I.column() != 10)
    if (I.column() != 0)
        Result |= Qt::ItemIsEditable;
    return Result;
}

/********************************************************/

View::View(QWidget *parent)
    : QTableView(parent) {

    //Model *M = new Model(this);
    Model_EditOnServer *M = new Model_EditOnServer(this);
    setModel(M);

    addActions(M->AllActions);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    setColumnHidden(0, true);
    setColumnHidden(1, true);
    //setColumnHidden(9, true);

    // prevent split long lines
    setWordWrap(false);
    setAlternatingRowColors(true);

    {
      QHeaderView *H = verticalHeader();
      H->setSectionResizeMode(QHeaderView::ResizeToContents);
    }{
      QHeaderView *H = horizontalHeader();
      H->setSectionResizeMode(QHeaderView::ResizeToContents);
      H->setSectionResizeMode(3, QHeaderView::Stretch);
    }

    setItemDelegateForColumn(10, new StatusDelegate(this,M->AllStatus));
}

View::~View() {

}

/********************************************************/



} // namespace Books
} // namespace STORE

