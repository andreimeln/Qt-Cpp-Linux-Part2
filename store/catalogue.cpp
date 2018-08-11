#include "catalogue.h"
#include "posaction.h"

#include <Qt>
#include <QtDebug>
#include <QAction>
#include <QMenu>
#include <QtSql>
#include <QMessageBox>

#define DATA_PTR(D,I,VALID)  \
    Item::Data *D = (Item::Data*)(I.internalPointer()); \
    if (!D) return VALUE

#define DATE_STR(DATE) ( \
    DATE.isValid() ? DATE.toString("dd.MM.yyyy") : QString())

namespace STORE {
namespace Catalogue {

/************************************************************/

/*
class PositionedAction : public QAction {
    Q_OBJECT
};
*/

/************************************************************/

Model::Model(QObject *parent)
//    : QAbstractTableModel(parent) {
    : QAbstractItemModel(parent), LastTempId(1) {


//    QSqlQuery qry(
    QSqlQuery qry;
    qry.setForwardOnly(true);
    qry.prepare(
                "select\n"
                "   iid,\n"
                "   code,\n"
                "   title,\n"
                "   valid_from,\n"
                "   valid_to,\n"
                "   islocal,\n"
                "   acomment,\n"
                "   rid_parent,\n"
                "   alevel\n"
                "from catalogue\n"
                "order by alevel;\n"
                ); // order by alevel - parents loaded before children
    if (qry.exec()) { // exec() no longer needed here,
    while (qry.next()) {
        bool OK = false;
        QVariant PI = qry.value("rid_parent");
        //bool N = PI.isNull();
        //int Id = qry.value("iid").toInt(); // guarantees it's a number
        int ParentId = PI.toInt(&OK); // guarantees it's a number
       Item::Data *P = 0;
       if (OK && !PI.isNull())
           P = Cat.findPointer(ParentId); // parent
        if (P) {
            Item::Data *D = new Item::Data(P,qry);
            P->Children.append(D);
            D->pParentItem = P;
        } else {
            Item::Data *D = new Item::Data(this);
            Cat.append(D);
            D->pParentItem = 0;
        }
    }
    } else {
       //qCritical() << "Cannot execute query";
        QSqlError Err = qry.lastError();
        qCritical() << Err.nativeErrorCode();
        qCritical() << Err.databaseText().toUtf8().data();
        qCritical() << Err.driverText().toUtf8().data();
        //qDebug() << qry.executedQuery();
    }

    /*
    // TODO This is test catalog. Replace with a proper one
    {
    Item::Data *D = new Item::Data(this); // it has no constructor, inherits one from QObject
    D->Code = "111";
    D->Title = "Physics";
    D->From = QDateTime::currentDateTime();
    D->To = QDateTime();
    D->isLocal = false;
    Cat.append(D);
    }

    {
    Item::Data *D = new Item::Data(this); // it has no constructor, inherits one from QObject
    D->Code = "222";
    D->Title = "Mathematics";
    D->From = QDateTime::currentDateTime();
    D->To = QDateTime();
    D->isLocal = false;
    Cat.append(D);
    }

    {
    Item::Data *D = new Item::Data(this); // it has no constructor, inherits one from QObject
    D->Code = "333";
    D->Title = "Biology";
    D->From = QDateTime::currentDateTime();
    D->To = QDateTime();
    D->isLocal = false;
    Cat.append(D);
    }

    {
    Item::Data *D = new Item::Data(this); // it has no constructor, inherits one from QObject
    D->Code = "444";
    D->Title = "Valeology";
    D->From = QDateTime::currentDateTime();
    D->To = QDateTime();
    D->isLocal = false;
    Cat.append(D);
    }

    {
    Item::Data *D = new Item::Data(this); // it has no constructor, inherits one from QObject
    D->Code = "555";
    D->Title = "Literature";
    D->From = QDateTime::currentDateTime();
    D->To = QDateTime();
    D->isLocal = false;
    D->Comment = "Check if correct";
    Cat.append(D);
    }
    */
}

Model::~Model() {
}

int Model::rowCount(const QModelIndex &parent) const {
    if(! parent.isValid()) {
        //qDebug() << parent << Cat.size();
        return Cat.size(); // TODO Replace with correct number
    } else {
        //Item::Data *P = (Item::Data *)parent.internalPointer();
        //if (!P)
        //    return 0;
        DATA_PTR(P,parent,0);
        //qDebug() << parent << P->Children.size();
        return P->Children.size();
    }
}

int Model::columnCount(const QModelIndex &parent) const {

    //if(! parent.isValid())
        return 6; // TODO Replace with correct number
    //DATA_PTR(P, parent, 0);
    //return P->Children.size() > 0 ? 6 : 0;
}

QVariant Model::dataDisplay(const QModelIndex &I) const {

    //Item::Data *D = Cat[I.row()];
    DATA_PTR(D,I,QVariant());
    //qDebug() << I.row() << I.column() << D->Code;
    switch(I.column()) {
    // for table and tree view
    //case 0: return D->Code;
    //case 1: return D->Title;
    // for column view
    case 0: return D->Title;
    case 1: return D->Code;
    //case 2: return D->From.isValid ? D->From.toString("dd.MM.yyyy") : "";
    //case 3: return D->To.isValid ? D->From.toString("dd.MM.yyyy") : "";
    case 2: return DATE_STR(D->From);
    case 3: return DATE_STR(D->To);
    case 4: return D->IsLocal ? tr("LOCAL") : QString();
    case 5: return D->Comment.isEmpty() ? QString : tr("CMT");
    default: return QVariant();
    }

}

QVariant Model::dataTextAlignment(const QModelIndex &I) const {

    int Result = Qt::AlignVCenter;
    Result |= I.column() == 0 ? Qt::AlignLeft : Qt::AlignHCenter;
    return Result; //QVariant();
}

/*
Item::Data *Model::dataDataBlock(const QModelIndex &I) const {

    int R = I.row();
    if (R < 0 || R >= Cat.size() )
        return 0;
    return Cat[R];
}
*/

QVariant Model::dataFont(const QModelIndex &I) const {
    //Item::Data *D = dataDataBlock(I);
    //if (!D)
    //    return QVariant(); // use default value
    DATA_PTR(D,I,QVariant());
    QFont F;
    if (D->Deleted)
        F.setStrikeOut(true);
    //F.setItalic(true);
    if (D->Changed)
        F.setItalic(true);
    return F;
}

QVariant Model::dataForeground(const QModelIndex &I) const {
    //Item::Data *D = dataDataBlock(I);
    //if (!D)
     //   return QVariant(); // use default value
    DATA_TR(D,I,QVariant());
    //if (!D->To.isValid())
    //    return QVariant();
    /*
    QColor Result;
    if (D->To > QDateTime::currentDateTime()) {
        if (D->IsLocal) {
            Result = QColor("blue"); // #0000FF
        } else {
        Result = QColor("black"); // #000000
        }
    } else {
        Result = QColor("#AAAAAA"); // gray; QVariant();
    }
    */
    QColor Result = D->IsLocal ? QColor("blue") : QColor("black");
    if (! D->isActive())
        Result.setAlphaF(1.0/3.0); //transparency - make pale
    return Result;
}

QVariant Model::dataBackground(const QModelIndex &I) const {
    DATA_TR(D,I,QVariant());
    if (! D->isNew())
        return QVariant;
    QColor Result = QColor("green");
    Result.setAlphaF(1.0/10.0);
    return Result;
}

QVariant Model::dataToolTip(const QModelIndex &I) const {
    //Item::Data *D = dataDataBlock(I);
    //if (!D)
    //    return QVariant();
    DAPA_PTR(D,I,QVariant());
    //switch(I.column()) {
    //case 2: {
        if (!D->To.isValid())
            return QVariant();
        return tr("Valid to: %1".arg(D->To.toString("dd.MM.yyyy")));
    //}
    //default:
    //    return QVariant();
    //}

    //return QVariant("ToolTip");
}

QVariant Model::data(const QModelIndex &I, int role) const {

    //if ( role != Qt::DisplayRole)
    //    return QVariant();

    //return QString("%1,%2").arg(I.row()).arg(I.column());
    /*
    Item::Data *D = Cat[I.row()];
    switch(I.column()) {
    case 0: return D->Code;
    case 1: return D->Title;
    case 2: return D->From;
    case 3: return D->To;
    case 4: return D->isLocal;
    case 5: return D->Comment;
    }
    */
    //return QVariant();

    switch (role) {
    case Qt::DisplayRole : return dataDisplay(I);
    case Qt::TextAlignmentRole : return dataTextAlignment(I);
    case Qt::ForegroundRole : return dataForeground(I);
    case Qt::FontRole : return dataFont(I);
    case Qt::ToolTipRole : return dataToolTip(I);
    case Qt::BackgroundRole : return dataBackground(I);
    //case Qt::UserRole : return QVariant(dataDataBlock(i));
    case Qt::UserRole+1 : {
        //Item::Data *D = dataDataBlock(I);
        //if (! D)
        //    return false;
        DATA_PTR(D,I,false);
        return D->Deleted;
    }
    default: return QVariant();
    }
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const {

    if(orientation != Qt::Horizontal)
        return QAbstractItemModel::headerData(section,orientation,role);

    switch(role) {
    case Qt::DisplayRole :
        switch(section) {
        // old order forTableView
        //case 0: return tr("Code");
        //case 1: return tr("Title");
        case 0: return tr("Title");
        case 1: return tr("Code");
        case 2: return tr("From");
        case 3: return tr("To");
        case 4: return tr("Local"); //? tr("LOCAL") : QString(); // blank QString
        case 5: return QVariant(); //D->Comment.isEmpty() ? QString() : tr("CMT");
    }
    case Qt::TextAlignmentRole :
        return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);

    case Qt::ForegroundRole:
    { // TODO make font bold
        QFont F;
        F.setBold(true);
        return F;
    }

    default: return QVariant();

    }
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const {

    if (parent.isValid()) {
        DAPA_PTR(D,parent,QModelIndex());
        //Item::Data *D = (Item::Data *)
        if (row < 0 || row >= D->Children.size())
            return QModelIndex();
        return createIndex(row, column, (void *)D->Children[row]);
        //return QModelIndex();
    } else {
        if (row < 0 || row >= Cat.size())
            return QModelIndex();
        return createIndex(row, column, (void *)Cat[row]); // (void*)0 - '0' here is pointer
    }
}

QModelIndex Model::parent(const QModelIndex &I) const {

    DATA_PTR(D,I,QModelIndex());
    Item::Data *P = D->pParentItem;
    if (!P) {
        return QModelIndex();
    //} else {
    int Row = -1; // get pointer to grandfather
    Item::Data *GP = P->pParentItem;
    if (GP) {
        for (int i =0; i < GP->Children.size();i++) {
            if (GP->Children[i]->isSameAs(P)) { // index of parent in grandfather context
                Row = i;
                break;
            }
        }
    } else { // if parent has no its own parent
        for (int i =0; i < Cat.size();i++) {
            if (Cat[i]->isSameAs(P)) {
                Row = i;
                break;
            }
    }
        if (Row < 0) {
            qWarning() << "Cannot find item";
            return QModelIndex();
        }
    return createIndex(Row,0,O);
    }
}

}

void Model::editItem(const QModelIndex &I, QWidget *parent) {
    DATA_PTR(D,T,);
    Item::Dialog Dia(parent);
    //Item::Data *D = dataDataBlock(I);
    //Item::Data *D = (Item::Data*)(I.internalPointer());
    //if (!D)
    //    return;
    Dia.setDataBlock(D);
    beginResetModel();
    Dia.exec();
    endResetModel(); // and now we can edit data
}

void Model::newItem(const QModelIndex &parentI, QWidget *parent) {
    //if (parentI.isValid()) {
        // TODO adding a new element, not necessarily to the root directory
    //    qWarning << "Cannot add non-toplevel item";
    //    return;
    //}
    Item::Data *P = 0;
    if (parentI.isValid()) {
        P = (Item::Data*)(parentI.internalPointer());
        if (!P) {
            qWarning() << "Invalid internal pointer";
            return;
        }
    }
    Item::Data = new Item::Data(this);
    if (!D) {
        qWarning() << "Cannot create new item";
    }

    Item::Dialog Dia(parent);
    Dia.setDataBlock(D);
    if (Dia.exec() == QDialog::Accepted) {
        beginResetModel();
        if (P) {
            P->Children.append(D);
            D->pParentItem = P;
        } else {
            Cat.append(D);
        }
        Cat.append(D);
        D->setProperty("temp_id",tempId());
        endResetModel();
    } else {
        delete D;
    }
}

void Model::delItem(const QModelIndex &I, QWidget *parent) {
    // TODO ask user if they are sure they want to delete the element
    //Item::Dialog Dia(parent);
    //Item::Data *D = dataDataBlock(I);
    //if (!D)
    //    return;
    DATA_PTR(D,I,); // par 3 - blank line
    // TODO model is supposed to be linear
    beginResetModel();
    //if (D->Id.isNull() || D->Id.isValid()) { // then this element is new
    if (D->isNew()) {
        Item::Data *P = D->pParentItem;
        if (P) {
            P->Children.removeAt(I.row());
        } else {
            Cat.removal(I.row);
        }
        delete D;
    } else {
        D->Deleted = ! D->Deleted; //true;
    }
    endResetModel();
}

bool Model::delete_all_from_db(Item::Data *D) {

    Item::List *Children = D ? &(D->Children) : &Cat; // pointer to ...
    Item::Data *X;
    foreach (X,*Children) {
        if (! delete_all_from_db(X))
            return false;
    }
    if (!D)             // there's nothing to delete
        return true;
    if (!D->Deleted)    // not marked for deletion
        return true;
    { // to avoid simultanious db queries
     QSqlQuery DEL;
     DEL.setForwardOnly(true);
     DEL.prepare('delete from catalogue where iid = :IID;'); // SQL query wth parameters; watch out spaces next to ':'
     DEL.bindValue(":IID",D->Id);
     if (DEL.exec())
         return true;
     qCritical() << DEL.lastError().databaseText();
     qCritical() << DEL.lastError().driverText();
     qCritical() << DEL.lastError().nativeErrorCode();
    }
    return false;
}

bool Model::delete_all_from_model(Item::Data *D) {

    Item::List *Children = D ? &(D->Children) : &Cat; // pointer to ...
    bool Result = true;
    beginResetModel();
    for (int k = Children->size()-1; k >= 0; k--) {
        if (Children->at(k)->Deleted) {
            Item::Data *X= Children->takeAt(k); // remove from the list
            delete X;
        } else {
            if (!delete_all_from_model(Children->at(k))) {
                Result = false;
                break;
            }
        }
    }
    endResetModel();
    return Result;
}

bool Model::delete_all() {

    //bool R = true;
    QSqlDatabase DB = QSqlDatabase::database();
    DB.transaction();
    //R = R && delete_all_from_db();
    //R = R && delete_all_from_model();
    if (delete_all_from_db()) {
        DB.commit();
        return delete_all_from_model();
    } else {
        DB.rollback();
        return false;
    }
}

void Model::save(void) {
    //qDebug() << "Model::save()";
    // Delete all elements marked for deletion
    // Save all changed elements
    // Save all new elements
    // Remove mark "changed" from all elements
    try {
        if (! delete_all())
            throw (int)1;
        if (!save_all())
            throw (int)2;
        if (!insert_all())
            throw (int)3;
        QMessageBox::information(0, tr("Info"), tr("Changed saved successfully"));
    } catch (int X) {
        QMessageBox::critical(0,tr("Error"),tr("Cannot save changes"));
    }
}

bool Model::save_all() {

    QSqlDatabase DB = QSqlDatabase::database();
    DB.transaction();
    if (save_all_to_db()) {
        DB.commit();
        return drop_change_mark();
    } else {
        DB.rollback();
        return false;
    }

}

bool Model::save_all_to_db(Item::Data *D) {

    Item::List *Children = D ? &(D->Children) : &Cat; // pointer to ...
    Item::Data *X;
    foreach (X,*Children) {
        if (! save_all_from_db(X))
            return false;
    }
    if (!D)
        return true;
    if (!D->Changed)
        return true;
    {
     QSqlQuery UPD;
     UPD.setForwardOnly(true);
     UPD.prepare("update catalogue set \n"
                "  code = :CODE,\n"
                "  title = :TITLE,\n"
                "  valid_from = :FROM,\n"
                "  valid_to = :TO,\n"
                "  islocal = :LOCAL,\n"
                "  acomment = :COMMENT\n"
                "where iid = :IID");
     UPD.bindValue(":CODE", D->Code);
     UPD.bindValue(":TITLE",D->Title);
     UPD.bindValue(":FROM", D->From);
     UPD.bindValue(":TO", D->To);
     UPD.bindValue(":LOCAL", D->IsLocal);
     UPD.bindValue(":COMMENT", D->Comment);
     UPD.bindValue(":IID", D->Id);
     if (UPD.exec())
         return true;
     qCritical() << UPD.lastError().databaseText();
     qCritical() << UPD.lastError().driverText();
     qCritical() << UPD.lastError().nativeErrorCode();
    }
    return false;

}

bool Model::drop_change_mark(Item::Data *D) {

    Item::List *Children = D ? &(D->Children) : &Cat;
    Item::Data *X;
    foreach (X,*Children) {
        X->Changed = false;
        drop_change_mark(X);
    }
    return true;
}

bool Model::insert_all_to_db(Item::Data *D) {

    bool must_be_saved = D ? D->isNew() : false;
    if (must_be_saved) {
        QSqlQuery INS;
        INS.setForwardOnly(true);
        INS.prepare("insert into catalogue ( \n"
                "  code,\n"
                "  title,\n"
                "  valid_from,\n"
                "  valid_to,\n"
                "  islocal,\n"
                "  acomment,\n"
                "  rid_parent"
                ") values (\n"
                "  :CODE,\n"
                "  :TITLE,\n"
                "  :FROM,\n"
                "  :TO,\n"
                "  :LOCAL,\n"
                "  :COMMENT,\n"
                "  :PARENT \n"
                ") returning iid,\n"
                "  code, title,\n"
                "  valid_from, valid_to,\n"
                "  islocal, acomment,\n"
                "  rid_parent, alevel; \n"
                    );
        INS.bindValue(":CODE", D->Code);
        INS.bindValue(":TITLE",D->Title);
        INS.bindValue(":FROM", D->From);
        INS.bindValue(":TO", D->To);
        INS.bindValue(":LOCAL", D->IsLocal);
        INS.bindValue(":COMMENT", D->Comment);

        QVariant IdParent = QVariant();
        if (D->pParentItem) {
            if (D->pParentItem->isNew()) { // new parent has no ID yet
                IdParent = D->pParentItem->property("new_id");
            } else {
                IdParent = D->pParentItem->Id;
            }
        }
        INS.bindValue(":PARENT", IdParent);
        if (INS.exec())
            return true;
        qCritical() << INS.lastError().databaseText().toUtf8().data();
        qCritical() << INS.lastError().driverText();
        qCritical() << INS.lastError().nativeErrorCode();
        return false;
       }
    while (INS.next()) {
        D->Code = INS.value("code").toString();
        D->Title = INS.value("title").toString();
        D->From = INS.value("valid_from").toDateTime();
        D->To = INS.value("valid_to").toDateTime();
        D->IsLocal = INS.value("islocal").toBool();
        D->Comment = INS.value("acomment").toString();
        qDebug() << INS.value("rid_parent") << INS.value("alevel");
        D->setProperty("new_id", INS.value("iid")); // set new property
    } {
      Item::List *Children = D ? &(D->Children) : &Cat; // pointer to ...
      Item::Data *X;
      foreach (X,*Children) {
          if (! insert_all_to_db(X))
              return false;
      }
    }
    return true;

}

bool Model::adjust_id_for_new(Item::Data *D) {
    bool must_be_saved = D ? D->isNew() : false;
    if (must_be_saved) {
        D->Id = D->property("new_id");
    }
    Item::List *Children = D ? &(D->Children) : &Cat;
    Item::Data *X;
    foreach (X,*Children) {
        if (! adjust_id_for_new(X))
            return false;
    }
    qDebug() << "Model::adjust_id_for_new";
    return true;
}

bool Model::insert_all() {

    QSqlDatabase DB = QSqlDatabase::database();
    DB.transaction();
    if (insert_all_to_db()) {
        DB.commit();
        return adjust_id_for_new();
    } else {
        DB.rollback();
        return false;
    }
}

/************************************************************/

TableView::TableView(QWidget *parent, Model *xModel)
    : QTableView(parent) {

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequested(QPoint)));

