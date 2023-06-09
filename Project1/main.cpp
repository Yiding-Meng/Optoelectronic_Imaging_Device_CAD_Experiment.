#include <stdio.h>
#include <stdlib.h>
#include <math.h>;
#include <windows.h>;
#include <graphics.h>;
#include <conio.h>;

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
int row, col;//行，列
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
int judge; //判断子
int m;//等电位数量

FILE* ParameterFile;
FILE* OutputFile;

void ReadData()
{
	int i;
	/*读入数据*/
	fopen_s(&ParameterFile, "D:\\CADTest\\2023\\26_LGM.dat", "r+");//读取相关参数文件
	fopen_s(&OutputFile, "D:\\CADTest\\2023\\26_LGM.res", "w+");

	if (ParameterFile == NULL)//如果打开失败，就输出"文件打开失败！"
	{
		printf("文件打开失败！");
		exit(1);
	}

	fscanf_s(ParameterFile, "电极总数：%d；\n", &n);
	fscanf_s(ParameterFile, "电极厚度：%lf；\n", &ET);

	z = (double*)calloc(n, sizeof(double));//分配电极距离数组大小
	fscanf_s(ParameterFile, "相邻电极之间的距离：%lf，", &z[0]);//扫描距离数组第一个元素
	n = n - 1;//使复制范围到倒数第二个元素
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf，", &z[i]);
	}//把第二个至倒数第二个距离元素都扫描到数组里
	fscanf_s(ParameterFile, "%lf；\n", &z[n]);//把最后一个距离元素扫描到数组里
	n = n + 1;//还原距离数组元素个数

	N = (double*)calloc(n, sizeof(double));//给步长数组分配内存空间
	fscanf_s(ParameterFile, "相邻电极划分的步长：%lf，", &N[0]);
	n = n - 1;
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf，", &N[i]);
	}
	fscanf_s(ParameterFile, "%lf；\n", &N[n]);
	n = n + 1;

	V = (double*)calloc(n, sizeof(double));//给电极电位数组分配内存空间
	fscanf_s(ParameterFile, "电极电位：%lf，", &V[0]);
	n = n - 1;
	for (i = 1; i < n; i++)
	{
		fscanf_s(ParameterFile, "%lf，", &V[i]);
	}
	fscanf_s(ParameterFile, "%lf；\n", &V[n]);
	n = n + 1;

	fscanf_s(ParameterFile, "电极内孔径半径：%lf；\n", &r1);//扫描电极内孔径半径
	fscanf_s(ParameterFile, "电极内孔径半径等步长划分的网格数：%d；\n", &M1);//扫描电极内孔径半径等步长划分的网格数
	fscanf_s(ParameterFile, "从电极内孔沿到封闭边界处的径向距离：%lf；\n", &r2);//扫描从电极内孔沿到封闭边界处的径向距离
	fscanf_s(ParameterFile, "从电极内孔沿到封闭边界处划分的网格数：%d；\n", &M2);//扫描从电极内孔沿到封闭边界处划分的网格数
	fscanf_s(ParameterFile, "迭代精度：%lf；\n", &e);//扫描迭代精度
	fscanf_s(ParameterFile, "输出打印空间电位时网格点间隔数：%d；\n", &NST);//扫描输出打印空间电位时网格点间隔数
	fscanf_s(ParameterFile, "要求扫描搜索等电位线的电位间隔值或电位值：%lf；\n", &dov);//扫描要求扫描搜索等电位线的电位间隔值或电位值

	EOE = (double*)calloc(100, sizeof(double)); //给等电位线数组分配内存空间
	dov = 0; //初始化电位间隔值为0
	i = -1; //初始化等电位线数量为-1
	do
	{
		i++; //等电位线数量加1
		fscanf_s(ParameterFile, "%lf；", &EOE[i]); //读取等电位线参数到数组中

	} while (EOE[i] != 0); //当读取到的等电位线参数为0时，停止读取
	if (i == 1)
		dov = EOE[0]; //如果只有一个等电位线参数，则将电位间隔值设置为该参数值
	judge = i; //记录等电位线数量
	double a = 0;
	m = 1;
	if (dov != 0) //如果电位间隔值不为0，则计算等电位线数量
	{
		for (m = 1; a < (V[n - 1] - dov); m++)   //计算电位数量
			a = a + dov;
		*EOE = dov; //将第一个等电位线参数设置为电位间隔值
		for (i = 1; i < m - 1; i++)
			EOE[i] = EOE[i - 1] + dov; //计算其他等电位线参数
	}
	else if (dov == 0) //如果电位间隔值为0，则直接统计等电位线数量
	{
		m = 0;
		for (i = 0; i < 100; i++)
		{
			if (EOE[i] != 0)
				m++;

		}
		m = m + 1;
	}
	row = M1 + M2 + 1; //计算矩阵行数
	col = 0;
	for (i = 0; i < n; i++)
	{
		col = col + *(N + i); //计算矩阵列数
	}
	col = col + n; //计算矩阵列数

	fclose(ParameterFile);//关闭文件
}

