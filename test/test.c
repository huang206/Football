#include "football.h"
//#include <stdio.h>
//#include <stdlib.h>


//#define TYPE unsigned long

// ************************************************************
// This function is used by the gambling company computing the new odds
//
// For team A versus team B,
// odds for A winning is x, odds for tie is y, B losing is z
//
// Assuming the gambling company want 10% as the income.
//
// this function is used to return the new odds
// For the input:
//      initial odds computed by the : x, y, z
//      the money bet on each column: a, b, c
//      the percent as interest (10% by default)
// For the returned:
//      the new values of odds


// returned error code: 1 for small odds
/*int compute_odds(double * x, double * y, double * z,
                 TYPE a, TYPE b, TYPE c,
                 double interest)
{
    if (*x < 1 || *y < 1 || *z < 1) {
        printf("The odds are weight too small!");
        return 1;
    }
    
    // money in the pool
    TYPE total = a + b + c;
    double expect_income = (1 - interest) * total;
    double increase_odds = (1 - 1.5 * interest) * total;
    
    // payout for A wining, losing, tie
    TYPE payout_win = (*x) * a;
    TYPE payout_tie = (*y) * b;
    TYPE payout_lose = (*z) * c;

    // ============== can be ignore =================
    // the maxium payout
    TYPE max_payout =
    payout_win > (payout_tie > payout_lose ? payout_tie : payout_lose) ?
    payout_win : (payout_tie > payout_lose ? payout_tie : payout_lose);
    
    // the payout is less than part of total
    if (max_payout < expect_income) {
        printf("Nothing should be changed!\n");
        return 0;
    }
    // ==============================================
    
    // modify the odds that is too high
    if (payout_win > expect_income) {
        *x = expect_income / a;
    }
    if (payout_tie > expect_income) {
        *y = expect_income / b;
    }
    if (payout_lose > expect_income) {
        *z = expect_income/ c;
    }
    
    // modify the odds that is too low
    // if the odds is too low, no one may bet on it
    if (payout_win < increase_odds) {
        *x = increase_odds / a;
    }
    if (payout_tie < increase_odds) {
        *y = increase_odds / b;
    }
    if (payout_lose < increase_odds) {
        *z = increase_odds / c;
    }
    
    
    return 0;
}*/
 
team league[20] = { {"Manchester United",5},
                    {"Manchester City",5},
                    {"Arsenal",4.5},
                    {"Chelsea",4.5},
                    {"Tottenham Hotspur",4.5},
                    {"Everton",4.5},
                    {"Liverpool",4},
                    {"West Bromwich Albion",3.5},
                    {"Swansea City",3.5},
                    {"Fulham",3.5},
                    {"Southampton",3.5},
                    {"West Ham United",3.5},
                    {"Newcastle United",3.5},
                    {"Norwich City",3},
                    {"Stoke City",3},
                    {"Aston Villa",3},
                    {"Sunderland",3},
                    {"Wigan Athletic",3},
                    {"Queens Park Rangers",2.5},
                    {"Reading",2.5} };

// probality of win,draw and lose based on difference of the two teams's strength
// name difference D here and we have p[D*2] represents the probality of a game
// based on the difference of these two teams
// pp[][0]:win,pp[][1]:draw,pp[][2]:lose
// Ex.team A's level is 4 and team'b level is 5,then their level difference is
// 1,so the probality is represented pp[1]
static double pp[6][3]={ {30,40,30},
                         {37.5,35,27.5},
                         {42.5,32.5,25},
                         {47.5,30,22.5},
                         {52.5,27.5,20},
                         {57.5,25.5,17.5} };

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

void test_round_generate(team t[],game *r)
{
    int i = 0;
    for(;i < 20;i++)
    {
        printf("%s %g\n",t[i].name,t[i].level);
    }
   
    round_generate(t,r);
    i = 0;
    for(;i < 10;i++)
    {
        printf("%s vs %s\n",(r[i].host)->name,(r[i].guest)->name);
    }

    return;
}

void init_odds(game *oneround)
{
    int i = 0;
    for(;i < 10;i++)
    {
        double host_level = oneround[i].host->level;
        double guest_level = oneround[i].guest->level;
        double tmp = 2 * ((host_level>guest_level) ? (host_level - guest_level) : (guest_level - host_level));
        int delta = (int) tmp;
        //printf("delta is %d\n",delta);
        //printf("%g %g %g\n",oneround[i].odd.win,pp[delta][1],pp[delta][2]);
        oneround[i].odd.win = 100.00 / pp[delta][0];
        oneround[i].odd.tie = 100.00 / pp[delta][1];
        oneround[i].odd.lose = 100.00 / pp[delta][2];
        printf("%g %g %g\n",oneround[i].odd.win,oneround[i].odd.tie,oneround[i].odd.lose);
    }
    return;
}

int main()
{
    /*double x = 1.2, y = 2.8, z = 3.0, interest = 0.1;
    TYPE a = 5000, b = 500, c = 200;
    
    compute_odds(&x, &y, &z, a, b, c, interest);
    
    printf("%f, %f, %f\n", x, y, z);*/

    //test code for round_generate()
    game oneround[10];
    test_round_generate(league,oneround);    
    init_odds(oneround);
    return 0;
}
