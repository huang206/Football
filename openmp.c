#include "football.h"
#include <omp.h>

//=============== OPENMP ====================
int main(int argc, char ** argv)
{
    int i,j,k;
    
    //genrate one round of league
    game oneround[10];
    round_generate(league,oneround);
    init_odds(oneround);
    //    test_round_generate(league,oneround);
    
    // all of the player
    gamester players[N];
    // money in the pool
    stack total_stack[10];
    // money should pay
    stack total_payoff[10];
    // ----------- DEBUGGING --------------
//    init_gamester(players, oneround);
//    init_stack(total_stack);
//    init_stack(total_payoff);
//
//    bet_iterate(players, oneround, total_stack, total_stack);
//    test_round_generate(league, oneround);
//    show_gamester(players, N);
//    
//    show_stack(total_payoff);
//    show_stack(total_stack);
    // ------------------------------------
    
    clock_t start = clock();

    for (i = 0; i < R; ++i) {
        init_gamester(players, oneround);

        // initial the stacks
        init_stack(total_stack);
        init_stack(total_payoff);

        #pragma omp parallel shared(players,oneround,total_stack,total_payoff) private(j,k)
        {
        #pragma omp sections nowait
         {
            #pragma omp section
            for (j = 0; j<N; ++j) {
                bet_iterate(players + j, oneround, total_stack, total_payoff);
            }

            #pragma omp section
            for (k = 0; k<M; ++k) {
                update_odds(oneround+k, total_stack+k, total_payoff+k, 0.1);
            }
         }/*end of sections*/
        }/*end of parallel section*/
    }

    double duration = (clock() - start) / CLOCKS_PER_SEC;
    printf("%f\n", duration);

    test_round_generate(league,oneround);

//=============== SEQUENTIAL ====================

// -------------- DEBUGGING -------------------
//    show_gamester(players, N);
//    show_stack(total_stack);
//    show_stack(total_payoff);
// --------------------------------------------
    
    return 0;
}
