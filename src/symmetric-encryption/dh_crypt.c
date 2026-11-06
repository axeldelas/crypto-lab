/// \file dh_crypt.c
/// \author Axel Delas
/// \date October 2020
/// \brief Beacons treatment depending on user parameters choice or error message

#include "dh_crypt.h"

void help_message(const char *program_name) {
/// \brief Error message to print
  fprintf(stderr,
        "Usage: %s -i <input_file> -o <output_file> -k <key> "
        "-m <method> [initialization_vector]\n\n"

        "Options:\n"
        "  -i <input_file>\n"
        "        Input file containing the message to encrypt or decrypt.\n\n"

        "  -o <output_file>\n"
        "        Output file where the result will be written.\n\n"

        "  -k <key>\n"
        "        Encryption key.\n\n"

        "  -m <method>\n"
        "        Encryption/decryption method.\n\n"

        "        Available methods:\n"
        "          xor\n"
        "              XOR encryption.\n\n"

        "          cbc-crypt <iv>\n"
        "              CBC encryption using the provided initialization vector.\n"
        "              The initialization vector must be exactly 16 characters long.\n\n"

        "          cbc-uncrypt <iv>\n"
        "              CBC decryption using the provided initialization vector.\n"
        "              The initialization vector must be exactly 16 characters long.\n\n"

        "Examples:\n"
        "  %s -i message.txt -o encrypted.bin -k secret -m xor\n"

        "  %s -i message.txt -o encrypted.bin -k secret "
        "-m cbc-crypt 1234567890abcdef\n"

        "  %s -i encrypted.bin -o message.txt -k secret "
        "-m cbc-uncrypt 1234567890abcdef\n",
        program_name,
        program_name,
        program_name,
        program_name
    );
}

int main(int argc, char **argv) {
/// \brief Beacons treatment with user parameters
/// \details Looking and save beacons informations
/// \param[in] argc Number of parameters
/// \param[in] argv Table contening parameters informations
/// \returns Code 0
  char *src_filename=NULL, *dst_filename=NULL, *k=NULL, *init_vec=NULL;

  if (has_h_beacon(argc, argv))
    help_message(argv[0]);
  else if (has_xor_arg(argc, argv)) {
    for (int i=1; i<argc; i++) {
      if (is_i_beacon(i, argv)) {
        src_filename=calloc(strlen(argv[i+1]), 1);
        strcpy(src_filename, argv[i+1]);
      }
      else if (is_o_beacon(i, argv)) {
        dst_filename=calloc(strlen(argv[i+1]), 1);
        strcpy(dst_filename, argv[i+1]);
      }
      else if (is_k_beacon(i, argv)) {
        k=calloc(strlen(argv[i+1]), 1);
        strcpy(k, argv[i+1]);
      }
    }
    if (src_filename==NULL || dst_filename==NULL || k==NULL)
      printf("Usage : dh_crypt -h\n");
    else if (fopen(src_filename, "r")==NULL)
      printf("The source file does not exist or is not readable\n");
    else {
      xor(src_filename, dst_filename, k);
      free(src_filename); free(dst_filename); free(k);
    }
  }
  else if (has_cbc_crypt_arg(argc, argv)) {
    for (int i=1; i<argc; i++) {
      if (is_i_beacon(i, argv)) {
        src_filename=calloc(strlen(argv[i+1]), 1);
        strcpy(src_filename, argv[i+1]);
      }
      else if (is_o_beacon(i, argv)) {
        dst_filename=calloc(strlen(argv[i+1]), 1);
        strcpy(dst_filename, argv[i+1]);
      }
      else if (is_k_beacon(i, argv)) {
        k=calloc(strlen(argv[i+1]), 1);
        strcpy(k, argv[i+1]);
      }
      else if (is_m_beacon(i, argv)) {
        init_vec=calloc(strlen(argv[i+1]), 1);
        strcpy(init_vec, argv[i+2]);
      }
    }
    if (src_filename==NULL || dst_filename==NULL || k==NULL || strlen(init_vec)!=17)
      printf("Usage : dh_crypt -h\n");
    else if (fopen(src_filename, "r")==NULL)
      printf("The source file does not exist or is not readable\n");
    else {
      cbc_crypt(src_filename, dst_filename, k, init_vec);
      free(src_filename); free(dst_filename); free(k); free(init_vec);
    }
  }
  else if (has_cbc_uncrypt_arg(argc, argv)) {
    for (int i=1; i<argc; i++) {
      if (is_i_beacon(i, argv)) {
        src_filename=calloc(strlen(argv[i+1]), 1);
        strcpy(src_filename, argv[i+1]);
      }
      else if (is_o_beacon(i, argv)) {
        dst_filename=calloc(strlen(argv[i+1]), 1);
        strcpy(dst_filename, argv[i+1]);
      }
      else if (is_k_beacon(i, argv)) {
        k=calloc(strlen(argv[i+1]), 1);
        strcpy(k, argv[i+1]);
      }
      else if (is_m_beacon(i, argv)) {
        init_vec=calloc(strlen(argv[i+1]), 1);
        strcpy(init_vec, argv[i+2]);
      }
    }
    if (src_filename==NULL || dst_filename==NULL || k==NULL || strlen(init_vec)!=17)
      printf("Usage : dh_crypt -h\n");
    else if (fopen(src_filename, "r")==NULL)
      printf("The source file does not exist or is not readable\n");
    else {
      cbc_uncrypt(src_filename, dst_filename, k, init_vec);
      free(src_filename); free(dst_filename); free(k); free(init_vec);
    }
  }
  else
    printf("Usage : dh_crypt -h\n");
  return 0;
}
