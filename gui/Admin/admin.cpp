#include "admin.h"
#include <QtCharts> 
QT_CHARTS_USE_NAMESPACE
#include "ui_admin.h"
#include "../sql/sql.h"
#include <QTableWidgetItem>
#include"../gui/Update/update.h"
#include"../gui/Insert/insert.h"
#include<QMessageBox>
#include <QtCharts>
#include <qpielegendmarker.h>
#include <qpieseries.h>
#include <qpieslice.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace QtCharts;

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
    this->ui->eventTable->setColumnWidth(2, 1000);
    this->ui->tableWidget->setColumnWidth(2, 1500);
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

void Admin::on_scheme_clicked()
{
    this->sch.decide();
    if (!this->sch.hasAnswer)
    {
		QMessageBox::information(NULL, "info", "没有符合条件的组合");
		return;
    }
    else
    {
        this->ui->tableWidget->clear();
        this->ui->tableWidget->setRowCount(this->sch.ev.size());
        int i = 0;
        for (auto e : this->sch.ev)
        {
            this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(e.name)));
            this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(e.time.toString())));
            string vtrs = "";
            for (auto i : this->sch.answer[i])
            {
                vtrs += this->sch.vv[i].name + " ";
            }
            this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(vtrs)));
            i++;
        }
    }
}

void Admin::on_decide_clicked()
{
    this->vv = this->sch.vv;
    this->ev = this->sch.ev;
	this->flushVtrTable();
	QMessageBox::information(NULL, "info", "确定成功");
}

void Admin::on_sync_clicked()
{
    this->sch.syncToDB();
    QMessageBox::information(NULL, "info", "同步成功");
}

void Admin::on_exportCSV_clicked()
{
    this->sch.exportAsCSV();
	QMessageBox::information(NULL, "info", "导出成功");
}

void Admin::on_tab_2_tabBarClicked(int index)
{
    if (index == 1) {
        Scheme s(this->vv,this->ev);
        this->sch = s;
    }
    else {
		
    }
}

//性别
void Admin::on_pushButton_2_clicked()
{
    int male=0, female=0;
    for (int i = 0; i < this->vv.size(); i++)
    {
        if (this->vv[i].gender == "M")
        {
            male++;
        }
        else
        {
            female++;
        }
    } 	
    QChart *chart = new QChart();
	chart->setTitle("性别比例");
	chart->setAnimationOptions(QChart::AllAnimations);
    QPieSeries* series = new QPieSeries();
    series->append("男", male);
    series->append("女", female);
    series->setLabelsVisible(true);
    series->setUseOpenGL(true);
    series->slices().at(0)->setColor(QColor(13, 128, 217));   //设置颜色
    series->slices().at(0)->setLabelColor(QColor(13, 128, 217));

    series->slices().at(1)->setColor(QColor(255, 0, 0));
    series->slices().at(1)->setLabelColor(QColor(255, 0, 0));
    chart->setTheme(QChart::ChartThemeLight);//设置白色主题
    chart->setDropShadowEnabled(true);//背景阴影

    //使用QChartView将QChart装载起来，以便显示QChart图表中的数据
    chart->addSeries(series);//添加系列到QChart上

    chart->setTitleBrush(QBrush(QColor(0, 0, 255)));//设置标题Brush
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(400, 400);
    chartView->setWindowTitle("性别统计");
    chartView->show();
}

