# include <population.h>	
# include <math.h>
# include <problem.h>
# include <simanmethod.h>
# include <QString>
# include <iostream>

//# define MAX_RULE	65536
# define MAX_RULE	256

#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline", "unsafe-math-optimizations")
#pragma GCC option("arch=native","tune=native","no-zero-upper")


/* Population constructor */
/* Input: genome count , genome size, pointer to Program instance */
Population::Population(int gcount,int gsize,Program *p)
{
	elitism=1;
	selection_rate = 0.1;
	mutation_rate  = 0.05;
	genome_count   = gcount;
	genome_size    = gsize;
	generation     = 0;
	program        = p;

	/* Create the population and based on genome count and size */
	/* Initialize the genomes to random */
	genome=new int*[genome_count];
	children=new int*[genome_count];
	for(int i=0;i<genome_count;i++)
	{
		genome[i]=new int[genome_size];
		children[i]=new int[genome_size];
			for(int j=0;j<genome_size;j++)
				genome[i][j]=rand()%MAX_RULE;
	}
	fitness_array=new double[genome_count];
}

/* Reinitialize the population to random */
void	Population::reset()
{
	generation = 0;
	for(int i=0;i<genome_count;i++)
		for(int j=0;j<genome_size;j++)
				genome[i][j]=rand()%MAX_RULE;
	for(int i=0;i<genome_count;i++)
			fitness_array[i]=-1e+8;
}

/* Return the fitness of a genome */
double 	Population::fitness(vector<int> &g)
{

	double f=program->fitness(g);
	return f;
}

/* The selection of the chromosomes according to the fitness value is performed */
void	Population::select()
{
	int itemp[genome_size];
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_count-1;j++)
		{
			if(fitness_array[j+1]<fitness_array[j])
			{
				double dtemp;
				dtemp=fitness_array[j];
				fitness_array[j]=fitness_array[j+1];
				fitness_array[j+1]=dtemp;
				
				memcpy(itemp,genome[j],genome_size*sizeof(int));
				memcpy(genome[j],genome[j+1],genome_size*sizeof(int));
				memcpy(genome[j+1],itemp,genome_size*sizeof(int));
			}
		}
	}
}

/* Crossover operation: based on tournament selection */
/* Select the tournament_size based on the genome count : */
/*     (if genome_count > 100 ) tournament_size = 10   else   tournament_size = 4 */
/* Select 2 chromosomes based on the tournament size and cross them over based on the crossover probability */
/* There is 1 crossover point and it is random */
void	Population::crossover()
{
        int parent[2];
        int nchildren=(int)((1.0 - selection_rate) * genome_count);
	if(!(nchildren%2==0)) nchildren++;
        const int tournament_size =(genome_count<=100)?4:10;
        int count_children=0;
        while(1)
        {
		// The two parents are selected here according to the tournament selection procedure
                for(int i=0;i<2;i++)
                {
                        double max_fitness=1e+100;
                        int    max_index=-1;
			int r;
			// Select the best parents of  the candidates 
                        for(int j=0;j<tournament_size;j++)
                        {
				r=rand() % (genome_count);
                                if(j==0 || fitness_array[r]<max_fitness)
                                {
                                        max_index=r;
                                        max_fitness=fitness_array[r];
                                }
                        }
                        parent[i]=max_index;
                }
        int pt1;
		// The one-point crossover is performed here (the point is pt1)
		pt1=rand() % genome_size;
		memcpy(children[count_children],
				genome[parent[0]],pt1 * sizeof(int));
		memcpy(&children[count_children][pt1],
			&genome[parent[1]][pt1],(genome_size-pt1)*sizeof(int));
		memcpy(children[count_children+1],
				genome[parent[1]],pt1 * sizeof(int));
		memcpy(&children[count_children+1][pt1],
			&genome[parent[0]][pt1],(genome_size-pt1)*sizeof(int));
		count_children+=2;
		if(count_children>=nchildren) break;
	}
	
	

	for(int i=0;i<nchildren;i++)
	{
		memcpy(genome[genome_count-i-1],
				children[i],genome_size * sizeof(int));
	}
}

