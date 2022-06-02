#include "insert.h"
#include "ui_insert.h"
#include<QMessageBox>
#include"../util/util.h"

Insert::Insert(Admin* admin, VtrVec* vv, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Insert)
{
    ui->setupUi(this);
	this->admin = admin;
	this->vv = vv;
}

Insert::~Insert()
{
    delete ui;
}

void Insert::on_pushButton_clicked()
{
	Volunteer v;
	v.id = -1;
	string name = this->ui->LineEditName->text().toStdString();
	if (name == "")
	{
		QMessageBox::information(this, "Error", "未输入姓名");
		return;
	}
	v.name = name;
	string password0 = this->ui->LineEdit_2->text().toStdString();
	if (password0 == "")
	{
		QMessageBox::information(this, "Error", "未输入密码！");
		return;
	}
	v.passwd = password0;
	string gender;
	if (this->ui->radioButton->isChecked())
	{
		gender = "M";
	}
	else if (this->ui->radioButton_2->isChecked())
	{
		gender = "F";
	}
	else
	{
		QMessageBox::information(this, "Error", "未选择性别！");
		return;
	}
	v.gender = gender;
	string ageStr = this->ui->LineEditAge->text().toStdString();
	if (ageStr == "")
	{
		QMessageBox::information(this, "Error", "未输入年龄");
		return;
	}
	int age = atoi(ageStr.c_str());
	if (age <= 0)
	{
		QMessageBox::information(this, "Error", "年龄不合法！");
		return;
	}
	v.age = age;
	string telephone = this->ui->LineEditTelephone->text().toStdString();
	if (telephone == "")
	{
		QMessageBox::information(this, "Error", "未输入电话号码");
		return;
	}
	v.telephone = telephone;
	string idCard = this->ui->LineEditIdCard->text().toStdString();
	if (idCard == "")
	{
		QMessageBox::information(this, "Error", "未输入身份证号");
		return;
	}
	v.idCard = idCard;
	string profile = this->ui->textEdit->toPlainText().toStdString();
	if (profile == "")
	{
		QMessageBox::information(this, "Error", "未输入个人简介");
		return;
	}
	v.profile = profile;
	v.hasExp = this->ui->CheckBox->isChecked();
	v.langCommand[0] = this->ui->radioButton_3->isChecked();
	v.langCommand[1] = this->ui->radioButton_4->isChecked();
	v.langCommand[2] = this->ui->radioButton_5->isChecked();
	v.langCommand[3] = this->ui->radioButton_6->isChecked();
	v.langCommand[4] = this->ui->radioButton_7->isChecked();
	v.langCommand[5] = this->ui->radioButton_8->isChecked();
	string timeStr = this->ui->LineEdit->text().toStdString();
	if (timeStr == "")
	{
		QMessageBox::information(this, "Error", "未输入时间");
		return;
	}
	vector<string> dayStr = split(timeStr, ",");
	if (dayStr.size() != DAYS_NUM)
	{
		QMessageBox::information(this, "Error", "时间格式不正确！");
		return;
	}
	for (int i = 0; i < DAYS_NUM; i++)
	{
		vector<string> tmp = split(dayStr[i], "-");
		if (tmp.size() != 2)
		{
			QMessageBox::information(this, "Error", "时间格式不正确！");
			return;
		}
		v.availTime[i][0] = stoi(tmp[0]);
		v.availTime[i][1] = stoi(tmp[1]);
		if (v.availTime[i][0] > v.availTime[i][1] || v.availTime[i][0] > 24 || v.availTime[i][0] < 0 || v.availTime[i][1] >24 || v.availTime[i][1] < 0)
		{
			QMessageBox::information(this, "Error", "时间格式不正确！");
			return;
		}
	}
	this->vv->push_back(v);
	string insert = v.insertSql();
	this->admin->addToSqls(insert);
	this->admin->flushVtrTable();
	QMessageBox::information(this, "Success", "添加成功！");
	this->close();
}

void Insert::on_pushButton_2_clicked()
{
    this->close();
}
