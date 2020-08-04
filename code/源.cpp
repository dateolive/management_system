#include <windows.h>//ע��ͷ�ļ�˳��
#include "stdio.h"
#include "winsock.h"
#include "mysql.h"
#include<iostream>
#include<fstream>
#include<cstring>
#include<map>
const int MAX = 200;
char query[150];//��ѯ���
using namespace std;
MYSQL mysql, * sock;   //����MySQL�ľ��
MYSQL_FIELD* fd;  //�ֶ�������
char field[32][32];  //���ֶ�����ά����
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���
/*Ա����*/
class person {
public:
	int num;/*����*/
	char name[20];/*����*/
	char sex[10]; /*�Ա�*/
	int age;/*����*/
};
/*��ʦ��*/
class Teacher :virtual public person {
public:
	char depart[20];//(ϵ��)
	char spec[20];//��רҵ��
	char title[20];//(ְ��)
	void input();
	void output();

};
Teacher Tea[MAX];
static int teacount;/*��ʦ����*/
static int sex_teacount = 0, depa_teacount = 0, title_teacount = 0;
void Teacher::input()
{
	cout << "�����빤��:"; cin >> num;
	cout << "����������:"; cin >> name;
	cout << "�������Ա�:"; cin >> sex;
	cout << "����������:"; cin >> age;
	cout << "������ϵ��:"; cin >> depart;
	cout << "������רҵ:"; cin >> spec;
	cout << "������ְ��:"; cin >> title;
	sprintf_s(query, "insert into teacher values(%d,'%s','%s',%d,'%s','%s','%s')",num,name,sex,age,depart,spec,title);
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		cout<<"���ݿ����ʧ��";
	}
	else
	{
		printf("Insert success\n");
		cout << "���ݿ����ɹ�";
	}


}
void Teacher::output()
{
	cout << "����: " << num << "  " << "����: " << name << "  " << "�Ա�: " << sex << "  " << "����: " << age << "  " << "����ϵ: " << depart << "  " << "רҵ: " << spec << "  " << "ְ��: " << title << endl;
	
}
/*��ʦ������*/
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
	cout << "�������ʦ�ı��"; cin >> tea_num;
	if (teacount == MAX)
	{
		cout << "��ʦ��������" << endl;
		return 0;
	}
	for (int i = 0; i < teacount; i++)
	{
		if (tea_num == Tea[i].num)
		{
			cout << "����Ѵ���" << endl;
			return 0;
		}
	}
	t.input();
	Tea[teacount] = t;
	teacount++;
	cout << "��ʦ��ӳɹ�" << endl;
	return true;
	


}
int Teamanager::search()
{
	int choice;
	int find_num, j;
	string find_sex, find_name, name_depart;
	cout << "===========1.���Ų�ѯ  2.������ѯ  3.�Ա��ѯ  0.�˳�==========" << endl;
	cout << "��������ҵķ�ʽ:"; cin >> choice;
	while (choice != 0)
	{
		if (choice == 1)
		{
			cout << "�����빤��:"; cin >> find_num;
			for (j = 0; j < teacount; j++)
			{
				if (find_num == Tea[j].num)
					break;
			}
			if (j == teacount)
				cout << "���޴���" << endl;
			else
				Tea[j].output();
			return true;
		}
		else if (choice == 2)
		{
			cout << "����������:"; cin >> find_name;
			for (j = 0; j < teacount; j++)
			{
				if (find_name == Tea[j].name)
					break;
			}
			if (j == teacount)
				cout << "���޴���" << endl;
			else
				Tea[j].output();
			return true;
		}
		else if (choice == 3)
		{
			cout << "�������Ա�:"; cin >> find_sex;
			for (j = 0; j < teacount; j++)
			{
				if (find_sex == Tea[j].sex)
				{
					Tea[j].output();
				}
			}
			if (j == teacount)
				cout << "���޴���" << endl;
			return true;
		}
		cin >> choice;

	}
}
void Teamanager::show()
{
	if (teacount == 0)
	{
		cout << "��¼Ϊ�գ�" << endl; return;
	}
	for (int i = 0; i < teacount; i++)
		Tea[i].output();
}
void Teamanager::edit()
{
	Teacher t1;
	int j, n;
	cout << "������Ҫ�༭���˵ı�ţ�"; cin >> n;
	for (j = 0; j < teacount; j++)
	{
		if (n == Tea[j].num) break;
	}
	if (j == teacount)
	{
		cout << "���޴��ˣ�" << endl; return;
	};
	cout << "�����޸ĺ����Ϣ����Ų��ܸ�:" << endl;
	t1.input();
	Tea[j] = t1;
	cout << "�༭�ɹ���" << endl;
}
bool Teamanager::mysqledit()
{
	Teacher t2;
	cout << "��������Ҫ�༭�ı��" << endl;
	char editnum[20];
	cin >> editnum;
	sprintf_s(query, "delete from teacher where num=%s;", editnum);
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("delete success\n");
		t2.input();
		printf("�༭�ɹ�\n");
		return 1;
		
	}

}
int Teamanager::Delete()
{
	int j, n;
	cout << "������Ҫɾ�����˵ı�ţ�"; cin >> n;
	for (j = 0; j < teacount; j++)
	{
		if (n == Tea[j].num) break;
	}
	if (j == teacount)
	{
		cout << "û�д��ˣ�" << endl; return 0;
	};
	for (j; j < teacount; j++)
	{
		Tea[j] = Tea[j + 1];
	}
	teacount--;
	cout << "ɾ���ɹ���" << endl;
	return 1;
}
bool Teamanager::Deletemysql()
{
	char query[100];
	char delnum[20];
	cout << "������Ҫɾ������Ա�ı��" << endl;
	cin >> delnum;
	sprintf_s(query, "delete from teacher where num=%s;",delnum);
	if (mysql_query(&mysql, query))        //ִ��SQL���
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
	map<string, int>::iterator it;//������ 
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
		cerr << "���ļ�ʧ��" << endl; return;
	}
	for (i = 0; i < teacount; i++)
		outfile.write((char*)&Tea[i], sizeof(Tea[i]));
	outfile.close();
	cout << "����ɹ���" << endl;
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
		cerr << "���ļ�ʧ��" << endl; return;
	}
	for (i = 0; i < teacount; i++)
		infile.read((char*)&Tea[i], sizeof(Tea[i]));
	infile.close();
	cout << "��ȡ�ɹ���" << endl;
}
bool Teamanager::QueryDatabase()
{
	mysql_query(&mysql, "set names gbk");
	//����0 ��ѯ�ɹ�������1��ѯʧ��
	if (mysql_query(&mysql, "select * from teacher"))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	res = mysql_store_result(&mysql);
	//��ӡ��������
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
	for (int i = 0; fd = mysql_fetch_field(res); i++)  //��ȡ�ֶ���
		strcpy_s(field[i], fd->name);
	int j = mysql_num_fields(res);  // ��ȡ����
	for (int i = 0; i < j; i++)  //��ӡ�ֶ�
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
		cout << "��ʦ����" << endl;
		cout << "==========================================================================================" << endl;
		cout << "1.�������    ";
		cout << "2.dat�ļ���ѯ    ";
		cout << "3.dat�ļ���ʾ    ";
		cout << "4.dat�ļ��༭    ";
		cout << "5.dat�ļ�ɾ��    \n";
		cout << "6.���ͳ��    ";
		cout << "7.dat�ļ�����    ";
		cout << "8.dat�ļ���ȡ    ";
		cout << "9.����ͳ��    ";
		cout << "10.���ݿ�ɾ��    \n";
		cout << "11.���ݿ�༭    ";
		cout << "12.���ݿ���ʾ     ";
		cout << "0.�˳�" << endl;
		cout << "==========================================================================================" << endl;
		cout << "��ѡ��"; cin >> b;
		switch (b)
		{
		case 1:TM.add(); break;
		case 2:TM.search(); break;
		case 3:TM.show(); break;
		case 4:TM.edit(); break;
		case 5:TM.Delete(); break;
		case 6:cout << "���н�ʦ������" << teacount << endl; break;
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
/*��ʦ�����*/

/*�����࿪ʼ*/
class Policer :virtual public person {
public:
	char politics[20];//: ��������ò��
	char title[20];// (ְ��)
	void input();
	void output();

};
Policer Pol[MAX];
static int polcount;/*��������*/
static int sex_polcount = 0, politics_polcount = 0, title_polcount = 0;
void Policer::input()
{
	cout << "�����빤��:"; cin >> num;
	cout << "����������:"; cin >> name;
	cout << "�������Ա�:"; cin >> sex;
	cout << "����������:"; cin >> age;
	cout << "������������ò:"; cin >> politics;
	cout << "������ְ��:"; cin >> title;
	sprintf_s(query, "insert into policer values(%d,'%s','%s',%d,'%s','%s')", num, name, sex, age, politics,title);
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		cout << "���ݿ����ʧ��";
	}
	else
	{
		printf("Insert success\n");
		cout << "���ݿ����ɹ�";
	}

}
void Policer::output()
{
	cout << "���: " << num << "  " << "����: " << name << "  " << "�Ա�: " << sex << "  " << "����: " << age << "  " << "������ò: " << politics << "  " << "ְ��: " << title << endl;
}
/*����������*/
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
	cout << "�����������ı��"; cin >> pol_num;
	if (polcount == MAX)
	{
		cout << "������������" << endl;
		return 0;
	}
	for (int i = 0; i < polcount; i++)
	{
		if (pol_num == Pol[i].num)
		{
			cout << "����Ѵ���" << endl;
			return 0;
		}
	}
	p.input();
	Pol[polcount] = p;
	polcount++;
	cout << "��ʦ��ӳɹ�" << endl;
	return true;

}
int Polmanager::search()
{
	int choice;
	int find_num, j;
	string find_sex, find_name, name_depart;
	cout << "===========1.���Ų�ѯ  2.������ѯ  3.�Ա��ѯ  0.�˳�==========" << endl;
	cout << "��������ҵķ�ʽ:"; cin >> choice;
	while (choice != 0)
	{
		if (choice == 1)
		{
			cout << "�����빤��:"; cin >> find_num;
			for (j = 0; j < polcount; j++)
			{
				if (find_num == Pol[j].num)
					break;
			}
			if (j == polcount)
				cout << "���޴���" << endl;
			else
				Pol[j].output();
			return true;
		}
		else if (choice == 2)
		{
			cout << "����������:"; cin >> find_name;
			for (j = 0; j < polcount; j++)
			{
				if (find_name == Pol[j].name)
					break;
			}
			if (j == polcount)
				cout << "���޴���" << endl;
			else
				Pol[j].output();
			return true;
		}
		else if (choice == 3)
		{
			cout << "�������Ա�:"; cin >> find_sex;
			for (j = 0; j < polcount; j++)
			{
				if (find_sex == Pol[j].sex)
				{
					Pol[j].output();
				}
			}
			if (j == polcount)
				cout << "���޴���" << endl;
			return true;
		}
		cin >> choice;

	}
}
void Polmanager::show()
{
	if (polcount == 0)
	{
		cout << "��¼Ϊ�գ�" << endl; return;
	}
	for (int i = 0; i < polcount; i++)
		Pol[i].output();
}
void Polmanager::edit()
{
	Policer p1;
	int j, n;
	cout << "������Ҫ�༭���˵ı�ţ�"; cin >> n;
	for (j = 0; j < polcount; j++)
	{
		if (n == Pol[j].num) break;
	}
	if (j == polcount)
	{
		cout << "���޴��ˣ�" << endl; return;
	};
	cout << "�����޸ĺ����Ϣ����Ų��ܸ�:" << endl;
	p1.input();
	Pol[j] = p1;
	cout << "�༭�ɹ���" << endl;
}
bool Polmanager::mysqledit()
{
	Policer p2;
	cout << "��������Ҫ�༭�ı��" << endl;
	char editnum[20];
	cin >> editnum;
	sprintf_s(query, "delete from policer where num=%s;", editnum);
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("delete success\n");
		p2.input();
		printf("�༭�ɹ�\n");
		return 1;

	}

}
int Polmanager::Delete()
{
	int j, n;
	cout << "������Ҫɾ�����˵ı�ţ�"; cin >> n;
	for (j = 0; j < polcount; j++)
	{
		if (n == Pol[j].num) break;
	}
	if (j == polcount)
	{
		cout << "û�д��ˣ�" << endl; return 0;
	};
	for (j; j < polcount; j++)
	{
		Pol[j] = Pol[j + 1];
	}
	polcount--;
	cout << "ɾ���ɹ���" << endl;
	return 1;
}
bool Polmanager::Deletemysql()
{
	char query[100];
	char delnum[20];
	cout << "������Ҫɾ������Ա�ı��" << endl;
	cin >> delnum;
	sprintf_s(query, "delete from policer where num=%s;", delnum);
	if (mysql_query(&mysql, query))        //ִ��SQL���
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
	map<string, int>::iterator it;//������ 
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
		cerr << "��ʧ��" << endl; return;
	}
	for (i = 0; i < polcount; i++)
		outfile.write((char*)&Pol[i], sizeof(Pol[i]));
	outfile.close();
	cout << "����ɹ���" << endl;
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
		cerr << "��ʧ��" << endl; return;
	}
	for (i = 0; i < polcount; i++)
		infile.read((char*)&Pol[i], sizeof(Pol[i]));
	infile.close();
	cout << "��ȡ�ɹ���" << endl;
}
bool Polmanager::QueryDatabase()
{
	mysql_query(&mysql, "set names gbk");
	//����0 ��ѯ�ɹ�������1��ѯʧ��
	if (mysql_query(&mysql, "select * from policer"))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	res = mysql_store_result(&mysql);
	//��ӡ��������
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
	for (int i = 0; fd = mysql_fetch_field(res); i++)  //��ȡ�ֶ���
		strcpy_s(field[i], fd->name);
	int j = mysql_num_fields(res);  // ��ȡ����
	for (int i = 0; i < j; i++)  //��ӡ�ֶ�
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
		cout << "��������" << endl;
		cout << "==========================================================================================" << endl;
		cout << "1.�������    ";
		cout << "2.dat�ļ���ѯ    ";
		cout << "3.dat�ļ���ʾ    ";
		cout << "4.dat�ļ��༭    ";
		cout << "5.dat�ļ�ɾ��    \n";
		cout << "6.���ͳ��    ";
		cout << "7.dat�ļ�����    ";
		cout << "8.dat�ļ���ȡ    ";
		cout << "9.����ͳ��    ";
		cout << "10.���ݿ�ɾ��    \n";
		cout << "11.���ݿ�༭    ";
		cout << "12.���ݿ���ʾ     ";
		cout << "0.�˳�" << endl;
		cout << "==========================================================================================" << endl;
		cout << "��ѡ��"; cin >> b;
		switch (b)
		{
		case 1:PO.add(); break;
		case 2:PO.search(); break;
		case 3:PO.show(); break;
		case 4:PO.edit(); break;
		case 5:PO.Delete(); break;
		case 6:cout << "��������������" << polcount << endl; break;
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
/*���������*/

/*�̸��࿪ʼ*/
/*
��̸�

Ӳ�̸�

*/
class Teacher_aids :virtual public person {
public:
	char depart_aids[20];//(���Ӳ�̸�)
	char title[20];//(ְ��) ʵ��Ա����� 
	void input();
	void output();

};
Teacher_aids Tea_aids[MAX];
static int tea_aidscount;/*�̸�����*/
static int sex_tea_aidscount = 0, depa_tea_aidscount = 0, title_tea_aidscount = 0;
void Teacher_aids::input()
{
	cout << "�����빤��:"; cin >> num;
	cout << "����������:"; cin >> name;
	cout << "�������Ա�:"; cin >> sex;
	cout << "����������:"; cin >> age;
	cout << "������̸����:"; cin >> depart_aids;
	cout << "������ְ��:"; cin >> title;
	sprintf_s(query, "insert into teacher_aids values(%d,'%s','%s',%d,'%s','%s')", num, name, sex, age, depart_aids,title);
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		cout << "���ݿ����ʧ��";
	}
	else
	{
		printf("Insert success\n");
		cout << "���ݿ����ɹ�";
	}

}
void Teacher_aids::output()
{
	cout << "����: " << num << "  " << "����: " << name << "  " << "�Ա�: " << sex << "  " << "����: " << age << "  " << "���ڽ̸�����: " << depart_aids << "  " << "ְ��: " << title << endl;
}
/*��ʦ������*/
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
	cout << "�������ʦ�ı��"; cin >> tea_aids_num;
	if (tea_aidscount == MAX)
	{
		cout << "��ʦ��������" << endl;
		return 0;
	}
	for (int i = 0; i < tea_aidscount; i++)
	{
		if (tea_aids_num == Tea_aids[i].num)
		{
			cout << "����Ѵ���" << endl;
			return 0;
		}
	}
	t_aids.input();
	Tea_aids[tea_aidscount] = t_aids;
	tea_aidscount++;
	cout << "��ʦ��ӳɹ�" << endl;
	return true;

}
int Tea_aidsmanager::search()
{
	int choice;
	int find_num, j;
	string find_sex, find_name, name_depart;
	cout << "===========1.���Ų�ѯ  2.������ѯ  3.�Ա��ѯ  0.�˳�==========" << endl;
	cout << "��������ҵķ�ʽ:"; cin >> choice;
	while (choice != 0)
	{
		if (choice == 1)
		{
			cout << "�����빤��:"; cin >> find_num;
			for (j = 0; j < tea_aidscount; j++)
			{
				if (find_num == Tea_aids[j].num)
					break;
			}
			if (j == tea_aidscount)
				cout << "���޴���" << endl;
			else
				Tea_aids[j].output();
			return true;
		}
		else if (choice == 2)
		{
			cout << "����������:" << endl; cin >> find_name;
			for (j = 0; j < tea_aidscount; j++)
			{
				if (find_name == Tea_aids[j].name)
					break;
			}
			if (j == tea_aidscount)
				cout << "���޴���" << endl;
			else
				Tea_aids[j].output();
			return true;
		}
		else if (choice == 3)
		{
			cout << "�������Ա�:"; cin >> find_sex;
			for (j = 0; j < tea_aidscount; j++)
			{
				if (find_sex == Tea_aids[j].sex)
				{
					Tea_aids[j].output();
				}
			}
			if (j == tea_aidscount)
				cout << "���޴���" << endl;
			return true;
		}
		cin >> choice;

	}
}
void Tea_aidsmanager::show()
{
	if (tea_aidscount == 0)
	{
		cout << "��¼Ϊ�գ�" << endl; return;
	}
	for (int i = 0; i < tea_aidscount; i++)
		Tea_aids[i].output();
}
void Tea_aidsmanager::edit()
{
	Teacher_aids t1_aids;
	int j, n;
	cout << "������Ҫ�༭���˵ı�ţ�"; cin >> n;
	for (j = 0; j < tea_aidscount; j++)
	{
		if (n == Tea_aids[j].num) break;
	}
	if (j == tea_aidscount)
	{
		cout << "���޴��ˣ�" << endl; return;
	};
	cout << "�����޸ĺ����Ϣ����Ų��ܸ�:" << endl;
	t1_aids.input();
	Tea_aids[j] = t1_aids;
	cout << "�༭�ɹ���" << endl;
}
bool Tea_aidsmanager::mysqledit()
{
	Teacher_aids t2_aids;
	cout << "��������Ҫ�༭�ı��" << endl;
	char editnum[20];
	cin >> editnum;
	sprintf_s(query, "delete from teacher_aids where num=%s;", editnum);
	if (mysql_query(&mysql, query))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return 0;
	}
	else
	{
		printf("delete success\n");
		t2_aids.input();
		printf("�༭�ɹ�\n");
		return 1;

	}

}
int Tea_aidsmanager::Delete()
{
	int j, n;
	cout << "������Ҫɾ�����˵ı�ţ�"; cin >> n;
	for (j = 0; j < tea_aidscount; j++)
	{
		if (n == Tea_aids[j].num) break;
	}
	if (j == tea_aidscount)
	{
		cout << "û�д��ˣ�" << endl; return 0;
	};
	for (j; j < tea_aidscount; j++)
	{
		Tea_aids[j] = Tea_aids[j + 1];
	}
	tea_aidscount--;
	cout << "ɾ���ɹ���" << endl;
	return 1;
}
bool Tea_aidsmanager::Deletemysql()
{
	char query[100];
	char delnum[20];
	cout << "������Ҫɾ������Ա�ı��" << endl;
	cin >> delnum;
	sprintf_s(query, "delete from teacher_aids where num=%s;", delnum);
	if (mysql_query(&mysql, query))        //ִ��SQL���
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
	map<string, int>::iterator it;//������ 
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
		cerr << "��ʧ��" << endl; return;
	}
	for (i = 0; i < tea_aidscount; i++)
		outfile.write((char*)&Tea_aids[i], sizeof(Tea_aids[i]));
	outfile.close();
	cout << "����ɹ���" << endl;
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
		cerr << "��ʧ��" << endl; return;
	}
	for (i = 0; i < tea_aidscount; i++)
		infile.read((char*)&Tea_aids[i], sizeof(Tea_aids[i]));
	infile.close();
	cout << "��ȡ�ɹ���" << endl;
}
bool Tea_aidsmanager::QueryDatabase()
{
	mysql_query(&mysql, "set names gbk");
	//����0 ��ѯ�ɹ�������1��ѯʧ��
	if (mysql_query(&mysql, "select * from teacher_aids"))        //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	res = mysql_store_result(&mysql);
	//��ӡ��������
	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
	for (int i = 0; fd = mysql_fetch_field(res); i++)  //��ȡ�ֶ���
		strcpy_s(field[i], fd->name);
	int j = mysql_num_fields(res);  // ��ȡ����
	for (int i = 0; i < j; i++)  //��ӡ�ֶ�
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
		cout << "�̸�����" << endl;
		cout << "==========================================================================================" << endl;
		cout << "1.�������    ";
		cout << "2.dat�ļ���ѯ    ";
		cout << "3.dat�ļ���ʾ    ";
		cout << "4.dat�ļ��༭    ";
		cout << "5.dat�ļ�ɾ��    \n";
		cout << "6.���ͳ��    ";
		cout << "7.dat�ļ�����    ";
		cout << "8.dat�ļ���ȡ    ";
		cout << "9.����ͳ��    ";
		cout << "10.���ݿ�ɾ��    \n";
		cout << "11.���ݿ�༭    ";
		cout << "12.���ݿ���ʾ     ";
		cout << "0.�˳�" << endl;
		cout << "==========================================================================================" << endl;
		cout << "��ѡ��"; cin >> b;
		switch (b)
		{
		case 1:TM_aids.add(); break;
		case 2:TM_aids.search(); break;
		case 3:TM_aids.show(); break;
		case 4:TM_aids.edit(); break;
		case 5:TM_aids.Delete(); break;
		case 6:cout << "���н̸�������" << tea_aidscount << endl; break;
		case 7:TM_aids.save(); break;
		case 8:TM_aids.read(); break;
		case 9:TM_aids.account(); break;
		case 10:TM_aids.Deletemysql(); break;
		case 11:TM_aids.mysqledit(); break;
		case 12:TM_aids.QueryDatabase(); break;
		default:cout << "\n error" << endl; break;
		case 0:break;
		}
		cout << "���س�������" << endl;

	} while (b != 0);
}

