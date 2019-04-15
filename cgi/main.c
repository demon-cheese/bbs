#define  _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<Windows.h>
#include "C:\web\mysql\include\mysql.h"
#pragma comment(lib, "C:/web/mysql/lib/libmysql.lib")
void new_web(void);
int main(void)
{
	MYSQL mysql, *sock; //声明MySQL的句柄 
	const char * host = "127.0.0.1"; //因为是作为本机测试，所以填写的是本地IP 
	const char * user = "root"; //这里改为你的用户名，即连接MySQL的用户名 
	const char * passwd = "358789"; //这里改为你的用户密码 
	const char * db = "run001"; //这里改为你要连接的数据库的名字,一个数据可能有几张表
	unsigned int port = 3306; //这是MySQL的服务器的端口，如果你没有修改过的话就是3306。 
	const char * unix_socket = NULL; //unix_socket这是unix下的，我在Windows下，所以就把它设置为NULL 
	unsigned long client_flag = 0; //这个参数一般为0 

	char query_user[80];// = "select xyz_passwd from xyz where xyz_name = ""test""; "; //查询语句，从那个表中查询,这里后面没有;

	MYSQL_RES * result; //保存结果集的
	MYSQL_ROW row; //代表的是结果集中的一行 

	//my_ulonglong row;

	char *str_len = NULL;//定义字符型指针
	int len = 0;
	char buf[100] = "";
	char user_l[20] = "";
	char passwd_l[20] = "";

	printf("%s\r\n\r\n", "Content-Type:text/html");//CGI标准头
	printf("<!DOCTYPE html>\n");
	printf("<html >\n <head> \n <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF -8\">\n<title>留言板</title><br>\n");
	str_len = getenv("CONTENT_LENGTH");//获取输入字符长度
	//如果str_len没有赋值成功,如果str_len有多过数值,如果len大于80则打印出错
	if ((str_len == NULL) || (sscanf(str_len, "%d", &len) != 1) || (len > 80))
		printf("sorry!error!");
	fgets(buf, len + 1, stdin); //从标准输入中读取len个字符
	int i = 0;
	int j = 0;
	int k = 0;
	for (; i <= len; i++) //从buf读取用户名到user_l中
	{
		if (buf[i] == 'n') {
			if (buf[i + 1] == 'a') {
				if (buf[i + 2] == 'm') {
					if (buf[i + 3] == 'e') {
						if (buf[i + 4] == '=') {
							for (; j <= 25 && buf[i + 5 + j] != '&'; j++) {
								user_l[j] = buf[i + 5 + j];
							}
							break;
						}
					}
				}
			}
		}
	}
	for (i = i + 6 + j; i <= len; i++)  //从buf读取用户密码名到passwd_l中
	{
		if (buf[i] == 'p') {
			if (buf[i + 1] == 'a') {
				if (buf[i + 2] == 's') {
					if (buf[i + 3] == 's') {
						if (buf[i + 4] == 'w') {
							if (buf[i + 5] == 'o') {
								if (buf[i + 6] == 'r') {
									if (buf[i + 7] == 'd') {
										if (buf[i + 8] == '=') {
											for (; k < (len - i - 9); k++) {
												passwd_l[k] = buf[i + 9 + k];
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	j = j - 1;
	k = k - 1;
	//printf("<body><h1>Welcome! hello %s %d %s %d</h1></body></html>", user_l, j, passwd_l, k);

	mysql_init(&mysql); //连接之前必须使用这个函数来初始化 
	sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag);
	sprintf(query_user,"select xyz_passwd from xyz where xyz_name=\"%s\";" ,user_l );
	
	if (mysql_query(&mysql, query_user) != 0)//如果连接成功，则开始查询 .成功返回0
	{
		printf("</head>\n<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/index_1.html\"></body></html>");
		exit(1);
		//printf("<body><h1>fail to query!</h1></body></html>");
		//exit(1);
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果 
		{
			printf("</head>\n<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/index_1.html\"></body></html>");
			exit(1);
			//printf("<body><h1>fail to store result!</h1></body></html>");
			//exit(1);
		}
		else
		{
			if ((row = mysql_fetch_row(result)) != NULL) //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
			{
				//printf("<body><h1>name is %s\t</h1></body></html>",row[0]);
			}
			else {
				printf("</head>\n<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/index_1.html\"></body></html>");
				exit(1);
			}
		}

	}
	for (int q=0;row[0][q] != '\0';q++)  {
		if (passwd_l[q] != row[0][q]) {
			printf("</head>\n<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/index_1.html\"></body></html>");
			exit(1);
		}
	}
	mysql_free_result(result); //释放结果集 
	mysql_close(sock); //关闭连接 
	new_web();
	exit(EXIT_SUCCESS);
	return 0;
}

void new_web()
{
	printf("<style type=\"text/css\"> \n");
	printf("body {width:700px;height:600px;background-image:url(../image/first.jpg);background-repeat:repeat;background-size:cover;}\n");
	printf("</style> \n </head> \n");
	printf("<body>\n");
	printf("<h1>神秘博士</h1>\n");
	printf("</body> \n </html>");
}
