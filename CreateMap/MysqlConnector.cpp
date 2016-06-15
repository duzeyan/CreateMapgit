#include "stdafx.h"

#include "MysqlConnector.h"




MysqlConnector::MysqlConnector(void)
{
	m_mysql=mysql_init((MYSQL*) 0);
}


MysqlConnector::~MysqlConnector(void)
{
	if (m_mysql!=NULL)
	{
		Close();
	}
	
}

int MysqlConnector::Connect(string hostAddress, string userName, string password, string dbName, int port)
{
	if (hostAddress.empty() || userName.empty() || password.empty() || dbName.empty())
	{
		return 0;
	}
	m_mysql=mysql_real_connect(m_mysql, hostAddress.c_str(), userName.c_str(), password.c_str(), dbName.c_str(),
		port, NULL, 0);
	if (m_mysql!=NULL)
	{
		//mysql_query(&m_mysql, "SET NAMES UTF-8"); //设置编码格式,否则在cmd下无法显示中文
		return 1;
	}
	return 0;
}

bool MysqlConnector::SelectDataBase(string dbName)
{
	if (mysql_select_db(m_mysql, dbName.c_str()) != 0)
	{
		return false;
	}
	return true;
}

void MysqlConnector::Close()
{
	mysql_close(m_mysql);
	m_mysql=NULL;
}

MYSQL* MysqlConnector::GetMysqlObject()
{
	return m_mysql;
}