int main()
{
	Teamanager Tea1;
	Tea_aidsmanager Tea_aids1;
	Polmanager Polimer;
	int a = 1;
	const char* host = "127.0.0.1";  //��Ϊ����Ϊ�������ԣ�������д���Ǳ���IP
	const char* user = "root";       //�����Ϊ����û�����������MySQL���û���
	const char* passwd = "357159."; //�����Ϊ����û�����
	const char* db = "test_db";      //�����Ϊ��Ҫ���ӵ����ݿ������
	unsigned int port = 3307;           //����MySQL�ķ������Ķ˿ڣ������û���޸Ĺ��Ļ�����3306��
	const char* unix_socket = NULL;    //unix_socket����unix�µģ�����Windows�£����ԾͰ�������ΪNULL
	unsigned long client_flag = 0;      //�������һ��Ϊ0
	const char* i_query = "select * from test"; //��ѯ���

	MYSQL_RES* result;                          //�������� 
	MYSQL_ROW row;                               //������ǽ�����е�һ��

	mysql_init(&mysql);                          //����֮ǰ����ʹ�������������ʼ��
	if ((sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag)) == NULL) //����MySQL
	{
		printf("����ʧ�ܣ�ԭ����: \n");
		fprintf(stderr, " %s\n", mysql_error(&mysql));
		exit(1);
	}
	else
	{
		fprintf(stderr, "����MySQL�ɹ�����\n");
		while (a)
		{
			system("cls");
			cout << endl;
			cout << "              ****��ӭʹ�ø�У��Ա��Ϣ����ϵͳ****" << endl;
			cout << "               =================================" << endl;
			cout << "               = 1.��ʦ����                    =" << endl;
			cout << "               = 2.����Ա����                  =" << endl;
			cout << "               = 3.�̸�Ա����                  =" << endl;
			cout << "               = 0.�˳�                        =" << endl;
			cout << "               =================================" << endl;
			cout << "��ѡ��:"; cin >> a;

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

		cout << endl << "ллʹ��" << endl;
	}

	mysql_close(sock);//�ر�����
	system("pause");
	exit(EXIT_SUCCESS);

	return 0;
}


/**/

