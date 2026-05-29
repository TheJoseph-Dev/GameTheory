#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_GAMES (1<<19)
char dp[MAX_GAMES][2];
int game2num(char game[3][3]) {
    int g = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(game[i][j] == -1) g |= 1<<(3*i + j);
            else if(game[i][j]) g |= 1<<(3*i + j + 9);
    printf("\ng: %d\n", g);
    return g;
}

// 0 = not win, 1 = win, 2 = lost
char win(int game) {
    char win = 0;
    for(int i = 0; i < 3; i++) {
        win |= (game&(7<<(3*i))) == (7<<(3*i)) ? 1 : 0;
        win |= (game&(73<<i)) == (73<<i) ? 1 : 0;
    }
    win |= (game&273) == 273 ? 1 : 0;
    win |= (game&84) == 84 ? 1 : 0;

    if (win) return win;
    for(int i = 0; i < 3; i++) {
        win |= (game&(7<<(3*i + 9))) == (7<<(3*i + 9)) ? 1 : 0;
        win |= (game&(73<<(i + 9))) == (73<<(i + 9)) ? 1 : 0;
    }
    win |= (game&(273<<9)) == (273<<9) ? 1 : 0;
    win |= (game&(84<<9)) == (84<<9) ? 1 : 0;

    return win ? 2 : 0;
}

char solve(int game, char p) {
    if(dp[game][p] != -1) return dp[game][p];
    char w = win(game); 
    if(w) w = ((w - 1) == p) ? 1 : 2;
    if(__builtin_popcount(game) == 9 || w) return dp[game][p] = w;
    char s = 0;
    for(int i = p*9; i < 18 - (!p)*9; i++) {
        if((game&(1<<i)) || (game&(1<<((i+9)%18)))) continue;
        char t = solve(game|(1<<i), !p);
        if (t == 1) t = 2;
        else if (t == 2) t = 1;
        s |= (1<<t);
    }
    
    if (s & (1 << 1)) s = 1;
    else if (s & (1 << 0)) s = 0;
    else s = 2;
    return dp[game][p] = s;
}

int main() {
    char game[3][3] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
    };
    int g = game2num(game);
    printf("w: %d\n", win(g));
    memset(dp, -1, sizeof(dp));
    char r = solve(g, 0);
    if(r == 0) puts("DRAW");
    else if(r == 1) puts("WIN");
    else puts("LOSE");
    return 0;
}