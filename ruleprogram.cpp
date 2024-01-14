#include "ruleprogram.h"
# include <QStringList>
#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline", "unsafe-math-optimizations")
#pragma GCC option("arch=native","tune=native","no-zero-upper")

double cmyif(const double *x)
{
    if(fabs(x[0])>1e-7)
      return x[1];
    else return -1e+100;
}

double cmyelse(const double *x)
{
    return x[0];
}
RuleProgram::RuleProgram(Dataset *mTrain,Dataset *mTest)
{
    train = mTrain;
    test  = mTest;
    program = new Cprogram(train->getdimension());
    setStartSymbol(program->getStartSymbol());
    mparser.resize(10);
    for(int i=0;i<mparser.size();i++)
    {
        mparser[i].AddFunction("myif",cmyif,2);
        mparser[i].AddFunction("myelse",cmyelse,1);
    }
    vars="";
    for(int i=0;i<train->getdimension();i++)
    {
        char str[100];
        sprintf(str,"x%d",i+1);

        vars=vars+str;
        if(i<train->getdimension()-1) vars=vars+",";
    }
}

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

double  RuleProgram::fitness(vector<int> &x)
{
    int redo = 0;

    string str = printRandomProgram(x,redo);
    if(redo>=REDO_MAX) return -1e+100;
    clist = split(str,"#");

    //QString qstr=str.c_str();
    //QStringList subStr  = qstr.split("#");
    for(int i=0;i<clist.size();i++)
    {
        string st =clist[i];
        int d = mparser[i].Parse(st,vars);
        if(d!=-1)
        {
          //  printf("Wrong expression %s value = %d \n",st.c_str(),d);
            return -1e+100;
        }

    }

     lastExpr = str;
    return -getTrainError();
}

double  RuleProgram::getTrainError()
{
    double sum = 0.0;
    int d = train->getdimension();
    double *xx=NULL;//new double[d];
    for(int i=0;i<train->getpatterns();i++)
    {
      //  for(int j=0;j<d;j++)
      //      xx[j]=train->getpoint(i)[j];
        double finalValue = -1e+100;
	xx = train->getpoint(i).data();
        for(int j=0;j<clist.size();j++)
        {
           // program->Parse(clist[j]);
            double vv = mparser[j].Eval(xx);
            if(vv>-1e+10)
            {
                finalValue = vv;
                break;
            }
        }
        double ff = finalValue;
        if(program->EvalError()) {delete[] xx;return 1e+100;}
        sum+=pow(ff-train->gety(i),2.0);
    }
  //  delete[] xx;
    return sum;
}

string  RuleProgram::printF(vector<int> &genome)
{
    string ret = "";
    int redo = 0;

    string str = printRandomProgram(genome,redo);
    if(redo>=REDO_MAX) return ret;
    vector<string> mclist = split(str,"#");

    //QString qstr=str.c_str();
    //QStringList subStr  = qstr.split("#");

    for(int i=0;i<mclist.size();i++)
    {
        string st =mclist[i];
        int d = program->Parse(st);
        if(!d)
        {
            //printf("Wrong expression %s \n",st.toStdString().c_str());
            return ret;
        }
        if(st.rfind("myif",0)==0)
        {
               string x = st.substr(5,st.size()-6);
               vector<string> ele = split(x,",");
               if(i>0)
                   ret=ret+"else\n";
               ret=ret+"if("+ele[0]+") value="+ele[1];
               ret+="\n";
        }
        else
        if(st.rfind("myelse",0)==0)
        {
            string x = st.substr(7,st.size()-8);
            ret = ret+"else value="+x;

        }

    }

    return ret;
}

double  RuleProgram::getTestError()
{
    double sum = 0.0;
    int d = test->getdimension();
    double *xx=new double[d];
    for(int i=0;i<test->getpatterns();i++)
    {
        for(int j=0;j<d;j++)
            xx[j]=test->getpoint(i)[j];
        double finalValue = -1e+100;
        for(int j=0;j<clist.size();j++)
        {
            double vv = mparser[j].Eval(xx);
            if(vv>-1e+10)
            {
                finalValue = vv;
                break;
            }
        }
        double ff = finalValue;
        sum+=pow(ff-test->gety(i),2.0);
    }
    delete[] xx;
    return sum;
}

double  RuleProgram::getClassError()
{
    double sum = 0.0;
    int d = test->getdimension();
    double *xx=new double[d];
    const double eps=1e-5;
    for(int i=0;i<test->getpatterns();i++)
    {
        for(int j=0;j<d;j++)
            xx[j]=test->getpoint(i)[j];
        double finalValue = -1e+100;
        for(int j=0;j<clist.size();j++)
        {

            double vv = mparser[j].Eval(xx);
            if(vv>-1e+10)
            {
                finalValue = vv;
                break;
            }
        }
        double ff = finalValue;
        double mclass = test->getMinDistClass(ff);
        sum+=fabs(mclass-test->gety(i))>eps;
    }
    delete[] xx;
    return sum*100.0/test->getpatterns();
}

RuleProgram::~RuleProgram()
{
    delete program;
}
