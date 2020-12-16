#include "liftsearchform.h"
#include "ui_liftsearchform.h"

#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QClipboard>

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
    connect( ui->inputForm, SIGNAL( enterPressed() ), this, SLOT( refreshQuery() ) );
    connect( ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(copyToClipboard(QModelIndex)) );
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

void LiftSearchForm::searchFor(const QString &form)
{
    Form f = ui->inputForm->form();
    f.setText(form);
    searchFor(f);
}

void LiftSearchForm::searchFor(const Form &form)
{
    ui->inputForm->suggestInput(form);
    refreshQuery();
}

void LiftSearchForm::refreshQuery() const
{
    mModel->setQueryString( ui->inputForm->form().text() );
    if( mModel->rowCount() == 1 )
    {
        ui->treeView->expand( mModel->index(0,0) );
    }
}

void LiftSearchForm::copyToClipboard(const QModelIndex &index)
{
    if( mModel != nullptr && index.isValid() )
    {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText( mModel->data(index).toString() );
    }
}

