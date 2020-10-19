#ifndef LIFTDATABASE_H
#define LIFTDATABASE_H

#include "lift-sqlite-adapter_global.h"
#include "datatypes/writingsystem.h"

#include <QString>

class QSqlDatabase;

class LIFT_SQLITE_ADAPTER_EXPORT LiftDatabase
{
public:
    LiftDatabase(const QString & filename, const QString & dbname = "LIFT_IMPORT_DB");

    void initialize();

    QSqlDatabase database() const;

    void setWritingSystems(const QHash<QString, WritingSystem> &writingSystems);
    void addWritingSystem(const WritingSystem & ws);

    QHash<QString, WritingSystem> writingSystems() const;

private:
    void createTables() const;
    void openDatabase(const QString & dbPath) const;

    QString mDatabaseName;
    QHash<QString,WritingSystem> mWritingSystems;
};

#endif // LIFTDATABASE_H
