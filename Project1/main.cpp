#include <stdio.h>
#include <stdlib.h>
#include <math.h>;
#include <windows.h>;
#include <graphics.h>;
#include <conio.h>;

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
int row, col;//�У���
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
int judge; //�ж���
int m;//�ȵ�λ����

FILE* ParameterFile;
FILE* OutputFile;

void ReadData()
{
	int i;
	/*��������*/
	fopen_s(&ParameterFile, "D:\\CADTest\\2023\\26_LGM.dat", "r+");//��ȡ��ز����ļ�
	fopen_s(&OutputFile, "D:\\CADTest\\2023\\26_LGM.res", "w+");

	if (ParameterFile == NULL)//�����ʧ�ܣ������"�ļ���ʧ�ܣ�"
	{
		printf("�ļ���ʧ�ܣ�");
		exit(1);
	}

	fscanf_s(ParameterFile, "�缫������%d��\n", &n);
	fscanf_s(ParameterFile, "�缫��ȣ�%lf��\n", &ET);

	z = (double*)calloc(n, sizeof(double));//����缫���������С
	fscanf_s(ParameterFile, "���ڵ缫֮��ľ��룺%lf��", &z[0]);//ɨ����������һ��Ԫ��
	n = n - 1;//ʹ���Ʒ�Χ�������ڶ���Ԫ��
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf��", &z[i]);
	}//�ѵڶ����������ڶ�������Ԫ�ض�ɨ�赽������
	fscanf_s(ParameterFile, "%lf��\n", &z[n]);//�����һ������Ԫ��ɨ�赽������
	n = n + 1;//��ԭ��������Ԫ�ظ���

	N = (double*)calloc(n, sizeof(double));//��������������ڴ�ռ�
	fscanf_s(ParameterFile, "���ڵ缫���ֵĲ�����%lf��", &N[0]);
	n = n - 1;
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf��", &N[i]);
	}
	fscanf_s(ParameterFile, "%lf��\n", &N[n]);
	n = n + 1;

	V = (double*)calloc(n, sizeof(double));//���缫��λ��������ڴ�ռ�
	fscanf_s(ParameterFile, "�缫��λ��%lf��", &V[0]);
	n = n - 1;
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf��", &V[i]);
	}
	fscanf_s(ParameterFile, "%lf��\n", &V[n]);
	n = n + 1;

	fscanf_s(ParameterFile, "�缫�ڿ׾��뾶��%lf��\n", &r1);//ɨ��缫�ڿ׾��뾶
	fscanf_s(ParameterFile, "�缫�ڿ׾��뾶�Ȳ������ֵ���������%d��\n", &M1);//ɨ��缫�ڿ׾��뾶�Ȳ������ֵ�������
	fscanf_s(ParameterFile, "�ӵ缫�ڿ��ص���ձ߽紦�ľ�����룺%lf��\n", &r2);//ɨ��ӵ缫�ڿ��ص���ձ߽紦�ľ������
	fscanf_s(ParameterFile, "�ӵ缫�ڿ��ص���ձ߽紦���ֵ���������%d��\n", &M2);//ɨ��ӵ缫�ڿ��ص���ձ߽紦���ֵ�������
	fscanf_s(ParameterFile, "�������ȣ�%lf��\n", &e);//ɨ���������
	fscanf_s(ParameterFile, "�����ӡ�ռ��λʱ�����������%d��\n", &NST);//ɨ�������ӡ�ռ��λʱ���������
	fscanf_s(ParameterFile, "Ҫ��ɨ�������ȵ�λ�ߵĵ�λ���ֵ���λֵ��%lf��\n", &dov);//ɨ��Ҫ��ɨ�������ȵ�λ�ߵĵ�λ���ֵ���λֵ

	EOE = (double*)calloc(100, sizeof(double)); //���ȵ�λ����������ڴ�ռ�
	dov = 0; //��ʼ����λ���ֵΪ0
	i = -1; //��ʼ���ȵ�λ������Ϊ-1
	do
	{
		i++; //�ȵ�λ��������1
		fscanf_s(ParameterFile, "%lf��", &EOE[i]); //��ȡ�ȵ�λ�߲�����������

	} while (EOE[i] != 0); //����ȡ���ĵȵ�λ�߲���Ϊ0ʱ��ֹͣ��ȡ
	if (i == 1)
		dov = EOE[0]; //���ֻ��һ���ȵ�λ�߲������򽫵�λ���ֵ����Ϊ�ò���ֵ
	judge = i; //��¼�ȵ�λ������
	double a = 0;
	m = 1;
	if (dov != 0) //�����λ���ֵ��Ϊ0�������ȵ�λ������
	{
		for (m = 1; a < (V[n - 1] - dov); m++)   //�����λ����
			a = a + dov;
		*EOE = dov; //����һ���ȵ�λ�߲�������Ϊ��λ���ֵ
		for (i = 1; i < m - 1; i++)
			EOE[i] = EOE[i - 1] + dov; //���������ȵ�λ�߲���
	}
	else if (dov == 0) //�����λ���ֵΪ0����ֱ��ͳ�Ƶȵ�λ������
	{
		m = 0;
		for (i = 0; i < 100; i++)
		{
			if (EOE[i] != 0)
				m++;

		}
		m = m + 1;
	}
	row = M1 + M2 + 1; //�����������
	col = 0;
	for (i = 0; i < n; i++)
	{
		col = col + *(N + i); //�����������
	}
	col = col + n; //�����������

	fclose(ParameterFile);//�ر��ļ�
}

