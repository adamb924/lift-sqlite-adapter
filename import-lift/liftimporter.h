#ifndef LIFTIMPORTER_H
#define LIFTIMPORTER_H

#include <QHash>
#include <QSqlQuery>
class QXmlStreamReader;

class LiftImporter
{
public:
    LiftImporter();

    void import( const QString & inputPath, const QString & outputPath );

    static QString mDbName;

    void setWritingSystemReplacements(const QHash<QString, QString> &writingSystemReplacements);

private:
    bool openDatabase( const QString & outputPath ) const;
    void createTables() const;
    void prepareQueries();
    void prepareQuery(QSqlQuery &q, const QString & queryString) const;

    void readEntry( QXmlStreamReader &xml );
    void readLexicalUnit( QXmlStreamReader &xml, qlonglong entry_id );
    void readTrait( QXmlStreamReader &xml, qlonglong entry_id );
    void readCitation( QXmlStreamReader &xml, qlonglong entry_id );
    void readSense( QXmlStreamReader &xml, qlonglong entry_id );
    void readGloss( QXmlStreamReader &xml, qlonglong entry_id );
    void readDefinition( QXmlStreamReader &xml, qlonglong entry_id );

    void readForms(QXmlStreamReader &xml, QSqlQuery &q, const QString & elementName, qlonglong entry_id);
    bool atEndOf( QXmlStreamReader &xml, const QString & elementName ) const;

    QString convertWritingSystemLabel(const QString & input) const;

    QSqlQuery mEntry;
    QSqlQuery mLexicalUnit;
    QSqlQuery mTrait;
    QSqlQuery mCitation;
    QSqlQuery mSense;
    QSqlQuery mGloss;
    QSqlQuery mDefinition;

    QHash<QString,QString> mWritingSystemReplacements;
};

#endif // LIFTIMPORTER_H
