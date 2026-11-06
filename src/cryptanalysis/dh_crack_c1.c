/// \file dh_crack_c1.c
/// \author Axel Delas
/// \date novembre 2020
/// \brief Crack par émondage des caractères possible pour la clef sur un fichier donné contenant le message chiffré.

#include "dh_crack.h"

void display_board_key(byte key_char[], int key_char_nb, int digit_key_nb) {
/// \brief Afficher le(s) caractère(s) d'une clé
/// \param[in] key_char Tableau contenant le(s) caractère(s) de la clé
/// \param[in] key_char_nb Nombre de caractères associé au digit
/// \param[in] digit_key_nb Digit de la clé qu'on souhaite afficher
  printf("key[%d]==>[",digit_key_nb);
  for (int i=0; i<key_char_nb; i++)
    if (i+1==key_char_nb)
      printf("%c", key_char[i]);
    else
      printf("%c,", key_char[i]);
  printf("]\n");
}

void display_all_board_keys(t_tab_keys keys) {
/// \brief Afficher le(s) caractère(s) de toute(s) les clés
/// \param[in] keys Structure des clés
  int key_char_nb;

  for (int i=0; i<keys.key_size; i++) {
    key_char_nb=strlen((char*)keys.tab_keys[i]);
    display_board_key(keys.tab_keys[i], key_char_nb, i);
  }
}

void dynamic_allocation_mat(t_tab_keys keys) {
/// \brief Allocation dynamique pour une matrice
/// \param[in] keys Structure des clés
  if (keys.tab_keys==NULL) {
    printf("Problème d'allocation"); exit (1); }
  for (int i=0; i<keys.key_size; i++) {
    if (i==0)
      keys.tab_keys[i]=calloc(MAX_VALID_KEY_CHARS-1, sizeof(byte));
    else
      keys.tab_keys[i]=calloc(MAX_VALID_KEY_CHARS, sizeof(byte));
    if (keys.tab_keys[i]==NULL) {
      printf("Problème d'allocation"); exit (2); }
  }
}

void init_key(t_tab_keys keys) {
/// \brief Initialisation des caractères de base d'une clé
/// \param[in] keys Structure des clés
  for (int i=0; i<keys.key_size; i++) {
    if (i==0) {
      for (int j=0; j<9; j++)
        keys.tab_keys[i][j]=j+1+'0';
    }
    else {
      for (int j=0; j<10; j++)
        keys.tab_keys[i][j]=j+'0';
    }
  }
}

void del_char(byte key_char[], int k, int *p_char_size) {
/// \brief Suppression du caractère ciblé de la clé
/// \param[in] key_char Tableau contenant les caractères de la clé
/// \param[in] k Indice du caractère à supprimer
/// \param[in] p_char_size Nombre de caractères de la clé
  (*p_char_size)--;
  for (int i=k; i<*p_char_size; i++)
      // De l'élément actuel au dernier, on décale les éléments
      key_char[i]=key_char[i+1];
  key_char=realloc(key_char, sizeof(byte)*(*p_char_size));
}

int char_nb(FILE *cypher_filemsg) {
/// \brief Nombre d'octet(s) dans le fichier contenant le message chiffré
/// \param[in] cypher_filemsg Fichier contenant le message chiffré
/// \returns Nombre d'octet(s)
  int char_nb=0;

  while (!((fgetc(cypher_filemsg))==EOF))
    char_nb++;
  rewind(cypher_filemsg);
  return char_nb;
}

void cypher_msg_recover(FILE *cypher_filemsg, byte cypher_msg[]) {
/// \brief Récupération du message chiffré du fichier dans un tableau
/// \param[in] cypher_filemsg Fichier source contenant le message chiffré
/// \param[in] cypher_msg Tableau contenant le message chiffré
  int c, i=0;

  while (!((c=fgetc(cypher_filemsg))==EOF)) {
    cypher_msg[i]=c;
    i++;
  }
  cypher_msg[i]='\0';
}

bool is_eligible(byte res) {
/// \brief Vérification de si l'octet est un alphanumérique ou une ponctuation ou un espace
/// \param[in] res Octet à tester
/// \returns Vrai ou faux
  if (!isalnum(res) && !ispunct(res) && !isspace(res))
    return false;
  return true;
}

void c1_treatment(t_tab_keys keys, byte cypher_msg[], int *p_char_size, int bytes_cypher_msg_nb) {
/// \brief Algorithme émondage des caractères possible pour la clef sur un tableau contenant le message chiffré
/// \param[in] keys Structure des clés
/// \param[in] cypher_msg Tableau contenant les octets du message chiffré
/// \param[in] p_char_size Nombre de caractère de la clé ciblée
/// \param[in] bytes_cypher_msg_nb Nombre d'octets du message chiffré
  int j, k;
  byte c, car, res;

  for (int i=0; i<keys.key_size; i++) {
    j=i;
    // On récupère le nombre de caractères du digit i
    *p_char_size=strlen((char*)keys.tab_keys[i]);
    // Tant qu'on a pas lu tous les octets du message chiffré
    while (j<bytes_cypher_msg_nb) {
      // On récupère l'octet associé au message chiffré associé au tour de boucle
      c=cypher_msg[j];
      k=0;
      // Tant qu'on a pas traité tous les caractères du digit i
      while (k<*p_char_size) {
        // On récupère le caractère d'indice k
        car=keys.tab_keys[i][k];
        // On déchiffre l'octet chiffré avec le caractère de la clé d'indice k
        res=c^car;
        // On vérifie qu'il soit admissible ou non, si il ne l'est pas, on supprime le caractère du digit, sinon on passe au suivant
        if (!is_eligible(res))
          del_char(keys.tab_keys[i], k, p_char_size);
        else
          k++;
      }
      // On incrémente j de la taille de la clé pour traiter les éléments i+taille_clé
      j+=keys.key_size;
    }
    keys.tab_keys[i][*p_char_size]='\0';
  }
}

void c1(char *cypher_filename, t_tab_keys keys) {
/// \brief Fonction principale du crack C1 sur un fichier donné.
/// \param[in] cypher_filename Nom du fichier contenant le message chiffré
/// \param[in] keys Structure des clés
  FILE *cypher_filemsg=fopen(cypher_filename, "r");
  int bytes_cypher_msg_nb=char_nb(cypher_filemsg), char_size;
  byte cypher_msg[bytes_cypher_msg_nb];

  dynamic_allocation_mat(keys);
  init_key(keys);
  printf("Created and initialised\n");
  display_all_board_keys(keys);

  cypher_msg_recover(cypher_filemsg, cypher_msg);

  printf("\nC1 starting...\n");
  c1_treatment(keys, cypher_msg, &char_size, bytes_cypher_msg_nb);
  printf("C1 finished\n");
  display_all_board_keys(keys);

  fclose(cypher_filemsg);
}