void Initial()
{
	/*�糡��λ��ʼ��*/
	int i, j,k;
	double* E;
	E = (double*)calloc(row * col, sizeof(double)); // ��̬�����ڴ�

	for (int i = 0; i < row; i++) {
		if (i == 0) { // ��һ��
			int s = 0; // �����Ͻ�
			int t = N[0] + 1; // �����½�
			for (int j = 0; j < n; j++) { // ����ÿ���缫
				double d = (V[j + 1] - V[j]) / N[j]; // ���㹫��
				for (int k = s; k < t; k++) { // �ڵ缫֮�����Ȳ�����
					E[i * col + k] = V[j] + d * (k - s);
				}
				E[i * col + t - 1] = V[j + 1]; // �������һ��Ԫ�غ���һ��Ԫ�ص�ֵ
				E[i * col + t] = V[j + 1];
				s = t + 1; // ���������Ͻ�
				t = s + N[j + 1] + 1; // ���������½�
			}
		}
		else if (i >= M1 && i < row - 1) { // �м��������ÿ��Ԫ��Ϊ0
			for (int j = 0; j < col; j++) {  
				E[i * col + j] = 0;
			}
		}
		else { // ���M1��
			for (int j = 0; j < col - 1; j++) { // ����ÿ��Ԫ��Ϊ0
				E[i * col + j] = 0;
			}
			E[i * col + col - 1] = V[n - 1]; // �������һ��Ԫ�ص�ֵ
		}
	}

	pon = (int*)calloc(n + 1, sizeof(int)); //���缫λ�ö�Ӧ���������������ڴ�ռ�
	*pon = 0; //����һ���缫λ�ö�Ӧ�����������Ϊ0
	int* p = pon + 1;
	for (i = 1; i < n; i++, p++) {
		*p = *(p - 1) + *(N + i - 1) + 1; //����ÿ���缫λ�ö�Ӧ����������ʼ����ֹλ��
	}
	*(pon + n) = col - 1; //�����һ���缫λ�ö�Ӧ����������ֹλ������Ϊ�����������1

	loz = (double*)calloc((col - 1) * (row - 1), sizeof(double)); //��z�᷽�����粽��������������ڴ�ռ�
	double* q = loz;
	for (i = 0; i < row - 1; i++) {
		for (j = 0; j < n; j++) {
			double step = *(z + j) / *(N + j); //����ÿ���缫��z�᷽������񲽳�
			int start = *(pon + j); //����õ缫�ھ����е���ʼλ��
			int end = *(pon + j + 1); //����õ缫�ھ����е���ֹλ��
			for (k = start; k < end; k++, q++) {
				*q = step; //���õ缫�ϵ����������z�᷽�򲽳���ֵΪ�õ缫�ϵ����񲽳�
			}
			if (j == n - 1) {
				*(q - 1) = ET; //�����һ�������z�᷽�򲽳���ֵΪ��ֹ��λֵ
			}
		}
	}
	poz = (double*)calloc(col, sizeof(double)); //��z����λ��������������ڴ�ռ�
	double* r = poz + 1;
	for (i = 0; i < col - 1; i++, r++) {
		*r = *(r - 1) + *(loz + i); //����ÿ��z����λ������
	}

	/*��r�᷽�����粽�����ָ�ֵ*/
	lor = (double*)calloc((col - 1) * (row - 1), sizeof(double)); //��r�᷽�����粽��������������ڴ�ռ�
	for (i = 0; i < row - 1; i++)
	{
		if (i < M1) //����ڵ�һ�����������ڣ���r�᷽������񲽳�����Ϊr1 / M1
		{
			for (j = 0; j < col - 1; j++)
				*(lor + i * (col - 1) + j) = r1 / M1;
		}
		else //����ڵڶ������������ڣ���r�᷽������񲽳�����Ϊr2 / M2
		{
			for (j = 0; j < col - 1; j++)
				*(lor + i * (col - 1) + j) = r2 / M2;
		}

	}
	/*��r����λ�ø�ֵ*/
	por = (double*)calloc(row, sizeof(double)); //��r����λ����������ڴ�ռ�
	*por = 0; //����һ��r����λ������Ϊ0
	for (i = 1; i < row; i++)
		*(por + i) = *(por + i - 1) + *(lor + (i - 1) * (col - 1)); //����ÿ��r����λ��
}

