#include "insert.h"
#include "ui_insert.h"

Insert::Insert(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Insert)
{
    ui->setupUi(this);
}

Insert::~Insert()
{
    delete ui;
}
