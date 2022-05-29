#include "core.h"
#include<string>
using namespace std;
int Time::duration()
{
    return this->end - this->start;
}

string Time::toString()
{
    char buffer[30];
    sprintf(buffer, "第%d天%d点到%d点", this->day+1,this->start,this->end);
	return buffer;
}

string Time::toStringWithoutDay()
{
	char buffer[30];
	sprintf(buffer, "%d点到%d点", this->start,this->end);
    return buffer;
}

bool Time::collapse(Time& t)
{
    if (this->day != t.day) 
    {
        return false;
    }
    else if (this->end <= t.start || t.end <= t.start)
    {
		return false;
	}
	else
    {
		return true;
    }
}

bool Event::collapse(Event& e)
{
    return this->time.collapse(e.time);
}

bool Volunteer::available(Event& e) 
{
    int day = e.time.day;
    return this->availTime[day][0]<e.time.start && this->availTime[day][1]>e.time.end;
}
string Volunteer::toTimeStr()
{
	string s;
	for(int i = 0; i < DAYS_NUM-1; i++)
    {
        s += to_string(this->availTime[i][0])+ "-" + to_string(this->availTime[i][1])+ ",";
	}
    s += to_string(this->availTime[DAYS_NUM-1][0]) + "-" + to_string(this->availTime[DAYS_NUM-1][1]);
	return s;
}
vector<string> Volunteer::deleteSql()
{
	vector<string> vs;
    char buffer1[50],buffer2[50];
	sprintf(buffer1, "delete from Event_Vtrs where vtrId=%d;", this->id);
	vs.push_back(buffer1);
	sprintf(buffer2, "delete from Volunteer where id=%d;", this->id);
	vs.push_back(buffer2);
	return vs;
}
string Volunteer::insertSql()
{
    string s="insert into Volunteer values";
	s += "(null,";
    s += "'" + this->name + "',";
    s += "'" + this->gender + "',";
    s += to_string(this->age) + ",";
    s += "'" + this->telephone + "',";
    s += "'" + this->idCard + "',";
    s += "'" + this->profile + "',";
    s += to_string(this->hasExp ? 1 : 0) + ",";
    s += to_string(this->getLangCode()) + ",";
    s += "'" + this->toTimeStr()+"',";
    s += "'" + this->passwd + "')";
    return s;
}

string Volunteer::updateSql()
{
	string s="update Volunteer set ";
	s += "`name`='" + this->name + "',";
    s += "`gender`='" + this->gender + "',";
    s += "`age`=" + to_string(this->age) + ",";
    s += "`telephone`='" + this->telephone + "',";
    s += "`idCard`='" + this->idCard + "',";
    s += "`profile`='" + this->profile + "',";
    s += "`hasExp`=" + to_string(hasExp ? 1 : 0) + ",";
    s += "`langCommand`=" + to_string(this->getLangCode()) + ",";
	s += "`availTime`='" + this->toTimeStr() + "',";
    s += "`passwd`='" + this->passwd + "' ";
    s+="where id="+to_string(this->id);
    return string();
}

int Volunteer::getLangCode()
{
    int value = 0;
    for(int i = 0; i <LANG_NUM-1; i++)
    {
        value += this->langCommand[i] ? 1 : 0;
        value <<= 1;
    }
    value += this->langCommand[LANG_NUM-1];
    return value;
}

