#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <wchar.h>
#include <locale.h>

#define MAX_USERS 30
#define GRID_HEIGHT 24
#define GRID_WIDTH 80

typedef struct{
    int x,y;
}Position;

typedef struct{
    Position position;
    char username[100];
    int level;
    int color;
    int difficulty;
    int music;
    int gold;
    int health_point;
    int food;
    int sum_of_gold;
    int number_of_games;
    int number_of_logins;
    int score;
    int has_sword;
    int has_arrow;
    int has_dagger;
    int has_wand;
    int has_mace;
}Player;

typedef struct{
    Position position;
}Pillar;

typedef struct{
    Position position;
}Door;

typedef struct{
    Position position;
}Trap;

typedef struct{
    Position position;
}Gold;

typedef struct{
    Position position;
}Blackgold;

typedef struct{
    Position position;
    int number;
}Arrow;

typedef struct{
    Position position;
    int number;
}Sword;

typedef struct{
    Position position;
    int number;
}Wand;

typedef struct{
    Position position;
    int number;
}Dagger;

typedef struct{
    Position position;
    int kind;
}Food;

typedef struct{
    Position position;
}Stairs;

typedef struct{
    Position position;
}Damage;

typedef struct{
    Position position;
}Speed;

typedef struct{
    Position position;
}Health;

typedef struct{
    Position position;
    int height,width;
    int number_of_doors;
    int isspell;
    Door doors[4];
} Room;

typedef struct{
    char username[100];
    char password[100];
    char email[100];
} User;

typedef struct{
    Position position;
    int dhealth;
    int steps;
}Deamon;

typedef struct{
    Position position;
    int dhealth;
    int steps;
}Fbmonster;

typedef struct{
    Position position;
    int dhealth;
    int steps;
}Giant;

typedef struct{
    Position position;
    int dhealth;
}Snake;

typedef struct{
    Position position;
    int dhealth;
    int steps;
}Undeed;

User user;
Player players[MAX_USERS];
int player_count = 0;
Door doors;
Pillar pillars[12];
Gold golds[24];
Arrow arrows[2000];
Sword swords[2000];
Wand wands[2000];
Dagger daggers[2000];
Blackgold blackgold[100];
Food foods[2000];
Damage damage[100];
Speed speed[100];
Health health[100];
Trap traps[12];
Room rooms[6];
Deamon deamons[12];
Giant giants[6];
Snake snakes[6];
Undeed undeeds[6];
Fbmonster fbmonsters[12];
Room treasure;
int visited[GRID_HEIGHT][GRID_WIDTH];
int randpass=687;
int passcheck=0;
int passworddoors[GRID_HEIGHT][GRID_WIDTH];
int count[GRID_HEIGHT][GRID_WIDTH];
int ever[GRID_HEIGHT][GRID_WIDTH];
int used=0;
Stairs stairs;
int isstair=0;
int arrow_number=0;
int food_collected=0;
int foodnumber=0;
int flag2=0;
int damagenumber=0;
int speednumber=0;
int healthnumber=0;
int spellsteps=0;
int is_guest=0;
int visit_count[6];
int spellindex[6];
int is_reversed[GRID_HEIGHT][GRID_WIDTH];
int isspell[GRID_HEIGHT][GRID_WIDTH];
int isdead=0;
int isinroom=-1;
int visit[GRID_HEIGHT][GRID_WIDTH];
int selectedweapon=-1;
int fcount=0;
int dcount=0;
int gcount=0;
int scount=0;
int ucount=0;
int gsteps=0;
int usteps=0;
int totalsteps=0;
int healthtime=0;
int speedtime=0;
int damagetime=0;
int healthactivated=1;
int speedactivated=1;
int damageactivated=1;

void create_room(Room *rooms, Player *player);
void draw_rooms(Room *rooms, char grid[GRID_HEIGHT][GRID_WIDTH], Player *player);
int pre_menu(Player *player);
int game_menu(int is_guest, Player *player);
void settings_menu(Player *player);
void music_menu();
int handle_input(int input, Player *player,char grid[GRID_HEIGHT][GRID_WIDTH], int *steps, int [GRID_HEIGHT][GRID_WIDTH]);
void player_setup(Player *player, char grid[GRID_HEIGHT][GRID_WIDTH],int visit[GRID_HEIGHT][GRID_WIDTH]);
void play_music(int music_id);
void draw_menu_border();
void create_new_account(User*);
int check_add_account(char*, char*, char*, char*, char*);
void load_player_data(const char *filename, Player players[], int *player_count) ;
int compare_players(const void *a, const void *b);
void display_leaderboard(const Player players[], int player_count, const char *current_user);
void fill_player_data(char *filename, char *username, Player *player);
int login_to_account(Player *player);
int validate_password(char*);
int validate_email(char*);
int validate_username(char*);
int player_move(int y, int x, Player *player,char[GRID_HEIGHT][GRID_WIDTH], int *steps);
int check_position(int y, int x, Player *player, char[GRID_HEIGHT][GRID_WIDTH], int *steps, int [GRID_HEIGHT][GRID_WIDTH]);
void generate_corridors(Room *rooms);
void init_grid(char[GRID_HEIGHT][GRID_WIDTH], int[GRID_HEIGHT][GRID_WIDTH]);
int is_valid_position(int y, int x);
void print_grid(char[GRID_HEIGHT][GRID_WIDTH], int[GRID_HEIGHT][GRID_WIDTH], Player *player);
void score(Player *player);
void print_grid2(char grid[GRID_HEIGHT][GRID_WIDTH], int visit[GRID_HEIGHT][GRID_WIDTH], Player *player);
void foodmenu(Player *player);
void spellmenu();
void weaponmenu(Player *player);
void draw_treasure(char grid[GRID_HEIGHT][GRID_WIDTH]);
void create_treasure(Player *player);
void save_quit(Player *player);
void final_menu();

char *coin="⛀";
char *blackcoin="⛃";
char *playerr= "☭";
char *arroww="➳";
char *daggerr="🗡";
char *swordd="⚔";
char *wandd="⚚";
char *foodd="⛾";
char *potionn="⚱";

int main() {
    setlocale(LC_ALL,"");
    cbreak();
    initscr();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    noecho();
    srand(time(NULL));


    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_CYAN, COLOR_BLACK);
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);
        init_pair(10, COLOR_YELLOW, COLOR_WHITE);
        init_color(11,691,609,847);
        init_pair(11,11, COLOR_BLACK);
        init_color(12,992,539,93);
        init_pair(12,12, COLOR_BLACK);
        init_color(13,972,937,93);
        init_pair(13,13,COLOR_BLACK);
        init_color(14,542,0,0);
        init_pair(14,14,COLOR_BLACK);
        init_color(15,726,554,136);
        init_pair(15,15,COLOR_BLACK);
        init_color(16,585,293,0);
        init_pair(16,16,COLOR_BLACK);
        init_color(17,562,929,562);
        init_pair(17,17,COLOR_BLACK); 
        init_color(18,500,0,500);
        init_pair(18,18,COLOR_BLACK);        
    }
    else{
        mvprintw(LINES/2, COLS/2-14, "Sorry! Your system does not support colors");
        return 0;
    }
    char grid[GRID_HEIGHT][GRID_WIDTH];
    init_grid(grid,visit);
    Player player;
    int ch;
    player.health_point=100;
    player.food=100;
    player.difficulty=1;
    player.color=3;
    player.level=1;
    player.score=0;
    player.has_arrow=0;
    player.has_dagger=0;
    player.has_wand=0;
    player.has_sword=0;
    player.has_mace=1;
    int steps=0;
    pre_menu(&player);
    create_room(rooms, &player);
    fcount=0;
    draw_rooms(rooms,grid, &player);
    player_setup(&player, grid, visit);
    print_grid(grid,visit,&player);
    player.score=0;
    player.gold=0;
    score(&player);
    while((ch = getch()) != 'q' && isdead==0){
        if(handle_input(ch,&player,grid, &steps, visit)==1)
            break;
        refresh();
    }
    // if(ch=='q' || ch=='Q'){
    //     endwin();
    //     return 0;
    // }
    clear();
    player.level+=1;
    isstair=0;
    init_grid(grid,visit);
    create_room(rooms, &player);
    fcount=0;
    draw_rooms(rooms,grid, &player);
    player_setup(&player, grid, visit);
    print_grid(grid,visit,&player);
    attron(COLOR_PAIR(4));
    mvprintw(0,0,"Welcome to the Second Floor!                                                          ");
    attroff(COLOR_PAIR(4));
    score(&player);
    while((ch = getch()) != 'q' && isdead==0){
        if(handle_input(ch,&player,grid, &steps, visit)==1)
            break;
        refresh();
    }
    // if(ch=='q' || ch=='Q'){
    //     endwin();
    //     return 0;
    // }
    clear();
    player.level+=1;
    isstair=0;
    init_grid(grid,visit);
    create_room(rooms, &player);
    fcount=0;
    draw_rooms(rooms,grid, &player);
    player_setup(&player, grid, visit);
    print_grid(grid,visit,&player);
    attron(COLOR_PAIR(4));
    mvprintw(0,0,"Welcome to the Third Floor!                                                          ");
    attroff(COLOR_PAIR(4));
    score(&player);
    while((ch = getch()) != 'q' && isdead==0){
        if(handle_input(ch,&player,grid, &steps, visit)==1)
            break;
        refresh();
    }
    // if(ch=='q' || ch=='Q'){
    //     endwin();
    //     return 0;
    // }
    clear();
    player.level+=1;
    isstair=0;
    init_grid(grid,visit);
    create_room(rooms, &player);
    fcount=0;
    draw_rooms(rooms,grid, &player);
    player_setup(&player, grid, visit);
    print_grid(grid,visit,&player);
    score(&player);
    attron(COLOR_PAIR(4));
    mvprintw(0,0,"Welcome to the Last Floor!                                                          ");
    attroff(COLOR_PAIR(4));
    while((ch = getch()) != 'q' && isdead==0){
        if(handle_input(ch,&player,grid, &steps, visit)==1)
            break;
        refresh();
    }
    // if(ch=='q' || ch=='Q'){
    //     endwin();
    //     return 0;
    // }
    init_grid(grid,visit);
    clear();
    create_treasure(&player);
    draw_treasure(grid);
    player.level+=1;
    player_setup(&player, grid, visit);
    print_grid(grid,visit,&player);
    score(&player);
    attron(COLOR_PAIR(12));
    mvprintw(0,0,"Treasure Room!                                                          ");
    attroff(COLOR_PAIR(12));
    while((ch = getch()) != 'q' && isdead==0){
        if(handle_input(ch,&player,grid, &steps, visit)==1)
            break;
        refresh();
    }
    // if(ch=='q' || ch=='Q'){
    //     endwin();
    //     return 0;
    // }
    if(is_guest==0)
        save_quit(&player);
    final_menu();
    endwin();
    return 0;
}

void init_grid(char grid[GRID_HEIGHT][GRID_WIDTH],int visit[GRID_HEIGHT][GRID_WIDTH]) {
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            grid[i][j] = ' ';
            visit[i][j]=0;
            passworddoors[i][j]=0;
            count[i][j]=0;
            ever[i][j]=0;
            isspell[i][j]=0;
            is_reversed[i][j]=rand() % 10;
        }
    }for(int i=0; i<6; i++){
        visit_count[i]=0;
        rooms[i].isspell=0;
        golds[i].position.x=0;
        golds[i+6].position.x=0;
        golds[i+12].position.x=0;
        blackgold[i].position.x=0;
        pillars[i].position.x=0;
        pillars[i+6].position.x=0; 
        traps[i].position.x=0;
        traps[i+6].position.x=0;   
        wands[i].position.x=0;   
        daggers[i].position.x=0;
        swords[i].position.x=0;
        arrows[i].position.x=0;
        foods[i].position.x=0;   
        spellindex[i]=-1;  
        health[i].position.x=0;
        health[i+6].position.x=0;
        health[i+12].position.x=0;
        health[i+18].position.x=0;
        damage[i].position.x=0;
        damage[i+6].position.x=0;
        damage[i+12].position.x=0;
        damage[i+18].position.x=0;
        speed[i].position.x=0;
        speed[i+6].position.x=0;
        speed[i+12].position.x=0;
        speed[i+18].position.x=0;
        deamons[i].dhealth=5;
        deamons[i+6].dhealth=5;
        deamons[i].steps=0;
        deamons[i+6].steps=0;
        deamons[i].position.x=0;
        deamons[i+6].position.x=0;
        fbmonsters[i].dhealth=10;
        fbmonsters[i+6].dhealth=10;
        fbmonsters[i].steps=0;
        fbmonsters[i+6].steps=0;
        fbmonsters[i].position.x=0;
        fbmonsters[i+6].position.x=0;  
        fcount=0;
        giants[i].steps=0;
        giants[i].position.x=0;
        giants[i].dhealth=15;
        gcount=0; 
        snakes[i].position.x=0;
        snakes[i].dhealth=20;
        scount=0; 
        undeeds[i].steps=0;
        undeeds[i].position.x=0;
        undeeds[i].dhealth=30;
        ucount=0;      
    }isinroom=-1;
    gsteps=0;
    usteps=0;
}

void score(Player *player){
    attroff(COLOR_PAIR);
    mvprintw((GRID_HEIGHT-1),1,"Level: %d", player->level);
    mvprintw((GRID_HEIGHT-1),16,"Health: %d", player->health_point);
    mvprintw((GRID_HEIGHT-1),31,"Gold: %d     ", player->gold);
    mvprintw((GRID_HEIGHT-1),46,"Food: %d", player->food);
    mvprintw((GRID_HEIGHT-1),61,"Score: %d       ", player->score);
}

void final_menu(){
    clear();
    draw_menu_border();
    attron(COLOR_PAIR(12));
    if(isdead==0){
        mvprintw(LINES/2-4, COLS/2 - 29, "Congratulations to all free workers of the world! You Won!");
    }else   
        mvprintw(LINES/2-4, COLS/2 - 35, "Sorry you lost! But Never is late for a free worker to build a Utopia");
    refresh();
    sleep(3);
    mvprintw(LINES/2, COLS/2- 23 ,"Thank you for playing! press any key to quit!");
    int ch;
    ch = getch();
    return;
}

