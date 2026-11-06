/// \file dh_crack_c2.c
/// \author Axel Delas
/// \date décembre 2020
/// \brief Crack par statistiques des lettres dans la langue anglaise d'un fichier donné.

#include "dh_crack.h"

void dynamic_keys_allocation(byte **keys_copy, int nb_of_keys, int key_size) {
/// \brief Allocation dynamique pour le tableau des clés
/// \param[in] keys_copy Tableau des clés
/// \param[in] nb_of_keys Nombre de clés à tester
/// \param[in] key_size Taille de la clé
  if (keys_copy==NULL) {
    printf("Problème d'allocation"); exit (1); }
  for (int i=0; i<nb_of_keys; i++) {
    keys_copy[i]=calloc(key_size+1, sizeof(byte));
    if (keys_copy[i]==NULL) {
      printf("Problème d'allocation"); exit (2); }
  }
}

void digits_init(byte digits[], int key_size) {
/// \brief Initialisation des digits associés à la taille de la clé.
/// \param[in] digits Tableau contenant la chaîne des digits
/// \param[in] key_size Taille de la clé
  for (int i=0; i<key_size; i++)
    digits[i]='0';
  digits[key_size]='\0';
}

bool is_eligible_char(byte c) {
/// \brief Vérification de si l'octet donné fait parti de la liste des caractères d'une clé
/// \param[in] c Octet à tester
/// \returns Vrai ou faux
  byte char_list[CHAR_NB]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

  for (int i=0; i<CHAR_NB; i++)
    if (c==char_list[i])
      return true;
  return false;
}

int eligible_char_nb(byte char_key[]) {
/// \brief Nombre de caractères admissibles associé au digit
/// \param[in] char_key Tableau contenant les caractères associé au digit
/// \returns Nombre de caractères admissibles
  int char_nb=0;

  while (is_eligible_char(char_key[char_nb]))
    char_nb++;
  return char_nb;
}

void next_digits(byte digits[], t_tab_keys keys, int nb_of_keys, int *tested_keys_nb) {
/// \brief Digits suivants
/// \param[in] digits Tableau contenant la chaîne des digits
/// \param[in] keys Structure des clés
/// \param[in] nb_of_keys Nombre de clés à tester
/// \param[in] tested_keys_nb Pointeur du nombre de clés qui ont déjà été testé
  int last_digits=keys.key_size-1;
  // Incrémentation du dernier digit
  digits[last_digits]++;
  // Tant qu'on a pas testé toutes les clés et que le dernier digit est égal au maximum de caractères qu'il contient
  while (*tested_keys_nb<nb_of_keys && digits[last_digits]==eligible_char_nb(keys.tab_keys[last_digits])+'0') {
    // Le dernier digit passe à 0
    digits[last_digits]='0';
    // On incrémente l'avant dernier digit
    digits[last_digits-1]++;
    // Le dernier digit devient l'avant dernier
    last_digits--;
  }
}

int keys_nb(t_tab_keys keys) {
/// \brief Nombre de clés à tester
/// \param[in] keys Structure des clés
/// \returns Nombre de clés à tester
  int keys_nb=1;

  for (int i=0; i<keys.key_size; i++)
    keys_nb*=eligible_char_nb(keys.tab_keys[i]);
  return keys_nb;
}

void key_combo_recover(t_tab_keys keys, byte digits[], byte key[]) {
/// \brief Composition d'une clé avec les digits
/// \param[in] keys Structure des clés
/// \param[in] digits Tableau contenant la chaîne des digits
/// \param[in] key Tableau contenant la chaîne clé
  for (int i=0; i<keys.key_size; i++)
    key[i]=keys.tab_keys[i][digits[i]-'0'];
  key[keys.key_size]='\0';
}

void freq_val_init(float freq[]) {
/// \brief Initialisation des valeurs de freq
/// \param[in] freq Tableau contenant les valeurs des fréquences mesurées pour une clé
  for (int i=0; i<NB_ALPHABET; i++)
    freq[i]=0;
}