void	Population::setElitism(int s)
{
	elitism = s;
}

/* Mutate the current population */
/* Standard mutation algorithm: mutate all chromosomes in the population based on the mutation probability */
void	Population::mutate()
{
	int start = elitism * (int)(genome_count*selection_rate);
	
	start = elitism;
	start = 1;

	for(int i=start;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++)
		{
			double r=rand()*1.0/RAND_MAX;
			if(r<mutation_rate)
			{
				genome[i][j]=rand() % MAX_RULE;
			}
		}
	}
}

/* Evaluate the fitness for all chromosomes in the current population */
void	Population::calcFitnessArray()
{
	vector<int> g;
	g.resize(genome_size);

	double dmin = 1e+100;
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++) g[j]=genome[i][j];	
			fitness_array[i]=fitness(g);	
		if(fabs(fitness_array[i])<dmin) dmin=fabs(fitness_array[i]);

	}

}

/* Return the current generation */
int	Population::getGeneration() const
{
	return generation;
}

/* Return the genome count */
int	Population::getCount() const
{
	return genome_count;
}

/* Return the size of the population */
int	Population::getSize() const
{
	return genome_size;
}

/* Evolve the next generation */
void	Population::nextGeneration()
{
	if(generation==0)
	calcFitnessArray();
	select();
	crossover();
	mutate();
	calcFitnessArray();
    extern int localSearchGenerations,localSearchChromosomes;
    if((generation+1)%localSearchGenerations==0)
    {
        for(int i=0;i<localSearchChromosomes;i++)
		localSearch(rand() % genome_count);
        select();
    }
	++generation;
}

class IntegerAnneal
{
private:
    Population *pop;
    vector<int> xpoint;
    double ypoint;
    vector<int> bestx;
    double besty;
    int neps=100;
    double T0;
    int k;
public:
    IntegerAnneal(Population *p);
    void    setNeps(int n);
    void    setT0(double t);
    void setPoint(vector<int> &g,double &y);
    void getPoint(vector<int> &g,double &y);
    void Solve();
    void    updateTemp();
    ~IntegerAnneal();
};
IntegerAnneal::IntegerAnneal(Population *p)
{
    pop = p;
    T0=1e+8;
    neps=200;
}
void    IntegerAnneal::setNeps(int n)
{
    neps  = n;
}

void    IntegerAnneal::updateTemp()
{
    const double alpha = 0.8;

    T0 =T0 * pow(alpha,k);
    k=k+1;
}
void    IntegerAnneal::setT0(double t)
{
    T0  = t;
}
void    IntegerAnneal::setPoint(vector<int> &g,double &y)
{
    xpoint = g;
    ypoint = y;

}
void    IntegerAnneal::getPoint(vector<int> &g,double &y)
{
    g = bestx;
    y = besty;
}

