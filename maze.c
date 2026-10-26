#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 7
#define COLS 14
#define MAX (ROWS * COLS)
#define MAX_ENEMIES 3
#define TOTAL_LEVELS 3

char maze[ROWS][COLS + 1];
int totalCoins = 0;

/* Different mazes for each level.
   P = Player, E = Enemy, $ = Coin, X = Exit, # = Wall */

const char *levelMaps[TOTAL_LEVELS][ROWS] = {
    /* LEVEL 1 - EASY: 1 enemy, 2 coins */
    {
        "##############",
        "#P..........E#",
        "#............#",
        "#.####.......#",
        "#............#",
        "#$....$....X.#",
        "##############"
    },

    /* LEVEL 2 - MEDIUM: 2 enemies, 3 coins */
    {
        "##############",
        "#P..........E#",
        "#............#",
        "#.###........#",
        "#......##E...#",
        "#$...$...$X..#",
        "##############"
    },

    /* LEVEL 3 - HARD: 3 enemies, 4 coins */
    {
        "##############",
        "#P..........E#",
        "#............#",
        "#.####.......#",
        "#..E......E..#",
        "#$..$..$..$X.#",
        "##############"
    }
};

struct Player {
    int x, y;
    int score;
    int coins;
};

struct Enemy {
    int x, y;
};

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

/* Check whether a position is inside the maze and not a wall */
int walkable(int x, int y) {
    return x >= 0 && x < ROWS &&
           y >= 0 && y < COLS &&
           maze[x][y] != '#';
}

/* Load the selected level and find the player, enemies and coins */
void loadLevel(int level, struct Player *p,
               struct Enemy enemies[], int *enemyCount) {
    *enemyCount = 0;
    totalCoins = 0;
    p->coins = 0;

    for (int i = 0; i < ROWS; i++) {
        strcpy(maze[i], levelMaps[level - 1][i]);
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            if (maze[i][j] == 'P') {
                p->x = i;
                p->y = j;
                maze[i][j] = '.';
            }
            else if (maze[i][j] == 'E') {
                if (*enemyCount < MAX_ENEMIES) {
                    enemies[*enemyCount].x = i;
                    enemies[*enemyCount].y = j;
                    (*enemyCount)++;
                }

                /* Enemy position is stored separately */
                maze[i][j] = '.';
            }
            else if (maze[i][j] == '$') {
                totalCoins++;
            }
        }
    }
}

/* Display the maze, player, enemies and score */
void printMaze(struct Player p, struct Enemy enemies[],
               int enemyCount, int level) {
    #ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("           MAZE CHASE\n");

    switch (level) {
        case 1:
            printf("Level: 1 - EASY\n");
            break;
        case 2:
            printf("Level: 2 - MEDIUM\n");
            break;
        case 3:
            printf("Level: 3 - HARD\n");
            break;
        default:
            printf("Invalid level\n");
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int displayed = 0;

            if (i == p.x && j == p.y) {
                printf("P");
                displayed = 1;
            }
            else {
                for (int k = 0; k < enemyCount; k++) {
                    if (i == enemies[k].x &&
                        j == enemies[k].y) {
                        printf("E");
                        displayed = 1;
                        break;
                    }
                }
            }

            if (!displayed) {
                printf("%c", maze[i][j]);
            }
        }
        printf("\n");
    }

    printf("\nScore: %d", p.score);
    printf(" | Coins: %d/%d\n", p.coins, totalCoins);
    printf("Enemies: %d\n", enemyCount);

    printf("\nControls:\n");
    printf("W = Up       S = Down\n");
    printf("A = Left     D = Right\n");
    printf("Q = Quit Game\n");
}

/* Check whether all coins have been collected */
int allCoinsCollected(struct Player p) {
    return p.coins == totalCoins;
}

/* Check whether an enemy is at the player's position */
int checkCollision(struct Player p,
                   struct Enemy enemies[], int enemyCount) {
    for (int i = 0; i < enemyCount; i++) {
        if (p.x == enemies[i].x &&
            p.y == enemies[i].y) {
            return 1;
        }
    }

    return 0;
}

/* BFS: Move one enemy along the shortest path to the player */
void moveEnemy(struct Enemy *e, struct Player p) {
    int visited[ROWS][COLS] = {0};
    int parentX[ROWS][COLS];
    int parentY[ROWS][COLS];

    int queueX[MAX];
    int queueY[MAX];

    int front = 0;
    int rear = 0;

    /* Initialize parent arrays */
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            parentX[i][j] = -1;
            parentY[i][j] = -1;
        }
    }

    /* Add the enemy's starting position to the queue */
    queueX[rear] = e->x;
    queueY[rear] = e->y;
    rear++;

    visited[e->x][e->y] = 1;

    /* Explore the maze using BFS */
    while (front < rear) {
        int x = queueX[front];
        int y = queueY[front];
        front++;

        if (x == p.x && y == p.y) {
            break;
        }

        /* Check up, down, left and right */
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (walkable(nx, ny) && !visited[nx][ny]) {
                visited[nx][ny] = 1;

                parentX[nx][ny] = x;
                parentY[nx][ny] = y;

                queueX[rear] = nx;
                queueY[rear] = ny;
                rear++;
            }
        }
    }

    /* No path to the player */
    if (!visited[p.x][p.y]) {
        return;
    }

    /* Trace the path backwards from the player */
    int x = p.x;
    int y = p.y;

    while (parentX[x][y] != e->x ||
           parentY[x][y] != e->y) {

        int px = parentX[x][y];
        int py = parentY[x][y];

        if (px == -1 || py == -1) {
            return;
        }

        x = px;
        y = py;
    }

    /* Move the enemy one step towards the player */
    e->x = x;
    e->y = y;
}

