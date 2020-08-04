#include <windows.h>//注意头文件顺序
#include "stdio.h"
#include "winsock.h"
#include "mysql.h"
#include<iostream>
#include<fstream>
#include<cstring>
#include<map>
const int MAX = 200;
char query[150];//查询语句
using namespace std;
MYSQL mysql, * sock;   //声明MySQL的句柄
MYSQL_FIELD* fd;  //字段列数组
char field[32][32];  //存字段名二维数组
MYSQL_RES* res; //这个结构代表返回行的一个查询结果集
MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列
/*员工类*/
class person {
public:
	int num;/*工号*/
	char name[20];/*姓名*/
	char sex[10]; /*性别*/
	int age;/*年龄*/
};
/*教师类*/
class Teacher :virtual public person {
public:
	char depart[20];//(系别)
	char spec[20];//（专业）
	char title[20];//(职称)
	void input();
	void output();

};
Teacher Tea[MAX];
static int teacount;/*教师人数*/
static int sex_teacount = 0, depa_teacount = 0, title_teacount = 0;
void Teacher::input()
{
	cout << "请输入工号:"; cin >> num;
	cout << "请输入姓名:"; cin >> name;
	cout << "请输入性别:"; cin >> sex;
	cout << "请输入年龄:"; cin >> age;
	cout << "请输入系别:"; cin >> depart;
	cout << "请输入专业:"; cin >> spec;
	cout << "请输入职称:"; cin >> title;
	sprintf_s(query, "insert into teacher values(%d,'%s','%s',%d,'%s','%s','%s')",num,name,sex,age,depart,spec,title);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		cout<<"数据库插入失败";
	}
	else
	{
		printf("Insert success\n");
		cout << "数据库插入成功";
	}


}
void Teacher::output()
{
	cout << "工号: " << num << "  " << "姓名: " << name << "  " << "性别: " << sex << "  " << "年龄: " << age << "  " << "所在系: " << depart << "  " << "专业: " << spec << "  " << "职称: " << title << endl;
	
}
/*教师管理类*/
class Teamanager {
public:
	int add();
	int search();
	void show();
	void edit();
	int Delete();
	void account();
	void save();
	void read();
	bool Deletemysql();
	bool mysqledit();
	bool QueryDatabase();
};
int Teamanager::add()
{
	Teacher t;
	int tea_num;
	cout << "请输入教师的编号"; cin >> tea_num;
	if (teacount == MAX)
	{
		cout << "教师人数已满" << endl;
		return 0;
	}
	for (int i = 0; i < teacount; i++)
	{
		if (tea_num == Tea[i].num)
		{
			cout << "编号已存在" << endl;
			return 0;
		}
	}
	t.input();
	Tea[teacount] = t;
	teacount++;
	cout << "教师添加成功" << endl;
	return true;
	


}
int Teamanager::search()
{
	int choice;
	int find_num, j;
	string find_sex, find_name, name_depart;
	cout << "===========1.工号查询  2.姓名查询  3.性别查询  0.退出==========" << endl;
	cout << "请输入查找的方式:"; cin >> choice;
	while (choice != 0)
	{
		if (choice == 1)
		{
			cout << "请输入工号:"; cin >> find_num;
			for (j = 0; j < teacount; j++)
			{
				if (find_num == Tea[j].num)
					break;
			}
			if (j == teacount)
				cout << "查无此人" << endl;
			else
				Tea[j].output();
			return true;
		}
		else if (choice == 2)
		{
			cout << "请输入姓名:"; cin >> find_name;
			for (j = 0; j < teacount; j++)
			{
				if (find_name == Tea[j].name)
					break;
			}
			if (j == teacount)
				cout << "查无此人" << endl;
			else
				Tea[j].output();
			return true;
		}
		else if (choice == 3)
		{
			cout << "请输入性别:"; cin >> find_sex;
			for (j = 0; j < teacount; j++)
			{
				if (find_sex == Tea[j].sex)
				{
					Tea[j].output();
				}
			}
			if (j == teacount)
				cout << "查无此人" << endl;
			return true;
		}
		cin >> choice;

	}
}
void Teamanager::show()
{
	if (teacount == 0)
	{
		cout << "记录为空！" << endl; return;
	}
	for (int i = 0; i < teacount; i++)
		Tea[i].output();
}
void Teamanager::edit()
{
	Teacher t1;
	int j, n;
	cout << "请输入要编辑的人的编号："; cin >> n;
	for (j = 0; j < teacount; j++)
	{
		if (n == Tea[j].num) break;
	}
	if (j == teacount)
	{
		cout << "查无此人！" << endl; return;
	};
	cout << "输入修改后的信息，编号不能改:" << endl;
	t1.input();
	Tea[j] = t1;
	cout << "编辑成功！" << endl;
}
bool Teamanager::mysqledit()
{
	Teacher t2;
	cout << "请输入你要编辑的编号" << endl;
	char editnum[20];
	cin >> editnum;
	sprintf_s(query, "delete from teacher where num=%s;", editnum);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("delete success\n");
		t2.input();
		printf("编辑成功\n");
		return 1;
		
	}

}
int Teamanager::Delete()
{
	int j, n;
	cout << "请输入要删除的人的编号："; cin >> n;
	for (j = 0; j < teacount; j++)
	{
		if (n == Tea[j].num) break;
	}
	if (j == teacount)
	{
		cout << "没有此人！" << endl; return 0;
	};
	for (j; j < teacount; j++)
	{
		Tea[j] = Tea[j + 1];
	}
	teacount--;
	cout << "删除成功！" << endl;
	return 1;
}
bool Teamanager::Deletemysql()
{
	char query[100];
	char delnum[20];
	cout << "请输入要删除的人员的编号" << endl;
	cin >> delnum;
	sprintf_s(query, "delete from teacher where num=%s;",delnum);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("Insert success\n");
		return 1;
	}
}
void Teamanager::account()
{
	map<string, int>tea_mp;
	for (int i = 0; i < teacount; i++)
	{
		tea_mp[Tea[i].sex]++;
		tea_mp[Tea[i].spec]++;
		tea_mp[Tea[i].title]++;
	}
	map<string, int>::iterator it;//迭代器 
	for (it = tea_mp.begin(); it != tea_mp.end(); it++)
		cout << it->first << ":" << it->second << "\n";
}
void Teamanager::save()
{
	int i;
	ofstream outfile, outfile1;
	outfile1.open("Teatop.dat", ios::out);
	outfile1 << teacount;
	outfile.open("Tea_data.dat", ios::binary);
	if (!outfile)
	{
		cerr << "打开文件失败" << endl; return;
	}
	for (i = 0; i < teacount; i++)
		outfile.write((char*)&Tea[i], sizeof(Tea[i]));
	outfile.close();
	cout << "保存成功！" << endl;
}
void Teamanager::read()
{
	int i;
	ifstream infile, infile1;
	infile1.open("Teatop.dat", ios::in);
	infile1 >> teacount;
	infile.open("Tea_data.dat", ios::binary);
	if (!infile)
	{
		cerr << "打开文件失败" << endl; return;
	}
	for (i = 0; i < teacount; i++)
		infile.read((char*)&Tea[i], sizeof(Tea[i]));
	infile.close();
	cout << "读取成功！" << endl;
}
bool Teamanager::QueryDatabase()
{
	mysql_query(&mysql, "set names gbk");
	//返回0 查询成功，返回1查询失败
	if (mysql_query(&mysql, "select * from teacher"))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	res = mysql_store_result(&mysql);
	//打印数据行数
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
	for (int i = 0; fd = mysql_fetch_field(res); i++)  //获取字段名
		strcpy_s(field[i], fd->name);
	int j = mysql_num_fields(res);  // 获取列数
	for (int i = 0; i < j; i++)  //打印字段
		printf("%10s\t", field[i]);
	printf("\n");
	while (column = mysql_fetch_row(res))
	{
		for (int i = 0; i < j; i++)
			printf("%10s\t", column[i]);
		printf("\n");
	}
	return true;
}
void Tea_mune(Teamanager TM)
{
	int b;
	char c;
	do {
		cout << "教师管理" << endl;
		cout << "==========================================================================================" << endl;
		cout << "1.数据添加    ";
		cout << "2.dat文件查询    ";
		cout << "3.dat文件显示    ";
		cout << "4.dat文件编辑    ";
		cout << "5.dat文件删除    \n";
		cout << "6.类别统计    ";
		cout << "7.dat文件保存    ";
		cout << "8.dat文件读取    ";
		cout << "9.数据统计    ";
		cout << "10.数据库删除    \n";
		cout << "11.数据库编辑    ";
		cout << "12.数据库显示     ";
		cout << "0.退出" << endl;
		cout << "==========================================================================================" << endl;
		cout << "请选择："; cin >> b;
		switch (b)
		{
		case 1:TM.add(); break;
		case 2:TM.search(); break;
		case 3:TM.show(); break;
		case 4:TM.edit(); break;
		case 5:TM.Delete(); break;
		case 6:cout << "共有教师人数：" << teacount << endl; break;
		case 7:TM.save(); break;
		case 8:TM.read(); break;
		case 9:TM.account(); break;
		case 10:TM.Deletemysql(); break;
		case 11:TM.mysqledit(); break;
		case 12:TM.QueryDatabase(); break;
		default:cout << "\n error" << endl; break;
		case 0:break;
		}

	} while (b != 0);
}
/*教师类结束*/

