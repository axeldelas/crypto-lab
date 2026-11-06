/// \file dh_bal.c
/// \author Axel Delas
/// \date Octobre 2020
/// \brief Fonctions pour pointer les balises utilisées lors de l'exécution des programmes.

#include "dh_bal.h"

bool has_h_beacon(int argc, char **argv) {
/// \brief Vérification si la balise -h (-help) figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "-h"))
      return true;
    i++;
  }
  return false;
}

bool has_o_beacon(int argc, char **argv) {
/// \brief Vérification si la balise -o (-outfile) figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "-o"))
      return true;
    i++;
  }
  return false;
}

bool has_xor_arg(int argc, char **argv) {
/// \brief Vérification si l'argument xor figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "xor"))
      return true;
    i++;
  }
  return false;
}

bool has_cbc_crypt_arg(int argc, char **argv) {
/// \brief Vérification si l'argument cbc-crypt figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "cbc-crypt"))
      return true;
    i++;
  }
  return false;
}

bool has_cbc_uncrypt_arg(int argc, char **argv) {
/// \brief Vérification si l'argument cbc-uncrypt figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "cbc-uncrypt"))
      return true;
    i++;
  }
  return false;
}

bool has_c1_arg(int argc, char **argv) {
/// \brief Vérification si l'argument c1 figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "c1"))
      return true;
    i++;
  }
  return false;
}

bool has_all_arg(int argc, char **argv) {
/// \brief Vérification si l'argument all figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "all"))
      return true;
    i++;
  }
  return false;
}

bool is_o_beacon(int i, char **argv) {
/// \brief Pointage de la balise -o (-outfile)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-o"))
    return true;
  return false;
}

bool is_i_beacon(int i, char **argv) {
/// \brief Pointage de la balise -i (-infile)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-i"))
    return true;
  return false;
}

bool is_k_beacon(int i, char **argv) {
/// \brief Pointage de la balise -k (-key)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-k"))
    return true;
  return false;
}

bool is_m_beacon(int i, char **argv) {
/// \brief Pointage de la balise -m (-method)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-m"))
    return true;
  return false;
}