    Model *M = xModel ? xModel : new Model(this);
    setModel(M);

    {
     PosAction *A = actEditItem = new PosAction(this);
     A->setText(tr("Edit"));
     connect(A,SIGNAL(editItem(QModelIndex,QWidget*)), M, SLOT(editItem(QModelIndex,QWidget*)));
     addAction(A);
    }

    {
     PosAction *A = actNewItem = new PosAction(this);
     A->setText(tr("Add"));
     connect(A,SIGNAL(newItem(QModelIndex,QWidget*)), M, SLOT(editItem(QModelIndex,QWidget*)));
     addAction(A);
    }

    {
     PosAction *A = actDelItem = new PosAction(this);
     A->setText(tr("Delete"));
     connect(A,SIGNAL(editItem(QModelIndex,QWidget*)), M, SLOT(delItem(QModelIndex,QWidget*)));
     addAction(A);
    }

    {
     PosAction *A = actRootItem = new PosAction(this);
     A->setText(tr("Show children"));
     connect(A,SIGNAL(editItem(QModelIndex,QWidget*)), this, SLOT(showChildren(QModelIndex,QWidget*)));
     addAction(A);
    }

    {
     QAction *A = actParentRootItem = new QAction(this);
     A->setText(tr("Show parent"));
     connect(A,SIGNAL(triggered()), this, SLOT(showParent()));
     addAction(A);
    }