/*行政类开始*/
class Policer :virtual public person {
public:
	char politics[20];//: （政治面貌）
	char title[20];// (职称)
	void input();
	void output();

};
Policer Pol[MAX];
static int polcount;/*行政人数*/
static int sex_polcount = 0, politics_polcount = 0, title_polcount = 0;
void Policer::input()
{
	cout << "请输入工号:"; cin >> num;
	cout << "请输入姓名:"; cin >> name;
	cout << "请输入性别:"; cin >> sex;
	cout << "请输入年龄:"; cin >> age;
	cout << "请输入政治面貌:"; cin >> politics;
	cout << "请输入职称:"; cin >> title;
	sprintf_s(query, "insert into policer values(%d,'%s','%s',%d,'%s','%s')", num, name, sex, age, politics,title);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		cout << "数据库插入失败";
	}
	else
	{
		printf("Insert success\n");
		cout << "数据库插入成功";
	}

}
void Policer::output()
{
	cout << "编号: " << num << "  " << "姓名: " << name << "  " << "性别: " << sex << "  " << "年龄: " << age << "  " << "政治面貌: " << politics << "  " << "职称: " << title << endl;
}
/*行政管理类*/
class Polmanager {
public:
	int add();
	int search();
	void show();
	void edit();
	int Delete();
	void account();
	void save();
	void read();
	bool Deletemysql();
	bool mysqledit();
	bool QueryDatabase();
};
int Polmanager::add()
{
	Policer p;
	int pol_num;
	cout << "请输入行政的编号"; cin >> pol_num;
	if (polcount == MAX)
	{
		cout << "行政人数已满" << endl;
		return 0;
	}
	for (int i = 0; i < polcount; i++)
	{
		if (pol_num == Pol[i].num)
		{
			cout << "编号已存在" << endl;
			return 0;
		}
	}
	p.input();
	Pol[polcount] = p;
	polcount++;
	cout << "教师添加成功" << endl;
	return true;

}
int Polmanager::search()
{
	int choice;
	int find_num, j;
	string find_sex, find_name, name_depart;
	cout << "===========1.工号查询  2.姓名查询  3.性别查询  0.退出==========" << endl;
	cout << "请输入查找的方式:"; cin >> choice;
	while (choice != 0)
	{
		if (choice == 1)
		{
			cout << "请输入工号:"; cin >> find_num;
			for (j = 0; j < polcount; j++)
			{
				if (find_num == Pol[j].num)
					break;
			}
			if (j == polcount)
				cout << "查无此人" << endl;
			else
				Pol[j].output();
			return true;
		}
		else if (choice == 2)
		{
			cout << "请输入姓名:"; cin >> find_name;
			for (j = 0; j < polcount; j++)
			{
				if (find_name == Pol[j].name)
					break;
			}
			if (j == polcount)
				cout << "查无此人" << endl;
			else
				Pol[j].output();
			return true;
		}
		else if (choice == 3)
		{
			cout << "请输入性别:"; cin >> find_sex;
			for (j = 0; j < polcount; j++)
			{
				if (find_sex == Pol[j].sex)
				{
					Pol[j].output();
				}
			}
			if (j == polcount)
				cout << "查无此人" << endl;
			return true;
		}
		cin >> choice;

	}
}
void Polmanager::show()
{
	if (polcount == 0)
	{
		cout << "记录为空！" << endl; return;
	}
	for (int i = 0; i < polcount; i++)
		Pol[i].output();
}
void Polmanager::edit()
{
	Policer p1;
	int j, n;
	cout << "请输入要编辑的人的编号："; cin >> n;
	for (j = 0; j < polcount; j++)
	{
		if (n == Pol[j].num) break;
	}
	if (j == polcount)
	{
		cout << "查无此人！" << endl; return;
	};
	cout << "输入修改后的信息，编号不能改:" << endl;
	p1.input();
	Pol[j] = p1;
	cout << "编辑成功！" << endl;
}
bool Polmanager::mysqledit()
{
	Policer p2;
	cout << "请输入你要编辑的编号" << endl;
	char editnum[20];
	cin >> editnum;
	sprintf_s(query, "delete from policer where num=%s;", editnum);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("delete success\n");
		p2.input();
		printf("编辑成功\n");
		return 1;

	}

}
int Polmanager::Delete()
{
	int j, n;
	cout << "请输入要删除的人的编号："; cin >> n;
	for (j = 0; j < polcount; j++)
	{
		if (n == Pol[j].num) break;
	}
	if (j == polcount)
	{
		cout << "没有此人！" << endl; return 0;
	};
	for (j; j < polcount; j++)
	{
		Pol[j] = Pol[j + 1];
	}
	polcount--;
	cout << "删除成功！" << endl;
	return 1;
}
bool Polmanager::Deletemysql()
{
	char query[100];
	char delnum[20];
	cout << "请输入要删除的人员的编号" << endl;
	cin >> delnum;
	sprintf_s(query, "delete from policer where num=%s;", delnum);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("Insert success\n");
		return 1;
	}
}
void Polmanager::account()
{
	map<string, int>pol_mp;
	for (int i = 0; i < polcount; i++)
	{
		pol_mp[Pol[i].sex]++;
		pol_mp[Pol[i].politics]++;
		pol_mp[Pol[i].title]++;
	}
	map<string, int>::iterator it;//迭代器 
	for (it = pol_mp.begin(); it != pol_mp.end(); it++)
		cout << it->first << ":" << it->second << "\n";
}
void Polmanager::save()
{
	int i;
	ofstream outfile, outfile1;
	outfile1.open("Poltop.dat", ios::out);
	outfile1 << polcount;
	outfile.open("Pol_data.dat", ios::binary);
	if (!outfile)
	{
		cerr << "打开失败" << endl; return;
	}
	for (i = 0; i < polcount; i++)
		outfile.write((char*)&Pol[i], sizeof(Pol[i]));
	outfile.close();
	cout << "保存成功！" << endl;
}
void Polmanager::read()
{
	int i;
	ifstream infile, infile1;
	infile1.open("Poltop.dat", ios::in);
	infile1 >> polcount;
	infile.open("Pol_data.dat", ios::binary);
	if (!infile)
	{
		cerr << "打开失败" << endl; return;
	}
	for (i = 0; i < polcount; i++)
		infile.read((char*)&Pol[i], sizeof(Pol[i]));
	infile.close();
	cout << "读取成功！" << endl;
}
bool Polmanager::QueryDatabase()
{
	mysql_query(&mysql, "set names gbk");
	//返回0 查询成功，返回1查询失败
	if (mysql_query(&mysql, "select * from policer"))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	res = mysql_store_result(&mysql);
	//打印数据行数
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
	for (int i = 0; fd = mysql_fetch_field(res); i++)  //获取字段名
		strcpy_s(field[i], fd->name);
	int j = mysql_num_fields(res);  // 获取列数
	for (int i = 0; i < j; i++)  //打印字段
		printf("%10s\t", field[i]);
	printf("\n");
	while (column = mysql_fetch_row(res))
	{
		for (int i = 0; i < j; i++)
			printf("%10s\t", column[i]);
		printf("\n");
	}
	return true;
}
void Pol_mune(Polmanager PO)
{
	int b;
	char c;
	do {
		cout << "行政管理" << endl;
		cout << "==========================================================================================" << endl;
		cout << "1.数据添加    ";
		cout << "2.dat文件查询    ";
		cout << "3.dat文件显示    ";
		cout << "4.dat文件编辑    ";
		cout << "5.dat文件删除    \n";
		cout << "6.类别统计    ";
		cout << "7.dat文件保存    ";
		cout << "8.dat文件读取    ";
		cout << "9.数据统计    ";
		cout << "10.数据库删除    \n";
		cout << "11.数据库编辑    ";
		cout << "12.数据库显示     ";
		cout << "0.退出" << endl;
		cout << "==========================================================================================" << endl;
		cout << "请选择："; cin >> b;
		switch (b)
		{
		case 1:PO.add(); break;
		case 2:PO.search(); break;
		case 3:PO.show(); break;
		case 4:PO.edit(); break;
		case 5:PO.Delete(); break;
		case 6:cout << "共有行政人数：" << polcount << endl; break;
		case 7:PO.save(); break;
		case 8:PO.read(); break;
		case 9:PO.account(); break;
		case 10:PO.Deletemysql(); break;
		case 11:PO.mysqledit(); break;
		case 12:PO.QueryDatabase(); break;
		default:cout << "\n error" << endl; break;
		case 0:break;
		}


	} while (b != 0);
}
/*行政类结束*/

