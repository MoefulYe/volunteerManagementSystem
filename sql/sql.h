/*
*基于sqlite3的C语言接口的二次封装
*从数据库中读取并返回志愿者和事程的vector
*并提供简单的sql操作
*/
#pragma once
#ifndef SQL_INCLUDED
#define SQL_INCLUDED
#include "../core/core.h"
void sqlOperator(string sql);
VtrVec getAllVtrs();
EventVec getAllEvents();
#endif // !SQL_INCLUDED
