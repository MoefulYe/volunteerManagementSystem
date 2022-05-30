# 亚运会志愿者管理系统

## 0.背景

## 1.技术栈、使用工具

技术栈 QT5  C++ sqlite3

visual studio

## 2.结构介绍

```mermaid
flowchart LR
	id0(进入系统)--->id1(登录)
	id0-->id5
	id5(注册)
	id2(进入前台志愿者登记系统)
	id3(进入后台管理系统)
	id1--输入电话-->id2
	id1--输入root-->id3
	id1--输入错误返回主菜单-->id0
	id4(相关信息)
	id0-->id4
```

```mermaid
flowchart TB
	id0(输入手机号)
	id1{数据库内是否有对应的记录}
	id0-->id1
	subgraph 志愿者界面
		direction TB
		id8(查看录用信息)
		id6(个人数据的修改)
		id7(注销)
	end
	id3{确认是否报名}
	id1--有-->志愿者界面
	id1--无-->id3
	id4(退出系统)
	id5(要求填写个人信息)
	id3--否-->id4
	id3--是-->id5
	id5-->志愿者界面
```

```mermaid
flowchart LR
	输入root-->id
	id(后台管理系统)
	id0(赛事和志愿者的增删查改)
	id1(分配安排)
	id1-->自动
	id1-->手动
	id3(输出excel表格)
	id4(相关信息的统计)
	id-->id0
	id-->id1
	id-->id3
	id-->id4
```


## 3. 数据库设计
```sqlite
CREATE TABLE Volunteer(
    `id` INTEGER PRIMARY KEY AUTOINCREMENT,
    `name` CHAR(20) NOT NULL,
    `gender` CHAR(1) NOT NULL,
    `age` TINYINT NOT NULL,
    `telephone` CHAR(20) NOT NULL,
    `idCard` CHAR(20) NOT NULL,
    `profile` CHAR(100) NOT NULL,
    `hasExp` TINYINT NOT NULL,
    `langCommand` TINYINT NOT  NULL,
    `availTime` CHAR(100) NOT NULL
);
CREATE TABLE Event(
	`id` INTEGER PRIMARY KEY AUTOINCREMENT,
    `name` CHAR(20) NOT NULL,
    `day` TINYINT NOT NULL,
    `start` TINYINT NOT NULL,
    `end` TINYINT NOT NULL,
    `needVtrsNum` TINYINT NOT NULL
);
CREATE TABLE Event_Vtrs(
	`eventId` INTEGER,
    `vtrId` INTEGER,
    FOREIGN KEY(eventId) REFERENCES Event(id),
    FOREIGN KEY(vtrId) REFERENCES Volunteer(id)
);
```

```c++
class Time 
{
public:
	Time();
	int day;
	//开始和结束的小时
	int start, end;
	//持续了多少小时
	int duration();
	string toString();
	string toStringWithoutDay();
	//判断时间是否冲突
	bool collapse(Time& t);
};
class Event
{
public:
	Event();
	int id;
	string name;
	Time time;
	bool neededLangs[LANG_NUM];
	int needVtrsNum;
	int vtrs[MAX_NEEDED_VTRS];
	bool collapse(Event& e);
};
#define EventVec vector<Event> 
class Volunteer 
{
public:
	Volunteer();
	int id;
	string name;
	string gender;
	int age;
	string telephone;
	//身份证
	string idCard;
	//个人简介
	string profile;
	bool hasExp;
	//存储语言掌握情况
	bool langCommand[LANG_NUM];
	//认为一天志愿者提供的时间是连续的，availTime[n][0]表示
	//志愿者开始的有空的时间，availTime[n][1]表示结束
	//在数据库里以"8-22,9-22,10-22,...."五个连续以','分割的一对数值表示
	//0-0表示当天没空
	int availTime[DAYS_NUM][2];
	int result[MAX_ATTEND_EVENT];
	bool available(Event& e);
	string toDeleteSql();
	string toInsertSql();
	string toUpdateSql();
};
```