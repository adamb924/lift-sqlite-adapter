#ifndef LIFTSEARCHFORM_H
#define LIFTSEARCHFORM_H

#include <QWidget>

#include "liftdatabase.h"

namespace Ui {
class LiftSearchForm;
}

#include "lift-ui_global.h"

class LiftQueryModel;

class LIFTUI_EXPORT LiftSearchForm : public QWidget
{
    Q_OBJECT

public:
    LiftSearchForm(const LiftDatabase & ldb, QWidget *parent = nullptr);
    LiftSearchForm(const LiftDatabase & ldb, const WritingSystem & ws, QWidget *parent = nullptr);
    ~LiftSearchForm();
    
private slots:
    void refreshQuery() const;

private:
    Ui::LiftSearchForm *ui;
    LiftDatabase mLdb;
    LiftQueryModel * mModel;
};

#endif // LIFTSEARCHFORM_H