//
//  football.h
//  foot
//
//  Created by Edison Ren on 13-4-6.
//  Copyright (c) 2013年 Edison Ren. All rights reserved.
//

#ifndef foot_h
#define foot_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// the type for money in the pool
#define TYPE unsigned long
// the scale of money used to bet
#define SCALE 9999
// the number of players
#define N 10000
// the number of games
#define M 10
// the number of rounds
#define R 300
// the number of threads
#define NUMTHREADS 4
// the switch for printing debugging information
#define DEBUG 1


typedef struct _odds {
    // the host wins
    double win;
    // it is a tie
    double tie;
    // the host lose
    double lose;
}odds;

// stack is the money bet on a game by one single gambler or the total fund in the pool
// assume that every gambler will bet on each game,so for a gambel
typedef struct _stack {
    // money bet on host wins
    TYPE win;
    // money bet on host ties
    TYPE tie;
    // money bet on host loses
    TYPE lose;
    //flag indicates which situation the gambler is in
    //1: bet on host wins
    //2: bet on host ties
    //3: bet on host loses
    //4: total fund on a game
    int flag;
}stack;


// simulate the behavior of the gambler
typedef struct _gamester
{
    // the money this gambler owns
    TYPE fund;
    // bet describes the ways this gambler bet on one game
    // it has 4 values:1,2,3,4
    // 1: greedy,looking for greatest profit
    // 2: bet according to the power level of the team
    // 3: conservative(always looking for the safest strategy and bet on the lower odds)
    // 4: only bet the odds decreases
    int bet;
    // the game picked from total 10 games
    int index;
//    // the odds when the gambler bet
//    odds* odd;
    // choice of the gamester
    // the money he puts in and which category he bets
    stack s[M];
}gamester;

// team
typedef struct _team
{
    char name[30];//a name of a team
    double level;//level of a team range from 1 - 5
}team;

// the odds of the game
typedef struct _game
{
    team *host;
    team *guest;
    odds odd;
}game;

// parameter list for pthread
typedef struct _args
{
    gamester * g;
    game * r;
    stack * total_s;
    stack * total_p;
}args;

// teams in the league and their level
static team league[20] = {  {"Manchester United",5},
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
    {"Reading",2.5}
};

// probality of win,draw and lose based on difference of the two teams's strength
// name difference D here and we have p[D*2] represents the probality of a game
// based on the difference of these two teams
// pp[][0]:win,pp[][1]:draw,pp[][2]:lose
// Ex.team A's level is 4 and team'b level is 5,then their level difference is
// 1,so the probality is represented pp[1]
static double pp[6][3]={    {30,40,30},
    {37.5,35,27.5},
    {42.5,32.5,25},
    {47.5,30,22.5},
    {52.5,27.5,20},
    {57.5,25.5,17.5}
};

// initialize all of the gambler
// Assuming:
// 60% focus on the odds, (bet_3)
// 30% only consider the level of team, (bet_2)
// 10% just chase for the largest profit (bet_1)
void init_gamester(gamester *, game *);
// show the state of the players
// mainly for debugging use
void show_gamester(gamester * gs, int size);
// based on the odds of a single game,
// or the level of the two teams to bet.
// here are three ways to gamble
// the input are:
//  the gamester and the round of games
//  the gamester's member stack will reflect the money bet
void bet_1(gamester *, game *);
void bet_2(gamester *, game *);
void bet_3(gamester *, game *);
void bet_4(gamester * player, game * g, game history[R][M], int r);
// generate one round of leaguage
void round_generate(team t[20],game r[10]);
// show the odds of this round
void test_round_generate(team t[],game *r);
// initialize the odds of this round at the begining
void init_odds(game *oneround);
void copy_odds(game * src, game *dst);
void show_history(game history[R][M]);
// size of the total_stack and total_payoff are both 10
void bet_iterate(gamester *g,game *r, game history[R][M], int rou, stack * total_stack, stack * total_payoff);
void init_stack(stack *);
void show_stack(stack *);

// generate one round of leaguage
//void round_generate(team* ,round*);
#endif
