#ifndef INSERT_H
#define INSERT_H

#include <QWidget>

namespace Ui {
class Insert;
}

class Insert : public QWidget
{
    Q_OBJECT

public:
    explicit Insert(QWidget *parent = nullptr);
    ~Insert();

private:
    Ui::Insert *ui;
};

#endif // INSERT_H
