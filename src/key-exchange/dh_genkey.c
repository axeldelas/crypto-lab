/// \file dh_genkey.c
/// \author Axel Delas
/// \date novembre 2020
/// \brief Traitement des balises en fonction de l'option souhaitée ou non par l'utilisateur.

#include "dh.h"

void help_message(const char *program_name) {
/// \brief Message d'affichage si utilisation de la balise -h (-help)
  fprintf(stderr,
        "Usage: %s -o <output_file>\n\n"

        "Options:\n"
        "  -o <output_file>\n"
        "        Output file where the generated Diffie-Hellman parameters\n"
        "        and keys will be written.\n\n"

        "Examples:\n"
        "  %s -o dh_keys.txt\n",
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
  srand((time(NULL)));
  int i=1, cpt=0;
  long p, g, o;
  FILE *dst_filename;

  if (argc==1)
    generate_shared_key(MIN_PRIME, MAX_PRIME);
  else if (has_h_beacon(argc, argv))
    help_message(argv[0]);
  else if (has_o_beacon(argc, argv)) {
    p=genPrimeSophieGermain(MIN_PRIME, MAX_PRIME, &cpt);
    g=seek_generator(3, p);
    o=puissance_mod_n(g, p-1, p);
    if (argv[i+1]==NULL)
      printf("Usage : dh_genkey -h\n");
    else {
      dst_filename=fopen(argv[i+1], "w");
      fprintf(dst_filename, "Génération de la clé partagée\np=%ld\ng=%ld d'ordre %ld\n",p, g, o);
      xchange_shared_key(g, p);
    }
  }
  else
    printf("Usage : dh_genkey -h\n");
  return 0;
}
