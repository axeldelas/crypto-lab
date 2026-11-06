/// \file dh_crack.c
/// \author Axel Delas
/// \date novembre 2020
/// \brief Traitement des balises en fonction de la méthode de crack souhaité par l'utilisateur.

#include "dh_crack.h"
#include "dh_bal.h"

void help_message(const char *program_name) {
/// \brief Message d'affichage si utilisation de la balise -h (-help)
  fprintf(stderr,
      "Usage: %s -i <input_file> -k <key_length> -m <method>\n\n"

      "Options:\n"
      "  -i <input_file>\n"
      "        Input file containing the encrypted message.\n\n"

      "  -k <key_length>\n"
      "        Length of the encryption key to recover.\n\n"

      "  -m <method>\n"
      "        Cryptanalysis method.\n\n"

      "        Available methods:\n"
      "          c1\n"
      "              Recover the key using character filtering analysis.\n\n"

      "          all\n"
      "              Execute all available attacks (c1 and c2).\n\n"

      "Examples:\n"
      "  %s -i encrypted.bin -k 8 -m c1\n"
      "  %s -i encrypted.bin -k 8 -m all\n",
      program_name,
      program_name,
      program_name
  );
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
    help_message(argv[0]);
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
      printf("The source file does not exist or is not readable\n");
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
      printf("The source file does not exist or is not readable\n");
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
