# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <get_options.h>
int random_seed=1;
int genome_count=500;
int genome_length=200;
double selection_rate=0.10;
double mutation_rate=0.05;
QString  train_file="";
QString  test_file="";
QString  output_file="";
QString  kind="neural";
int	generations=500;
int localSearchGenerations=50;
int localSearchChromosomes=20;
const char *short_options="hp:o:d:g:c:l:s:m:p:t:r:k:n:";

void	print_usage()
{
	printf("\t-h	Print this help screen.\n"
			"\t-p	Specify problem file.\n"
			"\t-c	Specify population size.\n"
			"\t-l	Specify genome length.\n"
			"\t-s	Specify selection rate.\n"
			"\t-m	Specify	mutation  rate.\n"
			"\t-r	Specify	random seed.\n"
            "\t-k	Specify kind of the network.(neural,ode,pde,sode,kdv)\n"
            "\t-o   Specify plot file.\n"
			"\t-n	Specify maximum number of generations.\n"
            "\t-g   Specify local search generations\n"
            "\t-d   Specify local search chromosomes\n"
			"\t-t	Specify test  file.\n");
        exit(EXIT_SUCCESS);
}

void	parse_cmd_line(int argc,char **argv)
{
	int next_option;

	do
	{
              next_option=getopt(argc,argv,short_options);
	      switch(next_option)
	      {
		      case 'c':
			      genome_count=atoi(optarg);
			      break;
		      case 'l':
			      genome_length=atoi(optarg);
			      break;
		      case 's':
			      selection_rate=atof(optarg);
			      break;
		      case 'm':
			      mutation_rate=atof(optarg);
			      break;
		      case 'p':
              train_file=optarg;
			      break;
          case 'd':
              localSearchChromosomes=atoi(optarg);
              break;
          case 'g':
              localSearchGenerations=atoi(optarg);
              break;
		      case 't':
              test_file=optarg;
			      break;
		      case 'r':
			      random_seed=atoi(optarg);
			      break;
		      case 'o':
                output_file=optarg;
				break;
		      case 'k':
                kind=optarg;
			      break;
		      case 'n':
			      generations = atoi(optarg);
			      break;
		      case 'h':
			      print_usage();
			      exit(0);
			      break;
		      case -1:
			      break;
		      case '?':
			      print_usage();
			      exit(1);
			      break;
		      default:
			      print_usage();
			      exit(1);
			      break;
	      }
	}while(next_option!=-1);
}
