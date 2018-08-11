#include "application.h"

#include <QtSql>

namespace STORE {

Application::Application(int argc, char *argv[])
    : QApplication(argc,argv)
{
    // created not pointer (dynamically), but the object itself
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL","MyDataBase");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("store");
    // TODO bad manner to keep login/password open
    // TODO user should be asked for login and password
    //db.setUserName("admin");
    //db.setPassword("1");
    db.open("admin","1");

}

Application::~Application()
{

}

} // namespace STORE
