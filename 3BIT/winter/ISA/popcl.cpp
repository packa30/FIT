#include <iostream>
#include <string>
#include "popcl.h"
#include "arguments.h"
#include "connection.h"
#include <string.h>
#include <openssl/x509_vfy.h>
#include <openssl/x509.h>
#include <openssl/bio.h>

using namespace std;


int main(int argc, char *argv[]){
  SSL_library_init();
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();

  arg arg(argc,argv);

  connection connection;
  connection.connection_open(arg);
  connection.authentif(arg);
  connection.download_all(arg);
  connection.close_all();
  return 0;
}