void save_quit(Player *player) {
    FILE *file = fopen("players.txt", "r+");
    if (!file) return;

    char line[768];
    int i = 0;
    int line_number = 0;
    long pos_to_update = -1;
    

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if ((line_number % 6) == 1) {
            line[strcspn(line, "\n")] = '\0';
            if (strcmp(line, player->username) == 0) {
                fprintf(file, "%d\n", player->gold);
                pos_to_update = ftell(file);
                fscanf(file, "%d", &player->sum_of_gold);
                player->sum_of_gold+=player->gold;
                fseek(file, pos_to_update, SEEK_SET);
                fprintf(file, "%d\n", player->sum_of_gold);
                pos_to_update = ftell(file);
                fscanf(file, "%d", &player->number_of_games);
                player->number_of_games++;
                fseek(file, pos_to_update, SEEK_SET);
                fprintf(file, "%d\n", player->number_of_games);
                pos_to_update = ftell(file);
                fscanf(file, "%d", &player->number_of_logins);
                fseek(file, pos_to_update, SEEK_SET);
                fprintf(file, "%d\n", player->number_of_logins);
                pos_to_update = ftell(file);
                int num=player->score;
                fscanf(file, "%d", &player->score);
                player->score+=num;
                fseek(file, pos_to_update, SEEK_SET);
                fprintf(file, "%d", player->score);
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}



int pre_menu(Player *player) {
    char* options[] = {"Create New Account", "Login to Account", "Enter as Guest"};
    int choice = 0;
    clear();
    draw_menu_border();
    int exit_check = 0;
    while (!exit_check) { 
        attron(COLOR_PAIR(4));
        for (int i = 0; i < 3; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(LINES / 2 + i - 1, COLS / 2 - 7, "%s", options[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == '\n') {
            switch (choice) {
                case 0:
                    create_new_account(&user);
                    break;
                case 1:
                    if (login_to_account(player)) {
                        is_guest=0; 
                        if(game_menu(is_guest,player) == 1){
                            clear();
                            draw_menu_border();
                            continue;
                        }else{
                            exit_check=1;
                            break;
                        }                         
                    } else {
                        clear();
                        draw_menu_border();
                        mvprintw(LINES / 2 + 2, COLS / 2 - 15, "Login failed! Please try again.");
                        refresh();
                        sleep(2); 
                    }
                    break;
                case 2:
                    mvprintw(LINES / 2 + 2, COLS / 2 - 18, "Guest mode: Progress will not be saved!");
                    refresh();
                    sleep(2); 
                    is_guest=1;
                    if (game_menu(is_guest,player)==1){
                        clear();
                        draw_menu_border();
                        continue;
                    }else{
                        exit_check=1;
                        break;
                    }
            }
            clear();
            draw_menu_border();
        }
    }
    attroff(COLOR_PAIR(4));
    return 0;
}

int game_menu(int is_guest, Player *player) {
    char* options[] = {"Load Game", "New Game", "Settings", "Score Board","Back to Main Menu"};
    int choice = 0;
    noecho();
    clear();
    draw_menu_border();
    int return_to_main_menu=0;
    int exit_check = 0;
    while (!exit_check) { 
        attron(COLOR_PAIR(4));
        for (int i = 0; i < 5; ++i) {
            if (is_guest && i == 0) continue; 
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(LINES / 2 + i - 3, COLS / 2 - 4, "%s", options[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 4 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 4) ? 0 : choice + 1;
        else if (ch == '\n') {
            switch (choice) {
                case 0:
                    if(!is_guest) {
                        exit_check = 1;
                    }
                    break;
                case 1:
                    return 0;
                case 2:
                    settings_menu(player);
                    break;
                case 3:
                    const char *filename = "players.txt";
                    const char *current_user = player->username; 
                    load_player_data(filename, players, &player_count);
                    qsort(players, player_count, sizeof(Player), compare_players);
                    display_leaderboard(players, player_count, current_user);
                    break;
                case 4:
                    return 1;
                    break;
            }
            clear();
            draw_menu_border();
        }
        if(return_to_main_menu){

        }
    }
    attroff(COLOR_PAIR(4));
    return 0;
}

void settings_menu(Player *player) {
    char* options[] = {"Change Player Color", "Change Game Level", "Change Player Character","Music Adjustment","Back to Previous Menu"};
    int choice = 0;
    int player_color = 0;
    int game_level = 1;   
    int character=1;
    clear();
    draw_menu_border();

    while (1) { 
        for (int i = 0; i < 5; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            mvprintw(LINES / 2 + i - 1, COLS / 2 - 8, "%s", options[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 4 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 4) ? 0 : choice + 1;
        else if (ch == '\n') {
            if (choice == 0) { 
                char* colors[] = {"Green", "Blue", "Red"};
                int color_choice = player_color; 
                while (1) {
                    clear();
                    draw_menu_border();
                    attron(COLOR_PAIR(4));
                    mvprintw(LINES / 2 - 6, COLS / 2 - 8, "Press b to go back");
                    mvprintw(LINES / 2 - 2, COLS / 2 - 9, "Select Player Color:");
                    for (int i = 0; i < 3; ++i) {
                        attron(COLOR_PAIR(1+i));
                        if (i == color_choice)
                            attron(A_REVERSE);
                        mvprintw(LINES / 2 + i - 1, COLS / 2 - 2, "%s", colors[i]);
                        if (i == color_choice)
                            attroff(A_REVERSE);
                        attroff(COLOR_PAIR(1+i));
                    }

                    int color_ch = getch();
                    if (color_ch == KEY_UP)
                        color_choice = (color_choice == 0) ? 2 : color_choice - 1;
                    else if (color_ch == KEY_DOWN)
                        color_choice = (color_choice == 2) ? 0 : color_choice + 1;
                    else if (color_ch == '\n') {
                        player->color = color_choice+1;
                        attron(COLOR_PAIR(4));
                        mvprintw(LINES / 2 + 3, COLS / 2 - 12, "Player color changed to %s!", colors[player->color-1]);
                        refresh();
                        sleep(2);
                        break;
                    } else if (color_ch == 'b') {
                        break; 
                    }
                }
            } else if (choice == 1) { 
                char* levels[] = {"Easy", "Medium", "Hard"};
                int level_choice = game_level; 
                while (1) {
                    clear();
                    draw_menu_border();
                    attron(COLOR_PAIR(4));
                    mvprintw(LINES / 2 - 6, COLS / 2 - 9, "Press b to go back");
                    mvprintw(LINES / 2 - 2, COLS / 2 - 9, "Select Game Level:");
                    for (int i = 0; i < 3; ++i) {
                        if (i == level_choice)
                            attron(A_REVERSE);
                        mvprintw(LINES / 2 + i - 1, COLS / 2 - 2, "%s", levels[i]);
                        if (i == level_choice)
                            attroff(A_REVERSE);
                    }

                    int level_ch = getch();
                    if (level_ch == KEY_UP)
                        level_choice = (level_choice == 0) ? 2 : level_choice - 1;
                    else if (level_ch == KEY_DOWN)
                        level_choice = (level_choice == 2) ? 0 : level_choice + 1;
                    else if (level_ch == '\n') {
                        player->difficulty = level_choice;
                        attron(COLOR_PAIR(4));
                        mvprintw(LINES / 2 + 3, COLS / 2 - 12, "Game level changed to %s!", levels[player->difficulty]);
                        refresh();
                        sleep(2);
                        break;
                    } else if (level_ch == 'b') {
                        break; 
                    }
                }
            } 
            else if(choice==2){
                char* characters[] = {"Best Thing Ever", "King", "Pirate"};
                int level_choice = character; 
                while (1) {
                    clear();
                    draw_menu_border();
                    attron(COLOR_PAIR(4));
                    mvprintw(LINES / 2 - 6, COLS / 2 - 9, "Press b to go back");
                    mvprintw(LINES / 2 - 2, COLS / 2 - 9, "Select Game Level:");
                    for (int i = 0; i < 3; ++i) {
                        if (i == level_choice)
                            attron(A_REVERSE);
                        mvprintw(LINES / 2 + i - 1, COLS / 2 - 8, "%s", characters[i]);
                        if (i == level_choice)
                            attroff(A_REVERSE);
                    }

                    int level_ch = getch();
                    if (level_ch == KEY_UP)
                        level_choice = (level_choice == 0) ? 2 : level_choice - 1;
                    else if (level_ch == KEY_DOWN)
                        level_choice = (level_choice == 2) ? 0 : level_choice + 1;
                    else if (level_ch == '\n') {
                        switch (character){
                            case 1:
                                playerr= "☭";
                                break;
                            case 2:
                                playerr= "♚";
                                break;
                            case 3:
                                playerr= "🕲";
                                break;
                                
                        }
                        attron(COLOR_PAIR(4));
                        mvprintw(LINES / 2 + 3, COLS / 2 - 24, "Player character changed to %s!", characters[level_choice]);
                        refresh();
                        sleep(2);
                        break;
                    } else if (level_ch == 'b') {
                        break; 
                    }
                }
            }
            else if (choice == 3) { 
                music_menu();
                break;
            }else if (choice == 4) { 
                return;
            }
        } 
        clear();
        draw_menu_border();
        attroff(COLOR_PAIR(4));
    }
}

void foodmenu(Player *player){
    clear();
    char *options[]={"Eat Food", "Back to the game"};
    int choice=0;
    attron(COLOR_PAIR(4));
    mvprintw(LINES/2-4, COLS/2-9, "Your have %d food packs", foodnumber);
    mvprintw(LINES/2-6,COLS/2-9,"Hunger Bar:  ");
    for (int i=0; i<((player->food)/10); i++){
        attron(COLOR_PAIR(10));
        mvprintw(LINES/2-6,COLS/2+4+i,"#");

    }for(int i=10; i>((player->food)/10); i--){
        attron(COLOR_PAIR(10));
        mvprintw(LINES/2-6,COLS/2+i+3," ");
    }
    attroff(COLOR_PAIR(10));
    while (1) {
        for (int i = 0; i < 2; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            mvprintw(LINES / 2 + i - 2, COLS / 2 - 5, "%s", options[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 1 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 1) ? 0 : choice + 1;
        else if (ch == '\n'){
            if (choice==0){
                if(foodnumber>0){
                    foodnumber-=1;
                    if((player->food+=(4-player->difficulty)*4)<=100)
                        player->food+=(4-player->difficulty)*4;
                    else    
                        player->food=100;
                    if((player->health_point+(4-player->difficulty)*4)<=100)
                        player -> health_point += (4-player->difficulty)*4;
                    else
                        player->health_point=100;
                    mvprintw(LINES/2, COLS/2-12, "You used one of your food packs!");
                    refresh();
                    sleep(2);
                }else{
                    mvprintw(LINES/2, COLS/2-9, "Sorry! You're out of food!");
                    refresh();
                    sleep(2);
                }break;
            }else if(choice==1){
                break;
            }
        }else if (ch == 'e' || ch=='E') {
                        break; 
                    }
    }attroff(COLOR_PAIR(4));
}

void spellmenu(){
    clear();
    char *options[]={"use", "use", "use", "Back to the game"};
    int choice=0;
    attron(COLOR_PAIR(4));
    mvprintw(LINES/2-3, COLS/2 - 9, "Your Spells' Count:");
    attron(COLOR_PAIR(3));
    mvprintw(LINES/2-1, COLS/2- 8, "Damage Spell: %d", damagenumber);
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(12));
    mvprintw(LINES/2, COLS/2- 8, "Speed Spell: %d", speednumber);
    attroff(COLOR_PAIR(12));
    attron(COLOR_PAIR(1));
    mvprintw(LINES/2+1, COLS/2- 8, "Health Spell: %d", healthnumber);
    attroff(COLOR_PAIR(1));
    while (1) {
        for (int i = 0; i < 4; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            if(i<=2)
                mvprintw(LINES / 2 + i - 1, COLS / 2 + 10, "%s", options[i]);
            else    
                mvprintw(LINES / 2 + i - 1, COLS / 2 - 8, "%s", options[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 3 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 3) ? 0 : choice + 1;
        else if (ch == '\n'){
            if(choice==0){
                if(damagenumber==0){
                    mvprintw(LINES/2+3, COLS/2 - 17, "Sorry! You're out of damage spells!");
                    refresh();
                    sleep(2);
                }else{
                    damagenumber-=1;
                    damagetime=5;
                    damageactivated=2;
                    mvprintw(LINES/2+3, COLS/2 - 12, "Damage Spell is activated");
                    refresh();
                    sleep(2);
                    break;
                }
            }else if(choice==1){
                if(speednumber==0){
                    mvprintw(LINES/2+3, COLS/2 - 17, "Sorry! You're out of speed spells!");
                    refresh();
                    sleep(2);
                }else{
                    speednumber-=1;
                    speedtime=20;
                    speedactivated=2;
                    mvprintw(LINES/2+3, COLS/2 - 12, "Speed Spell is activated");
                    refresh();
                    sleep(2);
                    break;
                }
            }else if(choice==2){
                if(healthnumber==0){
                    mvprintw(LINES/2+3, COLS/2 - 17, "Sorry! You're out of health spells!");
                    refresh();
                    sleep(2);
                }else{
                    healthnumber-=1;
                    healthtime=50;
                    healthactivated=2;
                    mvprintw(LINES/2+3, COLS/2 - 12, "Health Spell is activated");
                    refresh();
                    sleep(2);
                    break;
                }
            }else if(choice==3)
                break;
        }else if (ch == 'r' || ch=='R') {
                        break; 
                    }
    }attroff(COLOR_PAIR(4));
}

void weaponmenu(Player *player){
    clear();
    char *options[]={"Mace", "Sword", "Arrow","Dagger","Magic Wand"};
    int choice=0;
    attron(COLOR_PAIR(4));
    mvprintw(0,0,"To select weapons:     g=magicwand    d=dagger    a=arrow     s=sword     m=mace");
    mvprintw(LINES/2-5, COLS/2 - 7, "Weapon's Backpack");
    mvprintw(LINES/2-3, COLS/2-18,"Weapon Type     Number     Damage     Range");
    mvprintw(LINES/2-2, 0,"Short-Range Weapons:");
    mvprintw(LINES/2, 0,"Long-Range Weapons:");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(12));
    mvprintw(LINES/2-2, COLS/2 -2,"1");
    mvprintw(LINES/2-2, COLS/2 +9,"5");
    mvprintw(LINES/2-2, COLS/2 +20,"0");
    mvprintw(LINES/2-1, COLS/2 -2,"%d", player->has_sword);
    mvprintw(LINES/2-1, COLS/2 +9,"10");
    mvprintw(LINES/2-1, COLS/2 +20,"0");
    mvprintw(LINES/2, COLS/2 -2,"%d", player->has_arrow);
    mvprintw(LINES/2, COLS/2 +9,"5");
    mvprintw(LINES/2, COLS/2 +20,"5");
    mvprintw(LINES/2+1, COLS/2 -2,"%d", player->has_dagger);
    mvprintw(LINES/2+1, COLS/2 +9,"12");
    mvprintw(LINES/2+1, COLS/2 +20,"5");
    mvprintw(LINES/2+2, COLS/2 -2,"%d", player->has_wand);
    mvprintw(LINES/2+2, COLS/2 +9,"15");
    mvprintw(LINES/2+2, COLS/2 +20,"8");
    while (1) {
        mvprintw(LINES/2+4, COLS/2- 30, "                                                                  ");
        attron(COLOR_PAIR(4));
        for (int i = 0; i < 5; ++i) 
            mvprintw(LINES / 2 + i - 2, COLS / 2 - 18, "%s", options[i]);
        attroff(COLOR_PAIR(4));
        int ch=getch();
        if(ch=='w' || ch=='W'){
            selectedweapon=-1;
            mvprintw(LINES/2+4, COLS/2- 18, "Weapon was put into the packpack!");
            refresh();
            sleep(2);
        }else{
            if (ch == 'i' || ch=='I') {
                break; 
            }else if(ch!='g' && ch!='d' && ch!='a' && ch!='s' && ch!='m'){
                attron(COLOR_PAIR(4));
                mvprintw(LINES/2+4, COLS/2- 13, "Please Press A Valid Key!");
                refresh();
                sleep(2);
                attroff(COLOR_PAIR(4));
            }else if(selectedweapon!=-1){
                attron(COLOR_PAIR(4));
                mvprintw(LINES/2+4, COLS/2- 30, "Please put your current weapon in backpack first (press w)");
                refresh();
                sleep(2);
                attroff(COLOR_PAIR(4));
            }else{
                attron(COLOR_PAIR(4));
                if(ch=='m'){
                    selectedweapon=51;
                    attron(COLOR_PAIR(4));
                    mvprintw(LINES/2+4, COLS/2- 13, "Default weapon set to mace!");
                    attroff(COLOR_PAIR(4));
                    refresh();
                    sleep(2);
                    break;
                }else if(ch=='s'){
                    if((player->has_sword)>0){
                        selectedweapon=100;
                        mvprintw(LINES/2+4, COLS/2- 13, "Default weapon set to Sword!");
                        refresh();
                        sleep(2);
                        break;
                    }else{
                        mvprintw(LINES/2+4, COLS/2- 17, "You haven't collected any Swords yet!");
                        refresh();
                        sleep(2);
                    }
                }else if(ch=='a'){
                    if((player->has_arrow)>0){
                        selectedweapon=52;
                        mvprintw(LINES/2+4, COLS/2- 13, "Default weapon set to Arrows!");
                        refresh();
                        sleep(2);
                        break;
                    }else{
                        mvprintw(LINES/2+4, COLS/2- 17, "You haven't collected any Arrow yet!");
                        refresh();
                        sleep(2);
                    }
                }else if(ch=='d'){
                    if((player->has_dagger)>0){
                        selectedweapon=120;
                        mvprintw(LINES/2+4, COLS/2- 13, "Default weapon set to Daggers!");
                        refresh();
                        sleep(2);
                        break;
                    }else{
                        mvprintw(LINES/2+4, COLS/2- 17, "You haven't collected any Dagger yet!");
                        refresh();
                        sleep(2);
                    }
                }else if(ch=='g'){
                    if((player->has_wand)>0){
                        selectedweapon=150;
                        mvprintw(LINES/2+4, COLS/2- 15, "Default weapon set to Magic Wands!");
                        refresh();
                        sleep(2);
                        break;
                    }else{
                        mvprintw(LINES/2+4, COLS/2- 19, "You haven't collected any Magic Wand yet!");
                        refresh();
                        sleep(2);
                    }
                }attroff(COLOR_PAIR(4));
            }
        }
    }attroff(COLOR_PAIR(12));
}

void music_menu() {
    char* options[] = {"Play Music 1", "Play Music 2", "Play Music 3", "Play Music 4", "Disable Music", "Back"};
    int choice = 0;
    clear();
    draw_menu_border();
    while (1) {
        for (int i = 0; i < 6; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            mvprintw(LINES / 2 + i - 4, COLS / 2 - 6, "%s", options[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 5 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 5) ? 0 : choice + 1;
        else if (ch == '\n') {
            if (choice == 5) {
                break; 
            } else if (choice == 4) {
                //play_music(0); 
                break;
            } else {
                //play_music(choice + 1); 
                break;
            }
        }
    }attroff(COLOR_PAIR(4));
}

void draw_menu_border() {
    attron(COLOR_PAIR(4));
    clear();

    mvprintw(0, COLS / 2 - 9, "Welcome to Rogue!!");
    mvprintw(1, COLS / 2 - 16, "By Soheil Bolvardi - Winter 2025");

    for (int y = 0; y < LINES; ++y) {
        mvprintw(y, 0, "0");
        mvprintw(y, COLS - 1, "0");
    }
    attroff(COLOR_PAIR(4));
}

// void play_music(int music_id) {
//     if (SDL_Init(SDL_INIT_AUDIO) < 0) {
//         printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//         return;
//     }

//     Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0;

//     Mix_Music* music = NULL;

//     switch (music_id) {
//         case 1:
//             music = Mix_LoadMUS("music1.mp3");
//             break;
//         case 2:
//             music = Mix_LoadMUS("music2.mp3");
//             break;
//         case 3:
//             music = Mix_LoadMUS("music3.mp3");
//             break;
//         case 4:
//             music = Mix_LoadMUS("music4.mp3");
//             break;
//         default:
//             Mix_HaltMusic();
//             break;
//     }

//     if (music != NULL) {
//         Mix_PlayMusic(music, -1); 
//     }

//     Mix_FreeMusic(music);
//     Mix_CloseAudio();
// }

void create_new_account(User *user) {
    WINDOW *create_win = newwin(12, 60, 7, 10);
    box(create_win, 0, 0);
    mvprintw(LINES/2-6, COLS/2-18, "write \"back\" in any section to return");
    refresh();
    echo();
    attron(COLOR_PAIR(4));
    while (1) {
        mvwprintw(create_win, 1, 1, "Enter Username: ");
        wrefresh(create_win);
        wscanw(create_win, "%s", user->username);
        if(strcmp(user->username,"back")==0)
            return;
        if (validate_username(user->username)) break;
        mvwprintw(create_win, 2, 1, "Error: Username cannot be empty!          ");
    }

    while (1) {
        mvwprintw(create_win, 3, 1, "Enter Password: ");
        wrefresh(create_win);
        if(strcmp(user->password,"back")==0)
            return;
        wscanw(create_win, "%s", user->password);

        if (validate_password(user->password)) break;
        mvwprintw(create_win, 4, 1, "Password is not strong enough!");
    }

    while (1) {
        mvwprintw(create_win, 5, 1, "Enter Email: ");
        wrefresh(create_win);
        wscanw(create_win, "%s", user->email);
        if(strcmp(user->email,"back")==0)
            return;
        if (validate_email(user->email)) break;
        mvwprintw(create_win, 6, 1, "Error: Invalid email format!                  ");
    }

    noecho();

    int result = check_add_account("users.txt", "players.txt", user->username, user->password, user->email);
    if (result == 1) {
        mvwprintw(create_win, 8, 1, "Error: Username already exists!");
    } else if (result == 0) {
        mvwprintw(create_win, 8, 1, "Account created successfully!");
    } else {
        mvwprintw(create_win, 8, 1, "Error: Could not write to file.");
    }

    wrefresh(create_win);
    sleep(2); 
    delwin(create_win);
    attroff(COLOR_PAIR(4));
    clear();
}

int login_to_account(Player *player) {
    char username[100], password[100];
    char line[256];
    int found = 0;
    attron(COLOR_PAIR(4));
    WINDOW *login_win = newwin(8, 50, 7, 15);
    box(login_win, 0, 0);

    echo();

    mvwprintw(login_win, 1, 1, "Enter Username: ");
    wrefresh(login_win);
    wscanw(login_win, "%s", username);

    mvwprintw(login_win, 2, 1, "Enter Password: ");
    wrefresh(login_win);
    wscanw(login_win, "%s", password);

    FILE *file = fopen("users.txt", "r");
    if (!file) {
        mvwprintw(login_win, 4, 1, "Error: Could not open file.");
        wrefresh(login_win);
        sleep(2);
        delwin(login_win);
        clear();
        return 0; 
    }
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; 
        if (strcmp(line, username) == 0) {
            found = 1; 
            fgets(line, sizeof(line), file); 
            line[strcspn(line, "\n")] = '\0';
            if (strcmp(line, password) == 0) {
                mvwprintw(login_win, 4, 1, "Login successful!");
                strcpy(player->username,username);
                fill_player_data("players.txt", player->username, player);
                wrefresh(login_win);
                sleep(2); 
                fclose(file);
                delwin(login_win);
                return 1; 
            }
            else {
                mvwprintw(login_win, 4, 1, "Wrong password!");
            }
            break;
        }
    }

    if (!found) {
        mvwprintw(login_win, 4, 1, "User not found!");
    }

    wrefresh(login_win);
    sleep(2); 
    fclose(file);
    delwin(login_win);
    clear();
    attroff(COLOR_PAIR(4));
    return 0; 
}

int validate_username(char *username) {
    return strlen(username) > 0;
}

int validate_password(char *password) {
    int has_upper = 0, has_lower = 0, has_digit = 0;
    if (strlen(password) < 7) return 0;

    for (int i = 0; password[i]; i++) {
        if (isupper(password[i])) has_upper = 1;
        if (islower(password[i])) has_lower = 1;
        if (isdigit(password[i])) has_digit = 1;
    }

    return has_upper && has_lower && has_digit;
}

int validate_email(char *email) {
    char *at = strchr(email, '@');
    char *dot = strrchr(email, '.');
    return at && dot && at < dot && dot - at > 1;
}

int check_add_account(char *filename, char *filename2, char *username, char *password, char *email) {
    FILE *file = fopen(filename, "r+");
    FILE *file2 = fopen(filename2, "r+");
    if (!file) return -1;

    char line[256];
    int line_number = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if ((line_number % 3) == 1) { 
            line[strcspn(line, "\n")] = '\0';
            if (strcmp(line, username) == 0) {
                fclose(file);
                return 1;
            }
        }
    }

    fseek(file, 0, SEEK_END);
    fseek(file2, 0, SEEK_END);
    fprintf(file, "%s\n%s\n%s\n", username, password, email);
    fprintf(file2, "%s\n%d\n%d\n%d\n%d\n%d\n", username, 0, 0, 0, 0, 0 );
    fclose(file2);
    fclose(file);

    return 0;
}

void create_treasure(Player *player){
    treasure.position.x= GRID_WIDTH*5/16;
    treasure.position.y= GRID_HEIGHT/6;
    treasure.height= GRID_HEIGHT*2/3;
    treasure.width= GRID_WIDTH*3/8;
}

void draw_treasure(char grid[GRID_HEIGHT][GRID_WIDTH]){
    clear();
    for(int j=0; j<treasure.width; j++){
            grid[treasure.position.y][treasure.position.x+j] = '_';
            grid[treasure.position.y+treasure.height-1][treasure.position.x+j]= '_';
    }for(int j=0; j<treasure.height-1; j++){
            grid[treasure.position.y+j+1][treasure.position.x] ='|';
            grid[treasure.position.y+j+1][treasure.position.x+treasure.width-1]= '|';
        }
    for(int j=1; j<treasure.height-1; j++){
        for (int k=1; k<treasure.width-1; k++)
            grid[j+treasure.position.y][k+treasure.position.x] = '.';
    }int goldnum= rand() % 9  + 11;
    int blackgoldnum= rand() % 4 + 4;
    int trapnum= rand() % 5 + 10;
    for(int i=0; i<goldnum; i++){
        int y=rand()%(treasure.height-2)+treasure.position.y + 1;
        int x=rand()%(treasure.width-2)+treasure.position.x+1;
        grid[y][x]='$';
    }for(int i=0; i<blackgoldnum; i++){
        int y=rand()%(treasure.height-2)+treasure.position.y + 1;
        int x=rand()%(treasure.width-2)+treasure.position.x+1;
        grid[y][x]='b';
    }for(int i=0; i<trapnum; i++){
        int y=rand()%(treasure.height-2)+treasure.position.y + 1;
        int x=rand()%(treasure.width-2)+treasure.position.x+1;
        grid[y][x]='p';
    }
}

void create_room(Room *rooms, Player *player){
    int spellroomcount=rand() % 2 + 1;
    for (int i=0; i<spellroomcount; i++){
        spellindex[i]=rand() % 6;
    }
    for(int i=0; i<3; i++){
        rooms[i].position.y= (rand() % (GRID_HEIGHT/4-1))+1;
        do{
        rooms[i].position.x= (rand() % (GRID_WIDTH/8))+(GRID_WIDTH/3)*(i);
        }while(rooms[i].position.x==0);
        rooms[i].height= rand() % (GRID_HEIGHT/6) + 4;
        rooms[i].width = rand() % (GRID_WIDTH*9/80) + 6;
        rooms[i].number_of_doors= rand()%2 +1;
        for (int j=0; j<rooms[i].number_of_doors; j++){
            rooms[i].doors[j].position.x= rooms[i].position.x + rand() % (rooms[i].width-2) + 1;
            rooms[i].doors[j].position.y= rooms[i].position.y + (rand() % 2)* (rooms[i].height-1); 
        }for (int j=0; j<rooms[i].number_of_doors; j++){
            rooms[i].doors[j+2].position.x= rooms[i].position.x + (rand() % 2)* (rooms[i].width-1);
            rooms[i].doors[j+2].position.y= rooms[i].position.y + rand() % (rooms[i].height-2) + 1;
        }           
        if(rooms[i].height>4 && rooms[i].width>7){
            traps[i].position.x= rand() % (rooms[i].width - 2) + 1;
            traps[i].position.y= rand() % (rooms[i].height - 2) + 1;
            if(player->level<=2){
                deamons[i].position.x= rand() % (rooms[i].width - 2) + 1;
                deamons[i].position.y= rand() % (rooms[i].height - 2) + 1;
                if(rooms[i].height>6 && rooms[i].width>7){
                    deamons[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
                    deamons[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
                }fbmonsters[i].position.x= rand() % (rooms[i].width - 2) + 1;
                fbmonsters[i].position.y= rand() % (rooms[i].height - 2) + 1;
            }
            if(player->level<=3 && player->level>=2 && (rooms[i].height>6 || rooms[i].width>7)){
                giants[i].position.x= rand() % (rooms[i].width - 2) + 1;
                giants[i].position.y= rand() % (rooms[i].height - 2) + 1;
                snakes[i].position.x= rand() % (rooms[i].width - 2) + 1;
                snakes[i].position.y= rand() % (rooms[i].height - 2) + 1;
            }if(player->level<=4 && player->level>=3 && (rooms[i].height>6 || rooms[i].width>7)){
                snakes[i].position.x= rand() % (rooms[i].width - 2) + 1;
                snakes[i].position.y= rand() % (rooms[i].height - 2) + 1;
                undeeds[i].position.x= rand() % (rooms[i].width - 2) + 1;
                undeeds[i].position.y= rand() % (rooms[i].height - 2) + 1;
            } 
            if(rooms[i].height>7 && rooms[i].width>7){
                traps[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
                traps[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
            }            
        }
        if (rooms[i].height>4 && rooms[i].width>5){
            pillars[i].position.x= rand() % (rooms[i].width - 4) + 2;
            pillars[i].position.y= rand() % (rooms[i].height - 4) + 2;
            if(rooms[i].height>5 || rooms[i].width>6){
            foods[i].position.x= rand() % (rooms[i].width - 4) + 2;
            foods[i].position.y= rand() % (rooms[i].height - 4) + 2;
            }
            if(player->level==1){
                arrows[i].position.x=rand() % (rooms[i].width - 2) + 1;
                arrows[i].position.y=rand() % (rooms[i].height - 2) + 1;
            }
            else if(player->level==2){
                swords[i].position.x=rand() % (rooms[i].width - 2) + 1;
                swords[i].position.y=rand() % (rooms[i].height - 2) + 1;
            }else if(player->level==3 && (rooms[i].height>6 || rooms[i].width>6)){
                    daggers[i].position.x=rand() % (rooms[i].width - 2) + 1;
                    daggers[i].position.y=rand() % (rooms[i].height - 2) + 1;
            }if(player->level==4 && (rooms[i].height>6 || rooms[i].width>7)){
                    wands[i].position.x=rand() % (rooms[i].width - 2) + 1;
                    wands[i].position.y=rand() % (rooms[i].height - 2) + 1;
            }  
            if(rooms[i].height>6 && rooms[i].width>7){
                pillars[i+6].position.x= rand() % (rooms[i].width - 4) + 2;
                pillars[i+6].position.y= rand() % (rooms[i].height - 4) + 2;
            }
        }if(rooms[i].height>6 && rooms[i].width>7){
                blackgold[i].position.x= rand() % (rooms[i].width - 2) + 1;
                blackgold[i].position.y= rand() % (rooms[i].height - 2) + 1;
            }  
        if (rooms[i].height>7 || rooms[i].width>6){
            golds[i].position.x= rand() % (rooms[i].width - 2) + 1;
            golds[i].position.y= rand() % (rooms[i].height - 2) + 1;
            if (rooms[i].height>8 && rooms[i].width>7){
            golds[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
            golds[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
            }if(rooms[i].height>8 && rooms[i].width>10){
                golds[i+12].position.x= rand() % (rooms[i].width - 2) + 1;
                golds[i+12].position.y= rand() % (rooms[i].height - 2) + 1;
            }
        }
    }for(int i=3; i<6; i++){
        rooms[i].position.y= (rand() % (GRID_HEIGHT/4-1))+GRID_HEIGHT/2;
        do{
        rooms[i].position.x= (rand() % (GRID_WIDTH/8))+(GRID_WIDTH/3)*(i-3);
        }while(rooms[i].position.x==0);
        rooms[i].height= rand() % (GRID_HEIGHT/6) + 4;
        rooms[i].width = rand() % (GRID_WIDTH*9/80) + 6;
        rooms[i].number_of_doors= rand()%2 +1;
        for (int j=0; j<rooms[i].number_of_doors; j++){
            rooms[i].doors[j].position.x= rooms[i].position.x + rand() % (rooms[i].width-2) + 1;
            rooms[i].doors[j].position.y= rooms[i].position.y + (rand() % 2)* (rooms[i].height-1) ; 
        }for (int j=0; j<rooms[i].number_of_doors; j++){
            rooms[i].doors[j+2].position.x= rooms[i].position.x + (rand() % 2)* (rooms[i].width-1);
            rooms[i].doors[j+2].position.y= rooms[i].position.y + rand() % (rooms[i].height-2) + 1;
        }           
        if(rooms[i].height>4 && rooms[i].width>6){
            traps[i].position.x= rand() % (rooms[i].width - 2) + 1;
            traps[i].position.y= rand() % (rooms[i].height - 2) + 1;
            if(player->level<=2){
                fbmonsters[i].position.x= rand() % (rooms[i].width - 2) + 1;
                fbmonsters[i].position.y= rand() % (rooms[i].height - 2) + 1;
                deamons[i].position.x= rand() % (rooms[i].width - 2) + 1;
                deamons[i].position.y= rand() % (rooms[i].height - 2) + 1;
                if(rooms[i].height>6 && rooms[i].width>7){
                    deamons[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
                    deamons[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
                }
            }
            if(player->level<=3 && player->level>=2 && (rooms[i].height>6 || rooms[i].width>7)){
                giants[i].position.x= rand() % (rooms[i].width - 2) + 1;
                giants[i].position.y= rand() % (rooms[i].height - 2) + 1;
                snakes[i].position.x= rand() % (rooms[i].width - 2) + 1;
                snakes[i].position.y= rand() % (rooms[i].height - 2) + 1;
            }if(player->level<=4 && player->level>=3 && (rooms[i].height>6 || rooms[i].width>7)){
                snakes[i].position.x= rand() % (rooms[i].width - 2) + 1;
                snakes[i].position.y= rand() % (rooms[i].height - 2) + 1;
                undeeds[i].position.x= rand() % (rooms[i].width - 2) + 1;
                undeeds[i].position.y= rand() % (rooms[i].height - 2) + 1;
            } 
            if(rooms[i].height>7 && rooms[i].width>7){
                traps[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
                traps[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
            }            
        }
        if (rooms[i].height>4 && rooms[i].width>5){
            pillars[i].position.x= rand() % (rooms[i].width - 4) + 2;
            pillars[i].position.y= rand() % (rooms[i].height - 4) + 2;
            if(rooms[i].height>5 || rooms[i].width>6){
            foods[i].position.x= rand() % (rooms[i].width - 4) + 2;
            foods[i].position.y= rand() % (rooms[i].height - 4) + 2;
            }if(player->level==1){
                    arrows[i].position.x=rand() % (rooms[i].width - 2) + 1;
                    arrows[i].position.y=rand() % (rooms[i].height - 2) + 1;
                }
            else if(player->level==2){
                swords[i].position.x=rand() % (rooms[i].width - 2) + 1;
                swords[i].position.y=rand() % (rooms[i].height - 2) + 1;
            }else if(player->level==3 && rooms[i].height>6 || rooms[i].width>6){
                    daggers[i].position.x=rand() % (rooms[i].width - 2) + 1;
                    daggers[i].position.y=rand() % (rooms[i].height - 2) + 1;
            }if(player->level==4 && rooms[i].height>6 || rooms[i].width>7){
                    wands[i].position.x=rand() % (rooms[i].width - 2) + 1;
                    wands[i].position.y=rand() % (rooms[i].height - 2) + 1;
            }if(rooms[i].height>6 && rooms[i].width>7){
                blackgold[i].position.x= rand() % (rooms[i].width - 2) + 1;
                blackgold[i].position.y= rand() % (rooms[i].height - 2) + 1;
            }       
            if(rooms[i].height>6 && rooms[i].width>7){
                pillars[i+6].position.x= rand() % (rooms[i].width - 4) + 2;
                pillars[i+6].position.y= rand() % (rooms[i].height - 4) + 2;
            }if (rooms[i].height>6 || rooms[i].width>7){
            golds[i].position.x= rand() % (rooms[i].width - 2) + 1;
            golds[i].position.y= rand() % (rooms[i].height - 2) + 1;
            if(rooms[i].height > 8 && rooms[i].width>7){
            golds[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
            golds[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
                }if(rooms[i].height>8 && rooms[i].width>10){
                    golds[i+12].position.x= rand() % (rooms[i].width - 2) + 1;
                    golds[i+12].position.y= rand() % (rooms[i].height - 2) + 1;
                }
        }
        }attroff(COLOR_PAIR(3));
    }
}

void draw_rooms(Room *rooms, char grid[GRID_HEIGHT][GRID_WIDTH],Player *player){
    clear();
    int secret_door= rand() % 3 + 3;
    int password_door= rand() % 3;
    for(int i=0; i<6; i++){
        for(int j=0; j<rooms[i].width; j++){
            grid[rooms[i].position.y][rooms[i].position.x+j] = '_';
            grid[rooms[i].position.y+rooms[i].height-1][rooms[i].position.x+j]= '_';
            if(i==spellindex[0] || i==spellindex[1]){
                isspell[rooms[i].position.y+rooms[i].height-1][rooms[i].position.x+j]=1;
                isspell[rooms[i].position.y][rooms[i].position.x+j]=1;
                damage[i].position.x= rand() % (rooms[i].width - 2) + 1;
                damage[i].position.y= rand() % (rooms[i].height - 2) + 1;
                health[i].position.x= rand() % (rooms[i].width - 2) + 1;
                health[i].position.y= rand() % (rooms[i].height - 2) + 1;
                speed[i].position.x= rand() % (rooms[i].width - 2) + 1;
                speed[i].position.y= rand() % (rooms[i].height - 2) + 1;
                if (rooms[i].height>7 && rooms[i].width>8){
                    damage[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
                    damage[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
                    health[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
                    health[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
                    speed[i+6].position.x= rand() % (rooms[i].width - 2) + 1;
                    speed[i+6].position.y= rand() % (rooms[i].height - 2) + 1;
                }
            }
        }for(int j=0; j<rooms[i].height-1; j++){
            grid[rooms[i].position.y+j+1][rooms[i].position.x] ='|';
            grid[rooms[i].position.y+j+1][rooms[i].position.x+rooms[i].width-1]= '|';
            if(i==spellindex[0] || i==spellindex[1]){
                isspell[rooms[i].position.y+j+1][rooms[i].position.x]=1;
                isspell[rooms[i].position.y+j+1][rooms[i].position.x+rooms[i].width-1]=1;
            }
        }
        for(int j=1; j<rooms[i].height-1; j++){
            for (int k=1; k<rooms[i].width-1; k++){
                grid[j+rooms[i].position.y][k+rooms[i].position.x] = '.';
                if(i==spellindex[0] || i==spellindex[1]){
                    isspell[j+rooms[i].position.y][k+rooms[i].position.x]=1;
            }
        }}
        int room_index_copy=0;
        if(i>=3){
            int room_index= rand() % 3;
            if(i==5){
                do{
                    room_index= rand() % 3;
                }while (room_index==room_index_copy);
            }
            room_index_copy=room_index;
            int y_change=rand() % 4 + 3;
            int x1= rand() % (rooms[i].width-2) + rooms[i].position.x + 1 ;
            int y1= rand() % (rooms[i].height-2) + rooms[i].position.y + 1 ;
            int x2= rand() % (rooms[room_index].width-2) + rooms[room_index].position.x + 1 ;
            int y2= rand() % (rooms[room_index].height-2) + rooms[room_index].position.y + 1 ;
            int counter=0;
            while(y1 >= (y_change+y2)){
                if (counter==2)
                    break;
                if(grid[y1][x1]==' ')
                    grid[y1][x1]='#';
                else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){
                    if(secret_door==i){
                        grid[y1][x1]='u';
                    }else if(password_door==i-3){
                        grid[y1][x1]='@';
                    }                    
                    else
                        grid[y1][x1]='+';
                    counter++;
                }y1--;
            }counter=0;
            while(x1!=x2){
                if(counter==2)
                    break;
                if(grid[y1][x1]==' '){
                    grid[y1][x1]='#';
                }else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){
                    if(secret_door==i){
                        grid[y1][x1]='u';
                    }else if(password_door==i-3){
                        grid[y1][x1]='@';
                    }else
                        grid[y1][x1]='+';
                    counter++;
                }
                if(x1>x2)
                    x1--;
                else    
                    x1++;
            }counter=0;
            while(y1!=y2){
                if (counter>=2)
                    break;
                if(grid[y1][x1]==' ')
                    grid[y1][x1]='#';
                else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){
                    if(secret_door==i){
                        grid[y1][x1]='u';
                    }else if(password_door==i-3){
                        grid[y1][x1]='@';
                    }else
                        grid[y1][x1]='+';
                    counter++;
                }y1--;
            }
        }
        if(i==5){
            int index1,index2;
            do{
                index1=rand() % 3 + 3;
                index2=rand() % 3 + 3;
            }while(index1==index2);
            if(index2>index1){
                int temp=index2;
                index2=index1;
                index1=temp;
            }
            int x_change=rand() % 5 + 4;
            int x1= rand() % (rooms[index1].width-2) + rooms[index1].position.x + 1 ;
            int y1= rand() % (rooms[index1].height-2) + rooms[index1].position.y + 1 ;
            int x2= rand() % (rooms[index2].width-2) + rooms[index2].position.x + 1 ;
            int y2= rand() % (rooms[index2].height-2) + rooms[index2].position.y + 1 ;
            int counter=0;
            while(x1 >= (x_change+x2)){
                if (counter==2)
                    break;
                if(grid[y1][x1]==' ')
                    grid[y1][x1]='#';
                else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){
                    if(password_door==index2-3 || password_door==index1-3){
                        grid[y1][x1]='@';
                    }else
                        grid[y1][x1]='+';
                    counter++;
                }x1--;
            }counter=0;
            while(y1!=y2){
                if(counter==1)
                    break;
                if(grid[y1][x1]==' '){
                    grid[y1][x1]='#';
                }else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='u' || grid[y1][x1]=='+' || grid[y1][x1]=='@'){
                    if(password_door==index2-3 || password_door==index1-3){
                        grid[y1][x1]='@';
                    }else
                        grid[y1][x1]='+';
                    counter++;
                }
                if(y1>y2)
                    y1--;
                else    
                    y1++;
            }counter=0;
            while(x1!=x2){
                if (counter==2)
                    break;
                if(grid[y1][x1]==' ')
                    grid[y1][x1]='#';
                else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){
                    if(password_door==index2-3 || password_door==index1-3){
                        grid[y1][x1]='@';
                    }else
                        grid[y1][x1]='+';
                    counter++;
                }x1--;
            }
            for(int t=0; t<=1; t++){
                int x_change=rand() % 5 + 4;
                x2= rand() % (rooms[t].width-2) + rooms[t].position.x + 1 ;
                y2= rand() % (rooms[t].height-2) + rooms[t].position.y + 1 ;
                x1= rand() % (rooms[t+1].width-2) + rooms[t+1].position.x + 1 ;
                y1= rand() % (rooms[t+1].height-2) + rooms[t+1].position.y + 1 ;
                counter=0;
                while(x1 >= (x_change+x2)){
                    if (counter==2)
                        break;
                    if(grid[y1][x1]==' ')
                        grid[y1][x1]='#';
                    else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){
                        if((t+secret_door)==5){
                            grid[y1][x1]='u';
                        }else
                            grid[y1][x1]='+';
                        counter++;
                    }x1--;
                }counter=0;
                while(y1!=y2){
                    if(counter==2)
                        break;
                    if(grid[y1][x1]==' '){
                        grid[y1][x1]='#';
                    }else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){               
                        if(t+secret_door==5){
                            grid[y1][x1]='u';
                        }else
                            grid[y1][x1]='+';
                        counter++;
                    }if(y1>y2)
                        y1--;
                    else    
                        y1++;
                }counter=0;
                while(x1!=x2){
                    if (counter==1)
                        break;
                    if(grid[y1][x1]==' ')
                        grid[y1][x1]='#';
                    else if(grid[y1][x1]=='|' || grid[y1][x1]=='_' || grid[y1][x1]=='+' || grid[y1][x1]=='u' || grid[y1][x1]=='@'){
                        if(t+secret_door==5){
                            grid[y1][x1]='u';
                        }else
                            grid[y1][x1]='+';
                        counter++;
                    }x1--;
                }
            }
        }
        if (golds[i].position.x != 0 && spellindex[0]!=i && spellindex[1]!=i){
            grid[golds[i].position.y + rooms[i].position.y][golds[i].position.x + rooms[i].position.x] = '$';
            if(golds[i+6].position.x != 0){
                grid[golds[i+6].position.y + rooms[i].position.y][golds[i+6].position.x + rooms[i].position.x]= '$';
                if(golds[i+12].position.x != 0){
                    grid[golds[i+12].position.y + rooms[i].position.y][golds[i+12].position.x + rooms[i].position.x] = '$';
                    }
            }
        }
        if (traps[i].position.x != 0){
            grid[traps[i].position.y + rooms[i].position.y][traps[i].position.x + rooms[i].position.x] = 'p';
            if(traps[i+6].position.x != 0){
                grid[traps[i+6].position.y + rooms[i].position.y][traps[i+6].position.x + rooms[i].position.x]='p';
            }
        }if (fbmonsters[i].position.x != 0){
            grid[fbmonsters[i].position.y + rooms[i].position.y][fbmonsters[i].position.x + rooms[i].position.x] = 'F';
        }
        if (pillars[i].position.x != 0){
            grid[pillars[i].position.y + rooms[i].position.y][pillars[i].position.x + rooms[i].position.x] = 'O';
            if(pillars[i+6].position.x != 0){
                grid[pillars[i+6].position.y + rooms[i].position.y][pillars[i+6].position.x + rooms[i].position.x]='O';
            }
        }if (deamons[i].position.x != 0){
            grid[deamons[i].position.y + rooms[i].position.y][deamons[i].position.x + rooms[i].position.x] = 'D';
        }if(deamons[i+6].position.x != 0){
                grid[deamons[i+6].position.y + rooms[i].position.y][deamons[i+6].position.x + rooms[i].position.x]='D';
            }
        if (giants[i].position.x != 0){
            grid[giants[i].position.y + rooms[i].position.y][giants[i].position.x + rooms[i].position.x] = 'G';
        }if (snakes[i].position.x != 0){
            grid[snakes[i].position.y + rooms[i].position.y][snakes[i].position.x + rooms[i].position.x] = 'S';
        }if (undeeds[i].position.x != 0){
            grid[undeeds[i].position.y + rooms[i].position.y][undeeds[i].position.x + rooms[i].position.x] = 'U';
        }
        if (blackgold[i].position.x != 0){
            grid[blackgold[i].position.y + rooms[i].position.y][blackgold[i].position.x + rooms[i].position.x] = 'b';
            
        }
        if (arrows[i].position.x != 0 && player->level==1 && spellindex[0]!=i && spellindex[1]!=i){
            grid[arrows[i].position.y + rooms[i].position.y][arrows[i].position.x + rooms[i].position.x] = 'A';
        }if (swords[i].position.x != 0 && player->level==2  && spellindex[0]!=i && spellindex[1]!=i){
            grid[swords[i].position.y + rooms[i].position.y][swords[i].position.x + rooms[i].position.x] = 's';
        }if (daggers[i].position.x != 0 && player->level==3  && spellindex[0]!=i && spellindex[1]!=i){
            grid[daggers[i].position.y + rooms[i].position.y][daggers[i].position.x + rooms[i].position.x] = 'd';
        }if (wands[i].position.x != 0 && player->level==4 && spellindex[0]!=i && spellindex[1]!=i){
            grid[wands[i].position.y + rooms[i].position.y][wands[i].position.x + rooms[i].position.x] = 'W';
        }if (foods[i].position.x != 0 && spellindex[0]!=i && spellindex[1]!=i){
            grid[foods[i].position.y + rooms[i].position.y][foods[i].position.x + rooms[i].position.x] = 'f';
        }if (damage[i].position.x != 0){
            grid[damage[i].position.y + rooms[i].position.y][damage[i].position.x + rooms[i].position.x] = ')';
            if(damage[i+6].position.x != 0){
                grid[damage[i+6].position.y + rooms[i].position.y][damage[i+6].position.x + rooms[i].position.x]= ')';
                if(damage[i+12].position.x != 0){
                    grid[damage[i+12].position.y + rooms[i].position.y][damage[i+12].position.x + rooms[i].position.x] = ')';
                    }
            }
        }if (speed[i].position.x != 0){
            grid[speed[i].position.y + rooms[i].position.y][speed[i].position.x + rooms[i].position.x] = '(';
            if(speed[i+6].position.x != 0){
                grid[speed[i+6].position.y + rooms[i].position.y][speed[i+6].position.x + rooms[i].position.x]= '(';
                if(speed[i+12].position.x != 0){
                    grid[speed[i+12].position.y + rooms[i].position.y][speed[i+12].position.x + rooms[i].position.x] = '(';
                    }
            }
        }if (health[i].position.x != 0){
            grid[health[i].position.y + rooms[i].position.y][health[i].position.x + rooms[i].position.x] = '*';
            if(damage[i+6].position.x != 0){
                grid[health[i+6].position.y + rooms[i].position.y][health[i+6].position.x + rooms[i].position.x]= '*';
                if(health[i+12].position.x != 0){
                    grid[health[i+12].position.y + rooms[i].position.y][health[i+12].position.x + rooms[i].position.x] = '*';
                    }
            }
        }
    }for(int i=0; i<6; i++){
        for(int j=rooms[i].position.x; j<=rooms[i].position.x+rooms[i].width; j++){
            for (int k=rooms[i].position.y; k<=rooms[i].position.y+rooms[i].height; k++){
                if (grid[k][j]=='@'){
                    if(i%2==0){
                        grid[rooms[i].position.y+1][rooms[i].position.x+1]='&';
                    }else   
                        grid[rooms[i].position.y+rooms[i].height-2][rooms[i].position.x+rooms[i].width-2]='&';
                    break;
                }
            }
        }
    }
    refresh();
}


void print_grid(char grid[GRID_HEIGHT][GRID_WIDTH], int visit[GRID_HEIGHT][GRID_WIDTH], Player *player) {
    clear();
    attroff(COLOR_PAIR(3));
    for (int i = 1; i < GRID_HEIGHT-1; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if(visit[i][j])
            {   if(player->level>4 && grid[i][j]!='.')
                    attron(COLOR_PAIR(13));
                if(grid[i][j]=='$'){
                    attron(COLOR_PAIR(5));
                    mvprintw(i,j,"%s",coin);
                    attroff(COLOR_PAIR(5));
                    continue;
                }else if(grid[i][j]=='A'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",arroww);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='d'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",daggerr);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='s'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",swordd);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='W'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",wandd);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='f'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",foodd);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='D'){
                    attron(COLOR_PAIR(14));
                    mvprintw(i,j,"D");
                    attroff(COLOR_PAIR(14));
                    continue;
                }else if(grid[i][j]=='F'){
                    attron(COLOR_PAIR(15));
                    mvprintw(i,j,"F");
                    attroff(COLOR_PAIR(15));
                    continue;
                }else if(grid[i][j]=='G'){
                    attron(COLOR_PAIR(16));
                    mvprintw(i,j,"G");
                    attroff(COLOR_PAIR(16));
                    continue;
                }else if(grid[i][j]=='S'){
                    attron(COLOR_PAIR(17));
                    mvprintw(i,j,"S");
                    attroff(COLOR_PAIR(17));
                    continue;
                }else if(grid[i][j]=='U'){
                    attron(COLOR_PAIR(18));
                    mvprintw(i,j,"U");
                    attroff(COLOR_PAIR(18));
                    continue;
                }else if(grid[i][j]==')'){
                    attron(COLOR_PAIR(3));
                    mvprintw(i,j,"%s",potionn);
                    attroff(COLOR_PAIR(3));
                    continue;
                }else if(grid[i][j]=='('){
                    attron(COLOR_PAIR(12));
                    mvprintw(i,j,"%s",potionn);
                    attroff(COLOR_PAIR(12));
                    continue;
                }else if(grid[i][j]=='*'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",potionn);
                    attroff(COLOR_PAIR(1));
                    continue;
                }
                else if(grid[i][j]=='<' && player->level>=4){
                    attron(COLOR_PAIR(3));
                    mvprintw(i,j,"🏲");
                    attroff(COLOR_PAIR(3));
                    continue;
                }
                else if(grid[i][j]=='^'){
                    attron(COLOR_PAIR(5));
                    mvprintw(i,j,"∧");
                    attroff(COLOR_PAIR(5));
                    continue;
                }
                else if(grid[i][j]=='b'){
                    attron(COLOR_PAIR(13));
                    mvprintw(i,j,"%s",blackcoin);
                    attroff(COLOR_PAIR(13));
                    continue;
                }
                else if(grid[i][j]=='@' && passworddoors[i][j]==0)
                    attron(COLOR_PAIR(3));
                else if(grid[i][j]=='@' && passworddoors[i][j]==1)
                    attron(COLOR_PAIR(1));
                else if(grid[i][j]=='&')
                    attron(COLOR_PAIR(10));
                else if(grid[i][j]=='<')
                    attron(COLOR_PAIR(4));
                else if(isspell[i][j]==1)
                    attron(COLOR_PAIR(11));
                mvaddch(i, j, grid[i][j]);
                if(grid[i][j]=='p'){
                    attroff(COLOR_PAIR(13));
                    mvprintw(i,j,".");
                }if(grid[i][j]=='u'){
                    for(int k=0; k<6; k++){
                        if(((i==rooms[k].position.y && rooms[k].position.x<=j && j<rooms[k].position.x+rooms[k].width))
                        || ((i==(rooms[k].position.y + rooms[k].height-1)) && rooms[k].position.x<j && j<rooms[k].position.x+rooms[k].width-1)){
                            mvprintw(i,j,"_");
                            break;
                        }
                        else    
                            mvprintw(i,j,"|");
                    }
                }
                attroff(COLOR_PAIR(1));
                attroff(COLOR_PAIR(11));
                }
        }
    }
    attron(COLOR_PAIR(player->color));
    mvprintw(player->position.y, player->position.x,"%s",playerr);
    attroff(COLOR_PAIR(player->color));
    score(player);
    refresh();
}

void print_grid2(char grid[GRID_HEIGHT][GRID_WIDTH], int visit[GRID_HEIGHT][GRID_WIDTH], Player *player) {
    clear();
    attroff(COLOR_PAIR(3));
    for (int i = 1; i < GRID_HEIGHT-1; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            {   if(player->level>4 && grid[i][j]!='.')
                    attron(COLOR_PAIR(13));
                if(grid[i][j]=='$'){
                    attron(COLOR_PAIR(5));
                    mvprintw(i,j,"%s",coin);
                    attroff(COLOR_PAIR(5));
                    continue;
                }else if(grid[i][j]=='A'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",arroww);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='d'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",daggerr);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='s'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",swordd);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='W'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",wandd);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='f'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",foodd);
                    attroff(COLOR_PAIR(1));
                    continue;
                }else if(grid[i][j]=='b'){
                    attron(COLOR_PAIR(13));
                    mvprintw(i,j,"%s",blackcoin);
                    attroff(COLOR_PAIR(13));
                    continue;
                }else if(grid[i][j]=='D'){
                    attron(COLOR_PAIR(14));
                    mvprintw(i,j,"D");
                    attroff(COLOR_PAIR(14));
                    continue;
                }else if(grid[i][j]=='F'){
                    attron(COLOR_PAIR(15));
                    mvprintw(i,j,"F");
                    attroff(COLOR_PAIR(15));
                    continue;
                }else if(grid[i][j]=='G'){
                    attron(COLOR_PAIR(16));
                    mvprintw(i,j,"G");
                    attroff(COLOR_PAIR(16));
                    continue;
                }else if(grid[i][j]=='S'){
                    attron(COLOR_PAIR(17));
                    mvprintw(i,j,"S");
                    attroff(COLOR_PAIR(17));
                    continue;
                }else if(grid[i][j]=='U'){
                    attron(COLOR_PAIR(18));
                    mvprintw(i,j,"U");
                    attroff(COLOR_PAIR(18));
                    continue;
                }else if(grid[i][j]==')'){
                    attron(COLOR_PAIR(3));
                    mvprintw(i,j,"%s",potionn);
                    attroff(COLOR_PAIR(3));
                    continue;
                }else if(grid[i][j]=='('){
                    attron(COLOR_PAIR(12));
                    mvprintw(i,j,"%s",potionn);
                    attroff(COLOR_PAIR(12));
                    continue;
                }else if(grid[i][j]=='*'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"%s",potionn);
                    attroff(COLOR_PAIR(1));
                    continue;
                }
                else if(grid[i][j]=='<' && player->level>=4){
                    attron(COLOR_PAIR(3));
                    mvprintw(i,j,"🏲");
                    attroff(COLOR_PAIR(3));
                    continue;
                }
                else if(grid[i][j]=='^'){
                    attron(COLOR_PAIR(5));
                    mvprintw(i,j,"∧");
                    attroff(COLOR_PAIR(5));
                    continue;
                }else if(grid[i][j]=='@' && passworddoors[i][j]==0)
                    attron(COLOR_PAIR(3));
                else if(grid[i][j]=='@' && passworddoors[i][j]==1)
                    attron(COLOR_PAIR(1));
                else if(grid[i][j]=='&')
                    attron(COLOR_PAIR(10));
                else if(grid[i][j]=='<' && player->level<=3)
                    attron(COLOR_PAIR(4));
                else if(isspell[i][j]==1)
                    attron(COLOR_PAIR(11));
                mvaddch(i, j, grid[i][j]);
                if(grid[i][j]=='p')
                    mvprintw(i,j,".");
                if(grid[i][j]=='u'){
                    for(int k=0; k<6; k++){
                        if(((i==rooms[k].position.y && rooms[k].position.x<=j && j<rooms[k].position.x+rooms[k].width))
                        || ((i==(rooms[k].position.y + rooms[k].height-1)) && rooms[k].position.x<j && j<rooms[k].position.x+rooms[k].width-1)){
                            mvprintw(i,j,"_");
                            break;
                        }
                        else    
                            mvprintw(i,j,"|");
                    }
                }
                attroff(COLOR_PAIR(1));
                attroff(COLOR_PAIR(11));
                }
        }
    }
    attron(COLOR_PAIR(player->color));
    mvprintw(player->position.y, player->position.x,"%s",playerr);
    attroff(COLOR_PAIR(player->color));
    score(player);
    refresh();
}

void fill_player_data(char *filename, char *username, Player *player) {
    FILE *file = fopen(filename, "r+");
    if (!file) return;

    char line[768];
    int i = 0;
    int line_number = 0;
    long pos_to_update = -1;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if ((line_number % 6) == 1) {
            line[strcspn(line, "\n")] = '\0';
            if (strcmp(line, username) == 0) {
                fscanf(file, "%d", &player->gold);
                fscanf(file, "%d", &player->sum_of_gold);
                fscanf(file, "%d", &player->number_of_games);
                pos_to_update = ftell(file);
                fscanf(file, "%d", &player->number_of_logins);
                player->number_of_logins++;
                fseek(file, pos_to_update, SEEK_SET);
                fprintf(file, "\n%d\n", player->number_of_logins);
                fscanf(file, "%d", &player->score);
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}

void load_player_data(const char *filename, Player players[], int *player_count) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char line[768];
    int line_number = 0;
    int current_player = -1;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if ((line_number % 6) == 1) {
            current_player++;
            strcpy(players[current_player].username, strtok(line, "\n"));
        } else if ((line_number % 6) == 2) {
            players[current_player].gold = atoi(line);
        } else if ((line_number % 6) == 3) {
            players[current_player].sum_of_gold = atoi(line);
        } else if ((line_number % 6) == 4) {
            players[current_player].number_of_games = atoi(line);
        } else if ((line_number % 6) == 5) {
            players[current_player].number_of_logins = atoi(line);
        } else if ((line_number % 6) == 0) {
            players[current_player].score = atoi(line);
        }
    }

    *player_count = current_player + 1;
    fclose(file);
}

int compare_players(const void *a, const void *b) {
    return ((Player *)b)->score - ((Player *)a)->score;
}

void display_leaderboard(const Player players[], int player_count, const char *current_user) {
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);  
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);   
    init_pair(7, COLOR_YELLOW, COLOR_BLACK); 
    init_pair(8, COLOR_RED, COLOR_WHITE);     

    clear();
    mvprintw(LINES-2, COLS/2-13, "press any button to go back!");
    mvprintw(2, COLS/2-6, "Leaderboard");
    mvprintw(4, 2, "Rank  Username        Sum of Gold      Logins    Games    Score");
    mvprintw(5, 2, "---------------------------------------------------------------");

    for (int i = 0; i < player_count; i++) {
        int color_pair = 0;
        if (strcmp(players[i].username, current_user) == 0) {
            color_pair = 8;
        } else if (i == 0) {
            color_pair = 5;
            attron(COLOR_PAIR(1));
            mvprintw(6,70,"GOAT");
            mvprintw(6,2,"🏆");
        } else if (i == 1) {
            color_pair = 6;
            mvprintw(7,70,"LEGEND");
            mvprintw(7,2,"🏆");
        } else if (i == 2) {
            color_pair = 7;
            mvprintw(8,70,"HERO");
            mvprintw(8,2,"🏆");
        }

        if (color_pair != 0) {
            attron(COLOR_PAIR(color_pair));
        }

        mvprintw(6 + i, 5, "%d  %-13s  %12d  %9d  %7d  %7d", 
                 i + 1, players[i].username, players[i].sum_of_gold,
                 players[i].number_of_logins, players[i].number_of_games, players[i].score);

        if (color_pair != 0) {
            attroff(COLOR_PAIR(color_pair));
        }
    }

    refresh();
    getch();
    endwin();
}
void player_setup(Player *player, char grid[GRID_HEIGHT][GRID_WIDTH], int visit[GRID_HEIGHT][GRID_WIDTH]){
    if((player->level)>4){
        for (int i=treasure.position.y; i<treasure.position.y+treasure.height; i++){
        for(int j=treasure.position.x; j<treasure.position.x+treasure.width; j++){
            visit[i][j]=1;
        }
        }do{
        player->position.x= rand() % (treasure.width - 2) + 1 + treasure.position.x;
        player->position.y= rand() % (treasure.height - 2) + 1 + treasure.position.y;
        }while(grid[player->position.y][player->position.x]=='p'
            || grid[player->position.y][player->position.x]=='O'
            || grid[player->position.y][player->position.x]=='$');
        attron(COLOR_PAIR(player->color));
        mvprintw(player->position.y, player->position.x,"%s",playerr);
        do{
            stairs.position.x= rand() % (treasure.width - 2) + 1 + treasure.position.x;
            stairs.position.y= rand() % (treasure.height - 2) + 1 + treasure.position.y;
        }while(grid[stairs.position.y][stairs.position.x]=='p'
            || grid[stairs.position.y][stairs.position.x]=='O'
            || grid[stairs.position.y][stairs.position.x]=='$'
            || (stairs.position.y==player->position.y && stairs.position.x==player->position.x));
        attron(COLOR_PAIR(3));
        mvprintw(stairs.position.y, stairs.position.x, "🏲");
        attroff(COLOR_PAIR(3));
        grid[stairs.position.y][stairs.position.x]='<';
        refresh();
        return;
    }
    
    int room= rand() % 6;
    isinroom=room;
    usteps=0;
    gsteps=0;
    visit_count[room]=1;
    for (int i=rooms[room].position.y; i<rooms[room].position.y+rooms[room].height; i++){
        for(int j=rooms[room].position.x; j<rooms[room].position.x+rooms[room].width; j++){
            visit[i][j]=1;
        }
    }
    do{
    player->position.x= rand() % (rooms[room].width - 2) + 1 + rooms[room].position.x;
    player->position.y= rand() % (rooms[room].height - 2) + 1 + rooms[room].position.y;
    }while(grid[player->position.y][player->position.x]=='p'
        || grid[player->position.y][player->position.x]=='O'
        || grid[player->position.y][player->position.x]=='$');
    attron(COLOR_PAIR(player->color));
    mvprintw(player->position.y, player->position.x,"%s",playerr);
    refresh();
}
int check=0;
int map_check=0;
int xt=0,yt=0;
int handle_input(int input, Player *player, char grid[GRID_HEIGHT][GRID_WIDTH], int *steps, int visit[GRID_HEIGHT][GRID_WIDTH]){
    int xn=player->position.x;
    int yn=player->position.y;
    if(check==1 && grid[yt][xt]=='p'){
        attroff(COLOR_PAIR(3));
        mvprintw(yt,xt,".");
    }
    switch (input){
        case 'J':
        case 'j':
            if(speedactivated==2)
                yn-=2;
            else
                yn--;
            *steps+=1;
            break;
        case 'K':
        case 'k':
            if(speedactivated==2)
                yn+=2;
            else
                yn++;
            *steps+=1;
            break;
        case 'L':
        case 'l':
            if(speedactivated==2)
                xn+=2;
            else
                xn++;
            *steps+=1;
            break;
        case 'H':
        case 'h':
            if(speedactivated==2){
                xn-=2;
            }else
                xn--;
            *steps+=1;
            break;
        case 'Y':
        case 'y':
            yn--;
            xn--;
            *steps+=1;
            break;
        case 'U':
        case 'u':
            yn--;
            xn++;
            *steps+=1;
            break;
        case 'B':
        case 'b':
            yn++;
            xn--;            
            *steps+=1;
            break;
        case 'N':
        case 'n':
            yn++;
            xn++;
            *steps+=1;
            break;
        case 'F':
        case 'f':
            int ch=getch();
            switch (ch){
                case 'J':
                case 'j':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1)
                        yn--;
                    yn++;
                    *steps+=1;
                    break;
                case 'K':
                case 'k':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1)
                        yn++;
                    yn--;
                    *steps+=1;
                    break;
                case 'L':
                case 'l':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1)
                        xn++;
                    xn--;
                    *steps+=1;
                    break;
                case 'H':
                case 'h':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1)
                        xn--;
                    xn++;
                    *steps+=1;
                    break;
                case 'Y':
                case 'y':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1){
                        yn--;
                        xn--;}
                    yn++;
                    xn++;
                    *steps+=1;
                    break;
                case 'U':
                case 'u':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1)
                    {    yn--;
                        xn++;}
                    yn++;
                    xn--;
                    *steps+=1;
                    break;
                case 'B':
                case 'b':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1){
                        yn++;
                        xn--;}
                    yn--;
                    xn++;
                    *steps+=1;
                    break;
                case 'N':
                case 'n':
                    while(grid[yn][xn]!='|' && grid[yn][xn]!='_' && grid[yn][xn]!=' ' && grid[yn][xn]!='O' && grid[yn][xn]!='^' && visit[yn][xn]==1)
                    {   yn++;
                        xn++;}
                    yn--;
                    xn--;
                    *steps+=1;
                    break;
                default:
                    break;
            
            }
            break;
        case 'S':
        case 's':
            for(int i=xn-1; i<=xn+1; i++){
                for(int j=yn-1; j<=yn+1; j++){
                    if(!(i==xn && j==yn)){
                        if(grid[j][i]=='p'){
                            attroff(COLOR_PAIR(3));
                            mvprintw(j,i,"∧");
                            check=1;
                            xt=i;
                            yt=j;
                        }
                    }
                }
            }
            break;
        case 'M':
        case 'm':
            if(map_check==0){
            print_grid2(grid, visit, player);
            map_check=1;
            }else{
                print_grid(grid, visit, player);
                map_check=0;
            }
            break;
        case 'E':
        case 'e':
            foodmenu(player);
            print_grid(grid,visit,player);
            score(player);
            break;
        case 'R':
        case 'r':
            spellmenu(player);
            print_grid(grid,visit,player);
            score(player);
            break;
        case 'I':
        case 'i':
            weaponmenu(player);
            print_grid(grid,visit,player);
            score(player);
            break;
        default:
            break;
        
    }
    if(check_position(yn, xn, player, grid, steps, visit)==1)
        return 1;
}
int pass;
int steps2;
int check_position(int y, int x, Player *player,char grid[GRID_HEIGHT][GRID_WIDTH],int *steps, int visit[GRID_HEIGHT][GRID_WIDTH]){
    attroff(COLOR_PAIR);
    switch (grid[y][x]){
        case '&':
            used=0;
            attron(COLOR_PAIR(4));
            randpass= rand()%9000+1000;
            mvprintw(0,0,"the password is: %d       ", randpass);
            attroff(COLOR_PAIR(1));
            player_move(y,x,player,grid, steps);
            break;
        case '@':
            if (ever[y][x]==0){
            attron(COLOR_PAIR(4));
            mvprintw(0,28,"            Enter the password:      ");
            if(used>=2){
                randpass=687;
                used=0;
            }steps2=*steps;
            echo();
            move(0,60);
            scanw("%d", &pass);
            noecho();
            int reversepass=((randpass/1000) + (randpass/100-(randpass/1000)*10)*10 + 10*(randpass%100-randpass%10) + 1000*(randpass%10));
            if((pass==randpass && is_reversed[y][x]>2) || (pass==reversepass && is_reversed[y][x]<=2)){
                ever[y][x]=-1;
                passworddoors[y][x]=1;
                used++;
                for (int i=x-1; i>=x-5;i--){
                    if(grid[y][i]=='#'){
                        mvprintw(y,i,"#");
                        visit[y][i]=1;
                    }else if(grid[y][i]=='+'){
                        mvprintw(y,i,"+");
                        visit[y][i]=1;
                    }
                    if(grid[y][i]!='#' && grid[y][i]!='+')
                        break;
            }
            for (int i=x+1; i<=x+5;i++){
                    if(grid[y][i]=='#'){
                        mvprintw(y,i,"#");
                        visit[y][i]=1;
                    }else if(grid[y][i]=='+'){
                        mvprintw(y,i,"+");
                        visit[y][i]=1;
                    }if(grid[y][i]!='#' && grid[y][i]!='+')
                        break;
            }
            for (int i=y-1; i>=y-5;i--){
                    if(grid[i][x]=='#'){
                        mvprintw(i,x,"#");
                        visit[i][x]=1;
                    }else if(grid[i][x]=='+'){
                        mvprintw(i,x,"+");
                        visit[i][x]=1;
                    }if (grid[i][x]!='#' && grid[i][x]!='+')
                        break;
            }for (int i=y+1; i<=y+5;i++){
                    if(grid[i][x]=='#'){
                        mvprintw(i,x,"#");
                        visit[i][x]=1;
                    }else if(grid[i][x]=='+'){
                        mvprintw(i,x,"+");
                        visit[i][x]=1;
                    }if(grid[i][x]!='#' && grid[i][x]!='+')
                        break;
            }
                if(x<(GRID_WIDTH/3) && y<(GRID_HEIGHT/2-1)){
                for (int i=rooms[0].position.y; i<rooms[0].position.y+rooms[0].height; i++){
                for(int j=rooms[0].position.x; j<rooms[0].position.x+rooms[0].width; j++){
                    visit[i][j]=1;
                    visit_count[0]=1;
                    }
                }isinroom=0;
            }else if(x>=(GRID_WIDTH/3) && x<(GRID_WIDTH*2/3) && y<(GRID_HEIGHT/2-1)){
                for (int i=rooms[1].position.y; i<rooms[1].position.y+rooms[1].height; i++){
                for(int j=rooms[1].position.x; j<rooms[1].position.x+rooms[1].width; j++){
                    visit[i][j]=1;
                    visit_count[1]=1;
                    }
                }isinroom=1;
            }
            else if(x>=(GRID_WIDTH*2/3) && y<(GRID_HEIGHT/2-1)){
                for (int i=rooms[2].position.y; i<rooms[2].position.y+rooms[2].height; i++){
                for(int j=rooms[2].position.x; j<rooms[2].position.x+rooms[2].width; j++){
                    visit[i][j]=1;
                    visit_count[2]=1;
                    }
                }isinroom=2;
            }else if(x<(GRID_WIDTH/3) && y>=(GRID_HEIGHT/2-1)){
                for (int i=rooms[3].position.y; i<rooms[3].position.y+rooms[3].height; i++){
                for(int j=rooms[3].position.x; j<rooms[3].position.x+rooms[3].width; j++){
                    visit[i][j]=1;
                    visit_count[3]=1;
                    }
                }isinroom=3;
            }else if(x>=(GRID_WIDTH/3) && x<(GRID_WIDTH*2/3) && y>=(GRID_HEIGHT/2-1)){
                for (int i=rooms[4].position.y; i<rooms[4].position.y+rooms[4].height; i++){
                for(int j=rooms[4].position.x; j<rooms[4].position.x+rooms[4].width; j++){
                    visit[i][j]=1;
                    visit_count[4]=1;
                    }
                }isinroom=4;
            }else if(x>=(GRID_WIDTH*2/3) && y>=(GRID_HEIGHT/2-1)){
                for (int i=rooms[5].position.y; i<rooms[5].position.y+rooms[5].height; i++){
                for(int j=rooms[5].position.x; j<rooms[5].position.x+rooms[5].width; j++){
                    visit[i][j]=1;
                    visit_count[5]=1;
                    }
                }isinroom=5;
            }player_move(y,x,player,grid, steps);
            print_grid(grid, visit, player);
            }
            else{
                count[y][x]++;
                for (int i=0; i<84; i++){
                    mvprintw(0,i," ");
                }if (count[y][x]==1){
                    attron(COLOR_PAIR(5));
                    mvprintw(0,32,"WRONG PASSWORD!");
                }else if(count[y][x]==2){
                    attron(COLOR_PAIR(12));
                    mvprintw(0,32,"WRONG PASSWORD!");
                }else if(count[y][x]==3){
                    attron(COLOR_PAIR(3));
                    mvprintw(0,32,"WRONG PASSWORD!");
                    ever[y][x]=1;
                }
            }
            break;}
            else if(ever[y][x]==-1){
                player_move(y,x,player,grid, steps);
                break;
            }
            else if(ever[y][x]==1){
                return 0;
            }
        case '$':
            attron(COLOR_PAIR(5));
            mvprintw(0,0,"Golds + 10 !!                                                           ");
            attroff(COLOR_PAIR(5));
            player->gold+=10;
            player->score+=10;
            mvprintw(GRID_HEIGHT-1,37,"%d",player->gold);
            mvprintw(GRID_HEIGHT-1,68,"%d",player->score);
            player_move(y,x,player,grid, steps);
            break;
        case 'b':
            attron(COLOR_PAIR(5));
            mvprintw(0,0,"Black Golds + 50 !!                                                                ");
            attroff(COLOR_PAIR(5));
            player->gold+=50;
            player->score+=50;
            mvprintw(GRID_HEIGHT-1,37,"%d",player->gold);
            mvprintw(GRID_HEIGHT-1,68,"%d",player->score);
            player_move(y,x,player,grid, steps);
            break;
        case 'f':
            if(foodnumber>5){
                break;
            }else{
                foodnumber+=1;
                player_move(y,x,player,grid, steps);
                break;
            }
        case 'A':
            player->has_arrow+=20;
            attron(COLOR_PAIR(1));
            mvprintw(0,0,"Collected 20 arrows                                                    ");
            attroff(COLOR_PAIR(1));
            player_move(y,x,player,grid, steps);
            break;
        case 's':
            player->has_sword=1;
            attron(COLOR_PAIR(1));
            mvprintw(0,0,"Collected sword                                                    ");
            attroff(COLOR_PAIR(1));
            player_move(y,x,player,grid, steps);
            break;
        case 'd':
            player->has_dagger+=10;
            attron(COLOR_PAIR(1));
            mvprintw(0,0,"Collected 10 daggers                                                    ");
            attroff(COLOR_PAIR(1));
            player_move(y,x,player,grid, steps);
            break;
        case 'W':
            player->has_wand+=8;
            attron(COLOR_PAIR(1));
            mvprintw(0,0,"Collected 8 wands                                                    ");
            attroff(COLOR_PAIR(1));
            player_move(y,x,player,grid, steps);
            break;
        case '(':
            speednumber+=1;
            attron(COLOR_PAIR(4));
            mvprintw(0,0,"Collected Speed Spell                                                    ");
            attroff(COLOR_PAIR(4));
            player_move(y,x,player,grid, steps);
            break;
        case ')':
            damagenumber+=1;
            attron(COLOR_PAIR(4));
            mvprintw(0,0,"Collected Damage Spell                                                    ");
            attroff(COLOR_PAIR(4));
            player_move(y,x,player,grid, steps);
            break;
        case '*':
            healthnumber+=1;
            attron(COLOR_PAIR(4));
            mvprintw(0,0,"Collected Health Spell                                                    ");
            attroff(COLOR_PAIR(4));
            player_move(y,x,player,grid, steps);
            break;
        case 'u':
        case '?':
        case '+':
            for (int i=x-1; i>=x-5;i--){
                    if(grid[y][i]=='#'){
                        mvprintw(y,i,"#");
                        visit[y][i]=1;
                    }else if(grid[y][i]=='+'){
                        mvprintw(y,i,"+");
                        visit[y][i]=1;
                    }
                    if(grid[y][i]!='#' && grid[y][i]!='+')
                        break;
            }for (int i=x+1; i<=x+5;i++){
                    if(grid[y][i]=='#'){
                        mvprintw(y,i,"#");
                        visit[y][i]=1;
                    }else if(grid[y][i]=='+'){
                        mvprintw(y,i,"+");
                        visit[y][i]=1;
                    }if(grid[y][i]!='#' && grid[y][i]!='+')
                        break;
            }
            for (int i=y-1; i>=y-5;i--){
                    if(grid[i][x]=='#'){
                        mvprintw(i,x,"#");
                        visit[i][x]=1;
                    }else if(grid[i][x]=='+'){
                        mvprintw(i,x,"+");
                        visit[i][x]=1;
                    }if (grid[i][x]!='#' && grid[i][x]!='+')
                        break;
            }for (int i=y+1; i<=y+5;i++){
                    if(grid[i][x]=='#'){
                        mvprintw(i,x,"#");
                        visit[i][x]=1;
                    }else if(grid[i][x]=='+'){
                        mvprintw(i,x,"+");
                        visit[i][x]=1;
                    }if(grid[i][x]!='#' && grid[i][x]!='+')
                        break;
            }
            player_move(y,x,player,grid, steps);
            if(x<(GRID_WIDTH/3) && y<(GRID_HEIGHT/2-1)){
                for (int i=rooms[0].position.y; i<rooms[0].position.y+rooms[0].height; i++){
                for(int j=rooms[0].position.x; j<rooms[0].position.x+rooms[0].width; j++){
                    visit[i][j]=1;
                    visit_count[0]=1;
                    
                    }
                }isinroom=0;
            }else if(x>=(GRID_WIDTH/3) && x<(GRID_WIDTH*2/3) && y<(GRID_HEIGHT/2-1)){
                for (int i=rooms[1].position.y; i<rooms[1].position.y+rooms[1].height; i++){
                for(int j=rooms[1].position.x; j<rooms[1].position.x+rooms[1].width; j++){
                    visit[i][j]=1;
                    visit_count[1]=1;
                    }
                }isinroom=1;
            }
            else if(x>=(GRID_WIDTH*2/3) && y<(GRID_HEIGHT/2-1)){
                for (int i=rooms[2].position.y; i<rooms[2].position.y+rooms[2].height; i++){
                for(int j=rooms[2].position.x; j<rooms[2].position.x+rooms[2].width; j++){
                    visit[i][j]=1;
                    visit_count[2]=1;
                    }
                }isinroom=2;
            }else if(x<(GRID_WIDTH/3) && y>=(GRID_HEIGHT/2-1)){
                for (int i=rooms[3].position.y; i<rooms[3].position.y+rooms[3].height; i++){
                for(int j=rooms[3].position.x; j<rooms[3].position.x+rooms[3].width; j++){
                    visit[i][j]=1;
                    visit_count[3]=1;
                    }
                }isinroom=3;
            }else if(x>=(GRID_WIDTH/3) && x<(GRID_WIDTH*2/3) && y>=(GRID_HEIGHT/2-1)){
                for (int i=rooms[4].position.y; i<rooms[4].position.y+rooms[4].height; i++){
                for(int j=rooms[4].position.x; j<rooms[4].position.x+rooms[4].width; j++){
                    visit[i][j]=1;
                    visit_count[4]=1;
                    }
                }isinroom=4;
            }else if(x>=(GRID_WIDTH*2/3) && y>=(GRID_HEIGHT/2-1)){
                for (int i=rooms[5].position.y; i<rooms[5].position.y+rooms[5].height; i++){
                for(int j=rooms[5].position.x; j<rooms[5].position.x+rooms[5].width; j++){
                    visit[i][j]=1;
                    visit_count[5]=1;
                    }
                }isinroom=5;
            }
            print_grid(grid, visit, player);
            break;
        case '#':
            isinroom=-1;
            gsteps=0;
            usteps=0;
            for (int i=x-1; i>=x-5;i--){
                    if(grid[y][i]=='#'){
                        mvprintw(y,i,"#");
                        visit[y][i]=1;
                    }else if(grid[y][i]=='+'){
                        mvprintw(y,i,"+");
                        visit[y][i]=1;
                    }
                    if(grid[y][i]!='#' && grid[y][i]!='+')
                        break;
            }for (int i=x+1; i<=x+5;i++){
                    if(grid[y][i]=='#'){
                        mvprintw(y,i,"#");
                        visit[y][i]=1;
                    }else if(grid[y][i]=='+'){
                        mvprintw(y,i,"+");
                        visit[y][i]=1;
                    }if(grid[y][i]!='#' && grid[y][i]!='+')
                        break;
            }
            for (int i=y-1; i>=y-5;i--){
                    if(grid[i][x]=='#'){
                        mvprintw(i,x,"#");
                        visit[i][x]=1;
                    }else if(grid[i][x]=='+'){
                        mvprintw(i,x,"+");
                        visit[i][x]=1;
                    }if (grid[i][x]!='#' && grid[i][x]!='+')
                        break;
            }for (int i=y+1; i<=y+5;i++){
                    if(grid[i][x]=='#'){
                        mvprintw(i,x,"#");
                        visit[i][x]=1;
                    }else if(grid[i][x]=='+'){
                        mvprintw(i,x,"+");
                        visit[i][x]=1;
                    }if(grid[i][x]!='#' && grid[i][x]!='+')
                        break;
            }
            player_move(y,x,player,grid, steps);
            break;
        case 'p':
        case '^':
        case '.':
            player_move(y,x,player,grid, steps);
            break;
        case 'D':
        case 'F':
        case 'G':
        case 'S':
        case 'U':
            if(selectedweapon!=-1)
                player_move(y,x,player,grid, steps);
            else{
                attron(COLOR_PAIR(4));
                mvprintw(0,0,"Please choose a weapon first!                                                          ");
                attroff(COLOR_PAIR(4));
            }break;
        case '<':
            return 1;
        default:
            break;
    }
}


int flag=0;
int foodcount=0;

int player_move(int y, int x, Player *player,char grid[GRID_HEIGHT][GRID_WIDTH], int *steps){
    totalsteps+=1;
    if(healthtime>0){
        healthtime-=1;
        if(healthtime==0)
            healthactivated=1;
    }if(speedtime>0){
        speedtime-=1;
        if(speedtime==0)
            speedactivated=1;
    }
    if(healthactivated==2){
        if(totalsteps%(20)==0){
            if(player->health_point+(9-(player->difficulty)*2)<=100)
            player->health_point+=(9-(player->difficulty)*2);
        else    
            player->health_point=100;
        attron(COLOR_PAIR(1));
        mvprintw(0,0,"Hp +%d   %d!!                                                                  ",9-(player->difficulty)*2, healthactivated);
        attroff(COLOR_PAIR(1));
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        }
    }
    else{
    if(totalsteps%(40)==0 && (player->food)>(50+10*(player->difficulty))){
        if(player->health_point+(9-(player->difficulty)*2)<=100)
            player->health_point+=(9-(player->difficulty)*2);
        else    
            player->health_point=100;
        attron(COLOR_PAIR(1));
        mvprintw(0,0,"Hp +%d!!                                                              ",9-(player->difficulty)*2);
        attroff(COLOR_PAIR(1));
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
    }
    }
    attroff(COLOR_PAIR(player->color));
    if(grid[y][x]=='D' || grid[y][x]=='F' || grid[y][x]=='G' || grid[y][x]=='S' || grid[y][x]=='U'){
        attron(COLOR_PAIR(1));
        if(grid[y][x]=='D')
            mvprintw(0,0,"You killed a deamon!                                              ");
        else if(grid[y][x]=='F'){
            if(damagetime>0){
                damagetime-=1;
                if(damagetime==0)
                    damageactivated=1;
            }
            fbmonsters[isinroom].dhealth-=(selectedweapon/10)*damageactivated;
            if(fbmonsters[isinroom].dhealth<=0){
                mvprintw(0,0,"You killed a fire breathing monster!                                              ");
            }else{
                mvprintw(0,0,"The fire breathing monster was damaged %d units!                                        ",(selectedweapon/10)*damageactivated);
                attroff(COLOR_PAIR(1));
                if(grid[y-1][x]=='.' && !(player->position.y==y-1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(15));
                    mvprintw(y-1,x,"F");
                    attroff(COLOR_PAIR(15));
                    grid[y][x]='.';
                    grid[y-1][x]='F';
                }else if(grid[y+1][x]=='.' && !(player->position.y==y+1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(15));
                    mvprintw(y+1,x,"F");
                    attroff(COLOR_PAIR(15));
                    grid[y][x]='.';
                    grid[y+1][x]='F';
                }else if(grid[y][x-1]=='.' && !(player->position.y==y && player->position.x==x-1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(15));
                    mvprintw(y,x-1,"F");
                    attroff(COLOR_PAIR(15));
                    grid[y][x]='.';
                    grid[y][x-1]='F';
                }else if(grid[y][x+1]=='.' && !(player->position.y==y && player->position.x==x+1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(15));
                    mvprintw(y,x+1,"F");
                    attroff(COLOR_PAIR(15));
                    grid[y][x]='.';
                    grid[y][x+1]='F';
                }
                return 0;
            }
        }else if(grid[y][x]=='G'){
            if(damagetime>0){
                damagetime-=1;
                if(damagetime==0)
                    damageactivated=1;
            }
            giants[isinroom].dhealth-=(selectedweapon/10)*damageactivated;
            if(giants[isinroom].dhealth<=0){
                mvprintw(0,0,"You killed a Giant                                              ");
            }else{
                mvprintw(0,0,"The Giant was damaged %d units!                                        ",(selectedweapon/10)*damageactivated);
                attroff(COLOR_PAIR(1));
                if(grid[y-1][x]=='.' && !(player->position.y==y-1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(16));
                    mvprintw(y-1,x,"G");
                    attroff(COLOR_PAIR(16));
                    grid[y][x]='.';
                    grid[y-1][x]='G';
                }else if(grid[y+1][x]=='.' && !(player->position.y==y+1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(16));
                    mvprintw(y+1,x,"G");
                    attroff(COLOR_PAIR(16));
                    grid[y][x]='.';
                    grid[y+1][x]='G';
                }else if(grid[y][x-1]=='.' && !(player->position.y==y && player->position.x==x-1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(16));
                    mvprintw(y,x-1,"G");
                    attroff(COLOR_PAIR(16));
                    grid[y][x]='.';
                    grid[y][x-1]='G';
                }else if(grid[y][x+1]=='.' && !(player->position.y==y && player->position.x==x+1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(16));
                    mvprintw(y,x+1,"G");
                    attroff(COLOR_PAIR(16));
                    grid[y][x]='.';
                    grid[y][x+1]='G';
                }
                return 0;
            }
        }else if(grid[y][x]=='S'){
            if(damagetime>0){
                damagetime-=1;
                if(damagetime==0)
                    damageactivated=1;
            }
            snakes[isinroom].dhealth-=(selectedweapon/10)*damageactivated;
            if(snakes[isinroom].dhealth<=0){
                mvprintw(0,0,"You killed a Snake                                              ");
            }else{
                mvprintw(0,0,"The Snake was damaged %d units!                                        ",(selectedweapon/10)*damageactivated);
                attroff(COLOR_PAIR(1));
                if(grid[y-1][x]=='.' && !(player->position.y==y-1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(17));
                    mvprintw(y-1,x,"S");
                    attroff(COLOR_PAIR(17));
                    grid[y][x]='.';
                    grid[y-1][x]='S';
                }else if(grid[y+1][x]=='.' && !(player->position.y==y+1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(17));
                    mvprintw(y+1,x,"S");
                    attroff(COLOR_PAIR(17));
                    grid[y][x]='.';
                    grid[y+1][x]='S';
                }else if(grid[y][x-1]=='.' && !(player->position.y==y && player->position.x==x-1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(17));
                    mvprintw(y,x-1,"S");
                    attroff(COLOR_PAIR(17));
                    grid[y][x]='.';
                    grid[y][x-1]='S';
                }else if(grid[y][x+1]=='.' && !(player->position.y==y && player->position.x==x+1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(17));
                    mvprintw(y,x+1,"S");
                    attroff(COLOR_PAIR(17));
                    grid[y][x]='.';
                    grid[y][x+1]='S';
                }
                return 0;
            }
        }else if(grid[y][x]=='U'){
            if(damagetime>0){
                damagetime-=1;
                if(damagetime==0)
                    damageactivated=1;
            }
            snakes[isinroom].dhealth-=(selectedweapon/10)*damageactivated;
            if(snakes[isinroom].dhealth<=0){
                mvprintw(0,0,"You killed an Undeed                                              ");
            }else{
                mvprintw(0,0,"The Undeed was damaged %d units!                                        ",(selectedweapon/10)*damageactivated);
                attroff(COLOR_PAIR(1));
                if(grid[y-1][x]=='.' && !(player->position.y==y-1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(17));
                    mvprintw(y-1,x,"U");
                    attroff(COLOR_PAIR(17));
                    grid[y][x]='.';
                    grid[y-1][x]='U';
                }else if(grid[y+1][x]=='.' && !(player->position.y==y+1 && player->position.x==x)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(17));
                    mvprintw(y+1,x,"U");
                    attroff(COLOR_PAIR(17));
                    grid[y][x]='.';
                    grid[y+1][x]='U';
                }else if(grid[y][x-1]=='.' && !(player->position.y==y && player->position.x==x-1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(17));
                    mvprintw(y,x-1,"U");
                    attroff(COLOR_PAIR(17));
                    grid[y][x]='.';
                    grid[y][x-1]='U';
                }else if(grid[y][x+1]=='.' && !(player->position.y==y && player->position.x==x+1)){
                    mvprintw(y,x,".");
                    attron(COLOR_PAIR(18));
                    mvprintw(y,x+1,"U");
                    attroff(COLOR_PAIR(18));
                    grid[y][x]='.';
                    grid[y][x+1]='U';
                }
                return 0;
            }
        }attroff(COLOR_PAIR(1));
        grid[y][x]='.';
    }else if(grid[player->position.y][player->position.x-1]=='D' || grid[player->position.y-1][player->position.x]=='D'
    || grid[player->position.y][player->position.x+1]=='D' || grid[player->position.y+1][player->position.x]=='D'){
        player->health_point-=(4*(player->difficulty+1));
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        attron(COLOR_PAIR(3));
        mvprintw(0,0,"Lost %d health points due to deamon's attack!!                                                      ",3*(player->difficulty+1));
        attroff(COLOR_PAIR(3));
    }else if(grid[player->position.y][player->position.x-1]=='F' || grid[player->position.y-1][player->position.x]=='F'
    || grid[player->position.y][player->position.x+1]=='F' || grid[player->position.y+1][player->position.x]=='F'){
        player->health_point-=(4*(player->difficulty+1));
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        attron(COLOR_PAIR(3));
        mvprintw(0,0,"Lost %d health points due to fire Breathing Moster's attack!!                                     ",4*(player->difficulty+1));
        attroff(COLOR_PAIR(3));
    }else if(grid[player->position.y][player->position.x-1]=='G' || grid[player->position.y-1][player->position.x]=='G'
    || grid[player->position.y][player->position.x+1]=='G' || grid[player->position.y+1][player->position.x]=='G'){
        player->health_point-=(4*(player->difficulty+1));
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        attron(COLOR_PAIR(3));
        mvprintw(0,0,"Lost %d health points due to Giants's attack!!                                     ",5*(player->difficulty+1));
        attroff(COLOR_PAIR(3));
    }else if(grid[player->position.y][player->position.x-1]=='S' || grid[player->position.y-1][player->position.x]=='S'
    || grid[player->position.y][player->position.x+1]=='S' || grid[player->position.y+1][player->position.x]=='S'){
        player->health_point-=(5*(player->difficulty+1));
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        attron(COLOR_PAIR(3));
        mvprintw(0,0,"Lost %d health points due to Snake's attack!!                                     ",5*(player->difficulty+1));
        attroff(COLOR_PAIR(3));
    }else if(grid[player->position.y][player->position.x-1]=='U' || grid[player->position.y-1][player->position.x]=='U'
    || grid[player->position.y][player->position.x+1]=='U' || grid[player->position.y+1][player->position.x]=='U'){
        player->health_point-=(5*(player->difficulty+1));
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        attron(COLOR_PAIR(3));
        mvprintw(0,0,"Lost %d health points due to Undeed's attack!!                                     ",6*(player->difficulty+1));
        attroff(COLOR_PAIR(3));
    }
    else{
        int y=-1,x=-1;
        for(int i=player->position.y-2; i<=player->position.y+2;i++){
            for(int j=player->position.x-2; j<=player->position.x+2; j++){
                if(grid[i][j]=='F' && (y!=i || x!=j)){
                x=j;
                y=i;
                    if(player->position.x>j){
                        if(grid[y][x+1]!='|' && grid[y][x+1]!='_' && grid[y][x+1]!='O' && grid[y][x+1]!=' ' && grid[y][x+1]!='+'
                        && grid[y][x+1]!='@' && grid[y][x+1]!='p' && grid[y][x+1]!='?' && grid[y][x+1]!='#' && grid[y][x+1]!='D'
                        && grid[y][x+1]!='G' && grid[y][x+1]!='S' && grid[y][x+1]!='U' && !((x+1)==player->position.x && i==player->position.y)){
                            x+=1;
                        }
                    }else if(player->position.x<j){
                        if(grid[y][x-1]!='|' && grid[y][x-1]!='_' && grid[y][x-1]!='O' && grid[y][x-1]!=' ' && grid[y][x-1]!='+'
                        && grid[y][x-1]!='@' && grid[y][x-1]!='p' && grid[y][x-1]!='?' && grid[y][x-1]!='#' && grid[y][x-1]!='D'
                        && grid[y][x-1]!='G' && grid[y][x-1]!='S' && grid[y][x-1]!='U' && !((x-1)==player->position.x && i==player->position.y) ){
                            x-=1;
                        }
                    }if(player->position.y>i){
                        if(grid[y+1][x]!='|' && grid[y+1][x]!='_' && grid[y+1][x]!='O' && grid[y+1][x]!=' ' && grid[y+1][x]!='+'
                        && grid[y+1][x]!='@' && grid[y+1][x]!='p' && grid[y+1][x]!='?' && grid[y+1][x]!='#' && grid[y+1][x]!='D'
                        && grid[y+1][x]!='G' && grid[y+1][x]!='S' && grid[y+1][x]!='U' && !(x==player->position.x && (y+1)==player->position.y)){
                            y+=1;
                        }
                    }else if(player->position.y<i){
                        if(grid[y-1][x]!='|' && grid[y-1][x]!='_' && grid[y-1][x]!='O' && grid[y-1][x]!=' ' && grid[y-1][x]!='+'
                        && grid[y-1][x]!='@' && grid[y-1][x]!='p' && grid[y-1][x]!='?' && grid[y-1][x]!='#' && grid[y-1][x]!='D'
                        && grid[y-1][x]!='G' && grid[y-1][x]!='S' && grid[y-1][x]!='U' && !(x==player->position.x && (y-1)==player->position.y)){
                            y-=1;
                        }
                    }
                    if(visit[i][j]==1){
                        mvprintw(i,j,".");
                        attron(COLOR_PAIR(15));
                        mvprintw(y,x,"F");
                        attroff(COLOR_PAIR(15));
                        grid[i][j]='.';
                        grid[y][x]='F';
                    }
                }
            }
        }x=-1;
        y=-1;
        int x1=-1, x2=-1, y1=-1, y2=-1;
        for(int k=0; k<6; k++){
            if(isinroom==k && visit_count[k]==1){
                for(int i=rooms[k].position.y; i<rooms[k].position.y+rooms[k].height; i++){
                    for(int j=rooms[k].position.x; j<rooms[k].position.x+rooms[k].width; j++){
                        if(grid[i][j]=='G' && (y!=i || x!=j) && gsteps<=10){
                            x=j;
                            y=i;
                            if(player->position.x>j){
                                if(grid[y][x+1]!='|' && grid[y][x+1]!='_' && grid[y][x+1]!='O' && grid[y][x+1]!=' ' && grid[y][x+1]!='+'
                                && grid[y][x+1]!='@' && grid[y][x+1]!='p' && grid[y][x+1]!='?' && grid[y][x+1]!='#' && grid[y][x+1]!='D'
                                && grid[y][x+1]!='F' && grid[y][x+1]!='S' && grid[y][x+1]!='U' && !((x+1)==player->position.x && i==player->position.y)){
                                    x+=1;
                                }
                            }else if(player->position.x<j){
                                if(grid[y][x-1]!='|' && grid[y][x-1]!='_' && grid[y][x-1]!='O' && grid[y][x-1]!=' ' && grid[y][x-1]!='+'
                                && grid[y][x-1]!='@' && grid[y][x-1]!='p' && grid[y][x-1]!='?' && grid[y][x-1]!='#' && grid[y][x-1]!='D'
                                && grid[y][x-1]!='F' && grid[y][x-1]!='S' && grid[y][x-1]!='U' && !((x-1)==player->position.x && i==player->position.y) ){
                                    x-=1;
                                }
                            }if(player->position.y>i){
                                if(grid[y+1][x]!='|' && grid[y+1][x]!='_' && grid[y+1][x]!='O' && grid[y+1][x]!=' ' && grid[y+1][x]!='+'
                                && grid[y+1][x]!='@' && grid[y+1][x]!='p' && grid[y+1][x]!='?' && grid[y+1][x]!='#' && grid[y+1][x]!='D'
                                && grid[y+1][x]!='F' && grid[y+1][x]!='S' && grid[y+1][x]!='U' && !(x==player->position.x && (y+1)==player->position.y)){
                                    y+=1;
                                }
                            }else if(player->position.y<i){
                                if(grid[y-1][x]!='|' && grid[y-1][x]!='_' && grid[y-1][x]!='O' && grid[y-1][x]!=' ' && grid[y-1][x]!='+'
                                && grid[y-1][x]!='@' && grid[y-1][x]!='p' && grid[y-1][x]!='?' && grid[y-1][x]!='#' && grid[y-1][x]!='D'
                                && grid[y-1][x]!='F' && grid[y-1][x]!='S' && grid[y-1][x]!='U' && !(x==player->position.x && (y-1)==player->position.y)){
                                    y-=1;
                                }
                            }
                            if(visit[i][j]==1){
                                gsteps++;
                                mvprintw(i,j,".");
                                attron(COLOR_PAIR(16));
                                mvprintw(y,x,"G");
                                attroff(COLOR_PAIR(16));
                                grid[i][j]='.';
                                grid[y][x]='G';
                            }
                        }
                        }
                    }
                }
            }x=-1;
        y=-1;
        for(int k=0; k<6; k++){
            if(isinroom==k && visit_count[k]==1){
                for(int i=rooms[k].position.y; i<rooms[k].position.y+rooms[k].height; i++){
                    for(int j=rooms[k].position.x; j<rooms[k].position.x+rooms[k].width; j++){
                        if(grid[i][j]=='S' && (y!=i || x!=j)){
                            x=j;
                            y=i;
                            if(player->position.x>j){
                                if(grid[y][x+1]!='|' && grid[y][x+1]!='_' && grid[y][x+1]!='O' && grid[y][x+1]!=' ' && grid[y][x+1]!='+'
                                && grid[y][x+1]!='@' && grid[y][x+1]!='p' && grid[y][x+1]!='?' && grid[y][x+1]!='#' && grid[y][x+1]!='D'
                                && grid[y][x+1]!='F' && grid[y][x+1]!='U' && grid[y][x+1]!='G' && !((x+1)==player->position.x && i==player->position.y)){
                                    x+=1;
                                }
                            }else if(player->position.x<j){
                                if(grid[y][x-1]!='|' && grid[y][x-1]!='_' && grid[y][x-1]!='O' && grid[y][x-1]!=' ' && grid[y][x-1]!='+'
                                && grid[y][x-1]!='@' && grid[y][x-1]!='p' && grid[y][x-1]!='?' && grid[y][x-1]!='#' && grid[y][x-1]!='D'
                                && grid[y][x-1]!='F' && grid[y][x-1]!='U' && grid[y][x-1]!='G' && !((x-1)==player->position.x && i==player->position.y) ){
                                    x-=1;
                                }
                            }if(player->position.y>i){
                                if(grid[y+1][x]!='|' && grid[y+1][x]!='_' && grid[y+1][x]!='O' && grid[y+1][x]!=' ' && grid[y+1][x]!='+'
                                && grid[y+1][x]!='@' && grid[y+1][x]!='p' && grid[y+1][x]!='?' && grid[y+1][x]!='#' && grid[y+1][x]!='D'
                                && grid[y+1][x]!='F' && grid[y+1][x]!='U' && grid[y+1][x]!='G' && !(x==player->position.x && (y+1)==player->position.y)){
                                    y+=1;
                                }
                            }else if(player->position.y<i){
                                if(grid[y-1][x]!='|' && grid[y-1][x]!='_' && grid[y-1][x]!='O' && grid[y-1][x]!=' ' && grid[y-1][x]!='+'
                                && grid[y-1][x]!='@' && grid[y-1][x]!='p' && grid[y-1][x]!='?' && grid[y-1][x]!='#' && grid[y-1][x]!='D'
                                && grid[y-1][x]!='F' && grid[y-1][x]!='U' && grid[y-1][x]!='G' && !(x==player->position.x && (y-1)==player->position.y)){
                                    y-=1;
                                }
                            }
                            if(visit[i][j]==1){
                                mvprintw(i,j,".");
                                attron(COLOR_PAIR(17));
                                mvprintw(y,x,"S");
                                attroff(COLOR_PAIR(17));
                                grid[i][j]='.';
                                grid[y][x]='S';
                            }
                        }
                        }
                    }
                }
            }x=-1;
        y=-1;
        for(int k=0; k<6; k++){
            if(isinroom==k && visit_count[k]==1){
                for(int i=rooms[k].position.y; i<rooms[k].position.y+rooms[k].height; i++){
                    for(int j=rooms[k].position.x; j<rooms[k].position.x+rooms[k].width; j++){
                        if(grid[i][j]=='U' && (y!=i || x!=j) && usteps<=15){
                            x=j;
                            y=i;
                            if(player->position.x>j){
                                if(grid[y][x+1]!='|' && grid[y][x+1]!='_' && grid[y][x+1]!='O' && grid[y][x+1]!=' ' && grid[y][x+1]!='+'
                                && grid[y][x+1]!='@' && grid[y][x+1]!='p' && grid[y][x+1]!='?' && grid[y][x+1]!='#' && grid[y][x+1]!='D'
                                && grid[y][x+1]!='F' && grid[y][x+1]!='S' && grid[y][x+1]!='G' && !((x+1)==player->position.x && i==player->position.y)){
                                    x+=1;
                                }
                            }else if(player->position.x<j){
                                if(grid[y][x-1]!='|' && grid[y][x-1]!='_' && grid[y][x-1]!='O' && grid[y][x-1]!=' ' && grid[y][x-1]!='+'
                                && grid[y][x-1]!='@' && grid[y][x-1]!='p' && grid[y][x-1]!='?' && grid[y][x-1]!='#' && grid[y][x-1]!='D'
                                && grid[y][x-1]!='F' && grid[y][x-1]!='S' && grid[y][x-1]!='G' && !((x-1)==player->position.x && i==player->position.y) ){
                                    x-=1;
                                }
                            }if(player->position.y>i){
                                if(grid[y+1][x]!='|' && grid[y+1][x]!='_' && grid[y+1][x]!='O' && grid[y+1][x]!=' ' && grid[y+1][x]!='+'
                                && grid[y+1][x]!='@' && grid[y+1][x]!='p' && grid[y+1][x]!='?' && grid[y+1][x]!='#' && grid[y+1][x]!='D'
                                && grid[y+1][x]!='F' && grid[y+1][x]!='S' && grid[y+1][x]!='G' && !(x==player->position.x && (y+1)==player->position.y)){
                                    y+=1;
                                }
                            }else if(player->position.y<i){
                                if(grid[y-1][x]!='|' && grid[y-1][x]!='_' && grid[y-1][x]!='O' && grid[y-1][x]!=' ' && grid[y-1][x]!='+'
                                && grid[y-1][x]!='@' && grid[y-1][x]!='p' && grid[y-1][x]!='?' && grid[y-1][x]!='#' && grid[y-1][x]!='D'
                                && grid[y-1][x]!='F' && grid[y-1][x]!='S' && grid[y-1][x]!='G' && !(x==player->position.x && (y-1)==player->position.y)){
                                    y-=1;
                                }
                            }
                            if(visit[i][j]==1){
                                usteps++;
                                mvprintw(i,j,".");
                                attron(COLOR_PAIR(18));
                                mvprintw(y,x,"U");
                                attroff(COLOR_PAIR(18));
                                grid[i][j]='.';
                                grid[y][x]='U';
                            }
                        }
                        }
                    }
                }
            }
    }

    if((*steps)%(7*(5-player->difficulty))==0 && *steps!=0){
            player->food-=4;
            foodcount-=4;
            flag=0;
            mvprintw(GRID_HEIGHT-1,52,"%d ", player->food);
        }
    if(spellsteps!=0 && spellsteps%20==0){
        player->health_point-=5;
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        spellsteps+=1;
    }
    if (foodcount!=0 && foodcount%(4*(player->difficulty+1))==0 && flag==0 && player->food<50){
        player->health_point-=4*(1+player->difficulty);
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        flag=1;
    }if(player->health_point<=0 || player->food<=0)
        isdead=1;
    if(steps2-(*steps)==-30){
        for (int i=0; i<(GRID_WIDTH); i++)
            mvprintw(0,i," ");
    }if (grid[player->position.y][player->position.x]=='#'
        || grid[player->position.y][player->position.x]=='+'
        || grid[player->position.y][player->position.x]=='?'){
            if(isspell[player->position.y][player->position.x]==1){
                attron(COLOR_PAIR(11));
                spellsteps=0;
        }mvprintw(player->position.y,player->position.x,"%c",grid[player->position.y][player->position.x]);
        int sum=0;
        for(int i=0; i<6; i++){
            sum+=visit_count[i];
        }if(isstair==0){
            if(sum==6){
                int room= rand() % 6;
        
            do{
            stairs.position.x= rand() % (rooms[room].width - 2) + 1 + rooms[room].position.x;
            stairs.position.y= rand() % (rooms[room].height - 2) + 1 + rooms[room].position.y;
            }while(grid[stairs.position.y][stairs.position.x]=='p'
                || grid[stairs.position.y][stairs.position.x]=='O'
                || grid[stairs.position.y][stairs.position.x]=='$');
            attron(COLOR_PAIR(4));
            mvprintw(0,0,"Stairs Appeared                                                               ");
            if(player->level<=3)
                mvprintw(stairs.position.y, stairs.position.x, "<");
            else{
                attron(COLOR_PAIR(3));
                mvprintw(stairs.position.y, stairs.position.x, "🏲");
                attroff(COLOR_PAIR(3));
            }grid[stairs.position.y][stairs.position.x]='<';
            refresh();
            isstair=1;
            }
        }attroff(COLOR_PAIR(11));
    }
    else if(grid[player->position.y][player->position.x]=='^'){
        attron(COLOR_PAIR(5));
        mvprintw(player->position.y,player->position.x,"∧");
        attroff(COLOR_PAIR(5));
    }
    else if(grid[player->position.y][player->position.x]=='p'){
        player->health_point-=10;
        player->score-=10;
        mvprintw(GRID_HEIGHT-1,24,"%d ", player->health_point);
        mvprintw(GRID_HEIGHT-1,68,"%d     ",player->score);
        grid[player->position.y][player->position.x]='^';
        attron(COLOR_PAIR(5));
        mvprintw(player->position.y,player->position.x,"∧");
        mvprintw(0,0,"You walked on a trap! -10 hp                                                   ");
        attroff(COLOR_PAIR(5));
    }else if(grid[player->position.y][player->position.x]=='u'){
        if(isspell[player->position.y][player->position.x]==1){
            attron(COLOR_PAIR(11));
            spellsteps=0;
        }grid[player->position.y][player->position.x]='?';
        mvprintw(player->position.y,player->position.x,"?");
        int sum=0;
        for(int i=0; i<6; i++){
            sum+=visit_count[i];
        }if(isstair==0){
            if(sum==6){
                int room= rand() % 6;
        
            do{
            stairs.position.x= rand() % (rooms[room].width - 2) + 1 + rooms[room].position.x;
            stairs.position.y= rand() % (rooms[room].height - 2) + 1 + rooms[room].position.y;
            }while(grid[stairs.position.y][stairs.position.x]=='p'
                || grid[stairs.position.y][stairs.position.x]=='O'
                || grid[stairs.position.y][stairs.position.x]=='$');
            attron(COLOR_PAIR(4));
            mvprintw(0,0,"Stairs Appeared                                                               ");
            if(player->level<=3)
                mvprintw(stairs.position.y, stairs.position.x, "<");
            else{
                attron(COLOR_PAIR(3));
                mvprintw(stairs.position.y, stairs.position.x, "🏲");
                attroff(COLOR_PAIR(3));
            }grid[stairs.position.y][stairs.position.x]='<';
            refresh();
            isstair=1;
            }
        }attroff(COLOR_PAIR(11));
    }else if(grid[player->position.y][player->position.x]=='&'){
        grid[player->position.y][player->position.x]='&';
        attron(COLOR_PAIR(10));
        mvprintw(player->position.y,player->position.x,"&");
    }else if(grid[player->position.y][player->position.x]=='@' && (passworddoors[player->position.y][player->position.x]==1)){
        attron(COLOR_PAIR(1));
        mvprintw(player->position.y,player->position.x,"@");
        attroff(COLOR_PAIR(1));
        int sum=0;
        for(int i=0; i<6; i++){
            sum+=visit_count[i];
        }if(isstair==0){
            if(sum==6){
                int room= rand() % 6;
        
            do{
            stairs.position.x= rand() % (rooms[room].width - 2) + 1 + rooms[room].position.x;
            stairs.position.y= rand() % (rooms[room].height - 2) + 1 + rooms[room].position.y;
            }while(grid[stairs.position.y][stairs.position.x]=='p'
                || grid[stairs.position.y][stairs.position.x]=='O'
                || grid[stairs.position.y][stairs.position.x]=='$');
            attron(COLOR_PAIR(4));
            mvprintw(0,0,"Stairs Appeared                                                               ");
            if(player->level<=3)
                mvprintw(stairs.position.y, stairs.position.x, "<");
            else{
                attron(COLOR_PAIR(3));
                mvprintw(stairs.position.y, stairs.position.x, "🏲");
                attroff(COLOR_PAIR(3));
            }grid[stairs.position.y][stairs.position.x]='<';
            refresh();
            isstair=1;
            }
        }
    }else if((grid[player->position.y][player->position.x]=='@' && passworddoors[player->position.y][player->position.x]==0)
            || (grid[player->position.y][player->position.x]=='@' && ever[y][x]==1)){
        attron(COLOR_PAIR(3));
        mvprintw(player->position.y,player->position.x,"@");
        attroff(COLOR_PAIR(3));
    }
    else{
        if(isspell[player->position.y][player->position.x]==1){
            attron(COLOR_PAIR(11));
            spellsteps+=1;
        }attroff(COLOR_PAIR(13));
        
        if(grid[player->position.y][player->position.x]!='D' && grid[player->position.y][player->position.x]!='F'  && grid[player->position.y][player->position.x]!='G'
        && grid[player->position.y][player->position.x]!='S' && grid[player->position.y][player->position.x]!='U'){
            mvprintw(player->position.y,player->position.x,".");
            grid[player->position.y][player->position.x]='.';
        }else if(grid[player->position.y][player->position.x]=='D'){
            attron(COLOR_PAIR(14));
            mvprintw(player->position.y,player->position.x,"D");
            grid[player->position.y][player->position.x]='D';
            attroff(COLOR_PAIR(14));
        }else if(grid[player->position.y][player->position.x]=='F'){
            attron(COLOR_PAIR(15));
            mvprintw(player->position.y,player->position.x,"F");
            grid[player->position.y][player->position.x]='F';
            attroff(COLOR_PAIR(15));
        }else if(grid[player->position.y][player->position.x]=='G'){
            attron(COLOR_PAIR(16));
            mvprintw(player->position.y,player->position.x,"G");
            grid[player->position.y][player->position.x]='G';
            attroff(COLOR_PAIR(16));
        }else if(grid[player->position.y][player->position.x]=='S'){
            attron(COLOR_PAIR(17));
            mvprintw(player->position.y,player->position.x,"S");
            grid[player->position.y][player->position.x]='S';
            attroff(COLOR_PAIR(17));
        }else if(grid[player->position.y][player->position.x]=='U'){
            attron(COLOR_PAIR(18));
            mvprintw(player->position.y,player->position.x,"U");
            grid[player->position.y][player->position.x]='U';
            attroff(COLOR_PAIR(18));
        }
        attroff(COLOR_PAIR(11));
    }refresh();
    player->position.y=y;
    player->position.x=x;
    attron(COLOR_PAIR(player->color));
    mvprintw(player->position.y,player->position.x,"%s", playerr);
    move(player->position.y, player->position.x);
    refresh();
    return 0;
}