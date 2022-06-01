#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include "../core/core.h"

namespace Ui {
class Admin;
}

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
	Admin(VtrVec vv,QWidget *parent = nullptr);
    ~Admin();

private slots:
        void on_vtrTable_cellDoubleClicked(int row, int column);

private:
    Ui::Admin *ui;
    //Filter f;
	VtrVec vv;
    VtrVec visible;
    EventVec ev;
    void flushTable();
};

#endif // ADMIN_H
