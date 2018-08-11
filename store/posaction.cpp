#include "posaction.h"

namespace STORE {

PosAction::PosAction(QObject *parent)
    : QAction(parent)
{
    connect(this, SIGNAL(triggered()), this, SLOT(was_triggered()));
}

void PosActions::was_triggered() {
    emit editItem(I, pWidget);
}

//void PosActions::editItem(const QModelIndex &I, QWidget *parent);

} // namespace STORE
