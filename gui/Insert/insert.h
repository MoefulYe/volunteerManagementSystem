#ifndef INSERT_H
#define INSERT_H

#include <QWidget>
#include "../gui/Admin/admin.h"


namespace Ui {
class Insert;
}

class Insert : public QWidget
{
    Q_OBJECT

public:
    explicit Insert(Admin *admin,VtrVec *vv,QWidget *parent = nullptr);
    ~Insert();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Insert *ui;
	Admin *admin;
	VtrVec *vv;
};

#endif // INSERT_H
