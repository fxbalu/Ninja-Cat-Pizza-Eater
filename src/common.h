/**
 * \file common.h
 * \brief Common definitions
 *
 * Definitions of boolean enumeration (true and false).
 *
 * \author François-Xavier Balu \<fx.balu@gmail.com\>
 * \date 12 mars 2014
 */


#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED


#include <stdio.h>   /* fprintf(), stderr */


/**
 * \enum boolean
 * \brief Enum with two values : true and false
 */
typedef enum boolean
{
   true = (0==0),
   false = (0==1)
} boolean;


void logError(const char* str, const char* file, const int line);


#endif /* COMMON_H_INCLUDED */
