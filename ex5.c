/***********
 ID: 333135440
 NAME: Peleg Sacher
***********/

/*
    Makima or Reze?
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/**********************
    Length validation 
    defines
***********************/
#define GOOD_LENGTH 8
#define ZERO '0'
#define HIGHEST_DIGIT '9'
#define HIGHEST_TENS_DIGIT '5'

#define EQUAL_STRING 0
#define END_OF_STRING '\0'


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

/* HELPER FUNCTIONS */

//for adding a show
void placeShowInDB(char* show);

//for removing a show
void shrinkDB();
void reorganizeDataBaseForReduction();

//general helpers
int howMuchSpaceInDB();
void moveShowsDB();
int doesSeasonExist(TVShow *show, char *name);
int doesEpisodeExist(TVShow *show, char *episodeName, char *seasonName);

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
        freeAll();
        exit(1);
    }

    if(findShow(name) != NULL){
        printf("Show already exists.\n");
        free(name);
        return;
    }

    placeShowInDB(name);
}


void placeShowInDB(char* show){
    TVShow *temp, *newShow;
    
    newShow = (TVShow*)calloc(1, sizeof(TVShow));
    /* handling memory allocation errors*/
    if(newShow == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the new show in memory");
        freeAll();
        exit(1);
    }
    newShow -> name = show;
    newShow -> seasons = NULL;



    /* if not enough space increasing the dbSize*/
    if(howMuchSpaceInDB() == 0)
        expandDB();

    
    /* adding the new show and organizing it alphabetically*/
    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if (database[i][j] == NULL){
                database[i][j] = newShow;
                return;
            }
            if (strcmp(database[i][j] -> name, newShow -> name) > EQUAL_STRING){
                temp = database[i][j];
                database[i][j] = newShow;
                newShow = temp;
            }
        }
    }
}


void expandDB(){
    /* 
        in creasing the size of the data base
        by one more along each axis
    */
    dbSize++;
    database = (TVShow***)realloc(database, dbSize * sizeof(TVShow**));
    if(database == NULL){
        freeAll();
        exit(1);
    }
    for(int i = 0; i < dbSize - 1; i++){
        database[i] = (TVShow**)realloc(database[i], (dbSize) * sizeof(TVShow*));
        if(database[i] == NULL){
            freeAll();
            exit(1);
        }
        database[i][dbSize - 1] = NULL;
    }

    database[dbSize - 1] = (TVShow**)calloc(dbSize, sizeof(TVShow*));

    if(database[dbSize - 1] == NULL) {
        freeAll();
        exit(1);
    }

    /* moving the shows all the way to left to right and up to down*/
    moveShowsDB();
}


int howMuchSpaceInDB(){
    int counter = 0;
    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if (database[i][j] == NULL)
                counter++;
        }
    }
    return counter;
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
        pString[index] = END_OF_STRING;
        index++;   

        currChr = getchar();
    }

    return pString;
}




void deleteShow(){
    char* showName;
    printf("Enter the name of the show:\n");
    showName = getString();

    /* error handling*/
    if(showName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        free(showName);
        freeAll();
        exit(1);
    }

    /* misinput handling */
    if(findShow(showName) == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }

    /* frees the show from memory */
    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if(database[i][j])
            if (strcmp(database[i][j] -> name, showName) == EQUAL_STRING){
                freeShow(database[i][j]);
                database[i][j] = NULL;
                i = dbSize;

                break;
            }
        }
    }
    free(showName);

    /* if needed minimize the size of the data base */
    moveShowsDB();
    shrinkDB();
}


void moveShowsDB(){
    /* moves the shows in the following way:
        (0 is for null)
        for:
        1 0 2
        3 4 0
        0 0 0
        to:
        1 2 3
        4 0 0
        0 0 0
    */
    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if (database[i][j] != NULL)
                continue;

            if((j != dbSize - 1) && (database[i][j+1] != NULL)){
                database[i][j] = database[i][j+1];
                database[i][j+1] = NULL;
            }

            if((i != dbSize -1) && (j == dbSize - 1) && (database[i + 1][0] != NULL)){
                database[i][j] = database[i + 1][0];
                database[i + 1][0] = NULL;
            } 
        }
    }
}


