#include "liftimporter.h"

#include <QtDebug>
#include <QDir>
#include <QFileInfoList>
#include <QXmlStreamReader>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

LiftImporter::LiftImporter(const QString &outputPath) : mLdb(outputPath)
{
}

LiftImporter::LiftImporter(const LiftDatabase &ldb) : mLdb(ldb)
{
}

LiftDatabase LiftImporter::import(const QString &inputPath, const QString &outputPath)
{
    LiftDatabase ldb(outputPath);
    ldb.initialize();
    import( inputPath, ldb );
    return ldb;
}

LiftDatabase LiftImporter::import(const QString &inputPath, const LiftDatabase &ldb)
{
    LiftImporter li(ldb);

    li.prepareQueries();

    QSqlDatabase db = ldb.database();
    db.transaction();

    QFile file(inputPath);
    if( !file.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "Error opening: " << inputPath;
    }
    QXmlStreamReader xml(&file);
    while (!xml.atEnd())
    {
          xml.readNext();
          if( xml.isStartElement() && xml.name() == "entry" )
          {
//              qDebug() << "reading entry" << xml.attributes().value("id");
              li.readEntry(xml);
//              qDebug() << "done reading entry";
          }
    }

    db.commit();

    return ldb;
}

bool LiftImporter::openDatabase(const QString &outputPath) const
{
    /// Database initalization
    QSqlDatabase db = mLdb.database();
    db.setHostName("hostname");
    db.setDatabaseName(outputPath);
    if(!db.open())
    {
        qDebug() << "There was a problem in opening the database. The program said: " + db.lastError().databaseText();
        return false;
    }
    return true;
}

void LiftImporter::prepareQueries()
{
    prepareQuery( mEntry, "INSERT INTO entry (guid) VALUES (?);" );
    prepareQuery( mLexicalUnit, "INSERT INTO lexical_unit (entry_id,WritingSystem,Form) VALUES (?,?,?);" );
    prepareQuery( mTrait, "INSERT INTO trait (entry_id,name,value) VALUES (?,?,?);" );
    prepareQuery( mCitation, "INSERT INTO citation (entry_id,WritingSystem,Form) VALUES (?,?,?);" );
    prepareQuery( mSense, "INSERT INTO sense (entry_id) VALUES (?);" );
    prepareQuery( mGloss, "INSERT INTO gloss (sense_id,WritingSystem,Form) VALUES (?,?,?);" );
    prepareQuery( mDefinition, "INSERT INTO definition (sense_id,WritingSystem,Form) VALUES (?,?,?);" );
}

void LiftImporter::prepareQuery(QSqlQuery &q, const QString &queryString) const
{
    q = QSqlQuery(mLdb.database());
    if( !q.prepare(queryString) )
    {
        qWarning() << "Statement error: " << q.lastError().text() << q.lastQuery();
        return;
    }
}

void LiftImporter::readEntry(QXmlStreamReader &xml)
{
    mEntry.bindValue(0, xml.attributes().value("guid").toString() );
    if( !mEntry.exec() )
    {
        qWarning() << "Query error: " << mEntry.lastError().text() << mEntry.lastQuery();
        return;
    }
    qlonglong entry_id = mEntry.lastInsertId().toLongLong();

    while( !atEndOf(xml,"entry") )
    {
        xml.readNext();
        if( xml.isStartElement() )
        {
            if( xml.name() == "lexical-unit" )
            {
                readLexicalUnit(xml, entry_id);
            }
            else if( xml.name() == "trait" )
            {
                readTrait(xml, entry_id);
            }
            else if( xml.name() == "citation" )
            {
                readCitation(xml, entry_id);
            }
            else if( xml.name() == "sense" )
            {
                readSense(xml, entry_id);
            }
        }
    }
    Q_ASSERT(xml.name() == "entry" && xml.isEndElement());
}

void LiftImporter::readLexicalUnit(QXmlStreamReader &xml, qlonglong entry_id)
{
    readForms(xml, mLexicalUnit, "lexical-unit", entry_id);
}

