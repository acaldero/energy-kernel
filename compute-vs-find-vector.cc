
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#include <Memoization_uthash.h>

/*
 * Find it
 */

MemoSupport *tester ;

double findConsumption ( int n_eltos, float *utilization )
{
    double *ret2 ;

    ret2 = tester->find(n_eltos * sizeof(float), utilization) ;
    return ((ret2) ? (*ret2) : 0.0) ;
}


/*
 * Compute it
 */

double staticPower  = 10 ;
double constant     = 10 ;
double timeInterval = 10 ;

// CUBIC
double calculateConsumption_CUBIC ( int n_eltos, float *utilization )
{
    double consumption = 0 ;
    for (int i=0; i<n_eltos; i++)
         consumption += (staticPower + constant * pow(utilization[i], 3)) * timeInterval;
    return consumption;
}

// Linear
double calculateConsumption_LINEAR ( int n_eltos, float *utilization )
{
    double consumption = 0 ;
    for (int i=0; i<n_eltos; i++)
         consumption += (staticPower + constant * utilization[i]) * timeInterval;
    return consumption;
}

// Square
double calculateConsumption_SQUARE ( int n_eltos, float *utilization )
{
    double consumption = 0 ;
    for (int i=0; i<n_eltos; i++)
         consumption += (staticPower + constant * pow(utilization[i], 2)) * timeInterval;
    return consumption;
}

// SQRT
double calculateConsumption_SQRT ( int n_eltos, float *utilization )
{
    double consumption = 0 ;
    for (int i=0; i<n_eltos; i++)
         consumption += (staticPower + constant * sqrt(utilization[i])) * timeInterval;
    return consumption;
}

// summary
const char *algos_name[] = { "CUBIC", "LINEAR", "SQUARE", "SQRT", "FIND" } ;
   double (*algos_fn[])(int, float *) = { calculateConsumption_CUBIC, 
                                          calculateConsumption_LINEAR, 
                                          calculateConsumption_SQUARE, 
                                          calculateConsumption_SQRT, 
                                          findConsumption } ;


/*
 * Tester
 */

// http://www.binarytides.com/get-time-difference-in-microtime-in-c/
double time_diff ( struct timeval x , struct timeval y )
{
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}

#define   MAX_TESTS 1024
#define     N_TESTS 100
float    vint [MAX_TESTS] ;

#define  N_ALGOS     5
#define  N_ELTOS    65
#define  N_RESULTS   2
#define  R_VALUES  0
#define  R_TIMES   1
double   algos_results [N_ALGOS][N_ELTOS][N_RESULTS] ;

char * TIME_UNIT    = (char *)"Time elapsed (us)" ;
char * VALUE_UNIT   = (char *)"Values computed" ;
char * BASE_CAPTION = (char *)"Energy simulation models analysis for cloud computing systems" ;
char * BASE_LABEL   = (char *)"tabla_results_energy_models" ;


void main_tester_compute ( int aid, int n_eltos )
{
    int     i, j ;
    double  ret ;
    struct  timeval before, after ;

    for (j=0; j<n_eltos; j++)
    {
         gettimeofday(&before , NULL);
         for (i=0; i<N_TESTS; i++) {
              ret = algos_fn[aid](j, vint) ;
         }
         gettimeofday(&after , NULL);

         algos_results[aid][j][R_TIMES]  = time_diff(before, after) ;
         algos_results[aid][j][R_VALUES] = ret ;
    }
}


/*
 * Print
 */

#define  MSG(x) #x
#define MSG2(x) MSG(x)

void main_print_txt_header ( int n_eltos, char *unit )
{
    int j ;

    printf("\n") ;
    printf("Opt.;	Alg.;	Number-eltos -> %s\n", unit) ;
    printf("Opt.;	Alg.;	") ;
    for (j=0; j<(n_eltos-1); j++) {
         printf("%i;	", j) ;
    }
    printf("\n") ;
}

void main_print_txt_results ( int aid, int n_algos, int n_eltos, int values )
{
    int j ;

    printf("%s;	%s;	", MSG2(OPT), algos_name[aid]); 
    for (j=0; j<n_eltos; j++) {
         printf("%.0lf;	", algos_results[aid][j][values]) ;
    }
    printf("\n") ;
}

void main_print_txt_footer ( char *base_caption, char *base_label, char *suffix )
{
    printf("\n") ;
}


void main_print_tex_header ( int n_eltos, char *units )
{
    int j ;

    printf("%% header\n") ;

  //printf("\\begin{table}[hbt]\n") ;
    printf("\\begin{sidewaystable}[hbt]\n") ;
    printf("\\renewcommand{\\arraystretch}{1.3}\n") ;
    printf("\\centering \\footnotesize\n") ;
    printf("\n") ;
  //printf("\\begin{tabular}{|l|l|") ;
    printf("\\begin{tabularx}{\\linewidth}{|l|l|") ;
    for (j=0; j<(n_eltos-1); j++) {
         printf("l|") ;
    }
    printf("}\n") ;
    printf("\\hline\n") ;

    printf("\\multirow{2}{*}{Opt} &  \n") ;
    printf("\\multirow{2}{*}{Algorithm} & \n") ;
    printf("\\multicolumn{%d}{c|}{Number elements / %s} \\\\ \n", n_eltos-1, units) ;
    printf("\\cline{3-%d}\n", n_eltos+1) ;

    printf("\\multicolumn{1}{|l|}{} &\n") ;
    printf("\\multicolumn{1}{|l|}{} &\n") ;
    for (j=0; j<n_eltos-2; j++) {
         printf("\\multicolumn{1}{c}{%d} &\n", j) ;
    }
    printf("\\multicolumn{1}{c|}{%d} \\\\\n", n_eltos-2) ;
    printf("\\hline\n") ;
    printf("\n") ;
}