//年龄
void Admin::on_pushButton_3_clicked()
{
    int age[6];
	for (int i = 0; i < 6; i++)
	{
		age[i] = 0;
	}
    for (int i = 0; i < this->vv.size(); i++)
    {
        int a = this->vv.at(i).age;
        if (a < 20)
        {
            age[0]++;
        }
		else if (a < 30)
		{
			age[1]++;
		}
		else if (a < 40)
		{
			age[2]++;
		}
		else if (a < 50)
		{
			age[3]++;
		}
		else if (a < 60)
		{
			age[4]++;
		}
		else
		{
			age[5]++;
		}
	}
    QBarSet* set0 = new QBarSet("20岁以下");
    QBarSet* set1 = new QBarSet("20-30岁");
    QBarSet* set2 = new QBarSet("30-40岁");
    QBarSet* set3 = new QBarSet("40-50岁");
    QBarSet* set4 = new QBarSet("50-60岁");
    QBarSet* set5 = new QBarSet("60岁以上");
	*set0 << age[0];
    *set1 << age[1];
	*set2 << age[2];
    *set3 << age[3];
    *set4 << age[4];
	*set5 << age[5];
    QBarSeries* series = new QBarSeries();
	series->append(set0);
	series->append(set1);
	series->append(set2);
    series->append(set3);
    series->append(set4);
    series->append(set5);
	QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("年龄统计");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeLight);
    chart->setDropShadowEnabled(true);
    chart->setTitleBrush(QBrush(QColor(0, 0, 255)));
    chart->setTitleFont(QFont("微软雅黑"));
	QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 800);
    chartView->setWindowTitle("年龄统计");
    chartView->show();
}

//十大
void Admin::on_pushButton_4_clicked()
{
    vector<int> hours(this->sch.vv.size());
	for (int i = 0; i < this->sch.vv.size(); i++)
	{
		hours[i] = 0;
	}
    for (int i = 0; i < this->sch.ev.size(); i++)
    {
        int hour = this->sch.ev.at(i).time.duration();
        for (int j = 0; j < this->sch.answer.at(i).size(); j++)
        {
            hours.at(this->sch.answer.at(i).at(j))++;
        }
    }
    vector<int> sort(this->sch.vv.size());
	for (int i = 0; i < this->sch.vv.size(); i++)
	{
		sort[i] = i;
	}
	for (int i = 0; i < this->sch.vv.size(); i++)
	{
		for (int j = i + 1; j < this->sch.vv.size(); j++)
		{
			if (hours.at(sort.at(i)) < hours.at(sort.at(j)))
			{
				int temp = sort.at(i);
				sort.at(i) = sort.at(j);
				sort.at(j) = temp;
			}
		}
	}
    QBarSet* set[10];
    if (this->sch.vv.size() <= 10)
    {
        for (int i = 0; i < this->sch.vv.size(); i++)
        {
            set[i] = new QBarSet(QString::fromStdString(this->sch.vv.at(sort[i]).name));
			*set[i] << hours.at(sort[i]);
        }
		QBarSeries *series = new QBarSeries();
		for (int i = 0; i < this->sch.vv.size(); i++)
        {
			series->append(set[i]);
		}
		QChart *chart = new QChart();
		chart->addSeries(series);
		chart->setTitle("十大优秀志愿者");
		chart->setAnimationOptions(QChart::AllAnimations);
		chart->setTheme(QChart::ChartThemeLight);
		chart->setDropShadowEnabled(true);
		chart->setTitleBrush(QBrush(QColor(0, 0, 255)));
		chart->setTitleFont(QFont("微软雅黑"));
		QChartView *chartView = new QChartView(chart);
		chartView->setRenderHint(QPainter::Antialiasing);
		chartView->resize(800, 1000);
		chartView->setWindowTitle("十大优秀志愿者");
		chartView->show();
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            set[i] = new QBarSet(QString::fromStdString(this->sch.vv.at(sort[i]).name));
            *set[i] << hours.at(sort[i]);
        }
		QBarSeries *series = new QBarSeries();
        for (int i = 0; i < 10; i++)
        {
            series->append(set[i]);
        }
		QChart *chart = new QChart();
		chart->addSeries(series);
		chart->setTitle("十大优秀志愿者");
		chart->setAnimationOptions(QChart::AllAnimations);
		chart->setTheme(QChart::ChartThemeLight);
		chart->setDropShadowEnabled(true);
		chart->setTitleBrush(QBrush(QColor(0, 0, 255)));
		chart->setTitleFont(QFont("微软雅黑"));
		QChartView *chartView = new QChartView(chart);
		chartView->setRenderHint(QPainter::Antialiasing);
		chartView->resize(800, 1000);
		chartView->setWindowTitle("十大优秀志愿者");
		chartView->show();
    }
}
