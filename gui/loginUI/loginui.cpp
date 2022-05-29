#include "loginui.h"
#include "ui_loginui.h"
#include "../gui/SignUp/signup.h"
#include "../core/core.h"
#include "../sql/sql.h"
#include<QMessageBox>

loginUI::loginUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginUI)
{
    ui->setupUi(this);
}

loginUI::~loginUI()
{
    delete ui;
}

void loginUI::on_signinButton_clicked()
{
    VtrVec vv = getAllVtrs();
    string username = this->ui->lineEditAccount->text().toStdString();
	string password = this->ui->lineEditPasswd->text().toStdString();
    if (username == "root" && password == "123456")
    {
		QMessageBox::information(this, "now Login as root", "Login Success!");
        return;
    }
	for(Volunteer v : vv)
	{
		if(v.telephone == username && v.passwd == password)
		{
			QMessageBox::information(this,"now login as volunteer", "Login Success!");
			return;
		}
	}
	QMessageBox::information(this, "Login Failed", "Wrong Account or Password!");
    	
}

void loginUI::on_signupButton_clicked()
{
    SignUp* signup = new SignUp;
	signup->show();
}