float distance_calculation(float freq[], int freq_th[], int bytes_cypher_msg_nb) {
/// \brief Calcul de la distance entre deux tableaux freq et freq_th
/// \param[in] freq Tableau contenant les valeurs des fréquences mesurées pour une clé
/// \param[in] freq_th Tableau contenant les valeurs des fréquences théoriques en anglais
/// \param[in] bytes_cypher_msg_nb Nombre d'octets du message chiffré
/// \returns Nombre flottant qui représente le score de la clé testée
  float res=0;
  // Calcul des fréquences
  for (int i=0; i<NB_ALPHABET; i++)
    freq[i]/=bytes_cypher_msg_nb;
  // Calcul en suivant la formule donnée
  for (int i=0; i<NB_ALPHABET; i++)
    res+=(freq_th[i]-freq[i])*(freq_th[i]-freq[i]);
  return res;
}

void occurrence_calculation(int bytes_cypher_msg_nb, FILE *cypher_filemsg, byte key[], byte alphabet[], float freq[], int key_size) {
/// \brief Calcul du nombre d'occurence de chaque lettre pour une clé
/// \param[in] bytes_cypher_msg_nb Nombre d'octets du message chiffré
/// \param[in] cypher_filemsg Fichier contenant le message chiffré
/// \param[in] key Tableau contenant la chaîne de la clé
/// \param[in] alphabet Tableau contenant les caractères de l'alphabet anglais
/// \param[in] freq Tableau contenant les valeurs des fréquences pour une clé
/// \param[in] key_size Taille de la clé
  int i=0, c, res;

  while (!((c=fgetc(cypher_filemsg))==EOF)) {
    // On récupère l'octet déchiffré avec la clé à tester
    res=c^key[i%key_size];
    // On incrémente l'occurrence associée à la potentielle lettre qui appartient à l'alphabet anglais
    for (int j=0; j<NB_ALPHABET; j++)
      if (res==alphabet[j])
        freq[j]++;
    i++;
  }
  rewind(cypher_filemsg);
}

void best_key_update(float *best_distance, float d, byte best_key[], byte key[]) {
/// \brief Mise à jour de la meilleure clé
/// \param[in] best_distance Pointeur de la meilleure distance
/// \param[in] d Nouvelle distance calculée à comparer avec la meilleure distance
/// \param[in] best_key Tableau contenant la chaîne de la meilleure clé
/// \param[in] key Tableau contenant la chaîne de la clé testée
  *best_distance=d;
  strcpy((char*)best_key, (char*)key);
}

void key_update(int *i, float *best_distance, float d, byte best_key[], byte key[], float score_board[], byte **keys_copy, int key_size) {
/// \brief Mise à jour de la meilleure distance en fonction des résultats de la clé actuelle
/// \param[in] i Pointeur indice de la clé testée
/// \param[in] best_distance Pointeur de la meilleure distance
/// \param[in] d Nouvelle distance calculée à comparer avec la meilleure distance
/// \param[in] best_key Tableau contenant la chaîne de la meilleure clé
/// \param[in] key Tableau contenant la chaîne de la clé testée
/// \param[in] score_board Tableau contenant les scores de toutes les clés testées
/// \param[in] keys_copy Tableau contenant les chaînes de toutes les clés testées
/// \param[in] key_size Taille de la clé
  printf("Key %s Score %f",key, d);
  // Si c'est la première clé, on l'initialise comme la meilleure clé
  if (*i==0) {
    printf("\n");
    best_key_update(best_distance, d, best_key, key);
  }
  else {
    if (d<*best_distance) {
      printf(" ==> Better than key %s\n", best_key);
      best_key_update(best_distance, d, best_key, key);
    }
    else
      printf("\n");
  }
  score_board[*i]=d;
  strcpy((char*)keys_copy[*i], (char*)key);
  (*i)++;
}

int ind_min(float score_board[], int nb_of_keys) {
/// \brief Cherche l'indice du score le plus petit
/// \param[in] score_board Tableau contenant les scores de toutes les clés testées
/// \param[in] nb_of_keys Nombre de clés à tester
/// \returns Indice du score le plus petit
  int i_min=0;

  for (int i=0; i<nb_of_keys; i++)
    if (score_board[i]<score_board[i_min])
      i_min=i;
  score_board[i_min]=999;
  return i_min;
}

