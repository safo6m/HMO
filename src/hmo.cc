#include <cstdio>
#include <getopt.h>

#include "GA.h"
#include "FirstFit.h"

using namespace std;

/*
 * ulazni parametri
 *  ulazna datoteka                 = -f ili --input
 *  velicina populacije             = -n ili --population
 *  vjerojatnost mutacije           = -p ili --mutation
 *  broj jediniki za eliminaciju    = -m ili --elimination
 *  broj iteracija GA               = -i ili --iterations
 */

char filename[256]  = FILENAME;
int population      = POPULATION;
int mutation        = MUTATION;
int elimination     = ELIMINATION;
int iterations      = ITERATIONS;

int parse_arguments(int *argc, char*** argv) {
    int c, option_index;

    while (1) {
        static struct option long_options[] = {
            {"input",       required_argument, 0, 'f'},
            {"population",  required_argument, 0, 'n'},
            {"mutation",    required_argument, 0, 'p'},
            {"elimination", required_argument, 0, 'm'},
            {"iterations",  required_argument, 0, 'i'},
            {0, 0, 0, 0}};

        option_index = 0;
        c = getopt_long(*argc, *argv, "f:n:p:m:i:", long_options, &option_index);
        if (c == -1)  break;

        switch (c) {
            case 'f':
                strcpy(filename, optarg);
                break;
            case 'n':
                sscanf(optarg, "%d", &population);
                break;
            case 'p':
                sscanf(optarg, "%d", &mutation);
                break;
            case 'm':
                sscanf(optarg, "%d", &elimination);
                break;
            case 'i':
                sscanf(optarg, "%d", &iterations);
                break;
            case '?':
                break;
            default:
                return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if(parse_arguments(&argc, &argv)) {
        puts("Error while parsing arguments!");
        return 1;
    }

    /* Print GA parameters */
    printf("GA parameters:\n");
    printf("\tinput file = %s\n", filename);
    printf("\tpopulation size = %d\n", population);
    printf("\tmutation probability = %.3lf\n", mutation / 100.);
    printf("\telimination size = %d\n", elimination);
    printf("\tnumber of iterations = %d\n", iterations);

    /* run GA */
    unique_ptr<GA> solution(new GA(filename, population, mutation, elimination, iterations));

    if(solution->run()) {
        puts("Error while running GA!");
        return 1;
    }

    return 0;
}
