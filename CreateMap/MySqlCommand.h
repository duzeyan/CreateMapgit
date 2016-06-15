// 数据库连接类 对数据库进行操作
#ifndef MSQL_CMD_H
#define MSQL_CMD_H


#include "mysql.h"
#include <string>
#include <vector>
using namespace std ;


class MysqlCommand
{
public:
	MysqlCommand(MYSQL* mysql);
	~MysqlCommand();

public:
	bool Command(string sql); //执行sql语句
	bool Command(); //执行sql语句
	MYSQL_RES* GetResult(); //获取查询结果
	void Release();

private:
	MYSQL* m_ptMysql; //数据库了解对象
};

#endif