void key_sort(float score_board[], byte **keys_copy, byte **keys_board, int nb_of_keys, int key_size){
/// \brief Algorithme récursif du tri rapide
/// \param[in] score_board Tableau contenant les scores de toutes les clés testées
/// \param[in] first_ind Premier du début
/// \param[in] last_ind Indice de fin
/// \param[in] keys_copy Tableau contenant les chaînes de toutes les clés testées
/// \param[in] key_size Taille de la clé
  int i_min;

  for (int i=0; i<nb_of_keys; i++) {
    i_min=ind_min(score_board, nb_of_keys);
    strcpy((char*)keys_board[i], (char*)keys_copy[i_min]);
  }
}

void c2_treatment(int nb_of_keys, FILE *cypher_filemsg, byte digits[], t_tab_keys keys, byte key[], byte best_key[], int bytes_cypher_msg_nb, float score_board[], byte **keys_copy, byte **keys_board) {
/// \brief Algorithme de crack C2 sur un tableau contenant le message chiffré
/// \param[in] nb_of_keys Nombre de clé à tester
/// \param[in] cypher_filemsg Fichier contenant le message chiffré
/// \param[in] digits Tableau contenant la chaîne des digits
/// \param[in] keys Structure des clés
/// \param[in] key Tableau contenant la chaîne de la clé
/// \param[in] best_key Tableau contenant la chaîne de la meilleure clé
/// \param[in] bytes_cypher_msg_nb Nombre d'octets du message chiffré
/// \param[in] score_board Tableau contenant les scores de toutes les clés testées
/// \param[in] keys_copy Tableau contenant les chaînes de toutes les clés testées
/// \param[in] keys_board Tableau contenant les chaînes de toutes les clés testées dans l'ordre décroissant des scores
  byte alphabet[]={"abcdefghijklmnopqrstuvwxyz"};
  float freq[NB_ALPHABET], d, best_distance;
  int freq_th[NB_ALPHABET]={8,1,2,4,12,2,2,6,6,0,0,4,2,6,7,1,0,5,6,9,2,0,2,0,1,0}, i=0;
  // Tant qu'on a pas testé toutes les clés et qu'on a pas atteint la limite maximale
  while (i<nb_of_keys && i<MAX_KEYS) {
    // On récupère la prochaine clé à tester
    key_combo_recover(keys, digits, key);
    // On initialise les valeurs de freq à 0 pour la prchaine clé à tester
    freq_val_init(freq);
    // On récupère le nombre d'occurences de chaque lettre de l'alphabet anglais dans freq
    occurrence_calculation(bytes_cypher_msg_nb, cypher_filemsg, key, alphabet, freq, keys.key_size);
    // On calcule la distance entre freq et freq_th
    d=distance_calculation(freq, freq_th, bytes_cypher_msg_nb);
    // On met à jour la meilleure clé si on a trouvé un score <
    key_update(&i, &best_distance, d, best_key, key, score_board, keys_copy, keys.key_size);
    // Digits suivant pour récupérer la prochaine clé à tester
    next_digits(digits, keys, nb_of_keys, &i);
  }
  key_sort(score_board, keys_copy, keys_board, nb_of_keys, keys.key_size);
}

void c2(char cypher_filename[], t_tab_keys keys) {
/// \brief Fonction principale du crack C2 sur un fichier donné.
/// \param[in] cypher_filename Tableau contenant le nom du fichier source
/// \param[in] keys Structure des clés
  FILE *cypher_filemsg=fopen(cypher_filename, "r");
  int bytes_cypher_msg_nb=char_nb(cypher_filemsg), nb_of_keys=keys_nb(keys);
  byte digits[keys.key_size+1],
       key[keys.key_size+1],
       best_key[keys.key_size+1],
       // Récupération de toutes les clés
       **keys_copy=calloc(nb_of_keys, sizeof(byte*)),
       // Rangement des clés dans l'ordre des scores décroissants
       **keys_board=calloc(nb_of_keys, sizeof(byte*));
  float score_board[nb_of_keys];

  dynamic_keys_allocation(keys_copy, nb_of_keys, keys.key_size);
  dynamic_keys_allocation(keys_board, nb_of_keys, keys.key_size);

  digits_init(digits, keys.key_size);

  printf("\nC2 starting...\n");
  c2_treatment(nb_of_keys, cypher_filemsg, digits, keys, key, best_key, bytes_cypher_msg_nb, score_board, keys_copy, keys_board);
  printf("C2 finished :\n\tBest key = %s\n\tKeys number = %d\n",keys_board[0], nb_of_keys);

  fclose(cypher_filemsg);
  free(keys_copy); free(keys_board);
}
