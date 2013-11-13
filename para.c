#include "football.h"
#include "mpi.h"

int main(int argc, char ** argv)
{
    int i, j, chunk;
    int myid, numprocs;
    double start_time, total_wall_time;

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
    stack coll_stack[10];
    // money should pay
    stack total_payoff[10];
    stack coll_payoff[10];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
//    printf("%d in %d\n", myid, numprocs);
    
    if (myid == 0) {
        start_time = MPI_Wtime();
    }
    
    chunk = N / numprocs;
    
    for (i = 0; i < R; ++i) {
        init_gamester(players, oneround);

        // initial the stacks
        init_stack(total_stack);
        init_stack(total_payoff);
        init_stack(coll_stack);
        init_stack(coll_payoff);

        for (j = myid * chunk; j < (myid + 1)*chunk && j < N; ++j) {
            bet_iterate(players + j, oneround, odds_history, i, total_stack, total_payoff);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        // compute the collective money in the pool after this round
        for (j = 0; j<numprocs; ++j) {
            MPI_Reduce(&total_stack+j, &coll_stack+j, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Reduce(&total_payoff+j, &coll_payoff+j, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        // communicate the stacks and compute the odds
        if (myid == 0) {
            for (j = 0; j<M; ++j) {
                update_odds(oneround+j, total_stack+j, total_payoff+j, 0.1);
            }
        }
        
        for (j = 0; j<M; ++j) {
            MPI_Bcast(&(oneround+j)->odd.win, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Bcast(&(oneround+j)->odd.tie, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Bcast(&(oneround+j)->odd.lose, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
        
        copy_odds(oneround, odds_history[i]);
    }
    
    if (myid == 0) {
        total_wall_time = MPI_Wtime() - start_time;
        printf("%d, %f\n", numprocs, total_wall_time);
        //test_round_generate(league, oneround);
        //show_history(odds_history);
    }
    
    
    MPI_Finalize();
    
    return 0;
}