void Initial()
{
	/*电场电位初始化*/
	int i, j,k;
	double* E;
	E = (double*)calloc(row * col, sizeof(double)); // 动态分配内存

	for (int i = 0; i < row; i++) {
		if (i == 0) { // 第一行
			int s = 0; // 列数上界
			int t = N[0] + 1; // 列数下界
			for (int j = 0; j < n; j++) { // 遍历每个电极
				double d = (V[j + 1] - V[j]) / N[j]; // 计算公差
				for (int k = s; k < t; k++) { // 在电极之间插入等差数列
					E[i * col + k] = V[j] + d * (k - s);
				}
				E[i * col + t - 1] = V[j + 1]; // 设置最后一个元素和下一个元素的值
				E[i * col + t] = V[j + 1];
				s = t + 1; // 更新列数上界
				t = s + N[j + 1] + 1; // 更新列数下界
			}
		}
		else if (i >= M1 && i < row - 1) { // 中间的行设置每个元素为0
			for (int j = 0; j < col; j++) {  
				E[i * col + j] = 0;
			}
		}
		else { // 最后M1行
			for (int j = 0; j < col - 1; j++) { // 设置每个元素为0
				E[i * col + j] = 0;
			}
			E[i * col + col - 1] = V[n - 1]; // 设置最后一个元素的值
		}
	}

	pon = (int*)calloc(n + 1, sizeof(int)); //给电极位置对应格点坐标数组分配内存空间
	*pon = 0; //将第一个电极位置对应格点坐标设置为0
	int* p = pon + 1;
	for (i = 1; i < n; i++, p++) {
		*p = *(p - 1) + *(N + i - 1) + 1; //计算每个电极位置对应格点坐标的起始和终止位置
	}
	*(pon + n) = col - 1; //将最后一个电极位置对应格点坐标的终止位置设置为矩阵的列数减1

	loz = (double*)calloc((col - 1) * (row - 1), sizeof(double)); //给z轴方向网络步长划分数组分配内存空间
	double* q = loz;
	for (i = 0; i < row - 1; i++) {
		for (j = 0; j < n; j++) {
			double step = *(z + j) / *(N + j); //计算每个电极上z轴方向的网格步长
			int start = *(pon + j); //计算该电极在矩阵中的起始位置
			int end = *(pon + j + 1); //计算该电极在矩阵中的终止位置
			for (k = start; k < end; k++, q++) {
				*q = step; //将该电极上的所有网格的z轴方向步长赋值为该电极上的网格步长
			}
			if (j == n - 1) {
				*(q - 1) = ET; //将最后一个网格的z轴方向步长赋值为终止电位值
			}
		}
	}
	poz = (double*)calloc(col, sizeof(double)); //给z轴格点位置坐标数组分配内存空间
	double* r = poz + 1;
	for (i = 0; i < col - 1; i++, r++) {
		*r = *(r - 1) + *(loz + i); //计算每个z轴格点位置坐标
	}

	/*给r轴方向网络步长划分赋值*/
	lor = (double*)calloc((col - 1) * (row - 1), sizeof(double)); //给r轴方向网络步长划分数组分配内存空间
	for (i = 0; i < row - 1; i++)
	{
		if (i < M1) //如果在第一个介质区域内，则将r轴方向的网格步长设置为r1 / M1
		{
			for (j = 0; j < col - 1; j++)
				*(lor + i * (col - 1) + j) = r1 / M1;
		}
		else //如果在第二个介质区域内，则将r轴方向的网格步长设置为r2 / M2
		{
			for (j = 0; j < col - 1; j++)
				*(lor + i * (col - 1) + j) = r2 / M2;
		}

	}
	/*给r轴格点位置赋值*/
	por = (double*)calloc(row, sizeof(double)); //给r轴格点位置数组分配内存空间
	*por = 0; //将第一个r轴格点位置设置为0
	for (i = 1; i < row; i++)
		*(por + i) = *(por + i - 1) + *(lor + (i - 1) * (col - 1)); //计算每个r轴格点位置
}

