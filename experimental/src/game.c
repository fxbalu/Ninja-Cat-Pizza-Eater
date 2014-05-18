/**
 * \file game.c
 * \brief contains the necessary functions to create a new game, initialize it, load it or to close it
 *
 * Implementation of createGame_(), loadGame(), initGame() and destroyGame().
 *
 */

#include "game.h"
#include "font.h"
#include "map.h"
#include "input.h"
#include "draw.h"
#include "player.h"
#include "sound.h"

/*Fonction qui initialise le jeu*/

Game* createGame(){

    Game* game;

    if((game = (Game*)malloc(sizeof(Game))) == NULL){
        fprintf(stderr, "can't allocate memory for a Game\n");
    }
    else{

        game->screen = NULL;
        game->tileMenu = NULL;
        game->gameover = NULL;
        game->endLevel = NULL;
        game->HUD_coin = NULL;
        game->HUD_life = NULL;

        game->coin = 0;
        game->life = 0;

        game->music = NULL;
        game->miniondead_sound = NULL;
        game->jump_sound = NULL;
        game->coin_sound = NULL;
        game->deadplayer1_sound = NULL;
        game->switch_sound = NULL;

        game->objectNumber = 0;
        game->choice = 0;
        game->onMenu = 0;
        game->menuType = 0;
        game->go = 1;
        game->monsterinitialized =0;

        game->map = NULL;
        game->player = NULL;
        game->input = NULL;
        game->fontHUD = NULL;
        game->fontMenu = NULL;
        game->fontGameover = NULL;

    }

    return game;

}

void initGame(char *title, Game* game){

    if(SDL_Init(SDL_INIT_VIDEO)<0)          /*initialise SDL video*/
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* On crée la fenêtre, représentée par le pointeur jeu.screen en utilisant la largeur et la
    hauteur définies dans les defines (defs.h). On utilise aussi la mémoire vidéo
    (SDL_HWPALETTE) et le double buffer pour éviter que ça scintille
    (SDL_DOUBLEBUF) */

    game->screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_HWPALETTE|SDL_DOUBLEBUF);


    if(game->screen == NULL)
    {
        printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption(title,NULL); /*on affiche le titre de la fenetre*/

    SDL_ShowCursor(SDL_DISABLE);

    if(TTF_Init()<0)
    {
        printf("couldn't initialize  SDL_TTF: %s\n",SDL_GetError());
        exit(1);
    }
    game->fontHUD = loadFont("data/font/font1.ttf", 65);
    game->fontMenu = loadFont("data/font/font1.ttf", 45);
    game->fontGameover = loadFont("data/font/font1.ttf",65);


    int flags = MIX_INIT_FLAC; // Le mp3 ne marchait pas
    int initted = Mix_Init(flags);

    printf("%d\n",flags);
    printf("%d\n",initted);

    if( (initted & flags) != flags)
    {
        printf("Mix_Init: Failed to init SDL_Mixer\n");
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    /* open 44.1KHz, signed 16bit, system byte order,
           stereo audio, using 1024 byte chunks */
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
    {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(1);
    }

    /* Définit le nombre de channels à mixer */
    Mix_AllocateChannels(32);

    game->level = 1;
    game->onMenu = 1;
    game->choice = 0;
    game->menuType = START;
    game->map = createMap();
    game->input = createInput();
    game->player = createPlayer();

}

void loadGame(Game* game){

    char file[200];

    game->map->background = loadImage("data/graphics/backgroundworld1.png");
    game->map->backgroundMenu = loadImage("data/graphics/splashScreen.png");
    game->tileMenu = loadImage("data/graphics/spriteSheetMenu.png");
    game->map->tileSet = loadImage("data/graphics/all_tileset.png");
    game->gameover = loadImage("data/graphics/gameover.png");
    game->endLevel = loadImage("data/graphics/splashScreenEndLevel.png");
    game->HUD_life = loadImage("data/graphics/lifeHud.png");
    game->HUD_coin = loadImage("data/graphics/hud_coins.png");

    loadSong(-1,"data/music/Those of Us Who Fight.mp3",game);

    loadSound(game);


    /* Charge la map depuis le fichier */
    sprintf(file, "data/map/map%d.tmx", game->level );
    loadMap(file, game->map, game);

    game->coin = 0;
    game->life = 1;

 }

void destroyGame(Game* game){

    if(game != NULL){

        SDL_FreeSurface(game->tileMenu);
        SDL_FreeSurface(game->gameover);
        SDL_FreeSurface(game->HUD_coin);
        SDL_FreeSurface(game->HUD_life);
        SDL_FreeSurface(game->endLevel);
        SDL_FreeSurface(game->screen);

        closeFont(game->fontHUD);
        closeFont(game->fontMenu);
        closeFont(game->fontGameover);

        Mix_FreeMusic(game->music);
        freeSound(game);
        Mix_CloseAudio();


        destroyInput(game->input);
        destroyPlayer(game->player);
        destroyMap(game->map);

        free(game);


        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
    }
}

