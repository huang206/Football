#include "football.h"


//=============== SEQUENTIAL ====================
int main(int argc, char ** argv)
{
    int i, j;
    
    //genrate one round of league
    game oneround[10];
    game odds_history[R][M];
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

    // the odds updates R times
    for (i = 0; i < R; ++i) {
        // every time there are new gamblers
        init_gamester(players, oneround);

        // for N gambler
        for (j = 0; j<N; ++j) {
            // every gambler bet
            bet_iterate(players + j, oneround, odds_history, i, total_stack, total_payoff);
        }

        // for M games
        for (j = 0; j<M; ++j) {
            // the gambler maker updates the odds
            update_odds(oneround+j, total_stack+j, total_payoff+j, 0.1);
        }
        
        // copy the current odds into history record
        copy_odds(oneround, odds_history[i]);
    }

    double duration = (clock() - start) / CLOCKS_PER_SEC;
    printf("%f\n", duration);

//    test_round_generate(league,oneround);
    show_history(odds_history);

//=============== SEQUENTIAL ====================

// -------------- DEBUGGING -------------------
//    show_gamester(players, N);
//    show_stack(total_stack);
//    show_stack(total_payoff);
// --------------------------------------------
    
    return 0;
}
