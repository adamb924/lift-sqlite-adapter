#include <QCoreApplication>

#include <QtDebug>

#include "liftimporter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if( argc < 3 )
    {
        qCritical() << "Expecting at least two arguments. input, output";
        return 1;
    }

    QHash<QString,QString> replacements;
    replacements.insert("tk-TM","tk-TK");
    replacements.insert("en","en-US");

    LiftImporter importer;
    importer.setWritingSystemReplacements(replacements);
    importer.import( QString(argv[1]), QString(argv[2]) );
    qDebug() << "Done";

    return 0;
}
