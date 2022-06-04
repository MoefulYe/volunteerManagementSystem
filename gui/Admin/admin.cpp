#include "admin.h"
#include "ui_admin.h"
#include "../sql/sql.h"
#include <QTableWidgetItem>
#include"../gui/Update/update.h"
#include"../gui/Insert/insert.h"
#include<QMessageBox>

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
    f = Filter();
    this->vv = vv;
    this->visible = f.filter(vv);
    this->ev = getAllEvents();
    flushEventTable();
	flushVtrTable();
}

Admin::~Admin()
{
    delete ui;
}

void Admin::addToSqls(string sql)
{
    this->sqls.push_back(sql);
}

void Admin::flushVtrTable()
{
    this->visible = this->f.filter(this->vv);
    for (int row = this->ui->vtrTable->rowCount(); row > 0; row--)
    {
		this->ui->vtrTable->removeRow(row);
    }
    this->ui->vtrTable->setRowCount(this->visible.size());
    int i = 0;
    for (int index: this->visible)
    {
        Volunteer v = vv[index];
        this->ui->vtrTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(to_string(v.id))));
        this->ui->vtrTable->setItem(i, 1,new QTableWidgetItem(QString::fromStdString(v.name)));
        this->ui->vtrTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(v.gender)));
        this->ui->vtrTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(v.telephone)));
        this->ui->vtrTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(v.idCard)));
		this->ui->vtrTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(v.hasExp?"Y":"N")));
        this->ui->vtrTable->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(v.toLangsStr())));
		this->ui->vtrTable->setItem(i,7,new QTableWidgetItem(QString::fromStdString(v.passwd)));
        i++;
    }
}

void Admin::flushEventTable()
{
    this->ui->eventTable->setRowCount(this->ev.size());
    int	i = 0;
    for (Event e: this->ev)
    {
		this->ui->eventTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(to_string(e.id))));
		this->ui->eventTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(e.name)));
		this->ui->eventTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(e.time.toString())));
        i++;
    }
}

void Admin::syncToDB()
{
    if (this->sqls.size() == 0)return;
    for (int i = 0; i < this->sqls.size(); i++)
    {
        sqlOperator(this->sqls[i]);
    }
}

void Admin::on_vtrTable_cellDoubleClicked(int row, int column)
{
    Update *update = new Update(this,&this->vv,this->visible[row]);
    update->show();
}

void Admin::on_pushButton_clicked()
{
    QMessageBox::StandardButton result = QMessageBox::information(NULL, "提示信息", "确定要同步到数据库吗", QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::No)return;
    this->syncToDB();
    QMessageBox::information(NULL, "info", "同步成功");
}

void Admin::on_filter_clicked()
{
    string keyword = this->ui->lineEditKeyWord->text().toStdString();
    bool gender[2];
    gender[0] = this->ui->radioButton->isChecked();
    gender[1] = this->ui->radioButton_2->isChecked();
    bool hasExp=this->ui->checkBox->isChecked();
    bool lang[LANG_NUM];
    lang[0] = this->ui->radioButton_3->isChecked();
    lang[1] = this->ui->radioButton_4->isChecked();
    lang[2] = this->ui->radioButton_5->isChecked();
    lang[3] = this->ui->radioButton_6->isChecked();
    lang[4] = this->ui->radioButton_7->isChecked();
    lang[5] = this->ui->radioButton_8->isChecked();
    string time = this->ui->lineEdit->text().toStdString();
    this->f.setKeyword(keyword);
    this->f.setGender(gender);
    this->f.setLang(lang);
    if (time != "")
    {
        this->f.setTime(time);
    }
    else
    {
        this->f.setTime("-1,0,0");
    }
    this->flushVtrTable();
}

void Admin::on_insertBtn_clicked()
{
    Insert *insert = new Insert(this,&this->vv);
	insert->show();
}
