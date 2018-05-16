#include "perceptron.h"

perceptron::perceptron(vector<int>& constr)
{


    N=constr.size();
    inp_size=constr[0];
    out_size=constr[N-1];

    inp_ptr=new float[constr[0]];
    tg_ptr=new float[constr[N-1]];

    lr=new layer*[N];
    lrh=NULL;
    for( i=0;i<(N-1);i++)
    {
        lr[i]=new layer(constr[i],1,lrh);
        lrh=lr[i];
    }
    lr[N-1]=new layer(constr[N-1],1,lrh);

    out=new float*[constr[N-1]];
    for( i=0;i<constr[N-1];i++)
        out[i]=&(lr[N-1]->n[i].state);


}

int perceptron::getMaxInd()
{
    float max=-1000;
    int ind=0;
    for(i=0;i<lr[N-1]->size;i++)
    {
        if(lr[N-1]->n[i].state>max)
        {
            ind=i;
            max=lr[N-1]->n[i].state;
        }
    }
    return  ind;
}

float perceptron::reset_w()
{
    for ( i=1;i<(N);i++)
        lr[i]->reset_w();
}

float** perceptron::refresh(float* x)
{

    lr[0]->set(x);
    for(int i=1;i<N;i++)
        lr[i]->refresh();

    return out;
}

float** perceptron::refresh(vector<float>& x)
{
    for( i=0;i<lr[0]->size;i++)
        inp_ptr[i]=x[i];

    refresh(inp_ptr);
}

void perceptron::learn1(float* x,float* t)
{
    refresh(x);
    lr[N-1]->getErr(t);
    for( i=(N-1);i>1;i--)
    {
        lr[i]->pushErr();
    }
    for( i=1;i<N;i++)
        lr[i]->refreshW();
}

void perceptron::learn1(vector<float>& x,vector<float>& t)
{
    for( i=0;i<lr[0]->size;i++)
        inp_ptr[i]=x[i];

    for( i=0;i<lr[N-1]->size;i++)
        tg_ptr[i]=t[i];

    learn1(inp_ptr, tg_ptr);
}

void perceptron::learn1(float* x,float t)
{
    refresh(x);
    lr[N-1]->getErr(&t);
    for( i=(N-1);i>1;i--)
    {
        lr[i]->pushErr();
    }
    for( i=1;i<N;i++)
        lr[i]->refreshW();
}

void perceptron::learnFunc()
{}
