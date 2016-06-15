// ���ݿ������� ά�����ݿ����Ӷ���

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
	int Connect(string hostAddress, string userName, string password, string dbName, int port = 3306); //�������ݿ�����
	bool SelectDataBase(string dbName); //ѡ�����ݿ�
	void Close(); //�ر����ݿ�����
	MYSQL* GetMysqlObject(); //����MYSQL*

private:
	MYSQL *m_mysql;
};


#endif
