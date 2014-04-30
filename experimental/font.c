#include "font.h"

TTF_Font* loadFont(char* name, int size)
{

    TTF_Font* font = TTF_OpenFont(name, size);


    if (font == NULL)
    {
        printf("Failed to open Font %s: %s\n", name, TTF_GetError());

        exit(1);
    }

    return font;
}

void closeFont(TTF_Font *font)
{
    /* Ferme la police quand on n'en a plus besoin (avant de quitter) */

    if (font != NULL)
    {
        TTF_CloseFont(font);
    }
}

void drawString(char* text, int x, int y, int r,int b, int g, TTF_Font* font)
{
    SDL_Rect dest;
    SDL_Surface * surface;
    SDL_Color foregroundColor;

    foregroundColor.r = r;
    foregroundColor.b = b;
    foregroundColor.g = g;

    surface = TTF_RenderUTF8_Blended(font,text,foregroundColor);

    if(surface == NULL)
    {
        printf("Couldn't create String %s: %s\n", text, SDL_GetError());

        return;
    }

    dest.x = x;
    dest.y = y;
    dest.w = surface->w;
    dest.h = surface->h;

    SDL_BlitSurface(surface, NULL, jeu.screen,&dest);

    SDL_FreeSurface(surface);
}
