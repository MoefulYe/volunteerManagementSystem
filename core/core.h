#pragma once
#ifndef CORE_INCLUDED
#define CORE_INCLUDED
#include<string>
#include<vector>
using namespace std;
//假定只有6种语言
constexpr auto LANG_NUM = 6;
//一个事程至多有10个志愿者
constexpr auto MAX_NEEDED_VTRS = 10;
//一个志愿者至多参加10个事程
constexpr auto MAX_ATTEND_EVENT = 10;
//假定有5天
constexpr auto DAYS_NUM = 5;
//int转语言
const string  LANG_NAMES[LANG_NUM] = { "英", "汉", "日", "韩", "印", "俄" };

//语言枚举
enum lang_set
{
	english,
	chinese,
	japanese,
	korean,
	india,
	russia,
};

class Volunteer;
class Event;
class Scheme;
//_Time可不连续，Time只记录一天内的一段连续时间，两者的粒度都是一小时
class _Time;
class Time;
#define  VtrVec vector<Volunteer>
#define  PVtrVec vector<Volunteer*>
#define EventVec vector<Event>
#define Possibilities vector<vector<int>>
#define Possibility vector<int>
#define Answer vector<vector<int>>


class Time 
{
public:
	Time();
	Time(int day, int start, int end);
	int day;
	//开始和结束的小时
	int start, end;
	//持续了多少小时
	int duration();
	//转化为字符串
	string toString();
	//转化为字符串但不带日期
	string toStringWithoutDay();
	//判断时间是否冲突
	bool collapse(Time& t);
};

class _Time
{
public:
	_Time();
	//从Time中构造_Time
	_Time(const Time& T);
	//重载+号，加上Time对应的时间
	_Time operator+(const Time& t) const;
	//重载*号，判断重合
	bool operator*(const Time& t) const;
private:
	//内部用5个int表示
	unsigned int day[DAYS_NUM];
};

class Event
{
public:
	int id;
	string name;
	Time time;
	bool neededLangs[LANG_NUM];
	int needVtrsNum;
	int vtrs[MAX_NEEDED_VTRS];
	bool collapse(Event& e);
	vector<int> vtrs_may_attend(VtrVec* vv, vector<int>* sortedVtrs);
	vector<vector<int>> getAllPossibilities(VtrVec* vv, vector<int>* sortedVtrs);
	bool islangMet(PVtrVec pvv);
};
#define EventVec vector<Event> 
class Volunteer 
{
public:
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
	string passwd;
	int result[MAX_ATTEND_EVENT];
	bool available(Time& e);
	//bool available();
	string insertSql();
	string deleteSql();
	string updateSql();
	string toTimeStr();
	string toLangsStr();
	//int vtrHours();
private:
	int getLangCode();
};

class Filter 
{
public:
	Filter();
	vector<int> filter(VtrVec vv);
	void setTime(string time);
	void setLang(bool langs[LANG_NUM]);
	void setKeyword(string keyword);
	void setGender(bool gender[2]);
private:
	string keyword;
	bool gender[2];
	bool hasExp;
	bool commandLang[LANG_NUM];
	Time time;
	bool isMet(Volunteer& vtr);
};

class Scheme {
public:
	Scheme();
	Scheme(VtrVec vv, EventVec ev);
	VtrVec vv;
	EventVec ev;
	bool hasAnswer;
	void decide();
	void output();
	void syncToDB();
	void exportAsCSV();
	Answer answer;
private:
	vector<int> sorted;
	vector<Possibilities> vecPos;
	void solve(vector<_Time> times, Answer answer, int index);
};



vector<int> sortByHasEXp(VtrVec* vv);

#endif // !CORE_INCLUDED
