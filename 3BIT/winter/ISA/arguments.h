#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fstream>


#ifndef ISA_ARG
#define ISA_ARG

using namespace std;
class arg {
private:
  int argc;
  char **argv;

  string server;
  int port = 0;
  bool T = false;
  bool S = false;
  bool D = false;
  bool N = false;

  string auth_file;
  string out_dir;
  string certaddr;
  string certfile;

  string username;
  string password;

  int id_msg=0;

public:
  int count_of_msg;
  arg(int argc, char **argv);
  void print();
  string get_server();
  int get_port();
  bool get_T();
  bool get_S();
  bool get_D();
  bool get_N();
  string get_auth_file();
  string get_out_dir();
  string get_certaddr();
  string get_certfile();
  string get_username();
  string get_password();
  int get_id_msg();
  void call_help();
  void destroy();
};

#endif
