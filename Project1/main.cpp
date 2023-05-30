#include <stdio.h>
#include <stdlib.h>
#include <math.h>;
#include <windows.h>;
#include <graphics.h>;
#include <conio.h>;

//��������
void read1();
void read2();
void test();
void SOR();
void setfree();
void diedai();

//ȫ�ֱ��������ͳ�ʼ��
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

FILE* ParameterFile;
FILE* OutputFile;

int main()
{
	int i, j, k, s, t;
	/*��������*/
	fopen_s(&ParameterFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_��һ��.dat", "r+");//��ȡ��ز����ļ�
	fopen_s(&OutputFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_��һ��.res", "w+");

	if (ParameterFile == NULL)//�����ʧ�ܣ������"�ļ���ʧ�ܣ�"
	{
		printf("�ļ���ʧ�ܣ�");
		exit(1);
	}

	fscanf_s(ParameterFile, "�缫������%d;\n", &n);//ɨ��缫����
	fscanf_s(ParameterFile, "�缫��ȣ�%lf;\n", &ET);//ɨ��缫���
	z = (double*)calloc(n, sizeof(double));//����缫���������С���洢n������(double*)��ʾ��calloc�����ķ���ֵת��Ϊdouble*����
	fscanf_s(ParameterFile, "���ڵ缫֮��ľ���:%lf��", &z[0]);//ɨ����������һ��Ԫ��
	n = n - 1;//ʹ���Ʒ�Χ�������ڶ���Ԫ��
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf,", &z[i]);//�ѵڶ����������ڶ�������Ԫ�ض�ɨ�赽������
	}
	fscanf_s(ParameterFile, "%lf;\n", &z[n]);//ɨ�����һ������Ԫ��
	n = n + 1;//ʹ�缫�����ָ�
	N = (double*)calloc(n, sizeof(double));//����缫���������С
	fscanf_s(ParameterFile, "���ڵ缫���ֵĲ���:%lf��", &N[0]);//ɨ�貽�������һ��Ԫ��
	n = n - 1;//ʹ���Ʒ�Χ�������ڶ���Ԫ��
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf,", &N[i]);//�ѵڶ����������ڶ�������Ԫ�ض�ɨ�赽������
	}
	fscanf_s(ParameterFile, "%lf;\n", &N[n]);//ɨ�����һ������Ԫ��
	n = n + 1;//ʹ�缫�����ָ�
	V = (double*)calloc(n, sizeof(double));//����缫��λ�����С
	fscanf_s(ParameterFile, "�缫��λ:%lf��", &V[0]);//ɨ���λ�����һ��Ԫ��
	n = n - 1;//ʹ���Ʒ�Χ�������ڶ���Ԫ��
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf,", &V[i]);//�ѵڶ����������ڶ�����λԪ�ض�ɨ�赽������
	}
	fscanf_s(ParameterFile, "%lf;\n", &V[n]);//ɨ�����һ����λԪ��
	n = n + 1;//ʹ�缫�����ָ�

	fscanf_s(ParameterFile, "�缫�ڿ׾��뾶��%lf;\n", &r1);
	fscanf_s(ParameterFile, "�缫�ڿ׾��뾶�Ȳ������ֵ���������%d;\n", &M1);//ɨ��缫�ڿ׾��뾶�Ȳ������ֵ�������
	fscanf_s(ParameterFile, "�缫��׾��뾶��%lf;\n", &r2);
	fscanf_s(ParameterFile, "�缫��׾��뾶�Ȳ������ֵ���������%d;\n", &M2);//ɨ��缫��׾��뾶�Ȳ������ֵ�������
	fscanf_s(ParameterFile, "�������ȣ�%lf;\n", &e);//ɨ���������
	fscanf_s(ParameterFile, "�����ӡ�ռ��λʱ�����������%d;\n", &NST);//ɨ�������ӡ�ռ��λʱ���������
	fscanf_s(ParameterFile, "Ҫ��ɨ�������ȵ�λ�ߵĵ�λ���ֵ���λֵ:", &dov);

	EOE = (double*)calloc(100, sizeof(double));//����缫��λ�����С
	dov = 0;
	i = -1;
	do
	{
		i++;
		fscanf_s(ParameterFile, "%lf,", &EOE[i]);//�ѵڶ����������ڶ�����λԪ�ض�ɨ�赽������
	} while (EOE[i] != 0);
	if (i == 1)
		dov = EOE[0];
	judge = i;
	double a = 0;
	m = 1;
	if (dov != 0)
	{
		for (m = 1; a < (V[n - 1] - dov); m++) //����ȵ�λ������
		{
			a = a + dov;
		}
		*EOE = dov;
		for (i = 1; i < m - 1; i++) //����ȵ�λ�ߵ�λֵ
		{
			EOE[i] = EOE[i - 1] + dov;
		}

	}
	else if (dov == 0)
	{
		m = 0;
		for (i = 0; i < 100; i++) //����ȵ�λ������
		{
			if (EOE[i] != 0)
			{
				m++;
			}
		}
		m = m + 1;
	}
	row = M1 + M2 + 1;//����������
	lin = 0;//������"line"����һ����ʼֵ���Ա���������ֵ����ͨ��ѭ�����������
	for (i = 0; i < n; i++)
	{
		lin = lin + *(N + i);
	}
	lin = lin + n;

	fclose(ParameterFile);//�ر��ļ�

	read1();//�������

	/*�糡��λ��ʼ��*/
	E = (double*)calloc(lin * row, sizeof(double));
	s = 0;//��������
	t = *N + 1;//��������
	for (i = 0; i < row; i++)//��ѭ��
	{
		s = 0;//���Ͻ�
		t = *N + 1;//���½�
		if (i == row - 1)//��һ������
		{
			for (j = 0; j < n; j++)//�缫��λ
			{
				if (j == 0)
				{
					int f = 1;//����ѭ������
					for (k = s; k < t; k++)
					{
						double d = *(V + j) / *(N + j);//���㹫���Ƶ�����
						*(E + i * lin + k) = d * (f - 1);
						f = f + 1;
					}
					*(E + i * lin + k) = *(V + j);
					*(E + i * lin + k - 1) = *(V + j);
					int o = *(N + j + 1) + 1;
					s = t + 1;
					t = t + o;
				}
				if (j > 0)
				{
					int f = 1; //��������ѭ������
					for (k = s; k < t; k++)
					{
						double d = (*(V + j) - *(V + j - 1)) / *(N + j);//���㹫���Ƶ����漴��
						*(E + i * lin + k) = *(V + j - 1) + d * f;
						f = f + 1;
					}
					if (k < lin)
					{
						*(E + i * lin + k) = *(V + j);
						*(E + i * lin + k - 1) = *(V + j);
						int o = *(N + j + 1) + 1;
						s = t + 1;
						t = t + o;
					}
					if (k == lin)
					{
						*(E + i * lin + lin - 1) = *(V + j);
					}
				}

			}
		}
		if (i < row - 1 && i >= M1)
		{
			for (j = 0; j < n; j++)//�缫��λ
			{
				for (k = s; k < t; k++)
				{
					*(E + i * lin + k) = 0;
				}
				//���ж�����ܹ���
				if (k < lin)
				{
					*(E + i * lin + k - 1) = *(V + j);
					*(E + i * lin + k) = *(V + j);
					int o = *(N + j + 1) + 1;
					s = t + 1;
					t = t + o;
				}
				if (k = lin)
				{
					*(E + i * lin + lin - 1) = *(V + j);
				}
			}
		}
		if (i < M1)
		{
			for (j = 0; j < lin; j++)
			{
				*(E + i * lin + j) = 0;
			}
			*(E + i * lin + lin - 1) = *(V + n - 1);
		}
	}
	/*���缫λ�ö�Ӧ������긳ֵ*/
	pon = (int*)calloc(n + 1, sizeof(int));//���缫λ�ö�Ӧ�����������ڴ�ռ�
	*pon = 0;//ӫ�������λ��
	for (i = 1; i < n + 1; i++)
	{
		if (i == n)
		{
			*(pon + i) = lin - 1;//ӫ�������λ��
		}
		else
		{
			*(pon + i) = *(pon + i - 1) + *(N + i - 1) + 1;//�缫�Ҹ��λ��
		}
	}
	/*��z�᷽�����񲽳����ָ�ֵ*/
	loz = (double*)calloc((lin - 1) * (row - 1), sizeof(double));//��z�᷽�����񲽳����ַ����ڴ�ռ�
	for (i = 0; i < row - 1; i++)
	{
		for (j = 1; j < n + 1; j++)
		{
			if (j != n)
			{
				for (k = *(pon + j - 1); k < *(pon + j); k++)
				{
					*(loz + i * (lin - 1) + k) = *(z + j - 1) / *(N + j - 1);
				}
				int a = *(pon + j);
				*(loz + i * (lin - 1) + a - 1) = ET;
			}
			if (j == n)
			{
				for (k = *(pon + j - 1); k < *(pon + j); k++)
				{
					*(loz + i * (lin - 1) + k) = *(z + j - 1) / *(N + j - 1);
				}
			}
		}
	}
	//��z�᷽�����񲽳����ָ�ֵ
	poz = (double*)calloc(lin, sizeof(double));//��z�᷽�����񲽳����ַ����ڴ�ռ�
	*poz = 0;
	for (i = 1; i < lin; i++)
	{
		*(poz + i) = *(poz + i - 1) + *(loz + i - 1);
	}
	//��r�᷽�����񲽳����ָ�ֵ
	lor = (double*)calloc((lin - 1) * (row - 1), sizeof(double));//��r�᷽�����񲽳����ַ����ڴ�ռ�
	for (i = 0; i < row - 1; i++)
	{
		if (i < M1)
		{
			for (j = 0; j < lin - 1; j++)
			{
				*(lor + i * (lin - 1) + j) = r1 / M1;
			}
		}
		else
		{
			for (j = 0; j < lin - 1; j++)
			{
				*(lor + i * (lin - 1) + j) = r2 / M2;
			}
		}

	}
	//��r�᷽����λ�û��ָ�ֵ
	por = (double*)calloc(row, sizeof(double));
	*por = 0;
	for (i = 1; i < row; i++)
	{
		*(por + i) = *(por + i - 1) + *(lor + (i - 1) * (lin - 1));
	}


	read2();//�������
}