#ifndef SIMANMETHOD_H
#define SIMANMETHOD_H
# include "problem.h"
class SimanMethod
{
private:
    double T0;
    Data xpoint;
    double ypoint;
    Data bestx;
    double besty;
    string coolingMethod;
    int neps;
    double eps;
    Problem *myProblem;
    int k;
public:
    SimanMethod(Problem *p);
    void	setPoint(Data &x,double &y);
    void	getPoint(Data &x,double &y);
    void    setCoolingMethod(string s);
    string  getCoolingMethod() const;
    void    updateTemp();
    void    setT0(double t);
    double  getT0() const;
    void    setNeps(int n);
    int     getNeps() const;
    void    setEpsilon(double e);
    double  getEpsilon() const;
    void    Solve();
    Data    getX();
    double  getY();
    ~SimanMethod();
};
#endif // SIMANMETHOD_H
