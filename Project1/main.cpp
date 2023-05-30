#include <stdio.h>
#include <stdlib.h>
#include <math.h>;
#include <windows.h>;
#include <graphics.h>;
#include <conio.h>;

//函数声明
void read1();
void read2();
void test();
void SOR();
void setfree();
void diedai();

//全局变量声明和初始化
double ET;//电极厚度
int n;//电极总数
double* z;//相邻电极之间的距离
double* N;//相邻电极划分的步长
double* V;//电极电位
double r1;//电极内孔径半径
int M1;//电极内孔径半径等步长划分的网格数
double r2;//从电极内孔沿到封闭边界处的径向距离
int M2;//从电极内孔沿到封闭边界处划分的网格数
double e;//迭代精度
int NST;//输出打印空间电位时网格点间隔数
double* EOE;//要求扫描搜索等电位线的电位间隔值
double dov;//要求扫描等位线间隔值

/*定义计算参数*/
double* E;//电场分布
int row, lin;//行，列
int* pon;//电极位置对应格点坐标
double* loz;//z 轴方向网络步长划分
double* poz;//z 轴格点位置坐标
double* pozE;//z 轴等电位格点位置坐标
double* lor;//r 轴方向网络步长划分
double* por;//r 轴格点位置坐标
double* Tpor;//转换后 r 轴格点位置坐标
int* nopoE;//等位点个数
double* porE;//r 轴等电位格点位置坐标
double* TporE;//转换后 r 轴等电位格点位置坐标
double c1, c2, c3, c4, c0; //系数
double E2; //迭代后的格点电位
int turn; //迭代轮次
int time; //迭代次数
int atime; //迭代总次数
double w; //迭代因子
double* dV; //格点电位残差
double sdV; //格点电位残差和
double mdV; //格点电位最大残差
double adV; //格点电位平均残差
double adV11; //第二轮 12 次迭代格点电位平均残差
double adV12; //第二轮第 13 次迭代格点电位平均残差
double Lambda; //残差均值比值
double uLambda; //中间变量
double wLambda;
double wm; //修正后的 w
double wm1; //修正后的 w
int judge; //判断子
int m;//等电位数量

FILE* ParameterFile;
FILE* OutputFile;