void OutData1()
{
	/*����������� */
	int i;
	fprintf_s(OutputFile, "**********�����ȡ�����Լ����������Ҫ��*********\n");
	fprintf_s(OutputFile, "�缫������%d��\n", n);
	fprintf_s(OutputFile, "�缫��ȣ�%3.3lf��\n", ET);
	fprintf_s(OutputFile, "�缫�ڿ׾��뾶��%3.3lf��\n", r1);
	fprintf_s(OutputFile, "�缫�ڿ׾��뾶�Ȳ������ֵ���������%d��\n", M1);
	fprintf_s(OutputFile, "�ӵ缫�ڿ��ص���ձ߽紦�ľ�����룺%3.3lf��\n", r2);
	fprintf_s(OutputFile, "�ӵ缫�ڿ��ص���ձ߽紦���ֵ���������%d��\n", M2);
	fprintf_s(OutputFile, "�������ȣ�%lf��\n", e);
	fprintf_s(OutputFile, "��ӡ�ռ��λʱ�����������%d��\n", NST);

	fprintf_s(OutputFile, "���ڵ缫֮��ľ��룺%3.2lf��", *z);
	for (i = 1; i < n - 1; i++)
		fprintf_s(OutputFile, "%3.2lf��", *(z + i));
	fprintf_s(OutputFile, "%3.2lf��\n", *(z + i));  //ʹ����������һ��

	fprintf_s(OutputFile, "���ڵ缫���ֵĲ�����%3.2lf��", *N);
	for (i = 1; i < n - 1; i++)
		fprintf_s(OutputFile, "%3.2lf��", *(N + i));
	fprintf_s(OutputFile, "%3.2lf��\n", *(N + i));

	fprintf_s(OutputFile, "�缫��λ��%3.2lf��", *V);
	for (i = 1; i < n - 1; i++)
		fprintf_s(OutputFile, "%3.2lf��", *(V + i));
	fprintf_s(OutputFile, "%3.2lf��\n", *(V + i));

	if (judge == 1)
	{
		fprintf_s(OutputFile, "Ҫ��ɨ�������ȵ�λ�ߵĵ�λ���ֵ:%3.3lf��\n", dov);
		fprintf_s(OutputFile, "Ҫ��ɨ�������ȵ�λ�ߵĵ�λֵ:%3.3lf��", *EOE);
		for (i = 1; i < m - 1; i++)
			fprintf_s(OutputFile, "%3.3lf��", EOE[i]);
	}
	else
	{
		for (i = 0; i < m - 1; i++)
			fprintf_s(OutputFile, "Ҫ��ɨ�������ȵ�λ�ߵĵ�λֵ:%3.3lf��\t", EOE);
	}
	fprintf_s(OutputFile, "��ʼ����Ӧ������%d��\n", row);
	fprintf_s(OutputFile, "��ʼ����Ӧ������%d��\n", col);
}

void OutData2()
{
	/*�������*/
	int i;
	fprintf_s(OutputFile, "\n");
	fprintf_s(OutputFile, "***************�����ʼ���糡��ز���**************\n");
	fprintf_s(OutputFile, "���缫��Ӧ�������꣺");
	for (i = 0; i < n; i = i++)
		fprintf_s(OutputFile, "%d��", pon[i]);
	fprintf_s(OutputFile, "%d��\n", pon[n]);

	fprintf_s(OutputFile, "z�᷽����������꣺");
	for (i = 0; i < col - 1; i = i + NST)
		fprintf_s(OutputFile, "%2.2lf��", poz[i]);
	fprintf_s(OutputFile, "%2.2lf��\n", poz[col - 1]);

	fprintf_s(OutputFile, "r�᷽����������꣺");
	for (i = 0; i < row - 1; i = i + NST)
		fprintf_s(OutputFile, "%2.2lf��", por[i]);
	fprintf_s(OutputFile, "%2.2lf��\n", por[row - 1]);
}

void free()
{
	/*�ͷ��ڴ�*/
	free(z);
	free(N);
	free(V);
	free(EOE);
	free(E);
	free(pon);
	free(loz);
	free(poz);
	free(pozE);
	free(lor);
	free(por);
	free(Tpor);
	free(nopoE);
	free(porE);
	free(TporE);
}

int main()
{
	ReadData();//��������
	OutData1();//�������1
	Initial();//�糡��ʼ��
	OutData2();//�������2
	free();//�ͷ��ڴ�
}