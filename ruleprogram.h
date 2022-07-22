#ifndef RULEPROGRAM_H
#define RULEPROGRAM_H
# include <program.h>
# include <cprogram.h>
# include <dataset.h>
# include <QStringList>

class RuleProgram : public Program
{

private:
        Dataset *train,*test;
        Cprogram *program;
        string lastExpr;
        QStringList list;
        vector<string> clist;
        vector<FunctionParser> mparser;
        string vars;
public:
    RuleProgram(Dataset *mTrain,Dataset *mTest);
    double  fitness(vector<int> &x);
    double  getTrainError();
    double  getTestError();
    double  getClassError();
    string  printF(vector<int> &genome);
    ~RuleProgram();
};

#endif // RULEPROGRAM_H
