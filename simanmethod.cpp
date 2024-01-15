#include "simanmethod.h"

SimanMethod::SimanMethod(Problem *p)
{
    myProblem = p;
    T0=100000.0;
    neps = 2000;
    eps = 1e-5;
    xpoint.resize(p->getDimension());
    myProblem->getSample(xpoint);
    ypoint = myProblem->funmin(xpoint);
    bestx = xpoint;
    besty = ypoint;
    coolingMethod="exp";//posible values: exp, log, linear, quad
}

void    SimanMethod::setCoolingMethod(string s)
{
    if(s.size()!=0)
        coolingMethod = s;
}

string  SimanMethod::getCoolingMethod() const
{
    return coolingMethod;
}

void    SimanMethod::setT0(double t)
{
    if(t>0) T0 = t;
}

double  SimanMethod::getT0() const
{
    return T0;
}

void    SimanMethod::setNeps(int n)
{
    if(n>0) neps = n;
}
int     SimanMethod::getNeps() const
{
    return neps;
}

void    SimanMethod::setEpsilon(double e)
{
    if(e>0) eps = e;
}

double  SimanMethod::getEpsilon() const
{
    return eps;
}

void    SimanMethod::updateTemp()
{
    const double alpha = 0.8;

    if(coolingMethod=="exp")
    {
        T0 =T0 * pow(alpha,k);
        k=k+1;
    }
    else
    if(coolingMethod=="log")
    {
        T0 = T0/(1+alpha * log(1.0+k));
    }
    else
    if(coolingMethod == "linear")
    {
        T0 = T0/(1.0+alpha * k);
    }
    else
    if(coolingMethod=="quad")
    {
        T0 = T0/(1.0+alpha * k * k);
    }
}

void	SimanMethod::setPoint(Data &x,double &y)
{
	xpoint = x;
	ypoint = y;
}


void	SimanMethod::getPoint(Data &x,double &y)
{
	 x = bestx;
	 y = besty;
}

void    SimanMethod::Solve()
{
    int i;
    k=1;
    bestx = xpoint;
    besty = ypoint;
    while(true)
    {
        for(i=1;i<=neps;i++)
        {
            Data y ;
	    double fy;
	   y.resize(myProblem->getDimension()); myProblem->getSample(y);
            fy = myProblem->funmin(y);
	    if(isnan(fy) || isinf(fy)) continue;

            if(fy<ypoint)
            {
                xpoint = y;
                ypoint = fy;
                if(ypoint<besty)
                {
                    bestx = xpoint;
                    besty = ypoint;
                }
            }
            else
            {
                double r = fabs((rand()*1.0)/RAND_MAX);
                double ratio = exp(-(fy-ypoint)/T0);
                double xmin = ratio<1?ratio:1;
                if(r<xmin)
                {
                    xpoint = y;
                    ypoint = fy;
                    if(ypoint<besty)
                    {
                        bestx = xpoint;
                        besty = ypoint;
                    }
                }
            }
        }
        updateTemp();
        if(T0<=eps) break;
        printf("Iteration: %4d Temperature: %20.10lg Value: %20.10lg\n",
               k,T0,besty);
    }
}

Data    SimanMethod::getX()
{
    return bestx;
}

double  SimanMethod::getY()
{
    return besty;
}

SimanMethod::~SimanMethod()
{
    //nothing here
}
