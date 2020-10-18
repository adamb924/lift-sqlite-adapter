#include "liftdatabase.h"

#include <QtDebug>

#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>

LiftDatabase::LiftDatabase(const QString & filename, const QString & dbname) : mDatabaseName(dbname)
{
    openDatabase(filename);
}

QSqlDatabase LiftDatabase::database() const
{
    return QSqlDatabase::database(mDatabaseName);
}

void LiftDatabase::openDatabase(const QString &dbPath) const
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        qWarning() << "DatabaseAdapter::openDatabase()" << "Driver failed to open.";
        return;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", mDatabaseName );

    db.setDatabaseName( dbPath );
    db.setConnectOptions("QSQLITE_ENABLE_REGEXP");

    if(!db.open())
    {
        qWarning() << "DatabaseAdapter::openDatabase()" << "Database failed to open." << dbPath;
        return;
    }
    if( !db.isValid() )
    {
        qWarning() << "DatabaseAdapter::openDatabase()" << "Invalid database: " << dbPath;
    }

}

QHash<QString, WritingSystem> LiftDatabase::writingSystems() const
{
    return mWritingSystems;
}

void LiftDatabase::setWritingSystems(const QHash<QString, WritingSystem> &writingSystems)
{
    mWritingSystems = writingSystems;
}

void LiftDatabase::addWritingSystem(const WritingSystem &ws)
{
    mWritingSystems.insert(ws.abbreviation(), ws);
}
