#include "robohand.h"

Robohand::Robohand()
{
    k=3.14/180;
    min[0]=0;
    min[1]=30;
    max[0]=max[1]=180;
    min[2]=130;
    max[2]=210;
    data[0]=175;
    data[1]=90;
    data[2]=150;
    float scale=3;
    width=15*scale;
    length=50*scale;
    //    for(int i=0;i<3;i++)
    //        for(int j=0; j<4;j++)
    //            stat_bones[i][j]=

    set_form(0,1,1);
    set_form(1,.8,1);
    set_form(2,.5,0.3);


//    int i=1;
//    stat_origin[i]=(stat_bones[i-1][2]+stat_bones[i-1][3])/2;
//    int i=2;
//    stat_origin[i]=(stat_bones[i-1][2]+stat_bones[i-1][3])/2;
    get_bones();
}

void Robohand::get_bones()
{



    int i=0;
    if(data[i]>max[i])
        data[i]=max[i];
    else if(data[i]<min[i])
        data[i]=min[i];

    origin[i]=QPoint(70,110);
    float phi=-180;
    for(int j=0;j<4;j++)
    bones[i][j]=QPoint(stat_bones[i][j].rx()*cos((data[i]-phi)*k)-stat_bones[i][j].ry()*sin((data[i]-phi)*k)
                       ,(stat_bones[i][j].ry()*cos((data[i]-phi)*k)+stat_bones[i][j].rx()*sin((data[i]-phi)*k)));

    i=1;
    if(data[i]>max[i])
        data[i]=max[i];
    else if(data[i]<min[i])
        data[i]=min[i];

    origin[i]=origin[i-1]+(bones[i-1][2]+bones[i-1][3])/2.;
    phi=0+data[i-1];
    for(int j=0;j<4;j++)
    bones[i][j]=QPoint(stat_bones[i][j].rx()*cos((data[i]+phi)*k)-stat_bones[i][j].ry()*sin((data[i]+phi)*k)
                       ,(stat_bones[i][j].ry()*cos((data[i]+phi)*k)+stat_bones[i][j].rx()*sin((data[i]+phi)*k)));

    i=2;
    if(data[i]>max[i])
        data[i]=max[i];
    else if(data[i]<min[i])
        data[i]=min[i];

    phi=180+data[i-1]+data[i-2];
    for(int j=0;j<4;j++)
    bones[i][j]=QPoint(stat_bones[i][j].rx()*cos((data[i]+phi)*k)-stat_bones[i][j].ry()*sin((data[i]+phi)*k)
                       ,(stat_bones[i][j].ry()*cos((data[i]+phi)*k)+stat_bones[i][j].rx()*sin((data[i]+phi)*k)));

    origin[i]=origin[i-1]+(bones[i-1][2]+bones[i-1][3])/2.;
    i=3;
    origin[i]=origin[i-1]+(bones[i-1][2]+bones[i-1][3])/2.;
}

void Robohand::set_form(int i, float width_k, float length_k)
{
    stat_bones[i][0]=QPoint(-width/2.*width_k,0);
    stat_bones[i][1]=QPoint(width/2.*width_k,0);
    stat_bones[i][2]=stat_bones[i][1]+QPoint(0,length*length_k);
    stat_bones[i][3]=stat_bones[i][0]+QPoint(0,length*length_k);
}