void shrinkDB(){
    int takenSpaceDB = dbSize * dbSize - howMuchSpaceInDB();
    
    /* 
        checks if there is enough free space
         to decrease the database size
    */
    if (takenSpaceDB > (dbSize - 1) * (dbSize - 1)){
        return;
    }


    reorganizeDataBaseForReduction();
    
    /* reducing the space in memory of the DB */
    for (int i = 0; i < dbSize - 1; i++){
        database[i] = (TVShow**)realloc(database[i], (dbSize - 1) * sizeof(TVShow*));
    }
    free(database[dbSize - 1]);
    database = (TVShow***)realloc(database, (dbSize - 1) * sizeof(TVShow**));
    dbSize--;
}
  

void reorganizeDataBaseForReduction(){
    /*
        reorganizes the DB for the reduction of size in the following way
        from ordered left to right up to down to ordered the same way
        but with out the lase row and colunm
        (0 is for null in the example)
        from:
        1 2 3 
        4 0 0
        0 0 0
        to:
        1 2 0
        3 4 0
        0 0 0
    */
    for (int i = dbSize - 2; i > 0; i--){
        for(int j = dbSize - 2; j >= 0; j--){
            if(j >= i)
                database[i][j] = database[i][j-i];
            else
                database[i][j] = database[i - 1][dbSize - (i-j)];
            
        }
        database[i][dbSize - 1] = NULL;
    }
    database[0][dbSize - 1] = NULL;
}


void addSeason(){
    char *showName, *seasonName;
    int position;
    TVShow* show;
    Season *lastSeason, *newSeason, *nextSeason;

    /* getting all the input info */
    printf("Enter the name of the show:\n");
    showName = getString();

    if(showName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        freeAll();
        exit(1);
    }

    show = findShow(showName);
    if(show == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }

    printf("Enter the name of the season:\n");
    seasonName = getString();
    if(seasonName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the seasons name try again later");
        free(showName);
        freeAll();
        exit(1);
    }

    if(doesSeasonExist(show, seasonName)){
        printf("Season already exists.\n");
        free(showName);
        free(seasonName);
        return;
    }


    printf("Enter the position:\n");
    scanf(" %d", &position);
    

    /* placing the season */
    newSeason = (Season*)calloc(1, sizeof(Season));
    if(newSeason == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the new season in memory");
        free(showName);
        free(seasonName);
        freeAll();
        exit(1);
    }
    newSeason -> name = seasonName;
    newSeason -> next = NULL;
    /* in case the new season is the first */
    if ((position == 0) || (show -> seasons == NULL)){
        nextSeason = show -> seasons;
        show -> seasons = newSeason;
        newSeason -> next = nextSeason;

        free(showName);
        return;
    }


    nextSeason = show -> seasons;
    for(int i = 1; i <= position; i++){
        if(nextSeason == NULL)
            break;
        
        lastSeason = nextSeason;
        nextSeason = lastSeason -> next;
    }
    
    lastSeason -> next = newSeason;
    newSeason -> next = nextSeason;
    free(showName);
    return;
}


void deleteSeason(){
    char *showName, *seasonName;
    TVShow* show;
    Season *foundSeason;

    /* getting all the input info */
    printf("Enter the name of the show:\n");
    showName = getString();

    if(showName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        freeAll();
        exit(1);
    }

    show = findShow(showName);
    if(show == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }

    printf("Enter the name of the season:\n");
    seasonName = getString();
    if(seasonName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the seasons name try again later");
        free(showName);
        freeAll();
        exit(1);
    }


    /* searching for the season and deleting it*/
    foundSeason = findSeason(show, seasonName);
    if(foundSeason == NULL){
        printf("Season not found.");
        free(showName);
        free(seasonName);
        return;
    }
    
    free(showName);
    free(seasonName);
    freeSeason(foundSeason);
}


