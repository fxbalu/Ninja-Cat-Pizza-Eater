/**
 * \file font.h 
 * \brief header of font.c
 *
 * Declaration of loadFont() and closeFont()
 *
 * \author François-Xavier Balu, Gwendal Henry, Martin Parisot, Vincent Werner
 *
 */

#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "game.h"


TTF_Font* loadFont(char* name, int size);
void closeFont(TTF_Font *font);




#endif // FONT_H_INCLUDED
