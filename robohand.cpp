#include "robohand.h"

Robohand::Robohand()
{

    data[0]=175;
    float scale=3;
    width=15*scale;
    length=50*scale;
    //    for(int i=0;i<3;i++)
    //        for(int j=0; j<4;j++)
    //            stat_bones[i][j]=
    stat_origin[0]=QPoint(70,70);
    stat_bones[0][0]=QPoint(-width/2,0);
    stat_bones[0][1]=QPoint(width/2,0);
    stat_bones[0][2]=stat_bones[0][1]+QPoint(0,length);
    stat_bones[0][3]=stat_bones[0][0]+QPoint(0,length);

    int i=1;
    stat_origin[i]=(stat_bones[i-1][2]+stat_bones[i-1][3])/2;
    get_bones();
}

void Robohand::get_bones()
{
    int i=0;
    origin[0]=stat_origin[0];
    float phi=-180;
    for(int j=0;j<4;j++)
    bones[i][j]=QPoint(stat_bones[i][j].rx()*cos((data[i]-phi)*k)-stat_bones[i][j].ry()*sin((data[i]-phi)*k)
                       ,(stat_bones[i][j].ry()*cos((data[i]-phi)*k)+stat_bones[i][j].rx()*sin((data[i]-phi)*k)));
}
