#include "liftdatabase.h"

#include <QtDebug>

#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>

LiftDatabase::LiftDatabase(const QString & filename, const QString & dbname) : mDatabaseName(dbname)
{
    openDatabase(filename);
}

void LiftDatabase::initialize()
{
    createTables();
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

void LiftDatabase::createTables() const
{
    QSqlDatabase db = QSqlDatabase::database(mDatabaseName);

    /// entry
    db.exec("DROP TABLE IF EXISTS entry;");
    db.exec("CREATE TABLE entry ( _id integer primary key autoincrement );");

    /// citation
    db.exec("DROP TABLE IF EXISTS citation;");
    db.exec("CREATE TABLE citation ( _id integer primary key autoincrement, entry_id integer, WritingSystem text, Form text );");

    /// lexical_unit
    db.exec("DROP TABLE IF EXISTS lexical_unit;");
    db.exec("CREATE TABLE lexical_unit ( _id integer primary key autoincrement, entry_id integer, WritingSystem text, Form text );");

    /// trait
    db.exec("DROP TABLE IF EXISTS trait;");
    db.exec("CREATE TABLE trait ( _id integer primary key autoincrement, entry_id integer, name text, value text );");

    /// sense
    db.exec("DROP TABLE IF EXISTS sense;");
    db.exec("CREATE TABLE sense ( _id integer primary key autoincrement, entry_id integer );");

    /// gloss
    db.exec("DROP TABLE IF EXISTS gloss;");
    db.exec("CREATE TABLE gloss ( _id integer primary key autoincrement, sense_id integer, WritingSystem text, Form text );");

    /// definition
    db.exec("DROP TABLE IF EXISTS definition;");
    db.exec("CREATE TABLE definition ( _id integer primary key autoincrement, sense_id integer, WritingSystem text, Form text );");
}

void LiftDatabase::setWritingSystems(const QHash<QString, WritingSystem> &writingSystems)
{
    mWritingSystems = writingSystems;
}

void LiftDatabase::addWritingSystem(const WritingSystem &ws)
{
    mWritingSystems.insert(ws.abbreviation(), ws);
}
