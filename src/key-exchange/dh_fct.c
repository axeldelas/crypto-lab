/// \file dh_prime.c
/// \author Vincent Dugat
/// \date mai 2020
/// \brief Calculs sur les nombres premiers, génération, tests, etc.

#include "dh.h"
long random_long(long min, long max){
/// \brief génère un uint aléatoire entre min et max
/// \param[in] min et max des uint
/// \return n : min≤n≤max
  return (rand()%(max-min))+min;
}

long puissance_mod_n(long a, long e, long n) {
  /// /brief puissance modulaire, calcule a^e mod n
  /// a*a peut dépasser la capacité d'un long
  /// https://www.labri.fr/perso/betrema/deug/poly/exp-rapide.html
  /// vu au S1 en Python
  long p;
  for (p=1; e>0; e=e/2) {
    if (e%2 != 0)
      p=(p*a)%n;
    a=(a*a)%n;
  }
  return p;
}

int test_prime(long n) {
/// \brief test de primarité, crible d'Erathostène
/// \returns 1 le nombre est premier, 0 sinon
  long d;

  if (n%2==0)
    return (n==2);
  for (d=3; d*d <= n; d=d+2)
    if (n%d == 0)
      return 0;
  return 1;
}

int rabin(long a, long n) {
  /// \brief test de Rabin sur la pimarité d'un entier
  /// \brief c'est un test statistique. Il est plus rapide que le précédent.
  /// \param[in] a : on met 2, ça marche
  /// \param[in] n : le nombre à tester
  /// \returns 1 il est premier, 0 sinon
  long p, e, m;
  int i, k;

  e=m=n-1;
  for (k=0; e%2==0; k++)
    e=e/2;

  p=puissance_mod_n(a, e, n);
  if (p==1) return 1;

  for (i=0; i<k; i++) {
    if (p==m) return 1;
    if (p==1) return 0;
    p=(p*p)%n;
  }

  return 0;
}

long generePremierRabin(long min, long max, int *cpt){
  /// \brief fournit un nombre premier entre min et max.
  /// La primarité est vérifiée avec le test de rabin
  /// \param[in] min et max
  /// \param[out] cpt : le nombre d'essais pour trouver le nombre
  /// \returns un nombre premier p min≤p≤max
  long num;
  *cpt=1;
  int a=2;
  do{
    num=random_long(min, max);
  } while (num%2!=1);

  while (!rabin(a, num) && num<max){
    (*cpt)++;
    num=num+2;
  }
  return num;
}

long genPrimeSophieGermain(long min, long max, int *cpt){
  /// \brief fournit un nombre premier de Sophie Germain vérifié avec le test de rabin
  /// \param[in] min et max
  /// \param[out] cpt : le nombre d'essais pour trouver le nombre
  /// \returns un nombre premier p min≤p≤max && p = 2*q+1 avec q premier
  long num;
  *cpt=1;
  int a=2;
  do{
    num=random_long(min, max);
  } while (num%2!=1);

  while ((!rabin(a, num) || !rabin(a, 2*num+1)) && num<max){
    (*cpt)++;
    num=num+2;
  }
  return 2*num+1;
}

long seek_generator(long start, long p){
  /// \brief recherche d'un générateur du groupe (corps) cyclique Z/pZ avec p premier
  long q=(p-1)/2; /// \note p est un premier de Sophie Germain ie p =2q+1 avec q premier

  /// \note si p = \prod(i=1,k,{p_i}^{n_i}) alors g est un générateur si \forall i 1..k, g^p_i != 1 (mod p)
  /// comme p = 2q+1 et q premier, k = 2 et p_i = 2 et q.
  /// il suffit que g^2 et g^q soient tous les deux différents de 1 (mod p)
  while ((puissance_mod_n(start, 2, p)==1 || puissance_mod_n(start, q, p)==1) && start<p-1){
    start++;
  }
  if (start==p-1) return -1;
  return start;
}

long xchange_shared_key(long generateur, long premier){
  /// \brief similateur d'échange de clefs de Diffie-Hellman par réseau
  /// \param[in] premier : un nombre premier, generateur : un générateur du groupe Z/premierZ
  /// \returns la clef commune générée
  long alice_secret_nb, bob_secret_nb, alice_exchanged_nb, bob_exchanged_nb, key1, key2;
  alice_secret_nb=random_long(0, premier-1);
  while (!(alice_secret_nb!=(bob_secret_nb=random_long(0, premier-1))))
    ;
  printf("Alice a choisi le nombre secret a=%ld\nBob a choisi le nombre secret b=%ld\n",alice_secret_nb, bob_secret_nb);
  alice_exchanged_nb=puissance_mod_n(generateur, alice_secret_nb, premier);
  bob_exchanged_nb=puissance_mod_n(generateur, bob_secret_nb, premier);
  printf("Alice calcule A=g^a[p]=%ld\nBob calcule B=g^b[p]=%ld\n",alice_exchanged_nb, bob_exchanged_nb);
  printf("Alice communique A à Bob et Bob communique B à alice\n");
  key1=puissance_mod_n(bob_exchanged_nb, alice_secret_nb, premier);
  key2=puissance_mod_n(alice_exchanged_nb, bob_secret_nb, premier);
  printf("Alice calcule la clé c1=B^a[p]=%ld\nBob calcule la clé c2=A^b[p]=%ld\n",key1, key2);

  return key1;
}

long generate_shared_key(long min, long max){
  /// \brief calcule un nombre premier p de Sophie Germain et un générateur du groupe p/Zp.
  /// appelle le simulateur d'échange de clef partagée.
  /// \returns la clef partagée
  int cpt;
  printf("Génération de la clef partagée : \n");
  long premier = genPrimeSophieGermain(min, max, &cpt);
  printf("Premier (Sophie Germain) = %ld\n",premier);
  long generateur = seek_generator(3, premier); // exemple 100
  long ordre = puissance_mod_n (generateur, premier-1, premier); // generateur^{premier -1} (mod premier)
  printf("Générateur = %ld ordre = %ld\n\n",generateur,ordre);
  assert (generateur!=-1);
  return xchange_shared_key(generateur, premier);
}

long int_pow(long a, long e) {
  /// \brief puissance russe
  /// \param[in] : a l'entier et e l'exposant
  /// \returns : a^e
  long p;

  for (p=1; e>0; e=e/2) {
    if (e%2!=0)
      p=(p*a);
    a=(a*a);
  }
  return p;
}

int nb_digit_base10(long n){
  /// \brief compte le nombre de chiffres d'un nombre entier de type long
  /// \returns le nombre de chiffres calculés.
  int cpt = 0;
  while (n!=0){
    n = n/10;
    cpt++;
  }
  return cpt;
}
