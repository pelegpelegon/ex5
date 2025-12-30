/***********
 ID: 333135440
 NAME: Peleg Sacher
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0


typedef struct Episode {
    char *name;
    char *length;
    struct Episode *next;
} Episode;

typedef struct Season {
    char *name;
    Episode *episodes;
    struct Season *next;
} Season;

typedef struct TVShow {
    char *name;
    Season *seasons;
} TVShow;

TVShow ***database = NULL;
int dbSize = 0;

char *getString();
int getInt();

int validLength(char *s);
int countShows();

void shrinkDB();
void expandDB();

bool isShowInDB(char* show);
bool isThereSpaceDB();
void placeShowInDB(char* show);
int increaseDBSize();



void freeEpisode(Episode *e);
void freeSeason(Season *s);
void freeShow(TVShow *show);
void freeAll();

TVShow *findShow(char *name);
Season *findSeason(TVShow *show, char *name);
Episode *findEpisode(Season *season, char *name);

void addShow();
void addSeason();
void addEpisode();

void deleteShow();
void deleteSeason();
void deleteEpisode();

void printEpisode();
void printShow();
void printArray();

void addMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Add a TV show\n");
    printf("2. Add a season\n");
    printf("3. Add an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: addShow(); break;
        case 2: addSeason(); break;
        case 3: addEpisode(); break;
    }
}

void deleteMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Delete a TV show\n");
    printf("2. Delete a season\n");
    printf("3. Delete an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: deleteShow(); break;
        case 2: deleteSeason(); break;
        case 3: deleteEpisode(); break;
    }
}

void printMenuSub() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Print a TV show\n");
    printf("2. Print an episode\n");
    printf("3. Print the array\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: printShow(); break;
        case 2: printEpisode(); break;
        case 3: printArray(); break;
    }
}

void mainMenu() {
    printf("Choose an option:\n");
    printf("1. Add\n");
    printf("2. Delete\n");
    printf("3. Print\n");
    printf("4. Exit\n");
}

int main() {
    int choice;
    do {
        mainMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: addMenu(); break;
            case 2: deleteMenu(); break;
            case 3: printMenuSub(); break;
            case 4: freeAll(); break;
        }
    } while (choice != 4);
    return 0;
}

void addShow(){
    char* name;
    printf("Enter the name of the show:\n");
    name = getString();

    if(name == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        return;
    }

    if(isShowInDB(name)){
        printf("Show already exists.\n");
        return;
    }

    printf("got to the good point\n%s\n", name);
    placeShowInDB(name);
    
}

bool isShowInDB(char* show){
    if (dbSize == 0)
        return FALSE;

    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if(database[i][j] == NULL)
                return FALSE;

            if (strcmp(database[i][j] -> name, show) == 0){
                return TRUE;
            }
        }
    }

    return FALSE;
}

void placeShowInDB(char* show){
    TVShow *temp, *newShow = calloc(1, sizeof(TVShow));
    if(temp == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the new show in memory");
        exit(1);
    }

    newShow -> name = show;

    if(!isThereSpaceDB())
        increaseDBSize();

    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if (database[i][j] == NULL){
                database[i][j] = newShow;
                return;
            }
            if (strcmp(database[i][j] -> name, newShow -> name) > 0){
                temp = database[i][j];
                database[i][j] = newShow;
                newShow = temp;
            }
        }
    }
}

int increaseDBSize(){
    TVShow ***temp;

    dbSize++;
    temp = realloc(database, dbSize * sizeof(TVShow**));
    if(temp == NULL)
        exit(1);

    database = temp;
    database [dbSize - 1] = NULL;

    for(int i = 0; i < dbSize; i++){
        database[i] = realloc(database[i], (dbSize) * sizeof(TVShow*));
        if(database[i] == NULL)
            exit(1);

        database[i][dbSize - 1] = NULL;
    }
}


bool isThereSpaceDB(){
    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if (database[i][j] == NULL){
                return TRUE;
            }
        }
    }
    return FALSE;
}

char* getString(){
    char currChr, *pString = NULL;
    int index = 1;

    /* handles user not entering anything */
    do{
        scanf("%c[^\n]", &currChr);
    }while(currChr == '\n');

    /* dynamically increase string size to get the full show name */
    while(currChr != '\n'){
        pString = realloc(pString, (index + 1) * sizeof(char));

        if(pString == NULL)
            return NULL;
             
        pString[index - 1] = currChr;
        pString[index] = '\0';
        index++;   

        currChr = getchar();
    }

    return pString;
}

void printShow(){
    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if (database[i][j] != NULL){
                printf("\n%s\n", (database[i][j] -> name));
            }
            else
                printf("\nnull here\n");
        }
    }
}










void addSeason(){}

void addEpisode(){}


void deleteShow(){}
void deleteSeason(){}
void deleteEpisode(){}

void printEpisode(){}
void printArray(){}
void freeAll(){}