/* Display the level selection menu using switch-case */
int selectLevel(void) {
    int choice;

    printf("        MAZE CHASE GAME\n");

    printf("\nSelect difficulty:\n");
    printf("1. Easy   - 1 enemy, 2 coins\n");
    printf("2. Medium - 2 enemies, 3 coins\n");
    printf("3. Hard   - 3 enemies, 4 coins\n");

    printf("\nEnter your choice: ");

    if (scanf("%d", &choice) != 1) {
        return 0;
    }

    switch (choice) {
        case 1:
            return 1;

        case 2:
            return 2;

        case 3:
            return 3;

        default:
            printf("\nInvalid choice! Please select 1, 2 or 3.\n");
            return 0;
    }
}

int main(void) {
    
    struct Player p = {0, 0, 100, 0};
    struct Enemy enemies[MAX_ENEMIES];

    int enemyCount;
    int startingLevel;
    int quitGame = 0;

    char move;

    startingLevel = selectLevel();

    if (startingLevel == 0) {
        printf("\nGame closed. Run again and select a valid level.\n");
        return 0;
    }

    /* Start at the selected level and automatically progress */
    for (int level = startingLevel;
         level <= TOTAL_LEVELS && !quitGame;
         level++) {

        int won = 0;
        int gameOver = 0;

        loadLevel(level, &p, enemies, &enemyCount);

        printf("\nStarting Level %d...\n", level);
        printf("Collect all coins and reach X to win!\n");
        printf("Press Enter to start...");
        getchar(); /* Consume the leftover newline */
        getchar();

        while (!won && !gameOver && !quitGame) {
            printMaze(p, enemies, enemyCount, level);

            printf("\nYour move: ");

            if (scanf(" %c", &move) != 1) {
                quitGame = 1;
                break;
            }

            if (move == 'q' || move == 'Q') {
                quitGame = 1;
                printf("\nYou quit the game.\n");
                break;
            }

            int nx = p.x;
            int ny = p.y;

            /* Move the player */
            switch (move) {
                case 'w':
                case 'W':
                    nx--;
                    break;

                case 's':
                case 'S':
                    nx++;
                    break;

                case 'a':
                case 'A':
                    ny--;
                    break;

                case 'd':
                case 'D':
                    ny++;
                    break;

                default:
                    printf("Invalid key! Use W, A, S or D.\n");
                    continue;
            }

            /* Prevent the player from walking through walls */
            if (!walkable(nx, ny)) {
                printf("Wall ahead! Choose another direction.\n");
                continue;
            }

            /* Require all coins before entering the exit */
            if (maze[nx][ny] == 'X' &&
                !allCoinsCollected(p)) {
                printf("Collect all coins before reaching the exit!\n");
                continue;
            }

            /* Update the player's position */
            p.x = nx;
            p.y = ny;

            /* Reduce score for each valid move */
            if (p.score > 0) {
                p.score--;
            }

            /* Collect a coin */
            if (maze[p.x][p.y] == '$') {
                p.coins++;
                p.score += 20;

                maze[p.x][p.y] = '.';

                printf("\nCoin collected! +20 points\n");
            }

            /* Check if the player reaches the exit */
            if (maze[p.x][p.y] == 'X' &&
                allCoinsCollected(p)) {
                p.score += 50;
                won = 1;

                printMaze(p, enemies, enemyCount, level);

                printf("             YOU WIN!\n");
                printf("All coins collected!\n");
                printf("Exit reached!\n");
                printf("Winning bonus: +50 points\n");
                printf("Current total score: %d\n", p.score);

                break;
            }

            /* Check collision before moving enemies */
            if (checkCollision(p, enemies, enemyCount)) {
                gameOver = 1;
                break;
            }

            /* Every enemy chases the player using BFS */
            for (int i = 0; i < enemyCount; i++) {
                moveEnemy(&enemies[i], p);

                if (checkCollision(p, enemies, enemyCount)) {
                    gameOver = 1;
                    break;
                }
            }

            if (gameOver) {
                printMaze(p, enemies, enemyCount, level);

                printf("            GAME OVER!\n");
                printf("An enemy caught you!\n");
                printf("Coins collected: %d/%d\n",
                       p.coins, totalCoins);
                printf("Final score: %d\n", p.score);
            }
        }

        if (quitGame || gameOver) {
            break;
        }

        /* Ask whether to quit before the next level */
        if (won && level < TOTAL_LEVELS) {
    char nextChoice[20];

    printf("\nLevel %d completed!\n", level);
    printf("Press Enter to start Level %d automatically,\n",
           level + 1);
    printf("or enter Q to quit: ");

    /* Clear leftover characters from the previous input */
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Clear input buffer */
    }

    /* An empty line continues to the next level */
    if (fgets(nextChoice, sizeof(nextChoice), stdin) == NULL) {
        quitGame = 1;
        break;
    }

    if (nextChoice[0] == 'Q' || nextChoice[0] == 'q') {
        quitGame = 1;
        printf("\nYou chose to quit.\n");
        break;
    }

    /* Otherwise, the for-loop starts the next level */
}
else if (won && level == TOTAL_LEVELS) {
    printf("\nCongratulations! You completed all levels!\n");
    printf("Final score: %d\n", p.score);
}
       

    printf("\nThanks for playing Maze Chase!\n");

    return 0;
}
}

