#ifndef UPDATE_H
#define UPDATE_H

#include <QWidget>
#include"../gui/Admin/admin.h"

namespace Ui {
class Update;
}

class Update : public QWidget
{
    Q_OBJECT

public:
    explicit Update(Admin *a,VtrVec *vv,int index, QWidget* parent = nullptr);
    ~Update();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Update *ui;
	Admin *admin;
    VtrVec* vv;
    int index;
};

#endif // UPDATE_H
