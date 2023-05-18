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
/*定义全局变量*/
/*定义读入参数*/
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
FILE* ParameterFile;
FILE* OutputFile;
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

int main()
{
	int i, j, k, s, t;

	/*读入数据*/
	fopen_s(&ParameterFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_孟一丁.dat", "r+");//读取相关参数文件
	fopen_s(&OutputFile, "E:\\Optoelectronic_Imaging_Device_CAD_Experiment\\1120201237_孟一丁.res", "w+");
	/// <summary>备份
	/// errno_t err = fopen_s(&ParameterFile, "E:\\Learning(School)\\光电成像原理与技术\\光电成像实验\\程序\\1120160852.dat", "r+");//读取相关参数文件
	/// </summary>
	/// <returns></returns>


	if (ParameterFile == NULL)//detect the file 
	{
		printf("Open filefailure!");
		exit(1);
	}
}