#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_volunteermanagementsystem.h"

class volunteerManagementSystem : public QMainWindow
{
    Q_OBJECT

public:
    volunteerManagementSystem(QWidget *parent = Q_NULLPTR);

private:
    Ui::volunteerManagementSystemClass ui;
};