/*教辅类开始*/
/*
软教辅

硬教辅

*/
class Teacher_aids :virtual public person {
public:
	char depart_aids[20];//(软或硬教辅)
	char title[20];//(职称) 实验员这类的 
	void input();
	void output();

};
Teacher_aids Tea_aids[MAX];
static int tea_aidscount;/*教辅人数*/
static int sex_tea_aidscount = 0, depa_tea_aidscount = 0, title_tea_aidscount = 0;
void Teacher_aids::input()
{
	cout << "请输入工号:"; cin >> num;
	cout << "请输入姓名:"; cin >> name;
	cout << "请输入性别:"; cin >> sex;
	cout << "请输入年龄:"; cin >> age;
	cout << "请输入教辅类别:"; cin >> depart_aids;
	cout << "请输入职称:"; cin >> title;
	sprintf_s(query, "insert into teacher_aids values(%d,'%s','%s',%d,'%s','%s')", num, name, sex, age, depart_aids,title);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		cout << "数据库插入失败";
	}
	else
	{
		printf("Insert success\n");
		cout << "数据库插入成功";
	}

}
void Teacher_aids::output()
{
	cout << "工号: " << num << "  " << "姓名: " << name << "  " << "性别: " << sex << "  " << "年龄: " << age << "  " << "所在教辅分类: " << depart_aids << "  " << "职称: " << title << endl;
}
/*教师管理类*/
class Tea_aidsmanager {
public:
	int add();
	int search();
	void show();
	void edit();
	int Delete();
	void account();
	void save();
	void read();
	bool Deletemysql();
	bool mysqledit();
	bool QueryDatabase();
};
int Tea_aidsmanager::add()
{
	Teacher_aids t_aids;
	int tea_aids_num;
	cout << "请输入教师的编号"; cin >> tea_aids_num;
	if (tea_aidscount == MAX)
	{
		cout << "教师人数已满" << endl;
		return 0;
	}
	for (int i = 0; i < tea_aidscount; i++)
	{
		if (tea_aids_num == Tea_aids[i].num)
		{
			cout << "编号已存在" << endl;
			return 0;
		}
	}
	t_aids.input();
	Tea_aids[tea_aidscount] = t_aids;
	tea_aidscount++;
	cout << "教师添加成功" << endl;
	return true;

}
int Tea_aidsmanager::search()
{
	int choice;
	int find_num, j;
	string find_sex, find_name, name_depart;
	cout << "===========1.工号查询  2.姓名查询  3.性别查询  0.退出==========" << endl;
	cout << "请输入查找的方式:"; cin >> choice;
	while (choice != 0)
	{
		if (choice == 1)
		{
			cout << "请输入工号:"; cin >> find_num;
			for (j = 0; j < tea_aidscount; j++)
			{
				if (find_num == Tea_aids[j].num)
					break;
			}
			if (j == tea_aidscount)
				cout << "查无此人" << endl;
			else
				Tea_aids[j].output();
			return true;
		}
		else if (choice == 2)
		{
			cout << "请输入姓名:" << endl; cin >> find_name;
			for (j = 0; j < tea_aidscount; j++)
			{
				if (find_name == Tea_aids[j].name)
					break;
			}
			if (j == tea_aidscount)
				cout << "查无此人" << endl;
			else
				Tea_aids[j].output();
			return true;
		}
		else if (choice == 3)
		{
			cout << "请输入性别:"; cin >> find_sex;
			for (j = 0; j < tea_aidscount; j++)
			{
				if (find_sex == Tea_aids[j].sex)
				{
					Tea_aids[j].output();
				}
			}
			if (j == tea_aidscount)
				cout << "查无此人" << endl;
			return true;
		}
		cin >> choice;

	}
}
void Tea_aidsmanager::show()
{
	if (tea_aidscount == 0)
	{
		cout << "记录为空！" << endl; return;
	}
	for (int i = 0; i < tea_aidscount; i++)
		Tea_aids[i].output();
}
void Tea_aidsmanager::edit()
{
	Teacher_aids t1_aids;
	int j, n;
	cout << "请输入要编辑的人的编号："; cin >> n;
	for (j = 0; j < tea_aidscount; j++)
	{
		if (n == Tea_aids[j].num) break;
	}
	if (j == tea_aidscount)
	{
		cout << "查无此人！" << endl; return;
	};
	cout << "输入修改后的信息，编号不能改:" << endl;
	t1_aids.input();
	Tea_aids[j] = t1_aids;
	cout << "编辑成功！" << endl;
}
bool Tea_aidsmanager::mysqledit()
{
	Teacher_aids t2_aids;
	cout << "请输入你要编辑的编号" << endl;
	char editnum[20];
	cin >> editnum;
	sprintf_s(query, "delete from teacher_aids where num=%s;", editnum);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("delete success\n");
		t2_aids.input();
		printf("编辑成功\n");
		return 1;

	}

}
int Tea_aidsmanager::Delete()
{
	int j, n;
	cout << "请输入要删除的人的编号："; cin >> n;
	for (j = 0; j < tea_aidscount; j++)
	{
		if (n == Tea_aids[j].num) break;
	}
	if (j == tea_aidscount)
	{
		cout << "没有此人！" << endl; return 0;
	};
	for (j; j < tea_aidscount; j++)
	{
		Tea_aids[j] = Tea_aids[j + 1];
	}
	tea_aidscount--;
	cout << "删除成功！" << endl;
	return 1;
}
bool Tea_aidsmanager::Deletemysql()
{
	char query[100];
	char delnum[20];
	cout << "请输入要删除的人员的编号" << endl;
	cin >> delnum;
	sprintf_s(query, "delete from teacher_aids where num=%s;", delnum);
	if (mysql_query(&mysql, query))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("Insert success\n");
		return 1;
	}
}
void Tea_aidsmanager::account()
{
	map<string, int>tea_aids_mp;
	for (int i = 0; i < tea_aidscount; i++)
	{
		tea_aids_mp[Tea_aids[i].sex]++;
		tea_aids_mp[Tea_aids[i].title]++;
	}
	map<string, int>::iterator it;//迭代器 
	for (it = tea_aids_mp.begin(); it != tea_aids_mp.end(); it++)
		cout << it->first << ":" << it->second << "\n";
}
void Tea_aidsmanager::save()
{
	int i;
	ofstream outfile, outfile1;
	outfile1.open("Tea_aidstop.dat", ios::out);
	outfile1 << tea_aidscount;
	outfile.open("Tea_aidsdata.dat", ios::binary);
	if (!outfile)
	{
		cerr << "打开失败" << endl; return;
	}
	for (i = 0; i < tea_aidscount; i++)
		outfile.write((char*)&Tea_aids[i], sizeof(Tea_aids[i]));
	outfile.close();
	cout << "保存成功！" << endl;
}
void Tea_aidsmanager::read()
{
	int i;
	ifstream infile, infile1;
	infile1.open("Tea_aidstop.dat", ios::in);
	infile1 >> tea_aidscount;
	infile.open("Tea_aidsdata.dat", ios::binary);
	if (!infile)
	{
		cerr << "打开失败" << endl; return;
	}
	for (i = 0; i < tea_aidscount; i++)
		infile.read((char*)&Tea_aids[i], sizeof(Tea_aids[i]));
	infile.close();
	cout << "读取成功！" << endl;
}
bool Tea_aidsmanager::QueryDatabase()
{
	mysql_query(&mysql, "set names gbk");
	//返回0 查询成功，返回1查询失败
	if (mysql_query(&mysql, "select * from teacher_aids"))        //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	res = mysql_store_result(&mysql);
	//打印数据行数
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
	for (int i = 0; fd = mysql_fetch_field(res); i++)  //获取字段名
		strcpy_s(field[i], fd->name);
	int j = mysql_num_fields(res);  // 获取列数
	for (int i = 0; i < j; i++)  //打印字段
		printf("%10s\t", field[i]);
	printf("\n");
	while (column = mysql_fetch_row(res))
	{
		for (int i = 0; i < j; i++)
			printf("%10s\t", column[i]);
		printf("\n");
	}
	return true;
}
void Tea_aids_mune(Tea_aidsmanager TM_aids)
{
	int b;
	char c;
	do {
		cout << "教辅管理" << endl;
		cout << "==========================================================================================" << endl;
		cout << "1.数据添加    ";
		cout << "2.dat文件查询    ";
		cout << "3.dat文件显示    ";
		cout << "4.dat文件编辑    ";
		cout << "5.dat文件删除    \n";
		cout << "6.类别统计    ";
		cout << "7.dat文件保存    ";
		cout << "8.dat文件读取    ";
		cout << "9.数据统计    ";
		cout << "10.数据库删除    \n";
		cout << "11.数据库编辑    ";
		cout << "12.数据库显示     ";
		cout << "0.退出" << endl;
		cout << "==========================================================================================" << endl;
		cout << "请选择："; cin >> b;
		switch (b)
		{
		case 1:TM_aids.add(); break;
		case 2:TM_aids.search(); break;
		case 3:TM_aids.show(); break;
		case 4:TM_aids.edit(); break;
		case 5:TM_aids.Delete(); break;
		case 6:cout << "共有教辅人数：" << tea_aidscount << endl; break;
		case 7:TM_aids.save(); break;
		case 8:TM_aids.read(); break;
		case 9:TM_aids.account(); break;
		case 10:TM_aids.Deletemysql(); break;
		case 11:TM_aids.mysqledit(); break;
		case 12:TM_aids.QueryDatabase(); break;
		default:cout << "\n error" << endl; break;
		case 0:break;
		}
		cout << "按回车键继续" << endl;

	} while (b != 0);
}

