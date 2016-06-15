#include "stdafx.h"
#include "MySqlCommand.h"

MysqlCommand::MysqlCommand(MYSQL* mysql):m_ptMysql(mysql)
{
}

MysqlCommand::~MysqlCommand()
{
	Release();
}



bool MysqlCommand::Command(string sql)
{
	if (mysql_query(m_ptMysql, sql.c_str()) != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}



MYSQL_RES* MysqlCommand::GetResult()
{
	MYSQL_ROW row;
	MYSQL_RES* res_set;

	res_set = mysql_store_result(m_ptMysql);
	if (res_set == NULL)
	{
		return NULL;
	}
	return res_set;
	/*int rowCount = 0;
	while ((row = mysql_fetch_row(res_set)) != NULL)
	{
		vector<string> list;
		for (size_t i = 0; i < mysql_num_fields(res_set); i++)
		{
			list.push_back(row[i]);
		}
		dataList.push_back(list);
		++rowCount;
	}*/
}

void MysqlCommand::Release()
{
	
}