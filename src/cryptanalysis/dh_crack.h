#ifndef CRACK_H
#define CRACK_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define  MAX_SIZE_KEY 8  /* 7 + '\0' caractères max dans la clef */
#define  MAX_VALID_KEY_CHARS 10 // dix caractères numériques possibles pour chaque caractère de la clef
#define  MAX_KEYS 30000 // nombre max de clefs potentielles gérées
#define	 CHAR_NB 10 // nombre de caractères maximum dans un digit
#define  NB_ALPHABET 26 // nombre de lettres dans l'alphabet anglais

typedef unsigned char byte; // octet

typedef struct {
	int key_size; // longueur de la clef
	byte **tab_keys; // 2 dim : ligne i = liste des caractères admissibles pour clef[i]
} t_tab_keys;

typedef struct { // tableau des mots du message décodé pour chercher dans le dico
	byte **words_tab;
	int dim; // nombre de mots
} t_words_lst;

// Fonctions crack c1
void display_board_key(byte keys_char[], int key_char_nb, int digit_key_nb);
void display_all_board_keys(t_tab_keys keys);
void dynamic_allocation_mat(t_tab_keys keys);
void del_char(byte key_char[], int c, int *p_char_size);
int char_nb(FILE *cypher_filemsg);
void cypher_msg_recover(FILE *cypher_filemsg, byte cypher_msg[]);
void init_key(t_tab_keys keys);
bool is_eligible(byte res);
void c1_treatment(t_tab_keys keys, byte cypher_msg[], int *p_char_size, int bytes_cypher_msg_nb);
void c1(char cypher_filename[], t_tab_keys keys);
// Fonctions crack c2
void dynamic_keys_allocation(byte **keys_copy, int nb_of_keys, int key_size);
void digits_init(byte digits[], int key_size);
bool is_eligible_char(byte c);
int eligible_char_nb(byte char_key[]);
void next_digits(byte digits[], t_tab_keys keys, int nb_of_keys, int *tested_keys_nb);
int keys_nb(t_tab_keys keys);
void key_combo_recover(t_tab_keys keys, byte digits[], byte key[]);
void freq_val_init(float freq[]);
float distance_calculation(float freq[], int freq_th[], int bytes_cypher_msg_nb);
void occurrence_calculation(int bytes_cypher_msg_nb, FILE *cypher_filemsg, byte key[], byte alphabet[], float freq[], int key_size);
void best_key_update(float *best_distance, float d, byte best_key[], byte key[]);
void key_update(int *i, float *best_distance, float d, byte best_key[], byte key[], float score_board[], byte **keys_copy, int key_size);
int ind_min(float score_board[], int nb_of_keys);
void key_sort(float score_board[], byte **keys_copy, byte **keys_board, int nb_of_keys, int key_size);
void c2_treatment(int nb_of_keys, FILE *cypher_filemsg, byte digits[], t_tab_keys keys, byte key[], byte best_key[], int bytes_cypher_msg_nb, float score_board[], byte **keys_copy, byte **keys_board);
void c2(char cypher_filename[], t_tab_keys keys);

#endif
