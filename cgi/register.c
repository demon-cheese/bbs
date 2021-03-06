﻿#define  _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "C:\web\mysql\include\mysql.h"
#pragma comment(lib, "C:/web/mysql/lib/libmysql.lib")
#define BURSIZE 2048

int hex2dec(char c);
char dec2hex(short int c);
void urlencode(char url[]);
void urldecode(char url[]);
void new_web(char * name);

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

	char query_insert[180];//插入语句
	char query_mail[80];

	MYSQL_RES * result; //保存结果集的
	MYSQL_ROW row; //代表的是结果集中的一行 

	char *str_len = NULL;//定义字符型指针
	int len = 0;
	char buf[100] = "";
	char mail[25] = "";
	char passwd_1[20] = "";
	char passwd_2[20] = "";

	printf("%s\r\n\r\n", "Content-Type:text/html");//CGI标准头
	printf("<!DOCTYPE html>\n");
	printf("<html >\n <head> \n <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF -8\">\n<title>注册成功</title><br>\n");
	str_len = getenv("CONTENT_LENGTH");//获取输入字符长度
	//如果str_len没有赋值成功,如果str_len有多过数值,如果len大于80则打印出错
	if ((str_len == NULL) || (sscanf(str_len, "%d", &len) != 1) || (len > 80))
		printf("sorry!error!");
	fgets(buf, len + 1, stdin); //从标准输入中读取len个字符
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	urldecode(buf);//解码输入数据
	for (; i <= len; i++) //从buf读取邮箱到mail中
	{
		if (buf[i] == 'm') {
			if (buf[i + 1] == 'a') {
				if (buf[i + 2] == 'i') {
					if (buf[i + 3] == 'l') {
						if (buf[i + 4] == '=') {
							for (; j <= 25 && buf[i + 5 + j] != '&'; j++) {
								mail[j] = buf[i + 5 + j];
							}
							break;
						}
					}
				}
			}
		}
	}
	
	for (i = i + 11 + j; i <= len; i++)  //从buf读取第一次用户密码名到passwd_1中
	{
		if (buf[i] == 'o') {
			if (buf[i+1] == 'r') {
				if (buf[i + 2] == 'd') {
					if (buf[i + 3] == '_') {
						if (buf[i + 4] == '1') {
							if (buf[i + 5] == '=') {
								for (; k <= 20 && buf[i + 6 + k] != '&'; k++) {
									passwd_1[k] = buf[i + 6 + k];
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	for (i = i + 10 + k; i <= len; i++)  //从buf读取第二次用户密码名到passwd_2中
	{
		if (buf[i] == 'r') {
			if (buf[i + 1] == 'd') {
				if (buf[i + 2] == '_') {
					if (buf[i + 3] == '2') {
						if (buf[i + 4] == '=') {
							for (; l < (len-5-i) && buf[i+5+l]!='\0'; l++) {
								passwd_2[l] = buf[i + 5 + l];
							}
							break;
						}
					}
				}
			}
		}
	}
	
	if (k != l) {
		printf("<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/register_1.html\"></body></html>");
		return 0;
	}
	if (k <= 5) {
		printf("<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/register_3.html\"></body></html>");
		return 0;
	}
	j = j - 1;
	k = k - 1;
	l = l - 1;
	for (int m = 0; m <= k; m++) {
		if (passwd_1[k] != passwd_2[k]) {
			printf("<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/register_1.html\"></body></html>");
			return 0;
		}
	}
	int n = 0;
	for (int m=0;m<=j;m++) {
		if (mail[m] == '@') {
			n = 1;
			break;
		}
	}
	if (n == 0) {
		printf("<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/register_2.html\"></body></html>");
		return 0;
	}
	
	char name[10];
	int name_num;
	srand((unsigned)time(NULL));
	name_num = rand() % 1000 + 1000000000+ 1000*(rand() % 1000)+ 1000000 * (rand() % 1000);
	_itoa(name_num, name, 10);
	mysql_init(&mysql); //连接之前必须使用这个函数来初始化 
	sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag);
	
	sprintf(query_mail, "select xyz_name from xyz where xyz_mail=\"%s\";", mail);
	mysql_query(&mysql, query_mail);
	result = mysql_store_result(&mysql);
	if ((row = mysql_fetch_row(result)) != NULL) //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
	{
			printf("</head>\n<body><meta http-equiv=\"Refresh\" content=\"0;URL=../error/register_4.html\"></body></html>");
			return 0;
	}

	sprintf(query_insert, "insert into xyz(xyz_mail,xyz_name,xyz_passwd)values(\"%s\", \"%s\", \"%s\"); ", mail, name, passwd_2);
	mysql_query(&mysql, query_insert);
	mysql_free_result(result); //释放结果集 
	mysql_close(sock); //关闭连接 
	new_web(name);
	exit(EXIT_SUCCESS);
	return 0;

	
}
void new_web(char * name) {
	printf("<style type=\"text/css\"> \n");
	printf("body {width:700px;height:600px;background-image:url(../image/first.jpg);background-repeat:repeat;background-size:cover;}\n");
	printf(".logB a button {width: 500px;height: 100px;background-color: #ee7700;border: none;color: white;font-size:25px;position: absolute;top:0 ;left : 0;right : 0;bottom: 0;margin: auto;}");
	printf("</style> \n </head> \n");
	printf("<body>\n");
	printf("<div class = \"logB\">\n<a href = \"../index.html\" target = \"_self\">\n<button>请牢记你的用户名：%s<br>点击登录</button>\n</a>\n</div>\n",name);
	printf("</body> \n </html>");
}
//以下为url解码部分
int hex2dec(char c)
{
	if ('0' <= c && c <= '9')
	{
		return c - '0';
	}
	else if ('a' <= c && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if ('A' <= c && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else
	{
		return -1;
	}
}

char dec2hex(short int c)
{
	if (0 <= c && c <= 9)
	{
		return c + '0';
	}
	else if (10 <= c && c <= 15)
	{
		return c + 'A' - 10;
	}
	else
	{
		return -1;
	}
}


//编码一个url
void urlencode(char url[])
{
	int i = 0;
	int len = strlen(url);
	int res_len = 0;
	char res[BURSIZE];
	for (i = 0; i < len; ++i)
	{
		char c = url[i];
		if (('0' <= c && c <= '9') ||
			('a' <= c && c <= 'z') ||
			('A' <= c && c <= 'Z') ||
			c == '/' || c == '.')
		{
			res[res_len++] = c;
		}
		else
		{
			int j = (short int)c;
			if (j < 0)
				j += 256;
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1 * 16;
			res[res_len++] = '%';
			res[res_len++] = dec2hex(i1);
			res[res_len++] = dec2hex(i0);
		}
	}
	res[res_len] = '\0';
	strcpy(url, res);
}

// 解码url
void urldecode(char url[])
{
	int i = 0;
	int len = strlen(url);
	int res_len = 0;
	char res[BURSIZE];
	for (i = 0; i < len; ++i)
	{
		char c = url[i];
		if (c != '%')
		{
			res[res_len++] = c;
		}
		else
		{
			char c1 = url[++i];
			char c0 = url[++i];
			int num = 0;
			num = hex2dec(c1) * 16 + hex2dec(c0);
			res[res_len++] = num;
		}
	}
	res[res_len] = '\0';
	strcpy(url, res);
}