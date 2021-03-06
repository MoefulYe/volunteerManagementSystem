#include "core.h"
#include<string>
#include"../util/util.h"
#include"../sql/sql.h"
#include<stdlib.h>
using namespace std;

Time::Time()
{
}

Time::Time(int day,int start,int end)
{
    this->day = day;
	this->start = start;
	this->end = end;
}

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

vector<int> Event::vtrs_may_attend(VtrVec *vv,vector<int> *sortedVtrs)
{
    vector<int> result;
    int i = 0;
    for (int i : *sortedVtrs)
    {
        if (vv->at(i).available(this->time))
        {
			result.push_back(i);
        }
    }
    return result;
}

void combination(vector<int> *src,vector<vector<int>> *result,vector<int> tmp, int num, int index)
{
    if (num == 0)
    {
		result->push_back(tmp);
		return;
    }
    else if (index == src->size())
    {
		return;
    }
    else
    {
        combination(src, result, tmp, num, index + 1);
        tmp.push_back(src->at(index));
		combination(src, result, tmp, num - 1, index + 1);
    }
}

vector<vector<int>> Event::getAllPossibilities(VtrVec *vv,vector<int> *sortedVtrs)
{
	vector<vector<int>> result;
    vector<int> mayAttend = this->vtrs_may_attend(vv,sortedVtrs);
    if (mayAttend.size() < this->needVtrsNum)return result;
	int size = mayAttend.size();
	combination(&mayAttend,&result,vector<int>(), this->needVtrsNum, 0);
    int i = 0;
    for (vector<int> possibility : result)
	{
        bool flag[LANG_NUM];
		for (int i = 0; i < LANG_NUM; i++)
		{
			flag[i] = false;
		}
        for (int i : possibility)
        {
            for(int i=0; i < LANG_NUM; i++)
			{
                flag[i] == flag[i] || vv->at(i).langCommand[i];
			}
        }
		for(int i=0;i<LANG_NUM;i++)
        {
            if (!flag[i] && this->neededLangs[i])
            {
                result.erase(result.begin() + i);
				break;
            }
        }
        i++;
	}
	return result;
}

bool Event::islangMet(PVtrVec pvv)
{
    bool tmp[LANG_NUM];
    for (int i = 0; i < LANG_NUM; i++)
    {
        tmp[i] = false;
    }
    for(Volunteer *pv:pvv)
    {
        for (int i = 0; i < LANG_NUM; i++)
        {
            tmp[i] =tmp[i] || pv->langCommand[i];
        }
    }
    for (int i = 0; i < LANG_NUM; i++)
    {
        if (this->neededLangs[i] && !tmp[i])return false;
    }
    return true;
}


