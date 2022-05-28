#include "sqlite3.h"
#include "sql.h"
#include "core.h"
#include "util.h"
#include <iostream>
#include<vector>
constexpr auto SQL_PATH = "./volunteer.db";
using namespace std;

//使用sql语句操作数据库
void sqlOperator(string sql)
{
	sqlite3 *db;
	char *errmsg;
	int rc;
	rc = sqlite3_open(SQL_PATH, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return;
	}
	rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errmsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << errmsg << endl;
		sqlite3_free(errmsg);
	}
	sqlite3_close(db);
}

/*
*CREATE TABLE Event(
*	`id` INTEGER PRIMARY KEY AUTOINCREMENT,
*    `name` CHAR(20) NOT NULL,
*    `day` TINYINT NOT NULL,
*    `start` TINYINT NOT NULL,
*    `end` TINYINT NOT NULL,
*    `needVtrsNum` TINYINT NOT NULL
*);
* CREATE TABLE Volunteer(
*    `id` INTEGER PRIMARY KEY AUTOINCREMENT,
*    `name` CHAR(20) NOT NULL,
*    `gender` CHAR(1) NOT NULL,
*    `age` TINYINT NOT NULL,
*    `telephone` CHAR(20) NOT NULL,
*    `idCard` CHAR(20) NOT NULL,
*    `profile` CHAR(100) NOT NULL,
*    `hasExp` TINYINT NOT NULL,
*    `langCommand` TINYINT NOT  NULL,
*    `availTime` CHAR(100) NOT NULL
*);
* CREATE TABLE Event_Vtrs(
*	`eventId` INTEGER,
*    `vtrId` INTEGER,
*    FOREIGN KEY(eventId) REFERENCES Event(id),
*    FOREIGN KEY(vtrId) REFERENCES Volunteer(id)
*);
*/

//从数据库中读取所有的事程，并返回一个动态数组

EventVec getAllEvents()
{
	EventVec ev;
	sqlite3 *db;
	char* errmsg = nullptr;
	int rc;
	rc = sqlite3_open(SQL_PATH, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return ev;
	}
	string sql = "select * from event";
	sqlite3_stmt *stmt;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return ev;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		Event e;
		e.id = sqlite3_column_int(stmt, 0);
		e.name = (char*)sqlite3_column_text(stmt, 1);
		e.time.day = sqlite3_column_int(stmt, 2);
		e.time.start = sqlite3_column_int(stmt, 3);
		e.time.end = sqlite3_column_int(stmt, 4);
		e.needVtrsNum = sqlite3_column_int(stmt, 5);
		string sql2="select vtrId from Event_Vtrs where eventId="+to_string(e.id);
		sqlite3_stmt *stmt2;
		rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt2, NULL);
		if (rc != SQLITE_OK)
		{
			cout << "SQL error: " << sqlite3_errmsg(db) << endl;
			sqlite3_close(db);
			return ev;
		}
		int i = 0;
		while(sqlite3_step(stmt2) == SQLITE_ROW)
		{
			e.vtrs[i] = sqlite3_column_int(stmt2, 0);
			i++;
		}
		sqlite3_finalize(stmt2);
		for (; i < MAX_NEEDED_VTRS; i++) 
		{
			e.vtrs[i] = -1;
		}
		ev.push_back(e);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return ev;
}

//从数据库中读取所有的志愿者记录并返回一个动态数组
VtrVec getAllVtrs()
{
	VtrVec vv;
	sqlite3* db;
	char* errmsg;
	int rc;
	rc = sqlite3_open(SQL_PATH, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return vv;
	}
	string sql = "select * from volunteer";
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return vv;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		Volunteer v;
		v.id = sqlite3_column_int(stmt, 0);
		v.name = (char*)sqlite3_column_text(stmt, 1);
		v.age = sqlite3_column_int(stmt, 2);
		v.gender = (char*)sqlite3_column_text(stmt, 3);
		v.telephone = (char*)sqlite3_column_text(stmt, 4);
		v.idCard = (char*)sqlite3_column_text(stmt, 5);
		v.profile =(char*)sqlite3_column_text(stmt, 6);
		v.hasExp = sqlite3_column_int(stmt, 7)!=0?true:false;
		int value=sqlite3_column_int(stmt, 8);
		for (int i = 0; i < LANG_NUM; i++)
		{
			v.langCommand[i] = value%2==1 ? true : false;
			value/=2;
		}
		char* timeStr = (char*)sqlite3_column_text(stmt, 9);
		vector<string> vs = split(timeStr,",");
		for (int i = 0; i < DAYS_NUM; i++)
		{
			vector<string> vs1 = split(vs[i],"-");
			v.availTime[i][0] = stoi(vs1[0]);
			v.availTime[i][1] = stoi(vs1[1]);
		}
		string sql2= "select eventId from Event_Vtrs where vtrId=" + to_string(v.id);
		sqlite3_stmt* stmt2;
		rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt2, NULL);
		if (rc != SQLITE_OK)
		{
			cout << "SQL error: " << sqlite3_errmsg(db) << endl;
			sqlite3_close(db);
			return vv;
		}
		int i = 0;
		while (sqlite3_step(stmt2) == SQLITE_ROW)
		{
			v.result[i] = sqlite3_column_int(stmt2, 0);
			i++;
		}
		for (; i < MAX_ATTEND_EVENT; i++)
		{
			v.result[i] = -1;
		}
		vv.push_back(v);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return vv;
}
