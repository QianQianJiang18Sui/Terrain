#include "MathTool.h"

double CMathTool::getVectorNorm(const double v[])
{
    return qSqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

bool CMathTool::VectorNormalize(double v[])
{
    double norm = getVectorNorm(v);
    if (norm == 0)
    {
        return false;
    }
    v[0] /= norm;
    v[1] /= norm;
    v[2] /= norm;
    return true;
}

void CMathTool::SubArray(const double *array1, const double *array2, double *outArray, double n)
{
    for (int i=0; i<n; i++)
    {
        outArray[i] = array1[i] - array2[i];
    }
}

void CMathTool::AddArray(const double *array1, const double *array2, double *outArray, double n)
{
    for (int i=0; i<n; i++)
    {
        outArray[i] = array1[i] + array2[i];
    }
}

double CMathTool::VectorDot(const double v1[], const double v2[])
{
    return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

void CMathTool::VectorScale(double v[], double factor)
{
    v[0] *= factor;
    v[1] *= factor;
    v[2] *= factor;
}

double CMathTool::getVectorAngle(const double v1[], const double v2[])
{
    double dr = VectorDot(v1,v2);
    double l1 = getVectorNorm(v1);
    double l2 = getVectorNorm(v2);

    if (l1 ==0 ||l2 == 0)
    {
        return 0;
    }
    double temp = dr/(l1*l2);
    if (temp > 1)
    {
        temp =1;
    }
    if (temp < -1)
    {
        temp = -1;
    }
    return acos(temp);
}

double CMathTool::VectorMul(const double v1[], const double v2[], double v3[], double r[])
{
    double angle = getVectorAngle(v1,v2);
    double result[3] = {0};

    if (fabs(angle)<epsilon||fabs(M_PI - angle)<epsilon)
    {
        bool Is0[3] = {0};
        Is0[0] = fabs(v1[0])<epsilon;
        Is0[1] = fabs(v1[1])<epsilon;
        Is0[2] = fabs(v1[2])<epsilon;
        if (v3[0] == 0&&v3[1] == 0&&v3[2] == 0)
        {
            if (!Is0[0])
            {
                result[1] = result[2] = 1;
                result[0] = (-result[1]*v1[1] - result[2]*v1[2])/v1[0];
            }
            else if (!Is0[1])
            {
                result[0] = result[2] = 1;
                result[1] = (-result[0]*v1[0] - result[2]*v1[2])/v1[1];
            }
            else if (!Is0[2])
            {
                result[0] = result[1] = 1;
                result[2] = (-result[1]*v1[1] - result[0]*v1[0])/v1[2];
            }
        }
        else
        {
            memcpy(result, v3, sizeof(double) * 3);
        }
    }
    else
    {
        result[0] = v1[1]*v2[2] - v2[1]*v1[2];
        result[1] = v1[2]*v2[0] - v1[0]*v2[2];
        result[2] = v1[0]*v2[1] - v2[0]*v1[1];
    }
    memcpy(r, result, sizeof(double) * 3);
    return angle;
}

bool CMathTool::isBetweenPoints(const double start[], const double end[], const double pt[])
{
    if(pt[0] >= std::min(start[0],end[0]) && pt[0] <= std::max(start[0],end[0]) &&
            pt[1] >= std::min(start[1],end[1]) && pt[1] <= std::max(start[1],end[1]) &&
            pt[2] >= std::min(start[2],end[2]) && pt[2] <= std::max(start[2],end[2]) )
    {
        return true;
    }
    return false;
}

int CMathTool::getSegment2Plane(const double l1[], const double l2[], double PV[], double d, double outPt[])
{
    double LV[3] = {.0};
    SubArray(l2,l1,LV,3);
    double tempU, tempD;  //临时变量
    tempU = VectorDot(l1,PV) + d;

    tempD = VectorDot(LV,PV);

    if(fabs(tempD) <  epsilon)
    {//直线与平面平行或在平面上
        //return 0;
        if (tempU > 0)
        {
            return 2;
        }
        else if (tempU < 0)
        {
            return -2;
        }
        return 0;
    }
    //直线参数方程的参数t
    double t = -tempU/tempD;

    VectorScale(LV,t);
    AddArray(LV,l1,outPt,3);

    if(isBetweenPoints(l1,l2,outPt))
    {//交点在线段中
        if (tempU > 0)
        {
            if ((VectorDot(l2,PV) + d) == 0)
            {
                return 2;
            }
            return 1;
        }
        else if (tempU < 0)
        {
            if ((VectorDot(l2,PV) + d) == 0)
            {
                return -2;
            }
            return -1;
        }
        else
        {
            if ((VectorDot(l2,PV) + d) > 0)
            {
                return 2;
            }
            else
            {
                return -2;
            }
        }
    }
    else
    {//没有交点
        if (tempU > 0)
        {
            return 2;
        }
        else if (tempU < 0)
        {
            return -2;
        }
    }

    return 0;
}

void CMathTool::getPlaneEquation(const double pt1[], const double pt2[], const double pt3[], double TriangleV[], double *TriD)
{
    //三角形的边方向向量
    double VP12[3] = {0};
    double VP13[3] = {0};
    double vtemp[3] = {0};

    SubArray(pt2,pt1,VP12,3);
    SubArray(pt3,pt1,VP13,3);
    if(0 == VectorMul(VP12,VP13,vtemp,TriangleV))
    {
        TriangleV[0] = .0;
        TriangleV[1] = .0;
        TriangleV[2] = .0;
    }
    VectorNormalize(TriangleV);

    //计算常数项
    *TriD = -VectorDot(TriangleV,pt1);
}

double CMathTool::get2LineAngle(const double l1pt1[], const double l1pt2[], const double l2pt1[], const double l2pt2[])
{
    double v1[3];
    double v2[3];
    SubArray(l1pt1,l1pt2,v1,3);
    SubArray(l2pt1,l2pt2,v2,3);
    return getVectorAngle(v1,v2);
}

void CMathTool::getPtInLine(double LPt1[], double LPt2[], double Len, double OutPt[])
{
    double LineV[3];
    SubArray(LPt2,LPt1,LineV,3);
    double space = getVectorNorm(LineV);
    VectorScale(LineV,Len/space);
    AddArray(LineV,LPt1,OutPt,3);
}

bool CMathTool::Transform(const double pt1[], const double Ma[], double outPt[])
{
    double result[3] = {.0};
    result[0] = Ma[0]*pt1[0] + Ma[1]*pt1[1] + Ma[2] * pt1[2] + Ma[3];
    result[1] = Ma[4]*pt1[0] + Ma[5]*pt1[1] + Ma[6] * pt1[2] + Ma[7];
    result[2] = Ma[8]*pt1[0] + Ma[9]*pt1[1] + Ma[10] * pt1[2] + Ma[11];
    memcpy(outPt,result,sizeof(double) * 3);
    return true;
}

void CMathTool::CaculateNormalsAccordToVertex(float *&pNormals, float *pVertex, int TriangleCount)
{
    double tmp = 0.0f;
    pNormals = new float[TriangleCount * 3 * 3];
    //先清空一下pNormals
    memset(pNormals, 0, sizeof(float)*TriangleCount * 9);
    double pt1[3] = { 0 };
    double pt2[3] = { 0 };
    double pt3[3] = { 0 };
    for (int i = 0; i < TriangleCount; ++i)
    {
        pt1[0] = pVertex[i * 9];
        pt1[1] = pVertex[i * 9 + 1];
        pt1[2] = pVertex[i * 9 + 2];

        pt2[0] = pVertex[i * 9 + 3];
        pt2[1] = pVertex[i * 9 + 4];
        pt2[2] = pVertex[i * 9 + 5];

        pt3[0] = pVertex[i * 9 + 6];
        pt3[1] = pVertex[i * 9 + 7];
        pt3[2] = pVertex[i * 9 + 8];

        double V[3] = { 0 };
        getPlaneEquation(pt1, pt2, pt3, V, &tmp);
        VectorNormalize(V);

        pNormals[i * 9] = V[0];
        pNormals[i * 9 + 1] = V[1];
        pNormals[i * 9 + 2] = V[2];
        pNormals[i * 9 + 3] = V[0];
        pNormals[i * 9 + 4] = V[1];
        pNormals[i * 9 + 5] = V[2];
        pNormals[i * 9 + 6] = V[0];
        pNormals[i * 9 + 7] = V[1];
        pNormals[i * 9 + 8] = V[2];
    }
}