void OutData1()
{
	/*输出基本数据 */
	int i;
	fprintf_s(OutputFile, "**********输出读取变量以及基本矩阵必要量*********\n");
	fprintf_s(OutputFile, "电极总数：%d；\n", n);
	fprintf_s(OutputFile, "电极厚度：%3.3lf；\n", ET);
	fprintf_s(OutputFile, "电极内孔径半径：%3.3lf；\n", r1);
	fprintf_s(OutputFile, "电极内孔径半径等步长划分的网格数：%d；\n", M1);
	fprintf_s(OutputFile, "从电极内孔沿到封闭边界处的径向距离：%3.3lf；\n", r2);
	fprintf_s(OutputFile, "从电极内孔沿到封闭边界处划分的网格数：%d；\n", M2);
	fprintf_s(OutputFile, "迭代精度：%lf；\n", e);
	fprintf_s(OutputFile, "打印空间电位时网格点间隔数：%d；\n", NST);

	fprintf_s(OutputFile, "相邻电极之间的距离：%3.2lf，", *z);
	for (i = 1; i < n - 1; i++)
		fprintf_s(OutputFile, "%3.2lf，", *(z + i));
	fprintf_s(OutputFile, "%3.2lf；\n", *(z + i));  //使输出结果美观一点

	fprintf_s(OutputFile, "相邻电极划分的步长：%3.2lf，", *N);
	for (i = 1; i < n - 1; i++)
		fprintf_s(OutputFile, "%3.2lf，", *(N + i));
	fprintf_s(OutputFile, "%3.2lf；\n", *(N + i));

	fprintf_s(OutputFile, "电极电位：%3.2lf，", *V);
	for (i = 1; i < n - 1; i++)
		fprintf_s(OutputFile, "%3.2lf，", *(V + i));
	fprintf_s(OutputFile, "%3.2lf；\n", *(V + i));

	if (judge == 1)
	{
		fprintf_s(OutputFile, "要求扫描搜索等电位线的电位间隔值:%3.3lf；\n", dov);
		fprintf_s(OutputFile, "要求扫描搜索等电位线的电位值:%3.3lf；", *EOE);
		for (i = 1; i < m - 1; i++)
			fprintf_s(OutputFile, "%3.3lf；", EOE[i]);
	}
	else
	{
		for (i = 0; i < m - 1; i++)
			fprintf_s(OutputFile, "要求扫描搜索等电位线的电位值:%3.3lf；\t", EOE);
	}
	fprintf_s(OutputFile, "初始化对应行数：%d；\n", row);
	fprintf_s(OutputFile, "初始化对应列数：%d；\n", col);
}

void OutData2()
{
	/*输出数据*/
	int i;
	fprintf_s(OutputFile, "\n");
	fprintf_s(OutputFile, "***************输出初始化电场相关参数**************\n");
	fprintf_s(OutputFile, "各电极对应矩阵坐标：");
	for (i = 0; i < n; i = i++)
		fprintf_s(OutputFile, "%d，", pon[i]);
	fprintf_s(OutputFile, "%d；\n", pon[n]);

	fprintf_s(OutputFile, "z轴方向网格点坐标：");
	for (i = 0; i < col - 1; i = i + NST)
		fprintf_s(OutputFile, "%2.2lf，", poz[i]);
	fprintf_s(OutputFile, "%2.2lf；\n", poz[col - 1]);

	fprintf_s(OutputFile, "r轴方向网格点坐标：");
	for (i = 0; i < row - 1; i = i + NST)
		fprintf_s(OutputFile, "%2.2lf，", por[i]);
	fprintf_s(OutputFile, "%2.2lf；\n", por[row - 1]);
}

void free()
{
	/*释放内存*/
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
	ReadData();//读入数据
	OutData1();//输出数据1
	Initial();//电场初始化
	OutData2();//输出数据2
	free();//释放内存
}