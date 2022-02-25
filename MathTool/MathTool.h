#ifndef MATH_H
#define MATH_H

#include <QtMath>
#define epsilon 0.0000001
class CMathTool
{
public:
    CMathTool() = default;
    ~CMathTool() = default;

public:
    //向量的模
    static double getVectorNorm(const double v[3]);
    //向量单位化
    static bool VectorNormalize(double v[3]);

    //函数说明：两个double型的同大小一维数组减
    //输入参数：array1,array2输入的两个数组,n数组的大小
    //输出参数：outArray
    static void SubArray(const double *array1, const double *array2, double *outArray, double n = 3);

    //函数说明：两个double型的同大小一维数组加
    //输入参数：array1,array2输入的两个数组,n数组的大小
    //输出参数：outArray
    static void AddArray(const double *array1, const double *array2, double *outArray, double n = 3);

    //返回v1●v2
    //参数：	v1，v2两个向量
    //返回：	两个向量点乘结果
    //其它：	无错误提示
    static double VectorDot(const double v1[3], const double v2[3]);

    //向量放缩
    //输入：	v需要放缩的向量，factor放缩倍数；
    //输出：	v，放缩后的向量
    //说明：	当factor等于0的时候 v会被放缩成0向量
    static void VectorScale(double v[3], double factor);

    //函数说明：计算两个向量的夹角
    //参数：	v1，v2两个向量
    //返回：	向量夹角
    //其它：	MathComputeErr=1为计算成功，-1为v1或v2中的一个为0向量
    static double getVectorAngle(const double v1[3], const double v2[3]);

    // 函数说明：向量v1×向量v2
    //参数：	v1，v2两个向量
    //输入：	v3 是V1xV2或者V2xV1的结果，如果没有可输入（0,0,0）
    //输出：	r，差乘结果
    //返回：	向量夹角
    //其它：	无错误提示
    static double VectorMul(const double v1[3], const double v2[3], double v3[3], double r[3]);

    static bool isBetweenPoints(const double start[3], const double end[3], const double pt[3]);

    //线段与平面的交点
    //输入：	l1，l2直线
    //输入：	pv，d面方程
    //输出：	outPt交点
    //返回：	0无交点，直线与平面平行或在平面上,
    //			1有交点且点l1在面的正面，-1有交点且l1在面的反面,
    //			2无交点，且线在面的正面，-2无交点，且线在面的反面
    static int getSegment2Plane(const double l1[3], const double l2[3], double PV[3], double d, double outPt[3]);

    //函数说明:	根据三点求平面方程
    //参数：	pt1,pt2,pt3平面上的三点
    //输出：	TriangleV,平面法线，也就是三个平面方程的三个参数
    //输出：	TriD，平面方程常数项
    //返回：	无
    //其它：	无
    static void getPlaneEquation(const double pt1[3], const double pt2[3], const double pt3[3], double TriangleV[3], double *TriD);

    //求两条直线的夹角
    //输入：	l1pt1，l1pt2直线1；l2pt1，l2pt2直线2
    //返回：	直线夹角
    static double get2LineAngle(const double l1pt1[3],const double l1pt2[3],const double l2pt1[3],const double l2pt2[3]);

    //根据直线上的两个点和直线上另外一点（p）到直线起点的距离，求p
    //输入：	LPt1，LPt2直线，Len长度
    //输出：	OutPt
    static void getPtInLine(double LPt1[3],double LPt2[3],double Len,double OutPt[3]);

    //根据变换矩阵转换点的坐标
    //输入：	pt1输入的点
    //输入：	Ma变换矩阵
    //输出：	outPt变换后的点
    //说明：	pt1，outPt指向不同的地址
    static bool Transform(const double pt1[3],const double Ma[16],double outPt[3]);

    /**
     * @brief 由顶点计算平面法线
     * @detail 使用之前先用GetPlaneModel方法获得标准模型顶点
     *
     * @param pNormals 在方法内计算法向量，在外部使用，其中每三个点的法向量是相同的，因为同属于一个三角形
     * @param pVertex 要传入的平面顶点，按照三角形排列
     * @param TriangleCount 三角形数
     */
    static void CaculateNormalsAccordToVertex(float*& pNormals, float* pVertex, int TriangleCount);
};

#endif // MATH_H
