// 数据库连接类 维护数据库连接对象

#ifndef MSQL_CONNECTOR_H
#define MSQL_CONNECTOR_H


#include "mysql.h"
#include <string>
using std::string ;


class SelectResult;
struct dataPos;
class MysqlConnector
{
public:
	MysqlConnector(void);
	~MysqlConnector(void);


public:
	int Connect(string hostAddress, string userName, string password, string dbName, int port = 3306); //创建数据库连接
	bool SelectDataBase(string dbName); //选择数据库
	void Close(); //关闭数据库连接
	MYSQL* GetMysqlObject(); //返回MYSQL*

private:
	MYSQL *m_mysql;
};


#endif
