/// \file vernam_header.h
/// \author Vincent Dugat
/// \date mai 2020

#ifndef DH_H
#define DH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <ctype.h> // opérations sur les caractères

#define MAX_FACTEURS 10 // nb max de facteurs premiers d'un nombre
#define BLOCK_SIZE 16 // taille des blocs pour CBC
#define MAX_PRIME 4294967296/1000 // 2^32 = sqrt(2^64)
#define MIN_PRIME 100

typedef unsigned char byte; // octet
typedef byte block_t[BLOCK_SIZE]; // bloc CBC : un octet par case

long random_long(long min,long max);
int rabin(long a, long n);
long puissance_mod_n(long a, long e, long n);
void xor(char *msg, char* key,int msg_length,char *crypted);
long generePremierRabin(long min,long max,int *cpt);
long seek_generator(long start,long p);
int nb_digit_base10(long n);
long generate_shared_key(long min,long max);
long genPrimeSophieGermain(long min,long max,int *cpt);
long xchange_shared_key(long generateur, long premier);
long int_pow(long a, long e);

#endif