    {
     QAction *A = actSave = new QAction(this);
     A->setText(tr("Save"));
     connect(A,SIGNAL(triggered()), this, SLOT(save()));
     addAction(A);
    }

    {
     QHeaderView *H = verticalHeader();
     H->setSectionResizeMode(QHeaderView::ResizeToContents); // resize vertical lines
    } {
     QHeaderView *H = horizontalHeader();
     H->setSectionResizeMode(QHeaderView::ResizeToContents);
     H->setSectionResizeMode(0, QHeaderView::Stretch);
    }

    setColumnHidden(3,true);
    setColumnHidden(4,true);
}

TableView::~TableView() {

};

//void TableView::editItem() {

    /*
    Item::Dialog Dia(this);
    Dia.exec();
    */
//}

void TableView::contextMenuRequested(const QPoint &p) {

    QMenu M(this);
    QModelIndex I = indexAt(p);
    if (I.isValid()) {
        actEditItem->I = I;
        actEditItem->pWidget = this;
        M.addAction(actEditItem);
        actDelItem->I = I;
        actDelItem->pWidget = this;
        if (I.data(!t::UserRole+1).toBool()) {
            actDelItem->setText(tr("Restore"));
        } else {
            actDelItem->setText(tr("Delete"));
        }
        M.addAction(actDelItem);
    }
    actNewItem->I = rootIndex;
    actNewItem->pWidget = this;
    M.addAction(actNewItem);
    if (I.isValid()) {
        actRootItem->I = I;
        actRootItem->pWidget = this;
        M.addAction(actRootItem);
    }
    if (rootIndex().isValid())
        M.addAction(actParentRootItem);
    M.addAction(actSave);
    M.exec(mapToGLobal(p));
}

void TableView::showChildren(const QModelIndex &I, QWidget *) {
    if (rootIndex().isValid())
        setRootIndex(I);
}

void TableView::showParent(void) {
    setRootIndex(rootIndex().parent());
}

/************************************************************/

TreeView::TreeView(QWidget *parent, Model *xModel)
    : QTreeView(parent) {

    Model *M = xModel ? xModel : new Model(this);
    setModel(M);
    {
     QHeaderView *H = header();
     H->setSectionResizeMode(QHeaderView::ResizeToContents);
     H->setSectionResizeMode(1, QHeaderView::Stretch);
     //H->setSectionResizeMode(5, QHeaderView::Fixed);
    }
}

    setColumnHidden(3,true);
    setColumnHidden(4,true);
    //setColumnHidden(5,true);
}

TreeView::~TreeView() {

}


/************************************************************/

ColumnView::ColumnView(QWidget *parent, Model *xModel)
    : QColumnView(parent) {

    Model *M = xModel ? xModel : new Model(this);
    setModel(M);

    QList<int> L;
    L << 150;
    for (int k = 0; k < 10; k++) {
        L << 200;
    }
    setColumnWidths(L);

}

ColumnView::~ColumnView() {

}

void ColumnView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {

    QColumnView::currentChanged(current, previous);
    if (! current.isValid()) {
        emit item_selected(QVariant());
        return;
    }
    Item::Data *D = (Item::Data*)(current.internalPointer());
    if (!D) {
        emit item_selected(QVariant());
        return;
    }
    emit item_selected(D->Id);
    //qDebug() << previous << current;
}


/************************************************************/

} // namespace Catalogue
} // namespace STORE
