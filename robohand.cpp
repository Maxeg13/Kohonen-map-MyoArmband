#include "robohand.h"

Robohand::Robohand()
{
    k=3.14/180;
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
    set_form(2,.5,0.2);


//    int i=1;
//    stat_origin[i]=(stat_bones[i-1][2]+stat_bones[i-1][3])/2;
//    int i=2;
//    stat_origin[i]=(stat_bones[i-1][2]+stat_bones[i-1][3])/2;
    get_bones();
}

void Robohand::get_bones()
{



    int i=0;
    if(data[i]>180)
        data[i]=180;
    else if(data[i]<0)
        data[i]=0;

    origin[i]=QPoint(70,70);
    float phi=-180;
    for(int j=0;j<4;j++)
    bones[i][j]=QPoint(stat_bones[i][j].rx()*cos((data[i]-phi)*k)-stat_bones[i][j].ry()*sin((data[i]-phi)*k)
                       ,(stat_bones[i][j].ry()*cos((data[i]-phi)*k)+stat_bones[i][j].rx()*sin((data[i]-phi)*k)));

    i=1;
    if(data[i]>180)
        data[i]=180;
    else if(data[i]<0)
        data[i]=0;

    origin[i]=origin[i-1]+(bones[i-1][2]+bones[i-1][3])/2.;
    phi=0+data[i-1];
    for(int j=0;j<4;j++)
    bones[i][j]=QPoint(stat_bones[i][j].rx()*cos((data[i]+phi)*k)-stat_bones[i][j].ry()*sin((data[i]+phi)*k)
                       ,(stat_bones[i][j].ry()*cos((data[i]+phi)*k)+stat_bones[i][j].rx()*sin((data[i]+phi)*k)));

    i=2;
    if(data[i]>220)
        data[i]=220;
    else if(data[i]<140)
        data[i]=140;

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
