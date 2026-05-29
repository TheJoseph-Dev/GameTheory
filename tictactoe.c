#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX_GAMES (1<<18)+2
char dp[MAX_GAMES][2];
int game2num(char game[3][3]) {
    int g = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(game[i][j] == 1) g |= 1<<(3*i + j);
            else if(game[i][j] == 2) g |= 1<<(3*i + j + 9);
    printf("\ng: %d\n", g);
    return g;
}

// 0 = p1 won, 1 = not won, 2 = p2 won
char win(int game) {
    char w = 0;
    for(int i = 0; i < 3; i++) {
        w |= (game&(7<<(3*i))) == (7<<(3*i)) ? 1 : 0;
        w |= (game&(73<<i)) == (73<<i) ? 1 : 0;
    }
    w |= (game&273) == 273 ? 1 : 0;
    w |= (game&84) == 84 ? 1 : 0;

    if (w) return 0;
    for(int i = 0; i < 3; i++) {
        w |= (game&(7<<(3*i + 9))) == (7<<(3*i + 9)) ? 1 : 0;
        w |= (game&(73<<(i + 9))) == (73<<(i + 9)) ? 1 : 0;
    }
    w |= (game&(273<<9)) == (273<<9) ? 1 : 0;
    w |= (game&(84<<9)) == (84<<9) ? 1 : 0;

    return w ? 2 : 1;
}

char solve(int game, char p) {
    if(dp[game][p] != -1) return dp[game][p];
    char w = win(game);
    if(__builtin_popcount(game) == 9 || w != 1) return dp[game][p] = w;
    char best = p ? 0 : 2;
    for(int i = p*9; i < 18 - (!p)*9; i++) {
        if((game&(1<<i)) || (game&(1<<((i+9)%18)))) continue;
        char t = solve(game|(1<<i), !p);
        best = p ? MAX(best, t) : MIN(best, t);
    }
    return dp[game][p] = best;
}

int main() {
    char game[3][3] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
    };
    int g = game2num(game);
    //printf("w: %d\n", win(g));
    memset(dp, -1, sizeof(dp));
    char p = 0;
    char r = solve(g, p);
    if(r == 1) puts("DRAW");
    else puts(r == 2*p ? "WIN" : "LOSE");

    return 0;
}