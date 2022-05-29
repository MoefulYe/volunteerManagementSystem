#ifndef LOGINUI_H
#define LOGINUI_H

#include <QMainWindow>

namespace Ui {
class loginUI;
}

class loginUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginUI(QWidget *parent = nullptr);
    ~loginUI();

private slots:
    void on_signinButton_clicked();

    void on_signupButton_clicked();

private:
    Ui::loginUI *ui;
};

#endif // LOGINUI_H
