#include "liftsearchform.h"
#include "liftdatabase.h"
#include "datatypes/writingsystem.h"

#include <QApplication>
#include <QtDebug>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LiftDatabase db("lexicon.sqlite");
    db.addWritingSystem( WritingSystem("tk-TM") );
    db.addWritingSystem( WritingSystem("en") );

    LiftSearchForm * form = new LiftSearchForm(db);
    form->show();

    QMainWindow window;
    window.setCentralWidget(form);
    window.show();

    return a.exec();
}
