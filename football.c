#include "football.h"

// 1:15/15/30/40
// 2:25/25/25/25
// 3:30/15/15/40
// 4:40/10/40/10
// 5:10/40/10/40
void init_gamester(gamester * gs, game * r)
{
    srand((int)time(0));
    
    int i, temp;
    for (i = 0; i<N; ++i) {
        (gs+i)->fund = (TYPE)(rand()%SCALE + 1);
        
        temp = rand()%100;
        
        if (temp < 10) {
            (gs+i)->bet = 1;
        }
        else if (temp < 40) {
            (gs+i)->bet = 2;
        }
        else if (temp < 10) {
            (gs+i)->bet = 3;
        }
        else {
            (gs+i)->bet = 4;
        }
        
        (gs+i)->index = (int)rand()%10;
        
        // his choice should be assigned then
    }
}


// generate one round of leaguage
void round_generate(team t[20],game r[10])
{
    int flag[20];//indicate if a team has been choosen
    int i = 0;
    for(;i < 20;i++)
    {
        flag[i] = 0;
    }
    
    srand((int)time(0));
    
    int count = 0;
    while(count < 10)
    {
        int s = rand()%20;
        if(flag[s] == 0)
        {
            flag[s] = 1;
            r[count].host = &t[s];
            count++;
        }
    }
    
    count = 0;
    while(count < 10)
    {
        int s = rand()%20;
        if(flag[s] == 0)
        {
            flag[s] = 1;
            r[count].guest = &t[s];
            count++;
        }
    }
}

// initialize the odds of this round at the begining
void init_odds(game *oneround)
{
    int i = 0;
    for(;i < M;i++)
    {
        double host_level = oneround[i].host->level;
        double guest_level = oneround[i].guest->level;
        int delta = (int) (2 * ((host_level>guest_level) ? (host_level - guest_level) : (guest_level - host_level)));
        
        // --------------- DEBUGGING ---------------------
//        printf("delta is %d\n",delta);
//        printf("%g %g %g\n",pp[delta][0],pp[delta][1],pp[delta][2]);
        // -----------------------------------------------
        
        oneround[i].odd.win = 90.00 / pp[delta][0];
        oneround[i].odd.tie = 90.00 / pp[delta][1];
        oneround[i].odd.lose = 90.00 / pp[delta][2];
        
        // --------------- DEBUGGING ---------------------
//        printf("%g %g %g\n",oneround[i].odd.win,oneround[i].odd.tie,oneround[i].odd.lose);
        // -----------------------------------------------
    }
    
    return;
}

// copy a round into according slot history record
void copy_odds(game * src, game *dst)
{
    int i;
    for ( i = 0; i<M; ++i) {
        dst[i].odd.win = src[i].odd.win;
        dst[i].odd.tie = src[i].odd.tie;
        dst[i].odd.lose = src[i].odd.lose;
    }
    
    return;
}

void bet_1(gamester * player, game * game)
{
    srand((int)time(0));
   
    // a player will partition the money on every games
    int i;
    double money[M];
    double sum = 0;
    odds o;
    for (i = 0; i < M; ++i) {
        money[i] = (double)(rand()%100);
        sum += money[i];
    }
    for (i = 0; i < M; ++i) {
        money[i] = ((player->fund) * money[i]) / sum;
        
        o = game[i].odd;
        if (o.win >= o.tie && o.win >= o.lose) {
            player->s[i].win = (TYPE)money[i];
            player->s[i].tie = 0;
            player->s[i].lose = 0;
            player->s[i].flag = 1;
        }
        else if (o.tie >= o.win && o.tie >= o.lose) {
            player->s[i].tie = (TYPE)money[i];
            player->s[i].win = 0;
            player->s[i].lose = 0;
            player->s[i].flag = 2;
        }
        else {
            player->s[i].lose = (TYPE)money[i];
            player->s[i].win = 0;
            player->s[i].tie = 0;
            player->s[i].flag =3;
        }
    }
}


