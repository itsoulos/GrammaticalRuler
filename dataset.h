#ifndef DATASET_H
#define DATASET_H
# include <QVector>
# include <QString>
typedef QVector<double> Data;

class Dataset
{
private:
    int dimension;
    QVector<Data> xpoint;
    Data  ypoint;
    Data dclass;
public:
    Dataset();
    Dataset(const char *filename);
    int     getdimension() const;
    Data    getpoint(int pos);
    double  gety(int pos);
    int     getpatterns() const;
    void    normalizeZeroOne();
    double  getNearestClass(double y);
    double getMinDistClass(double y);
    ~Dataset();
};

#endif // DATASET_H
