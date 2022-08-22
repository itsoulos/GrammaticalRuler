#include <get_options.h>
# include <population.h>
# include <dataset.h>
# include <ruleprogram.h>
# include <stdio.h>

RuleProgram *program = NULL;
Population *pop = NULL;
Dataset *train= NULL;
Dataset *test = NULL;

void init_program()
{
	srand(random_seed);
	srand48(random_seed);
    FILE *fp=fopen(train_file.toStdString().c_str(),"r");
    if(!fp)
        exit(fprintf(stderr,"File %s not ready for reading\n",train_file.toStdString().c_str()));
    fclose(fp);
    fp=fopen(test_file.toStdString().c_str(),"r");
    if(!fp)
        exit(fprintf(stderr,"File %s not ready for reading\n",test_file.toStdString().c_str()));
    fclose(fp);
    train = new Dataset(train_file.toStdString().c_str());
    test  = new Dataset(test_file.toStdString().c_str());
    program = new RuleProgram(train,test);
    pop = new Population(genome_count,genome_length,program);
    pop->setMutationRate(mutation_rate);
    pop->setSelectionRate(selection_rate);
}

void run_program()
{
     vector<int> bestGenome;
    for(int i=0;i<generations;i++)
    {
        pop->nextGeneration();
        printf("Iteration: %04d Best fitness: %20.10lg\n",
               i,pop->getBestFitness());
        bestGenome = pop->getBestGenome();
        string x= program->printF(bestGenome);
        printf("Best Program:\n%s\n",x.c_str());
    program->fitness(bestGenome);
    double tt  = program->getTestError();
    double tc  = program->getClassError();
    printf("Test  error: %20.10lg \n",tt);
    printf("Class error: %20.10lg%%\n",tc);
    }
    program->fitness(bestGenome);
    double tt  = program->getTestError();
    double tc  = program->getClassError();
    printf("Testerror: %20.10lg ",tt);
    printf("Classerror: %20.10lg\n",tc);
}

void done_program()
{
    delete train;
    delete test;
    delete program;
    delete pop;
}


int main(int argc, char *argv[])
{
    parse_cmd_line(argc,argv);
    if(train_file.isEmpty() ||
            test_file.isEmpty())
        print_usage();
    init_program();
    run_program();
    done_program();
    return 0;
}
