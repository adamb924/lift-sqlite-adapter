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

QHash<QString, ME::WritingSystem> LiftDatabase::writingSystems() const
{
    return mWritingSystems;
}

void LiftDatabase::createTables() const
{
    QSqlDatabase db = QSqlDatabase::database(mDatabaseName);
    QSqlQuery q(db);

    /// entry
    q.exec("DROP TABLE IF EXISTS entry;");
    q.exec("CREATE TABLE entry ( _id integer primary key autoincrement, guid text );");

    /// citation
    q.exec("DROP TABLE IF EXISTS citation;");
    q.exec("CREATE TABLE citation ( _id integer primary key autoincrement, entry_id integer, WritingSystem text, Form text );");

    /// lexical_unit
    q.exec("DROP TABLE IF EXISTS lexical_unit;");
    q.exec("CREATE TABLE lexical_unit ( _id integer primary key autoincrement, entry_id integer, WritingSystem text, Form text );");

    /// trait
    q.exec("DROP TABLE IF EXISTS trait;");
    q.exec("CREATE TABLE trait ( _id integer primary key autoincrement, entry_id integer, name text, value text );");

    /// sense
    q.exec("DROP TABLE IF EXISTS sense;");
    q.exec("CREATE TABLE sense ( _id integer primary key autoincrement, entry_id integer );");

    /// gloss
    q.exec("DROP TABLE IF EXISTS gloss;");
    q.exec("CREATE TABLE gloss ( _id integer primary key autoincrement, sense_id integer, WritingSystem text, Form text );");

    /// definition
    q.exec("DROP TABLE IF EXISTS definition;");
    q.exec("CREATE TABLE definition ( _id integer primary key autoincrement, sense_id integer, WritingSystem text, Form text );");

    /// pronunciation
    q.exec("DROP TABLE IF EXISTS pronunciation;");
    q.exec("CREATE TABLE pronunciation ( _id integer primary key autoincrement, entry_id integer, WritingSystem text, Form text );");
}

void LiftDatabase::setWritingSystems(const QHash<QString, ME::WritingSystem> &writingSystems)
{
    mWritingSystems = writingSystems;
}

void LiftDatabase::addWritingSystem(const ME::WritingSystem &ws)
{
    mWritingSystems.insert(ws.abbreviation(), ws);
}
