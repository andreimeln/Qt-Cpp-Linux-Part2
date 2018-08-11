#include "catitemedit.h"

#include <QtSql>

namespace STORE {
namespace Catalogue {
namespace Item {

/*******************************************************************/

Data *List::findPointer(int Id) const {

    Data *D;
    foreach(D, *this) {
        bool OK;
        int cId = D->Id.toInt(&OK);
        if (OK && cId == Id)
            return D;
        Data *R = D->Children.findPointer(Id);
        if (R)
            return R;
    }
    return 0;
}

/*******************************************************************/

Data::Data(QObject *parent, QSqlQuery &qry)
    : QObject(parent) {

    Id = qry.value("iid");
    Code = qry.value("code").toString();
    Title = qry.value("title").toString();
    From = qry.value("valid_from").toDateTime();
    To = qry.value("valid_to").toDateTime();
    IsLocal = qry.("islocal").toBool();
    Comment = qry.value("acomment").toString;
    pParentItem = 0;
    Deleted = false;
    Changed = false;
    //"   rid_parent,\n"
    //"   alevel\n"

}

bool Data::isActive() const {
    bool Active = true;
    if (From.isValid()) {
        if (From > QDateTime::currentDateTime())
            return false;
    }
    if (To.isValid()) {
        if (To < QDateTime::currentDateTime())
            return false;
    }
    return true;
}

bool Data::isNew() const {

    if (! Id.isValid())
        return true;
    if (Id.isNull())
        return true;
    return false;
}

bool Data::isSameAs(Data *D) const {

    if (isNew()) {
        if (D->isNew())
            return false;
        return property("temp_id") == D->property("temp_id"); // dynamic property
    } else {
        if (D->isNew())
            return false;
        return D->Id == Id; // !!
    }
}

/*******************************************************************/

Frame::Frame(QWidget *parent)
    : QFrame(parent) {

    ui.setupUi(this);
    Block = 0;

}

Frame::~Frame() {

}

void Frame::load() {
    if (!Block)
        return;

    ui.edtCode->setText(Block->Code);
    ui.edtTitle->setText(Block->Title);
    ui.cbxLocal->setChecked(Block->isLocal);
    ui.edtComment->setText(Block->Comment);
    ui.cbxFromEnabled->setChecked(Block->From.isValid);
    ui.edtFrom->setDateTime(Block->From);
    ui.cbxToEnabled->setChecked(Block->To.isValid);
    ui.edtTo->setDateTime(Block->To);
}

bool Frame::save() {

    /*
     * moved to dialogtpl.cpp
     *
    bool OK = false;
    if (!Block)
        return false;
    is_good(&OK);
    if (!OK)
        return false;
        */

    if (!Block)
        return false;

    Block->Code = ui.edtCode->text().simplified();
    Block->Title = ui.edtTitle->text().simplified();
    Block->isLocal = ui.cbxLocal->isChecked();
    Block->Comment = ui.edtComment->toPlainText().trimmed();
    if (ui.cbxFromEnabled->isChecked()) {
        Block->From = ui.edtFrom->dateTime();
    } else {
        Block->From = QDateTime();
    }
    if (ui.cbxToEnabled->isChecked()) {
        Block->To = ui.edtTo->dateTime();
    } else {
        Block->To = QDateTime();
    }

    Block->Changed = true;
    return true;
}

/** TODO: do proper data check **/
void Frame::is_good(bool *pOK) {
    *pOK = true;
}

/*******************************************************************/

Dialog::Dialog(QWidget *parent)
   : CommonDialog(parent) {

    //{
    Frame *F = pFrame = new Frame(this);
    setCentralFrame(F);
    //}
}

Dialog::~Dialog() {

}

/*******************************************************************/

} // namespace Item
} // namespace Catalogue
} // namespace STORE
