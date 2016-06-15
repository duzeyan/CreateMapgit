// ���ݿ������� �����ݿ���в���
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
	bool Command(string sql); //ִ��sql���
	bool Command(); //ִ��sql���
	MYSQL_RES* GetResult(); //��ȡ��ѯ���
	void Release();

private:
	MYSQL* m_ptMysql; //���ݿ��˽����
};

#endif