void bet_2(gamester * player, game * game)
{
    srand((int)time(0));
    
    int i;
    double money[M];
    double sum = 0;
    // if the difference of level of the two team is closed, then chose the one with lower odds
    double host_level, guest_level, hostVSguest;
    odds o;
    
    // a player will partition the money on every games
    for (i = 0; i < M; ++i) {
        money[i] = (double)(rand()%100);
        sum += money[i];
    }
    for (i = 0; i < M; ++i) {
        money[i] = ((player->fund) * money[i]) / sum;
        
        host_level = game[i].host->level;
        guest_level = game[i].guest->level;
        hostVSguest = (host_level > guest_level)? (host_level - guest_level):(guest_level - host_level);
        
        if (hostVSguest < 1) {
            o = game[i].odd;
            if (o.win <= o.tie && o.win < o.lose) {
                player->s[i].win = (TYPE)money[i];
                player->s[i].tie = 0;
                player->s[i].lose = 0;
                player->s[i].flag = 1;
            }
            else if (o.tie <= o.win && o.tie <= o.lose) {
                player->s[i].tie = (TYPE)money[i];
                player->s[i].win = 0;
                player->s[i].lose = 0;
                player->s[i].flag = 2;
            }
            else {
                player->s[i].lose = (TYPE)money[i];
                player->s[i].win = 0;
                player->s[i].tie = 0;
                player->s[i].flag =3;
            }
        }
        else {
            if (host_level > guest_level) {
                player->s[i].win = (TYPE)money[i];
                player->s[i].tie = 0;
                player->s[i].lose = 0;
                player->s[i].flag = 1;
            }
            else {
                player->s[i].lose = (TYPE)money[i];
                player->s[i].win = 0;
                player->s[i].tie = 0;
                player->s[i].flag = 3;
            }
        }
    }
}


void bet_3(gamester * player, game * game)
{
    srand((int)time(0));
    
    // a player will partition the money on every games
    int i;
    double money[M];
    double sum = 0;
    for (i = 0; i < M; ++i) {
        money[i] = (double)(rand()%100);
        sum += money[i];
    }
    for (i = 0; i < M; ++i) {
        money[i] = ((player->fund) * money[i]) / sum;
        
        odds o = game[i].odd;
        if (o.win <= o.tie && o.win <= o.lose) {
            player->s[i].win = (TYPE)money[i];
            player->s[i].tie = 0;
            player->s[i].lose = 0;
            player->s[i].flag = 1;
        }
        else if (o.tie <= o.win && o.tie <= o.lose) {
            player->s[i].tie = (TYPE)money[i];
            player->s[i].win = 0;
            player->s[i].lose = 0;
            player->s[i].flag = 2;
        }
        else {
            player->s[i].lose = (TYPE)money[i];
            player->s[i].win = 0;
            player->s[i].tie = 0;
            player->s[i].flag =3;
        }
    }
}

// odds going down
void bet_4(gamester * player, game * g, game history[R][M], int r)
{
    srand((int)time(0));
    
    if (r < 1) {
        bet_3(player, g);
    }
    // a player will partition the money on every games
    int i;
    double money[M];
    double diffwin, difftie, difflose;
    double sum = 0;
    for (i = 0; i < M; ++i) {
        money[i] = (double)(rand()%100);
        sum += money[i];
    }
    for (i = 0; i < M ; ++i) {
        money[i] = ((player->fund) * money[i]) / sum;
        
        diffwin = history[r][i].odd.win - history[r-1][i].odd.win;
        difftie = history[r][i].odd.tie - history[r-1][i].odd.tie;
        difflose = history[r][i].odd.lose - history[r-1][i].odd.lose;
        
        odds o = g[i].odd;
        if (diffwin < 0 && diffwin < difflose && diffwin < difftie) {
            player->s[i].win = (TYPE)money[i];
            player->s[i].tie = 0;
            player->s[i].lose = 0;
            player->s[i].flag = 1;
        }
        else if (difftie < 0 && difftie < difflose && difftie < diffwin) {
            player->s[i].win = 0;
            player->s[i].tie = (TYPE)money[i];
            player->s[i].lose = 0;
            player->s[i].flag = 2;
        }
        else if (difflose < 0 && difflose < difftie && difflose < diffwin) {
            player->s[i].win = 0;
            player->s[i].tie = 0;
            player->s[i].lose = (TYPE)money[i];
            player->s[i].flag = 3;
        }
        else {
            if (o.win <= o.tie && o.win <= o.lose) {
                player->s[i].win = (TYPE)money[i];
                player->s[i].tie = 0;
                player->s[i].lose = 0;
                player->s[i].flag = 1;
            }
            else if (o.tie <= o.win && o.tie <= o.lose) {
                player->s[i].tie = (TYPE)money[i];
                player->s[i].win = 0;
                player->s[i].lose = 0;
                player->s[i].flag = 2;
            }
            else {
                player->s[i].lose = (TYPE)money[i];
                player->s[i].win = 0;
                player->s[i].tie = 0;
                player->s[i].flag =3;
            }
        }
    }
    
}


