#ifndef USER_H
#define USER_H

#include <QWidget>
#include "../core/core.h"


namespace Ui {
class user;
}

class user : public QWidget
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = nullptr);
    user(Volunteer v);
    ~user();
    void setDefault();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    //注销账号
    void on_pushButton_clicked();

private:
    Volunteer v;
    Ui::user *ui;
};

#endif // USER_H