void    IntegerAnneal::Solve()
{
    bestx = xpoint;
    besty = ypoint;
    int i;
    k=1;
    vector<int> y;
    y.resize(bestx.size());
    while(true)
    {
        for(i=1;i<=neps;i++)
        {
        double fy;
        for(int j=0;j<y.size();j++)
                y[j]=rand() % MAX_RULE;
        fy = pop->fitness(y);
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
        if(T0<=1e-5) break;
        printf("Iteration: %4d Temperature: %20.10lg Value: %20.10lg\n",
               k,T0,besty);

    }
}

IntegerAnneal::~IntegerAnneal()
{
    //nothing here
}

class PopulationProblem: public Problem
{
private:
    Population *pop;
    vector<int> currentGenome;
public:
    PopulationProblem(Population *p,vector<int> &x)
    {
        pop = p;
        currentGenome = x;
        setDimension(x.size());
        lmargin.resize(x.size());
        rmargin.resize(x.size());
        for(int i=0;i<x.size();i++)
        {
        lmargin[i]=0;
        rmargin[i]=MAX_RULE;
        }
	setLeftMargin(lmargin);
	setRightMargin(rmargin);
    }

    double dmax(double a,double b){return a>b?a:b;}
    virtual double funmin(Data x)
    {
        for(int i=0;i<x.size();i++) {
        currentGenome[i]=(int)fabs(x[i]);
        if(isnan(x[i]) || isinf(x[i])){ return 1e+100;}

       // if(currentGenome[i]<0) currentGenome[i]=0;
        }
        double f= pop->fitness(currentGenome);
        return f;

    }
    virtual void	granal(Data x,Data &g)
    {
        for(int i=0;i<x.size();i++)
        {
        double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
        x[i]+=eps;
        double v1=funmin(x);
        x[i]-=2.0 *eps;
        double v2=funmin(x);
        g[i]=(v1-v2)/(2.0 * eps);
        x[i]+=eps;
        }
    }

};

void	Population::localSearch(int gpos)
{
	vector<int> g;
	g.resize(genome_size);
    int pos=gpos;
    for(int i=0;i<genome_size;i++) g[i]=genome[pos][i];
extern QString kind;
    double t = fitness_array[pos];

if(kind=="siman")
{
        /*
    PopulationProblem *pr=new PopulationProblem(this,g);
    double y;
    Data x;
    x.resize(genome_size);
    y=fitness_array[pos];
    for(int i=0;i<genome_size;i++) x[i]=g[i];


SimanMethod mt(pr);
    mt.setPoint(x,y);
    mt.Solve();
    mt.getPoint(x,y);
    
    MinInfo Info;
    Info.p = pr;
    Info.iters=2001;
    y=tolmin(x,Info);

    fitness_array[pos]=y;*/

        IntegerAnneal mt(this);
        mt.setPoint(g,t);
        mt.Solve();
        double y;
        mt.getPoint(g,y);
        fitness_array[pos]=y;
    fprintf(stderr,"LOCAL SEARCH[%20.10lg]=>[%20.10lg]\n",t,y);
    for(int i=0;i<genome_size;i++)
    {
        genome[pos][i]=g[i];
        if(genome[pos][i]<0) genome[pos][i]=0;
    }
   // delete pr;
    return;
}
else
if(kind == "cross")
{
	for(int iters=1;iters<=100;iters++)
	{
		int randgenome=rand() % genome_count;
		int randpos=rand() % genome_size;
		for(int i=0;i<randpos;i++) g[i]=genome[pos][i];
		for(int i=randpos;i<genome_size;i++) g[i]=genome[randgenome][i];
		double f=fitness(g);
		if(fabs(f)<fabs(fitness_array[pos]))
		{
			for(int i=0;i<genome_size;i++) genome[pos][i]=g[i];
            fitness_array[pos]=f;
            fprintf(stderr,"LOCAL SEARCH[%20.10lg]=>[%20.10lg]\n",t,f);

		}
		else
		{
			for(int i=0;i<randpos;i++) g[i]=genome[randgenome][i];
			for(int i=randpos;i<genome_size;i++) g[i]=genome[pos][i];
			f=fitness(g);
			if(fabs(f)<fabs(fitness_array[pos]))
			{
				for(int i=0;i<genome_size;i++) genome[pos][i]=g[i];
                fitness_array[pos]=f;
                fprintf(stderr,"LOCAL SEARCH[%20.10lg]=>[%20.10lg]\n",t,f);

			}
		}
	}
    return;
}
else
if(kind == "mutate")
{
    for(int i=0;i<genome_size;i++)
    {
        int ipos = rand() % genome_size;
        int new_value;
        for(int k=0;k<20;k++)
        {
        int old_value = genome[pos][ipos];
        new_value = rand() % 256;
        genome[pos][ipos]=new_value;
        for(int j=0;j<genome_size;j++) g[j]=genome[pos][j];
        double trial_fitness=fitness(g);
        if(fabs(trial_fitness)<fabs(fitness_array[pos]))
        {
                fitness_array[pos]=trial_fitness;
                printf("NEW BEST VALUE[%4d] = %20.10lg \n",pos,fitness_array[pos]);
                return;
        }
        else	genome[pos][ipos]=old_value;
        }
    }
}
}

/* Set the mutation rate */
void	Population::setMutationRate(double r)
{
	if(r>=0 && r<=1) mutation_rate = r;
}

/* Set the selection rate */
void	Population::setSelectionRate(double r)
{
	if(r>=0 && r<=1) selection_rate = r;
}

/* Return the selection rate */
double	Population::getSelectionRate() const
{
	return selection_rate;
}

/* Return the mutation rate */
double	Population::getMutationRate() const
{
	return mutation_rate;
}

/* Return the best fitness for this population */
double	Population::getBestFitness() const
{
	return  fitness_array[0];
}

/* Return the best chromosome */
vector<int> Population::getBestGenome() const
{
	vector<int> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];
	return g;
}

