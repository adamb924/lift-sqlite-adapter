#include "liftsearchform.h"
#include "liftdatabase.h"
#include "datatypes/writingsystem.h"
#include "liftimporter.h"

#include <QApplication>
#include <QtDebug>
#include <QMainWindow>
#include <QFileInfo>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /// vaguely cheaty: if the database doesn't exist, offer to import it
    if( ! QFileInfo::exists("lexicon.sqlite") )
    {
        QString fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Import LIFT Lexicon"),
                                                        "",
                                                        QObject::tr("LIFT Lexicon (*.lift)"));
        if( fileName.isEmpty() )
        {
            return 0;
        }
        else
        {
            LiftImporter::import( fileName, "lexicon.sqlite" );
        }
    }

    LiftDatabase db("lexicon.sqlite");
    db.addWritingSystem( ME::WritingSystem("tk-TM") );
    db.addWritingSystem( ME::WritingSystem("en") );

    LiftSearchForm * form = new LiftSearchForm(db, ME::WritingSystem("tk-TM"));
    form->show();

    QMainWindow window;
    window.setCentralWidget(form);
    window.show();

    return a.exec();
}
