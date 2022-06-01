#include "admin.h"
#include "ui_admin.h"
#include "../sql/sql.h"
#include <QTableWidgetItem>
#include"../gui/Update/update.h"

Admin::Admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
}

Admin::Admin(VtrVec vv, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Admin)
{
	ui->setupUi(this);
    this->vv = vv;
    this->visible = vv;
    this->ev = getAllEvents();
    flushTable();
}

Admin::~Admin()
{
    delete ui;
}

void Admin::flushTable()
{
    this->ui->vtrTable->setRowCount(this->vv.size());
    this->ui->eventTable->setRowCount(this->ev.size());
    for (int i = 0; i < vv.size(); i++)
    {
        Volunteer v = vv[i];
        this->ui->vtrTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(to_string(v.id))));
        this->ui->vtrTable->setItem(i, 1,new QTableWidgetItem(QString::fromStdString(v.name)));
        this->ui->vtrTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(v.gender)));
        this->ui->vtrTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(v.telephone)));
        this->ui->vtrTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(v.idCard)));
		this->ui->vtrTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(v.hasExp?"Y":"N")));
        this->ui->vtrTable->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(v.toLangsStr())));
		this->ui->vtrTable->setItem(i,7,new QTableWidgetItem(QString::fromStdString(v.passwd)));
    }
    for (int i = 0; i < ev.size(); i++)
    {
        Event e = ev[i];
		this->ui->eventTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(to_string(e.id))));
		this->ui->eventTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(e.name)));
		this->ui->eventTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(e.time.toString())));
    }
}

void Admin::on_vtrTable_cellDoubleClicked(int row, int column)
{
    Volunteer v = visible[row];
    Update *update = new Update(this,v);
    update->show();
}