void freeSeason(Season* season){
    Episode *episode, *nextEpisode;

    if(season == NULL) 
        return;
    
    /* deleting episodes */
    episode = season -> episodes;
    while (episode != NULL){
        nextEpisode = episode -> next;
        freeEpisode(episode);
        episode = nextEpisode;
    }

    if(season -> name != NULL)
        free(season -> name);

    free(season);
}

//fix dete show by pointer
void freeShow(TVShow* show){
    Season *season, *nextSeason = NULL;
    if(show == NULL)
        return;

    /* deleting seasons */
    season = show -> seasons;
    while (season != NULL){
        nextSeason = season -> next;
        freeSeason(season);
        season = nextSeason;
    }

    free(show -> name);
    free(show);
}


void addEpisode(){
    char *showName, *seasonName, *episodeName, *length;
    int position;
    TVShow* show;
    Season *season;
    Episode *nextEpisode, *lastEpisode, *newEpisode;

    /* getting all the input info */
    printf("Enter the name of the show:\n");
    showName = getString();

    if(showName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        freeAll();
        exit(1);
    }

    show = findShow(showName);
    if(show == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }

    printf("Enter the name of the season:\n");
    seasonName = getString();
    if(seasonName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the seasons name try again later");
        free(showName);
        freeAll();
        exit(1);
    }

    if(!doesSeasonExist(show, seasonName)){
        printf("Season not found.\n");
        free(showName);
        free(seasonName);
        return;
    }

    printf("Enter the name of the episode:\n");
    episodeName = getString();
    if(episodeName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the episode name try again later");
        free(showName);
        free(seasonName);
        freeAll();
        exit(1);
    }

    if(doesEpisodeExist(show, episodeName, seasonName)){
        printf("Episode already exists.\n");
        free(showName);
        free(seasonName);
        free(episodeName);
        return;
    }


    printf("Enter the length (xx:xx:xx):\n");
    length = getString();

    while (!validLength(length)){
        printf("Invalid length, enter again:\n");
        free(length);
        length = getString();
    }


    printf("Enter the position:\n");
    scanf(" %d", &position);

    
    /* gets the season */
    season = show -> seasons;
    while (season != NULL){
        if (strcmp(season -> name, seasonName) == EQUAL_STRING)
            break;

        season = season -> next;
    }

    /* creates and puts the episode in */
    newEpisode = (Episode*)calloc(1, sizeof(Episode));
    newEpisode -> name = episodeName;
    newEpisode -> length = length;

    /* in case the new episode is the first */
    if ((position == 0) || (season -> episodes == NULL)){
        nextEpisode = season -> episodes;
        season -> episodes = newEpisode;
        newEpisode -> next = nextEpisode;
        free(showName);
        free(seasonName);
        return;
    }


    nextEpisode = season -> episodes;
    for(int i = 1; i <= position; i++){
        if(nextEpisode == NULL)
            break;
        
        lastEpisode = nextEpisode;
        nextEpisode = lastEpisode -> next;
    }
    
    lastEpisode -> next = newEpisode;
    newEpisode -> next = nextEpisode;


    free(showName);
    free(seasonName);
    return;
}


