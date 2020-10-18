#include "liftsearchform.h"
#include "ui_liftsearchform.h"

#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>

#include "liftquerymodel.h"
#include "lingedit.h"

#include <QAbstractItemModelTester>

LiftSearchForm::LiftSearchForm(const LiftDatabase &ldb, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LiftSearchForm),
    mLdb(ldb),
    mModel( new LiftQueryModel(ldb,this) )
{
    ui->setupUi(this);
    ui->inputForm->setWritingSystems( ldb.writingSystems().values() );
    ui->treeView->setModel(mModel);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setHeaderHidden(true);

    connect( ui->inputForm->lingEdit(), SIGNAL( returnPressed() ), this, SLOT( refreshQuery() ) );
    //    new QAbstractItemModelTester(mModel, QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

LiftSearchForm::LiftSearchForm(const LiftDatabase &ldb, const WritingSystem &ws, QWidget *parent) : LiftSearchForm(ldb,parent)
{
    ui->inputForm->setFixedWritingSystem(ws);
}

LiftSearchForm::~LiftSearchForm()
{
    delete ui;
}

void LiftSearchForm::refreshQuery() const
{
    mModel->setQueryString( ui->inputForm->lingEdit()->text() );
}