/* Evaluate and return the best fitness for all chromosomes in the population */
double	Population::evaluateBestFitness() 
{
	vector<int> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];	
	return fitness(g);
}

void	Population::setBest(vector<int> &g,double f)
{
	double tf=fitness(g);
	if(tf<fitness_array[0] && fabs(tf-f)>1e-4) 
	{
		return;
	}
	if(g.size()>genome_size)
	{
		int *old=new int[genome_size];
		for(int i=0;i<genome_count;i++)
		{
			for(int j=0;j<genome_size;j++) old[j]=genome[i][j];
			delete[] genome[i];
			genome[i]=new int[g.size()];
			for(int j=0;j<g.size();j++) 	genome[i][j]=0;
			for(int j=0;j<genome_size;j++) genome[i][j]=old[j];
			delete[] children[i];
			children[i]=new int[g.size()];
		}
		genome_size=g.size();
		extern int genome_length;
		genome_length=genome_size;
		delete[] old;
	}
    int pos=0;
	for(int i=0;i<genome_size;i++) 
	{
		genome[pos][i]=g[i];	
	}
	fitness_array[pos]=f;
}

void	Population::setGenome(int pos,vector<int> &g,double f,int k)
{
	double tf=fitness(g);
	if(tf<fitness_array[pos] && fabs(tf-f)>1e-4) return;
	if(g.size()>genome_size)
	{
		int *old=new int[genome_size];
		for(int i=0;i<genome_count;i++)
		{
			for(int j=0;j<genome_size;j++) old[j]=genome[i][j];
			delete[] genome[i];
			genome[i]=new int[g.size()];
			for(int j=0;j<g.size();j++) genome[i][j]=0;
			int ipos=0;
			for(int l=0;l<k;l++)
			{
				for(int j=0;j<genome_size/k;j++)
				{
					genome[i][l*g.size()/k+j]=old[ipos];
					ipos++;
				}
			}
			delete[] children[i];
			children[i]=new int[g.size()];
		}
		genome_size=g.size();
		extern int genome_length;
		genome_length=genome_size;
		delete[] old;
	}
	for(int i=0;i<genome_size;i++) 
	{
		genome[pos][i]=g[i];	
	}
	fitness_array[pos]=f;
}

void	Population::setGenome(int pos,vector<int> &g,double f)
{
	double tf=fitness(g);
	if(tf<fitness_array[pos] && fabs(tf-f)>1e-4) return;
	if(g.size()>genome_size)
	{
		int *old=new int[genome_size];
		for(int i=0;i<genome_count;i++)
		{
			for(int j=0;j<genome_size;j++) old[j]=genome[i][j];
			delete[] genome[i];
			genome[i]=new int[g.size()];
			for(int j=0;j<g.size();j++) genome[i][j]=0;
			for(int j=0;j<genome_size;j++) genome[i][j]=old[j];
			delete[] children[i];
			children[i]=new int[g.size()];
		}
		genome_size=g.size();
		extern int genome_length;
		genome_length=genome_size;
		delete[] old;
	}
	for(int i=0;i<genome_size;i++) 
	{
		genome[pos][i]=g[i];	
	}
	fitness_array[pos]=f;
}

void	Population::getGenome(int pos,vector<int> &g)
{
	g.resize(genome_size);
	for(int i=0;i<g.size();i++) g[i]=genome[pos][i];
}


/* Destructor */
Population::~Population()
{
	for(int i=0;i<genome_count;i++)
	{
		delete[] children[i];
		delete[] genome[i];
	}
	delete[] genome;
	delete[] children;
	delete[] fitness_array;
}