void deleteEpisode(){
    char *showName, *seasonName, *episodeName;
    TVShow* show;
    Season *season;
    Episode *foundEpisode;

    /* getting all the input info */
    printf("Enter the name of the show:\n");
    showName = getString();

    if(showName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        freeAll();
        exit(1);
    }

    show = findShow(showName);
    if(show == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }

    printf("Enter the name of the season:\n");
    seasonName = getString();
    if(seasonName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the seasons name try again later");
        free(showName);
        freeAll();
        exit(1);
    }

    if(!doesSeasonExist(show, seasonName)){
        printf("Season not found.\n");
        free(showName);
        free(seasonName);
        return;
    }

    printf("Enter the name of the episode:\n");
    episodeName = getString();
    if(episodeName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the episode name try again later");
        free(showName);
        free(seasonName);
        freeAll();
        exit(1);
    }

    if(!doesEpisodeExist(show, episodeName, seasonName)){
        printf("Episode not found.\n");
        free(showName);
        free(seasonName);
        free(episodeName);
        return;
    }

    /* searching for the season */
    season = show -> seasons;
    while (season != NULL){
        if (strcmp(season -> name, seasonName) == EQUAL_STRING)
            break;

        season = season -> next;
    }

    /* searching the episode and deleting it */
    foundEpisode = findEpisode(season, episodeName);
    if(foundEpisode == NULL){
        printf("Episode not found.");
        free(showName);
        free(seasonName);
        free(episodeName);
        return;
    }

    free(showName);
    free(seasonName);
    free(episodeName);
    freeEpisode(foundEpisode);
    return;
}


Episode *findEpisode(Season *season, char *name){
    /* finds episode and disconnecting it from linked list
       therefore only used in delete episode
    */
    Episode *lastEpisode, *foundEpisode, *nextEpisode;

    lastEpisode = season -> episodes;
    if(lastEpisode == NULL)
        return NULL;

    if (strcmp(lastEpisode -> name, name) == EQUAL_STRING){
        season -> episodes = lastEpisode -> next;
        return lastEpisode;
    }

    while (lastEpisode != NULL){
        nextEpisode = lastEpisode -> next;
        if (strcmp(nextEpisode -> name, name) == EQUAL_STRING){
            /* disconnecting the season from linked list */
            foundEpisode = nextEpisode;
            lastEpisode -> next = foundEpisode -> next;
            return foundEpisode;
        }

        lastEpisode = nextEpisode;
    }

    return NULL;
}


void freeEpisode(Episode* episode){
    if (episode == NULL)
        return;
    
    
    free(episode -> name);
    free(episode -> length);
    free(episode);
}



Season *findSeason(TVShow *show, char *name){
    /* finds season and disconnecting it from linked list
       therefore only used in delete season
    */
    Season *lastSeason, *foundSeason, *nextSeason;

    lastSeason = show -> seasons;
    if(lastSeason == NULL)
        return NULL;

    if (strcmp(lastSeason -> name, name) == EQUAL_STRING){
        show -> seasons = lastSeason -> next;
        return lastSeason;
    }

    while (lastSeason != NULL){
        nextSeason = lastSeason -> next;
        if (strcmp(nextSeason -> name, name) == EQUAL_STRING){
            /* disconnecting the season from linked list */
            foundSeason = nextSeason;
            lastSeason -> next = foundSeason -> next;
            return foundSeason;
        }

        lastSeason = nextSeason;
    }

    return NULL;
}


TVShow* findShow(char* show){
    /* returns null if show not in database */
    if (dbSize == 0)
        return NULL;

    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if(database[i][j] == NULL)
                return NULL;

            if (strcmp(database[i][j] -> name, show) == EQUAL_STRING){
                return database[i][j];
            }
        }
    }

    return NULL;
}


int doesSeasonExist(TVShow *show, char *name){
    Season* season;

    season = show -> seasons;
    while (season != NULL){
        if (strcmp(season -> name, name) == EQUAL_STRING)
            return TRUE;

        season = season -> next;
    }

    return FALSE;
}


int doesEpisodeExist(TVShow *show, char *episodeName, char *seasonName){
    Season* season;
    Episode* episode;

    /* going through all episodes */
    season = show -> seasons;
    while (season != NULL){
        if(strcmp(season -> name, seasonName) == EQUAL_STRING){
            episode = season -> episodes;
            while (episode != NULL){
                if(strcmp(episode -> name, episodeName) == EQUAL_STRING)
                    return TRUE;

                episode = episode -> next;
            }
            return FALSE;
        }
        season = season -> next;
    }
    return FALSE;
}


