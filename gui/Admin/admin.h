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
    explicit Admin(VtrVec vv,QWidget *parent = nullptr);
    ~Admin();
    void addToSqls(string sql);
public slots:
    void flushVtrTable();
	
private slots:
    void on_vtrTable_cellDoubleClicked(int row, int column);

    void on_pushButton_clicked();

    void on_filter_clicked();

    void on_insertBtn_clicked();

    void on_scheme_clicked();

    void on_decide_clicked();

    void on_sync_clicked();

    void on_exportCSV_clicked();

    void on_tab_2_tabBarClicked(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    bool flag;
    Ui::Admin *ui;
    Filter f;
    vector<string> sqls;
	VtrVec vv;
    vector<int> visible;
    EventVec ev;
    Scheme sch;
    void flushEventTable();
    void syncToDB();
};

#endif // ADMIN_H
