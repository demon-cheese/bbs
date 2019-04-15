#define  _CRT_SECURE_NO_WARNINGS 
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

	char query_user[80]; //查询语句;
	char query_text[80];
	char query_name[80];
	MYSQL_RES * result; //保存结果集的
	MYSQL_RES * result2; //保存结果集的
	MYSQL_ROW row; //代表的是结果集中的一行 
	MYSQL_ROW row_text; //代表的是结果集中的一行 
	MYSQL_ROW row_name; //代表的是结果集中的一行 


	char *str_len = NULL;//定义字符型指针
	int len = 0;
	char buf[100] = "";
	char user_l[20] = "";
	char passwd_l[20] = "";
	char date[12];
	char ip_text[40];

	time_t time_data;
	struct tm *td;
	time(&time_data);
	td = gmtime(&time_data);
	sprintf(date, "%d-%d-%d", 1900 + td->tm_year, 1 + td->tm_mon, td->tm_mday);//将系统时间写入date

	printf("%s\r\n\r\n", "Content-Type:text/html");//CGI标准头
	printf("<!DOCTYPE html>\n");
	printf("<html >\n <head> \n <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF -8\">\n<title>留言板</title><br>\n");
	str_len = getenv("CONTENT_LENGTH");//获取输入字符长度
	char *ip = getenv("REMOTE_ADDR");//获取用户ip地址
	char ip_content[45];
	sprintf(ip_text, "c:\\website\\ip\\%s.txt", ip);//转换为地址+ip.text
	//如果str_len没有赋值成功,如果str_len有多过数值,如果len大于80则打印出错
	if ((str_len == NULL) || (sscanf(str_len, "%d", &len) != 1) || (len > 80))
		printf("sorry!error!");
	fgets(buf, len + 1, stdin); //从标准输入中读取len个字符
	urldecode(buf);//对输入数据进行解码
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
											for (; k < (len - i - 9) && buf[i + 5 + k] != '\0'; k++) {
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
				sprintf(ip_content, "%s",user_l);
				FILE *fp = fopen(ip_text, "w");//创建一个名为用户ip的文档
				fprintf(fp, ip_content);//写入用户名
				fclose(fp);
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
	
	new_web();
	sprintf(query_text, "select hjk_text from hjk order by hjk_order desc limit 17;");
	//mysql_query(&mysql, query_text);
	//result = mysql_store_result(&mysql);
	//row_text = mysql_fetch_row(result);
	
	if (mysql_query(&mysql, query_text) != 0)//如果连接成功，则开始查询 .成功返回0
	{
		printf("</head>\n<body><p>query not ok</p></body></html>");
		exit(1);
		
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果 
		{
			printf("</head>\n<body><p>result not ok</p></body></html>");
			exit(1);
			
		}

	}

	
	sprintf(query_name, "select hjk_name from hjk order by hjk_order desc limit 17;");
	mysql_query(&mysql, query_name);
	result2 = mysql_store_result(&mysql);
	

	for (int m=0;m<16;m++) {//格式化输出最后17条用户留言
		row_text = mysql_fetch_row(result);
		row_name = mysql_fetch_row(result2);
		printf("<div class=\"logD logDtip\">\n");
		printf("<p class=\"p1\">%s</p>\n",row_text[0]);
		printf("<p class=\"p2\">作者：%s</p>\n",row_name[0]);
		printf("</div>\n");
	}
	
	printf("</body> \n </html>");//html结尾
	mysql_free_result(result); //释放结果集
	mysql_free_result(result2); //释放结果集
	mysql_close(sock); //关闭连接 
	exit(EXIT_SUCCESS);
	return 0;
}

void new_web()
{
	printf("<style type=\"text/css\"> \n");
	printf("body {width:700px;height:600px;background-image:url(../image/first.jpg);background-repeat:repeat;background-size:cover;}\n");
	printf("#wrap .logGet {width: 800px;  position: absolute  ;background-color:#FFFFFF;  background-repeat:repeat-y;  left: 0;  right: 0;  margin: auto;}\n");
	printf(".logGet .logD.logDtip .p1 {display:inline-block;  font-size:20px;  margin-top:50px;  margin-bottom:5px;  width:86%%;}\n");
	printf(".logGet .logD.logDtip .p2 {font-size:15px;  margin-top:8px;  margin-bottom:-22px;  text-align:right;}\n");
	printf("#wrap .logGet .logD.logDtip {width: 86%%;  border-bottom: 1px solid #ee7700;  margin-bottom:10px;  margin-top:0px;  margin-right:auto;  margin-left:auto;}\n");
	printf(".logGet .lgD textarea {width: 100%%;  height: 90px;}\n");
	printf("#wrap .logGet .lgD {width: 86%%;position: relative;margin-bottom: 25px;margin-top: 25px;margin-right: auto;margin-left: auto;}\n");
	printf("input[type=SUBMIT] {width:70px;  height:30px;  background-color:#ee7700;  border:none;  color:white;  font-size:15px;  float:right;}\n");
	printf("</style> \n </head> \n");
	printf("<body>\n");
	printf("<div class=\"wrap\" id=\"wrap\">\n");
	printf("<div class=\"logGet\">\n");
	printf("<form ACTION=\"/cgi/text.cgi\" METHOD=\"POST\" accept-charset=\"UTF - 8\">\n");
	printf("<div class=\"lgD\">\n");
	printf("<textarea NAME=\"text\" rows=\"5\" cols=\"50\"></textarea>\n");
	printf("<div>\n");
	printf("<input type=\"SUBMIT\" VALUE=\"提 交\"/>\n");
	printf("</div>\n");
	printf("</div>\n");
	printf("</form>\n");
	//printf("\n");

}
//以下部分为编码与解码url
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
