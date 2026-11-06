/// \file dh_crack.c
/// \author Axel Delas
/// \date novembre 2020
/// \brief Traitement des balises en fonction de la méthode de crack souhaité par l'utilisateur.

#include "dh_crack.h"
#include "../Balises/dh_bal.h"

void help_message(void) {
/// \brief Message d'affichage si utilisation de la balise -h (-help)
  printf("Usage : dh_crack [OPTIONS] SRC_FILENAME KEY_LENGTH METHOD\n");
  printf("Crack message file only using c1 or c1 and c2 method.\n");
  printf("\n  -i filename\t\tfile crypted message source\n");
  printf("  -k key\t\tkey length\n");
  printf("  -m c1|all\t\tcrack method : c1 or all method\n");
}

int main(int argc, char **argv) {
/// \brief Traitement de(s) balise(s) et des argument(s)
/// \details Recherche de(s) balise(s) pointée(s) par des fonctions du header dh_bal.h
/// \param[in] argc Le nombre de paramètre(s) donné(s) à l'exécution du programme.
/// \param[in] argv Tableau contenant le(s) paramètre(s) donné(s) à l'exécution du programme.
/// \returns Code de retour 0.
  char *src_filename=NULL;
  t_tab_keys keys;

  if (has_h_beacon(argc, argv))
    help_message();
  else if (has_c1_arg(argc, argv)) {
    for (int i=1; i<argc; i++) {
      if (is_i_beacon(i, argv)) {
        src_filename=calloc(strlen(argv[i+1]), 1);
        strcpy((char*)src_filename, argv[i+1]);
      }
      else if (is_k_beacon(i, argv)) {
        keys.key_size=atoi(argv[i+1]);
        keys.tab_keys=calloc(keys.key_size, sizeof(byte*));
      }
    }
    if (src_filename==NULL || keys.key_size<=0 || keys.key_size>7)
      printf("Usage : dh_crack -h\n");
    else if (fopen(src_filename, "r")==NULL)
      printf("Le fichier donné n'existe pas ou n'est pas accessible\n");
    else {
      c1(src_filename, keys);
      free(src_filename); free(keys.tab_keys);
    }
  }
  else if (has_all_arg(argc, argv)) {
    for (int i=1; i<argc; i++) {
      if (is_i_beacon(i, argv)) {
        src_filename=calloc(strlen(argv[i+1]), 1);
        strcpy((char*)src_filename, argv[i+1]);
      }
      else if (is_k_beacon(i, argv)) {
        keys.key_size=atoi(argv[i+1]);
        keys.tab_keys=calloc(keys.key_size, sizeof(byte*));
      }
    }
    if (src_filename==NULL || keys.key_size<=0 || keys.key_size>7)
      printf("Usage : dh_crack -h\n");
    else if (fopen(src_filename, "r")==NULL)
      printf("Le fichier donné n'existe pas ou n'est pas accessible en lecture\n");
    else {
      c1(src_filename, keys);
      c2(src_filename, keys);
      free(src_filename); free(keys.tab_keys);
    }
  }
  else
    printf("Usage : dh_crack -h\n");
  return 0;
}
