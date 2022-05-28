#include "volunteermanagementsystem.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "sql.h"
#include "core.h"
#include "util.h"
int main(int argc, char *argv[])
{
    VtrVec vv=getAllVtrs();
	EventVec ev=getAllEvents();
    QApplication a(argc, argv);
    volunteerManagementSystem w;
    w.resize(2000, 1500);
    w.setWindowTitle("亚运会志愿者管理系统");
    QVBoxLayout layout(&w);
    w.show();
    return a.exec();
}