int validLength (char *length){
    //checks if the length is valid
    if (strlen(length) != 8)
        return FALSE;
    
    //not pretty but easy to understand and most importantly it works
    if((length[0] <= HIGHEST_DIGIT) && (length[0] >= ZERO) &&
       (length[1] <= HIGHEST_DIGIT) && (length[1] >= ZERO) &&
       (length[2] == ':') &&
       (length[3] <= HIGHEST_TENS_DIGIT) && (length[3] >= ZERO) &&
       (length[4] <= HIGHEST_DIGIT) && (length[4] >= ZERO) &&
       (length[5] == ':') &&
       (length[6] <= HIGHEST_TENS_DIGIT) && (length[6] >= ZERO) &&
       (length[7] <= HIGHEST_DIGIT) && (length[7] >= ZERO))
       return TRUE;
    
    return FALSE;       
}

void printShow(){
    char *showName;
    TVShow* show;
    Season* season;
    Episode* episode;
    int seasonCount = 0, episodeCount = 0;

    /* getting all the input info */
    printf("Enter the name of the show:\n");
    showName = getString();

    if(showName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        freeAll();
        exit(1);
    }

    show = findShow(showName);
    if(show == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }


    printf("Name: %s\nSeasons:\n", showName);
    season = show -> seasons;
    while (season != NULL){
        printf("    Season %d: %s\n", seasonCount, season -> name);

        episode = season -> episodes;
        while (episode != NULL){
            printf("        Episode %d: %s (%s)\n", episodeCount, episode -> name, episode -> length);
            episode = episode -> next;
            episodeCount++;
        }

        episodeCount = 0;
        seasonCount++;
        season = season -> next;
    }
    
    free(showName);
}

void printEpisode(){
    char *showName, *seasonName, *episodeName;
    TVShow* show;
    Season* season;
    Episode* nextEpisode;

    /* getting all the input info */
    printf("Enter the name of the show:\n");
    showName = getString();

    if(showName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the shows name try again later");
        freeAll();
        exit(1);
    }

    show = findShow(showName);
    if(show == NULL){
        printf("Show not found.\n");
        free(showName);
        return;
    }

    printf("Enter the name of the season:\n");
    seasonName = getString();
    if(seasonName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the seasons name try again later");
        free(showName);
        freeAll();
        exit(1);
    }

    if(!doesSeasonExist(show, seasonName)){
        printf("Season not found.\n");
        free(showName);
        free(seasonName);
        return;
    }

    printf("Enter the name of the episode:\n");
    episodeName = getString();
    if(episodeName == NULL){
        printf("sorry we have encountered a memory allocation error \nwhile trying to get the episodes name try again later");
        free(showName);
        free(seasonName);
        freeAll();
        exit(1);
    }

    if(!doesEpisodeExist(show, episodeName, seasonName)){
        printf("Episode not found.\n");
        free(showName);
        free(seasonName);
        free(episodeName);
        return;
    }
    //gets the season
    season = show -> seasons;
    while (season != NULL){
        if (strcmp(season -> name, seasonName) == EQUAL_STRING)
            break;
        season = season -> next;
    }

    //gets the episode
    nextEpisode = season -> episodes;
    while (nextEpisode != NULL){
        if (strcmp(nextEpisode -> name, episodeName) == EQUAL_STRING){
            printf("Name: %s\nLength: %s", nextEpisode -> name, nextEpisode -> length);
            free(showName);
            free(seasonName);
            free(episodeName);
            return;
        } 
        nextEpisode = nextEpisode -> next;
    }
}


void printArray(){
    for (int i = 0; i < dbSize; i++){
        for(int j = 0; j < dbSize; j++){
            if (database[i][j] != NULL){
                printf("[%s] ", (database[i][j] -> name));
            }
            else
                printf("[NULL] ");
        }
        printf("\n");
    }
}


void freeAll(){
    for(int i = 0; i < dbSize; i++){
        for (int j = 0; j < dbSize; j++){
            freeShow(database[i][j]);
        }
        free(database[i]);
    }
    free(database);
    dbSize = 0;
    return;
}