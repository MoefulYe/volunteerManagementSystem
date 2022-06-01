#include "./gui/loginUI/loginui.h"
#include<QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	loginUI login;
	login.show();
	return app.exec();
}