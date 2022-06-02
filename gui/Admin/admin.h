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
    void addToSqls(string sql);
public slots:
    void flushVtrTable();
	
private slots:
    void on_vtrTable_cellDoubleClicked(int row, int column);

    void on_pushButton_clicked();

    void on_filter_clicked();

    void on_insertBtn_clicked();

private:
    Ui::Admin *ui;
    Filter f;
    vector<string> sqls;
	VtrVec vv;
    vector<int> visible;
    EventVec ev;
    void flushEventTable();
    void syncToDB();
};

#endif // ADMIN_H
