#include "../sql/sql.h"
#include "./core/core.h"
#include "./util/util.h"
#include "./gui/loginUI/loginui.h"
#include "./gui/SignUp/signup.h"
#include<QApplication>

int main(int argc, char* argv[])
{
	VtrVec vv = getAllVtrs();
	QApplication app(argc, argv);
	loginUI login;
	login.show();
	return app.exec();
}