int main()
{
	int i, j, k, s, t;
	/*读入数据*/
	fopen_s(&ParameterFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_孟一丁.dat", "r+");//读取相关参数文件
	fopen_s(&OutputFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_孟一丁.res", "w+");

	if (ParameterFile == NULL)//如果打开失败，就输出"文件打开失败！"
	{
		printf("文件打开失败！");
		exit(1);
	}

	fscanf_s(ParameterFile, "电极总数：%d;\n", &n);//扫描电极总数
	fscanf_s(ParameterFile, "电极厚度：%lf;\n", &ET);//扫描电极厚度
	z = (double*)calloc(n, sizeof(double));//分配电极距离数组大小，存储n个数，(double*)表示将calloc函数的返回值转换为double*类型
	fscanf_s(ParameterFile, "相邻电极之间的距离:%lf，", &z[0]);//扫描距离数组第一个元素
	n = n - 1;//使复制范围到倒数第二个元素
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf,", &z[i]);//把第二个至倒数第二个距离元素都扫描到数组里
	}
	fscanf_s(ParameterFile, "%lf;\n", &z[n]);//扫描最后一个距离元素
	n = n + 1;//使电极总数恢复
	N = (double*)calloc(n, sizeof(double));//分配电极步长数组大小
	fscanf_s(ParameterFile, "相邻电极划分的步长:%lf，", &N[0]);//扫描步长数组第一个元素
	n = n - 1;//使复制范围到倒数第二个元素
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf,", &N[i]);//把第二个至倒数第二个步长元素都扫描到数组里
	}
	fscanf_s(ParameterFile, "%lf;\n", &N[n]);//扫描最后一个步长元素
	n = n + 1;//使电极总数恢复
	V = (double*)calloc(n, sizeof(double));//分配电极电位数组大小
	fscanf_s(ParameterFile, "电极电位:%lf，", &V[0]);//扫描电位数组第一个元素
	n = n - 1;//使复制范围到倒数第二个元素
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf,", &V[i]);//把第二个至倒数第二个电位元素都扫描到数组里
	}
	fscanf_s(ParameterFile, "%lf;\n", &V[n]);//扫描最后一个电位元素
	n = n + 1;//使电极总数恢复

	fscanf_s(ParameterFile, "电极内孔径半径：%lf;\n", &r1);
	fscanf_s(ParameterFile, "电极内孔径半径等步长划分的网格数：%d;\n", &M1);//扫描电极内孔径半径等步长划分的网格数
	fscanf_s(ParameterFile, "电极外孔径半径：%lf;\n", &r2);
	fscanf_s(ParameterFile, "电极外孔径半径等步长划分的网格数：%d;\n", &M2);//扫描电极外孔径半径等步长划分的网格数
	fscanf_s(ParameterFile, "迭代精度：%lf;\n", &e);//扫描迭代精度
	fscanf_s(ParameterFile, "输出打印空间电位时网格点间隔数：%d;\n", &NST);//扫描输出打印空间电位时网格点间隔数
	fscanf_s(ParameterFile, "要求扫描搜索等电位线的电位间隔值或电位值:", &dov);

	EOE = (double*)calloc(100, sizeof(double));//分配电极电位数组大小
	dov = 0;
	i = -1;
	do
	{
		i++;
		fscanf_s(ParameterFile, "%lf,", &EOE[i]);//把第二个至倒数第二个电位元素都扫描到数组里
	} while (EOE[i] != 0);
	if (i == 1)
		dov = EOE[0];
	judge = i;
	double a = 0;
	m = 1;
	if (dov != 0)
	{
		for (m = 1; a < (V[n - 1] - dov); m++) //计算等电位线数量
		{
			a = a + dov;
		}
		*EOE = dov;
		for (i = 1; i < m - 1; i++) //计算等电位线电位值
		{
			EOE[i] = EOE[i - 1] + dov;
		}

	}
	else if (dov == 0)
	{
		m = 0;
		for (i = 0; i < 100; i++) //计算等电位线数量
		{
			if (EOE[i] != 0)
			{
				m++;
			}
		}
		m = m + 1;
	}
	row = M1 + M2 + 1;//计算行数量
	lin = 0;//给变量"line"赋予一个初始值，以便它的最终值可以通过循环计算出来。
	for (i = 0; i < n; i++)
	{
		lin = lin + *(N + i);
	}
	lin = lin + n;

	fclose(ParameterFile);//关闭文件

	read1();//输出参数

	/*电场电位初始化*/
	E = (double*)calloc(lin * row, sizeof(double));
	s = 0;//列数上限
	t = *N + 1;//列数下限
	for (i = 0; i < row; i++)//行循环
	{
		s = 0;//列上界
		t = *N + 1;//列下界
		if (i == row - 1)//第一行输入
		{
			for (j = 0; j < n; j++)//电极电位
			{
				if (j == 0)
				{
					int f = 1;//最终循环次数
					for (k = s; k < t; k++)
					{
						double d = *(V + j) / *(N + j);//计算公差移到外面
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
					int f = 1; //定义最终循环次数
					for (k = s; k < t; k++)
					{
						double d = (*(V + j) - *(V + j - 1)) / *(N + j);//计算公差移到外面即可
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
			for (j = 0; j < n; j++)//电极定位
			{
				for (k = s; k < t; k++)
				{
					*(E + i * lin + k) = 0;
				}
				//加判断语句规避光屏
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
	/*给电极位置对应格点坐标赋值*/
	pon = (int*)calloc(n + 1, sizeof(int));//给电极位置对应格点坐标分配内存空间
	*pon = 0;//荧光屏格点位置
	for (i = 1; i < n + 1; i++)
	{
		if (i == n)
		{
			*(pon + i) = lin - 1;//荧光屏格点位置
		}
		else
		{
			*(pon + i) = *(pon + i - 1) + *(N + i - 1) + 1;//电极右格点位置
		}
	}
	/*给z轴方向网格步长划分赋值*/
	loz = (double*)calloc((lin - 1) * (row - 1), sizeof(double));//给z轴方向网格步长划分分配内存空间
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
	//给z轴方向网格步长划分赋值
	poz = (double*)calloc(lin, sizeof(double));//给z轴方向网格步长划分分配内存空间
	*poz = 0;
	for (i = 1; i < lin; i++)
	{
		*(poz + i) = *(poz + i - 1) + *(loz + i - 1);
	}
	//给r轴方向网格步长划分赋值
	lor = (double*)calloc((lin - 1) * (row - 1), sizeof(double));//给r轴方向网格步长划分分配内存空间
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
	//给r轴方向格点位置划分赋值
	por = (double*)calloc(row, sizeof(double));
	*por = 0;
	for (i = 1; i < row; i++)
	{
		*(por + i) = *(por + i - 1) + *(lor + (i - 1) * (lin - 1));
	}


	read2();//输出参数
}