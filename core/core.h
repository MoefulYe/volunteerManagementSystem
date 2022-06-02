#pragma once
#ifndef CORE_INCLUDED
#define CORE_INCLUDED
#include<string>
#include<vector>
using namespace std;
constexpr auto LANG_NUM = 6;
constexpr auto MAX_NEEDED_VTRS = 10;
constexpr auto MAX_ATTEND_EVENT = 10;
constexpr auto DAYS_NUM = 5;

const string  LANG_NAMES[LANG_NUM] = { "英", "汉", "日", "韩", "印", "俄" };

enum lang_set
{
	english,
	chinese,
	japanese,
	korean,
	india,
	russia,
};
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
	string toString();
	string toStringWithoutDay();
	//判断时间是否冲突
	bool collapse(Time& t);
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
	string insertSql();
	string deleteSql();
	string updateSql();
	string  toTimeStr();
	string toLangsStr();
private:
	int getLangCode();
};

#define  VtrVec vector<Volunteer>
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
 
#endif // !CORE_INCLUDED


