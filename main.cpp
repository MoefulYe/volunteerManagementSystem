#include "volunteermanagementsystem.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "sqlite3.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    volunteerManagementSystem w;
    w.resize(500, 300);
    w.setWindowTitle("亚运会志愿者管理系统");
    w.show();
    return a.exec();

}
