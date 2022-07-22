# ifndef __POPULATION__H
# include <program.h>
class Population
{
	private:
		int	**children;
		int	**genome;
		double *fitness_array;
		double	mutation_rate,selection_rate;
		int	genome_count;
		int	genome_size;
		int	generation;
		Program	*program;

		double 	fitness(vector<int> &g);
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;
	public:
		Population(int gcount,int gsize,Program *p);
		void	setElitism(int s);
		int	getGeneration() const;
		int	getCount() const;
		int	getSize() const;
		void	nextGeneration();
		void	setMutationRate(double r);
		void	setSelectionRate(double r);
		double	getSelectionRate() const;
		double	getMutationRate() const;
		double	getBestFitness() const;
		double	evaluateBestFitness();
		vector<int> getBestGenome() const;
		void	reset();
		void	setBest(vector<int> &g,double f);
		void	getGenome(int pos,vector<int> &genome);
		void	setGenome(int pos,vector<int> &genome,double f);
		void	setGenome(int pos,vector<int> &genome,double f,int k);
		void	select();
		void	localSearch(int x);
		~Population();
		
};
# define __POPULATION__H
# endif
