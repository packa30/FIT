#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <inttypes.h>

#include "arguments.h"
#include <vector>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <openssl/x509v3.h>
#include <sys/stat.h>
#include <regex.h>


using namespace std;

class connection {
public:
  void connection_determine(arg arg);
  void connection_open(arg arg);
  void authentif(arg arg);
  void download_all(arg arg);
  void authentif(BIO *bio_sock,arg);
  void delete_msg(int id);
  BIO *get_bio();
  SSL *get_ssl();
  SSL_CTX *get_ctx();
  void close_all();
  void err(string where);
private:
  int sock;
  BIO *bio_sock = NULL;
  SSL_CTX *ctx = NULL;
  SSL *ssl = NULL;
  void isok(string buf,string why);
  int stat(arg arg);
  int count_msg(string buf);
  string get_line();
  void clear_buffer(char *buffer);
  bool STLS = false;
  void Write(string what);
  bool get_STLS();
  void free_all();
  int get_sock();
  string find_name(string msg,int k);
  SSL_CTX *certfile_control(arg arg);
};