// size of the total_stack and total_payoff are both 10
void bet_iterate(gamester *g, game *r, game history[R][M], int rou, stack * total_stack, stack * total_payoff)
{
    // the choice of the gamester is reflected from g->stack
    switch(g->bet)
    {
        case 1:
            bet_1(g, r);
            break;
        case 2:
            bet_2(g, r);
            break;
        case 3:
            bet_3(g, r);
            break;
        case 4:
            bet_4(g, r, history, rou);
    }
    
    int i;
    for ( i = 0; i<M; ++i) {
        // update the local sum of the money pool
        // need synchronization when implemented parallely
        switch(g->s[i].flag)
        {
            case 1:
                total_stack[i].win += g->s[i].win;
                total_payoff[i].win += g->s[i].win * r[i].odd.win;
//            printf("%lu, %f\n", g->s.win, g->odd.win);
                break;
            case 2:
                total_stack[i].tie += g->s[i].tie;
                total_payoff[i].tie += g->s[i].tie * r[i].odd.tie;
//            printf("%lu, %f\n", g->s.tie, g->odd.tie);
                break;
            case 3:
                total_stack[i].lose += g->s[i].lose;
                total_payoff[i].lose += g->s[i].lose * r[i].odd.lose;
//            printf("%lu, %f\n", g->s.lose, g->odd.lose);
                break;
        }
    }
    
    return;
}

// update the odds of a game
// knowing the odds of the game and the payoff for the previous odds
// decrease it when it is too low (inspire the buyers)
// increase the odds when it is too high (leave profit)
// the default interest rate could be 10%
// the plausible interest rate is 10% - 20%
void update_odds(game * g, stack * total, stack * pay, double interest)
{
    // money in the pool for this game
    TYPE income = total->win + total->tie + total->lose;
    // money should be paid
    TYPE outcome = pay->win + pay->tie + pay->lose;
    // expected income and the accepeted largest income
    double expect_income = (1 - interest) * income;
    double increase_odds = (1 - 1.5 * interest) * income;
    
    // modify the odds that is too high
    if (pay->win > expect_income) {
        g->odd.win = expect_income / total->win;
    }
    if (pay->tie > expect_income) {
        g->odd.tie = expect_income / total->tie;
    }
    if (pay->lose > expect_income) {
        g->odd.lose = expect_income / total->lose;
    }
    
    // modify the odds that is too low
    // if the odds is too low, no one may bet on it
    if (pay->win < increase_odds && total->win != 0) {
        g->odd.win = increase_odds / total->win;
    }
    if (pay->tie < increase_odds && total->tie != 0) {
        g->odd.tie = increase_odds / total->tie;
    }
    if (pay->lose < increase_odds && total->lose != 0) {
        g->odd.lose = increase_odds / total->lose;
    }
    
    return;
}


// show the gamesters
void show_gamester(gamester * gs, int size)
{
    int i , j;
    for (i = 0; i<size; ++i) {
        for (j = 0; j<M; ++j) {
            printf("Player %d: fund %lu, bet %d, %lu, %lu, %lu, %d.\n", i, gs[i].fund, gs[i].bet, gs[i].s[j].win, gs[i].s[j].tie, gs[i].s[j].lose, gs[i].s[j].flag);
        }
        printf("=======================\n");
    }
}


// show the odds
void test_round_generate(team t[],game *r)
{
    int i = 0;
    //    for(;i < 20;i++)
    //    {
    //        printf("%s %g\n",t[i].name,t[i].level);
    //    }
    
    round_generate(t,r);
    i = 0;
    for(;i < 10;i++)
    {
        printf("%s(%.1f) vs %s(%.1f) : %f, %f, %f\n",(r[i].host)->name, (r[i].host)->level, (r[i].guest)->name, (r[i].guest)->level, r[i].odd.win, r[i].odd.tie, r[i].odd.lose);
    }
    
    printf("**********************************************\n");
    return;
}

void init_stack(stack * s)
{
    int i = 0;
    for(;i < 10;i++)
    {
        s[i].win = 0;
        s[i].tie = 0;
        s[i].lose = 0;
        s[i].flag = 4;
    }
    return;
}

void show_stack(stack * s)
{
    int i;
    for (i = 0; i<10; ++i) {
        printf("%lu, %lu, %lu, %d\n", s[i].win, s[i].tie, s[i].lose, s[i].flag);
    }
}

void show_history(game history[R][M])
{
    int i, j;
    for (i = 0; i<M; ++i) {
        printf("\n//////////////////////\nMatch %d:\n", i);
        printf("WIN: ");
        for (j = 0; j<R; j += N/R) {
            printf("%f,", history[j][i].odd.win);
        }
        printf("\nTIE: ");
        for (j = 0; j<R; j += N/R) {
            printf("%f,", history[j][i].odd.tie);
        }
        printf("\nLOSE: ");
        for (j = 0; j<R; j += N/R) {
            printf("%f,", history[j][i].odd.lose);
        }
    }
}


