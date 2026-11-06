#ifndef DH_BAL
#define DH_BAL

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Vérification si ces données figure
bool has_h_beacon(int argc, char **argv);
bool has_o_beacon(int argc, char **argv);
bool has_xor_arg(int argc, char **argv);
bool has_cbc_crypt_arg(int argc, char **argv);
bool has_cbc_uncrypt_arg(int argc, char **argv);
bool has_c1_arg(int argc, char **argv);
bool has_all_arg(int argc, char **argv);
// Pointage des données figurant
bool is_o_beacon(int i, char **argv);
bool is_i_beacon(int i, char **argv);
bool is_k_beacon(int i, char **argv);
bool is_m_beacon(int i, char **argv);

#endif