bool Volunteer::available(Time& t)
{
    int day = t.day;
    return this->availTime[day][0]<t.start && this->availTime[day][1]>t.end;
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
string Volunteer::toLangsStr()
{
    string s = "";
    for (int i = 0; i < LANG_NUM; i++)
    {
        if (this->langCommand[i])s += LANG_NAMES[i];
    }
    return s;
}
string Volunteer::deleteSql()
{
    char buffer1[80],buffer2[80];
	sprintf(buffer1, "delete from Event_Vtrs where vtrId=%d;", this->id);
	sprintf(buffer2, "delete from Volunteer where id=%d;", this->id);
	string s = buffer1;
    s += ";";
	s += buffer2;
    return s;
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
    return  s;
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


Filter::Filter()
{
    this->keyword = "";
    this->gender[0] = true;
    this->gender[1] = true;
    this->hasExp = false;
    this->commandLang[english] = false;
    this->commandLang[chinese] = false;
    this->commandLang[japanese] = false;
    this->commandLang[korean] = false;
    this->commandLang[india] = false;
    this->commandLang[russia] = false;
    this->time = Time(-1, 0, 0);
	
}

vector<int> Filter::filter(VtrVec vv)
{
    vector<int> v;
    for (int i = 0; i < vv.size(); i++)
    {
		if(this->isMet(vv[i]))
        {
			v.push_back(i);
        }
    }
	return v;
}

void Filter::setTime(string time)
{
	vector<string> v = split(time, ",");
    int day = stoi(v[0]);
	int start = stoi(v[1]);
	int end = stoi(v[2]);
	this->time = Time(day, start, end);
}

void Filter::setLang(bool langs[LANG_NUM])
{
    for (int i = 0; i < LANG_NUM; i++)
    {
        this->commandLang[i] = langs[i];
    }
}

void Filter::setKeyword(string keyword)
{
    this->keyword = keyword;
}

void Filter::setGender(bool gender[2])
{
    this->gender[0] = gender[0];
    this->gender[1] = gender[1];
}

bool Filter::isMet(Volunteer& vtr)
{
    if (vtr.name.find(this->keyword) == -1)
    {
        return false;
    }
    if (!(this->gender[0] && vtr.gender == "M" || this->gender[1] && vtr.gender == "F"))
    {
        return false;
    }
    for (int i = 0; i < LANG_NUM; i++)
    {
		if(this->commandLang[i]&&!vtr.langCommand[i])
        {
            return false;
        }
    }
	if(this->time.day!=-1&&vtr.available(time))
    {

    }
    return true;
}

Scheme::Scheme()
{
}

Scheme::Scheme(VtrVec vv, EventVec ev)
{
    this->ev = ev;
    this->vv = vv;
    this->sorted = sortByHasEXp(&vv);
    for (auto e : ev)
    {
        auto p = e.getAllPossibilities(&this->vv, &this->sorted);
        this->vecPos.push_back(p);
    }
    this->hasAnswer = false;
}

void Scheme::solve(vector<_Time> times, Answer answer, int index)
{
    if (this->hasAnswer)return;
    if (index == this->ev.size())
    {
        this->answer = answer;
		this->hasAnswer = true;
        return;
    }
	for (auto p : this->vecPos[index])
	{
		bool flag = true;
		Answer a = answer;
		a.push_back(p);
		vector<_Time> t=times;
		//是否有冲突
		for(int i:p)
        {
            if (t[i] * this->ev.at(index).time)
            {
				flag = false;
                break;
            }
            t[i] = t[i] + this->ev.at(index).time;
        }
		if(flag)
		{
			this->solve(t, a, index + 1);
		}
        else 
        {
		    continue;
        }
	}
}

void Scheme::decide()
{
    this->hasAnswer = false;
    vector<_Time> times;
    int size = vv.size();
	if(size!=0)
    {
        _Time time;
        for (int i = 0; i < size; i++)
        {
            times.push_back(time);
        }
    }
	Answer answer;
    this->solve(times, answer, 0); 
}

void Scheme::output()
{
	
}

void Scheme::syncToDB()
{
    string sql = "delete from Event_Vtrs";
    sqlOperator(sql);
    for (int i = 0; i < this->ev.size(); i++)
    {
        for (int j : this->answer[i])
        {
            string sql = "insert into Event_Vtrs values (" + to_string(this->ev[i].id) + "," + to_string(this->vv[j].id) + ");";
            sqlOperator(sql);
        }
    }
}

void Scheme::exportAsCSV()
{
    FILE *f=fopen("result.csv", "w");
    fprintf(f, "事程名称,时间,志愿者\n");
    for (int i = 0; i < this->ev.size(); i++)
	{
        string vtrs="";
		for(auto i: this->answer[i])
		{
            vtrs += this->vv[i].name+" ";
		}
		fprintf(f, "%s,%s,%s\n", this->ev[i].name.c_str(),this->ev[i].time.toString().c_str(),vtrs.c_str());
	}
	fclose(f);
}

vector<int> sortByHasEXp(VtrVec* vv)
{
	vector<int> result,tmp;
    int i= 0;
	while(i<vv->size())
	{
		if(vv->at(i).hasExp)
		{
			result.push_back(i);
		}
		else
		{
			tmp.push_back(i);
		}
		i++;
	}
	for(int i=0;i<tmp.size();i++)
	{
		result.push_back(tmp[i]);
	}
	return result;
}

_Time::_Time()
{
    for (int i = 0; i < DAYS_NUM; i++)
    {
        this->day[i] = 0;
    }
}

_Time::_Time(const Time &T)
{
    for (int i = 0; i < DAYS_NUM; i++)
    {
        this->day[i] = 0;
    }
    int day = T.day;
    for (int i = T.start-1; i < T.end; i++)
    {
        this->day[day] |= (1 << i);
    }
}

_Time _Time::operator+(const Time& t) const
{
    _Time t1 = _Time(t);
    _Time result;
	for (int i = 0; i < DAYS_NUM; i++)
    {
        result.day[i] = this->day[i] | t1.day[i];
	}
	return result;
}

bool _Time::operator*(const Time& t) const
{
    _Time t1 = _Time(t);
    for(int i=0;i<DAYS_NUM;i++)
	{
		if(this->day[i]&t1.day[i])
		{
			return true;
		}
	}
	return false;
}