void LiftImporter::readTrait(QXmlStreamReader &xml, qlonglong entry_id)
{
    mTrait.bindValue(0, entry_id);
    mTrait.bindValue(1, xml.attributes().value("name").toString() );
    mTrait.bindValue(2, xml.attributes().value("value").toString() );
    if( !mTrait.exec() )
    {
        qWarning() << "Query error: " << mTrait.lastError().text() << mTrait.lastQuery();
        return;
    }
    xml.readNext(); /// should move to the "end element" of the (self-closing) tag
}

void LiftImporter::readCitation(QXmlStreamReader &xml, qlonglong entry_id)
{
    readForms(xml, mCitation, "citation", entry_id);
}

void LiftImporter::readSense(QXmlStreamReader &xml, qlonglong entry_id)
{
    mSense.bindValue(0, entry_id);
    if( !mSense.exec() )
    {
        qWarning() << "Query error: " << mEntry.lastError().text() << mEntry.lastQuery();
        return;
    }
    qlonglong sense_id = mEntry.lastInsertId().toLongLong();

    while( !atEndOf(xml,"sense") )
    {
        xml.readNext();
        if( xml.isStartElement() )
        {
            if( xml.name() == "gloss" )
            {
                readGloss(xml, sense_id);
            }
            else if( xml.name() == "definition" )
            {
                readDefinition(xml, sense_id);
            }
        }
    }
}

void LiftImporter::readGloss(QXmlStreamReader &xml, qlonglong entry_id)
{
    QString ws = convertWritingSystemLabel( xml.attributes().value("lang").toString() );
    xml.readNextStartElement(); /// this should be <text>
    Q_ASSERT(xml.name() == "text");
    QString text = xml.readElementText(QXmlStreamReader::IncludeChildElements);

    mGloss.bindValue(0, entry_id);
    mGloss.bindValue(1, ws);
    mGloss.bindValue(2, text);
    if( !mGloss.exec() )
    {
        qWarning() << "Query error: " << mGloss.lastError().text() << mGloss.lastQuery();
        return;
    }

    xml.readNext(); /// get to the end tag

    Q_ASSERT(xml.name() == "gloss" && xml.isEndElement());
}

void LiftImporter::readDefinition(QXmlStreamReader &xml, qlonglong entry_id)
{
    readForms(xml, mDefinition, "definition", entry_id);
}

void LiftImporter::readForms(QXmlStreamReader &xml, QSqlQuery &q, const QString &elementName, qlonglong entry_id)
{
    while( !atEndOf(xml, elementName) )
    {
        xml.readNext();
        if( xml.isStartElement() && xml.name() == "form" )
        {
            QString ws = convertWritingSystemLabel( xml.attributes().value("lang").toString() );
            xml.readNextStartElement(); /// this should be <text>
            Q_ASSERT(xml.name() == "text");
            QString text = xml.readElementText(QXmlStreamReader::IncludeChildElements);

            q.bindValue(0, entry_id);
            q.bindValue(1, ws);
            q.bindValue(2, text);
            if( !q.exec() )
            {
                qWarning() << "Query error: " << q.lastError().text() << q.lastQuery();
                return;
            }
        }
    }
    Q_ASSERT(xml.name() == elementName && xml.isEndElement());
}

bool LiftImporter::atEndOf(QXmlStreamReader &xml, const QString &elementName) const
{
    if( xml.atEnd() )
    {
        qCritical() << "Uh-oh, unexpected end of file..." << xml.lineNumber() << "Or error:" << xml.errorString();
        return true;
    }
    return xml.isEndElement() && xml.name() == elementName;
}

QString LiftImporter::convertWritingSystemLabel(const QString &input) const
{
    return mWritingSystemReplacements.value( input, input );
}

void LiftImporter::setWritingSystemReplacements(const QHash<QString, QString> &writingSystemReplacements)
{
    mWritingSystemReplacements = writingSystemReplacements;
}
