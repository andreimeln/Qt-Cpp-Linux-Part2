#include "mainwindow.h"
#include "catalogue.h"
#include "filter.h"

#include <QDockWidget>
#include "books.h"

namespace STORE {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Catalogue::TableView *W = new Catalogue::TableView(this);
    //Catalogue::TreeView *W = new Catalogue::TreeView(this);
    //Catalogue::ColumnView *W = new Catalogue::ColumnView(this);

    Books::View *B = new Books::View(this);
    setCentralWidget(B);

    //Catalogue::Model *M = 0;
    Catalogue::ColumnView *W = 0;
    Filter *F = 0;

    //setCentralWidget(W);
    {
      QDockWidget *D = new QDockWidget(this);
      D->setWindowTitle(tr("Catalogue"));
      W = new Catalogue::ColumnView(D);
      //D->setWidget(new Catalogue::ColumnView(D,M));
      setWidget(W);
      addDockWidget(Qt::TopDockWidgetArea, D);
      //M = qobject_cast<Catalogue::Model*>(W->model());
    }{
      QDockWidget *D = new QDockWidget(this);
      D->setWindowTitle(tr("Filter"));
      F =new Filter(D);
      //D->setWidget(new Catalogue::TreeView(D,M));
      D->setWidget(F);
      addDockWidget(Qt::LeftDockWidgetArea, D);
    }

    connect(W,SIGNAL(item_selected(QVariant), B->model, SLOT(cat_item_selected(QVariant)));
    connect(F, SIGNAL(apply_filter(QObject*)), B->model(), SLOT(apply_filter(QObject*)));

}

MainWindow::~MainWindow()
{

}

} // namespace STORE