int main()
{
	Teamanager Tea1;
	Tea_aidsmanager Tea_aids1;
	Polmanager Polimer;
	int a = 1;
	const char* host = "127.0.0.1";  //因为是作为本机测试，所以填写的是本地IP
	const char* user = "root";       //这里改为你的用户名，即连接MySQL的用户名
	const char* passwd = "357159."; //这里改为你的用户密码
	const char* db = "test_db";      //这里改为你要连接的数据库的名字
	unsigned int port = 3307;           //这是MySQL的服务器的端口，如果你没有修改过的话就是3306。
	const char* unix_socket = NULL;    //unix_socket这是unix下的，我在Windows下，所以就把它设置为NULL
	unsigned long client_flag = 0;      //这个参数一般为0
	const char* i_query = "select * from test"; //查询语句

	MYSQL_RES* result;                          //保存结果集 
	MYSQL_ROW row;                               //代表的是结果集中的一行

	mysql_init(&mysql);                          //连接之前必须使用这个函数来初始化
	if ((sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag)) == NULL) //连接MySQL
	{
		printf("连接失败，原因是: \n");
		fprintf(stderr, " %s\n", mysql_error(&mysql));
		exit(1);
	}
	else
	{
		fprintf(stderr, "连接MySQL成功！！\n");
		while (a)
		{
			system("cls");
			cout << endl;
			cout << "              ****欢迎使用高校人员信息管理系统****" << endl;
			cout << "               =================================" << endl;
			cout << "               = 1.教师管理                    =" << endl;
			cout << "               = 2.行政员管理                  =" << endl;
			cout << "               = 3.教辅员管理                  =" << endl;
			cout << "               = 0.退出                        =" << endl;
			cout << "               =================================" << endl;
			cout << "请选择:"; cin >> a;

			switch (a)
			{
			case 1:Tea_mune(Tea1); break;
			case 2:Pol_mune(Polimer); break;
			case 3:Tea_aids_mune(Tea_aids1); break;
			case 0:break;
			default:cout << "\n error" << endl;
				break;
			}
		}

		cout << endl << "谢谢使用" << endl;
	}

	mysql_close(sock);//关闭连接
	system("pause");
	exit(EXIT_SUCCESS);

	return 0;
}


/**/