void main_print_tex_results ( int aid, int n_algos, int n_eltos, int values )
{
    int j ;

    if (0 == aid) {
        printf("%% results for %s\n", algos_name[aid]) ;
        printf("\\multirow{2}{*}{%s} &\n", MSG2(OPT)) ;
    }
    else {
        printf("%% results for %s\n", algos_name[aid]) ;
        printf("\\multicolumn{1}{|l|}{} &\n") ;
    }

    printf("\\multicolumn{1}{|l|}{%s} &\n", algos_name[aid]) ;
    for (j=0; j<n_eltos-2; j++) {
         printf("\\multicolumn{1}{c}{%.0lf} &\n", algos_results[aid][j][values]) ;
    }
    printf("\\multicolumn{1}{c|}{%.0lf} \\\\\n",  algos_results[aid][n_eltos-1][values]) ;

    if (aid == (n_algos - 1)) {
         printf("\\hline\n") ;
    }
    else {
         printf("\\cline{2-%d}\n", n_eltos+1) ;
    }
    printf("\n") ;
}

void main_print_tex_footer ( char *base_caption, char *base_label, char *suffix )
{
  //printf("\\end{tabular}\n") ;
    printf("\\end{tabularx}\n") ;
    printf("\n") ;
    printf("\\caption{%s.}\n", base_caption) ;
    printf("\\label{%s-%s}\n", base_label, suffix) ;
  //printf("\\end{table}\n") ;
    printf("\\end{sidewaystable}\n") ;
    printf("\n") ;
}


/*
 * Main
 */

int main ( int argc, char *argv[] )
{
    int    i, j ;
    double ret ;
    int    n_eltos ;

    // 0.- parameters
    if (argc < 2) 
    {
        printf("Usage: %s <number of elements>\n"
               "[--tex-header-times]  [--tex-times]  [--tex-footer-times]\n"
               "[--tex-header-values] [--tex-values] [--tex-footer-values]\n"
               "[--txt-header-times]  [--txt-times]  [--txt-footer-times]\n"
               "[--txt-header-values] [--txt-values] [--txt-footer-values]\n", argv[0]) ;
        exit(-1) ;
    }
    n_eltos = atoi(argv[1]) ;
    if (n_eltos > N_ELTOS) {
        n_eltos = N_ELTOS ;
        //printf("> N_ELTOS = %d\n", n_eltos) ;
    }

    // 1.- init
    for (i=0; i<n_eltos; i++) {
         vint[i] = (float) i / (float)n_eltos ;
    }

    tester = new MemoSupport("testing") ;
    for (i=0; i<n_eltos; i++) {
         ret = calculateConsumption_SQRT(i, vint) ;
         tester->add(i * sizeof(float), (float *)vint, (float)ret) ;
    }

    // 2.- testing...
    for (i=0; i<N_ALGOS; i++) {
         main_tester_compute(i, n_eltos) ;
    }

    // 3.- printing...
    for (j=1; j<argc; j++)
    {
              if (!strcmp(argv[j],"--tex-header-times")) {
             main_print_tex_header(n_eltos, TIME_UNIT) ;
         }
         else if (!strcmp(argv[j],"--txt-header-times")) {
             main_print_txt_header(n_eltos, TIME_UNIT) ;
         }
         else if (!strcmp(argv[j],"--tex-header-values")) {
             main_print_tex_header(n_eltos, VALUE_UNIT) ;
         }
         else if (!strcmp(argv[j],"--txt-header-values")) {
             main_print_txt_header(n_eltos, VALUE_UNIT) ;
         }

         else if (!strcmp(argv[j],"--tex-times")) 
         {
	    for (i=0; i<N_ALGOS; i++) {
                 main_print_tex_results(i, N_ALGOS, n_eltos, R_TIMES) ;
	    }
         }
         else if (!strcmp(argv[j],"--txt-times")) 
         {
	    for (i=0; i<N_ALGOS; i++) {
		 main_print_txt_results(i, N_ALGOS, n_eltos, R_TIMES) ;
	    }
         }
         else if (!strcmp(argv[j],"--tex-values")) 
         {
	    for (i=0; i<N_ALGOS; i++) {
                 main_print_tex_results(i, N_ALGOS, n_eltos, R_VALUES) ;
	    }
         }
         else if (!strcmp(argv[j],"--txt-values")) 
         {
	    for (i=0; i<N_ALGOS; i++) {
		 main_print_txt_results(i, N_ALGOS, n_eltos, R_VALUES) ;
	    }
         }

         else if (!strcmp(argv[j],"--tex-footer-times")) {
             main_print_tex_footer(BASE_CAPTION, BASE_LABEL, (char *)"-times") ;
         }
         else if (!strcmp(argv[j],"--txt-footer-times")) {
             main_print_txt_footer(BASE_CAPTION, BASE_LABEL, (char *)"-times") ;
         }
         else if (!strcmp(argv[j],"--tex-footer-values")) {
             main_print_tex_footer(BASE_CAPTION, BASE_LABEL, (char *)"-values") ;
         }
         else if (!strcmp(argv[j],"--txt-footer-values")) {
             main_print_txt_footer(BASE_CAPTION, BASE_LABEL, (char *)"-values") ;
         }
    }

    // end
    return 0;
}

