#include <stdio.h>;
#include <stdlib.h>;
#include <math.h>;
#include <windows.h>;
#include <graphics.h>;
#include <conio.h>;
void read1();
void read2();
void test();
void SOR();
void setfree();
void diedai();
/////////////
/*����ȫ�ֱ���*/
/*����������*/
double ET;//�缫���
int n;//�缫����
double* z;//���ڵ缫֮��ľ���
double* N;//���ڵ缫���ֵĲ���
double* V;//�缫��λ
double r1;//�缫�ڿ׾��뾶
int M1;//�缫�ڿ׾��뾶�Ȳ������ֵ�������
double r2;//�ӵ缫�ڿ��ص���ձ߽紦�ľ������
int M2;//�ӵ缫�ڿ��ص���ձ߽紦���ֵ�������
double e;//��������
int NST;//�����ӡ�ռ��λʱ���������
double* EOE;//Ҫ��ɨ�������ȵ�λ�ߵĵ�λ���ֵ
double dov;//Ҫ��ɨ���λ�߼��ֵ

/*����������*/
double* E;//�糡�ֲ�
int row, lin;//�У���
int* pon;//�缫λ�ö�Ӧ�������
double* loz;//z �᷽�����粽������
double* poz;//z ����λ������
double* pozE;//z ��ȵ�λ���λ������
double* lor;//r �᷽�����粽������
double* por;//r ����λ������
double* Tpor;//ת���� r ����λ������
int* nopoE;//��λ�����
double* porE;//r ��ȵ�λ���λ������
double* TporE;//ת���� r ��ȵ�λ���λ������
FILE* ParameterFile;
FILE* OutputFile;
double c1, c2, c3, c4, c0; //ϵ��
double E2; //������ĸ���λ
int turn; //�����ִ�
int time; //��������
int atime; //�����ܴ���
double w; //��������
double* dV; //����λ�в�
double sdV; //����λ�в��
double mdV; //����λ���в�
double adV; //����λƽ���в�
double adV11; //�ڶ��� 12 �ε�������λƽ���в�
double adV12; //�ڶ��ֵ� 13 �ε�������λƽ���в�
double Lambda; //�в��ֵ��ֵ
double uLambda; //�м����
double wLambda;
double wm; //������� w
double wm1; //������� w
int judge; //�ж���
int m;//�ȵ�λ����

int main()
{
	int i, j, k, s, t;

	/*��������*/
	fopen_s(&ParameterFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_��һ��.dat", "r+");//��ȡ��ز����ļ�
	fopen_s(&OutputFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_��һ��.res", "w+");
	/// <summary>����
	/// errno_t err = fopen_s(&ParameterFile, "E:\\Learning(School)\\������ԭ���뼼��\\������ʵ��\\����\\1120160852.dat", "r+");//��ȡ��ز����ļ�
	/// </summary>
	/// <returns></returns>


	if (ParameterFile == NULL)//detect the file 
	{
		printf("Open filefailure!");
		exit(1);
	}
}