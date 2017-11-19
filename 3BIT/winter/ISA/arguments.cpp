#include "arguments.h"


using namespace std;

/**
* @brief <function compares a console input arguments and presets all variabiles
          in class object arg for values taken from console input arguments
* @param [int] <argc> < number of input arguments
* @param [char **] <argv> < pointer to a array of arguments from console input pointer
* @return no return value just presets all variabiles in object arg
* @details <details>
*/
arg::arg(int argc, char **argv){
  this->argc=argc;
  this->argv=argv;
  int c;
  while ((c=getopt(argc,argv,"a:o:p:C:dnSTc:C:")) != -1)
  {
    switch (c) {
      case 'a':
        this->auth_file = optarg;
        break;
      case 'o':
        this->out_dir = optarg;
        break;
      case 'p':
        if(atoi(optarg))
          this->port = atoi(optarg);
        else{
          call_help();
          std::cout << '\n' << "nesprávna hodnota parametru -p" << '\n';
          exit(1);
        }
        break;
      case 'T':case'S':
        if(c=='T')
          this->T = true;
        else
          this->S=true;
        break;
      case 'c':case 'C':
        if(c=='c')
          this->certfile = optarg;
        else
          this->certaddr = optarg;
        break;
      case 'd':
        this->D = true;
        /*if(atoi(optarg))
          this->id_msg=atoi(optarg);*/
        break;
      case 'n':
        this->N = true;
        break;
      default:
        call_help();
        exit(1);

      }
  }

  int index=optind;
  while(index<argc){index++;}
  if((index-1)!=optind)
  {
    call_help();
    std::cout << '\n' << "Nevyhovujuce argumenty/Nie je mozne prist na to ktorym myslite server" << '\n';
    index=optind;
    while(index<argc){
      std::cout << argv[index] << '\n';
      index++;
    }
    exit(1);
  }
  else
    this->server=argv[optind];

  if(this->port == 0){
    if(this->T==true)
      this->port=995;
    else
      this->port=110;
  }
  if (this->server.empty() || this->auth_file.empty() || this->out_dir.empty()){
    call_help();
    cout << '\n' << "chyba povinny parameter/parametre" << '\n' << '\n' ;
    exit(1);
  }
  if(this->T == true && this->S == true)
  {
    call_help();
    std::cout << '\n' << "nepovolena kombinacia -T a -S" << '\n';
    exit(1);
  }
  ifstream file;
  file.open(this->auth_file);
  if(!file.is_open()){
    call_help();
    cout << '\n' << "neda sa otvorit subor s authentifikacnymi udajmi" << '\n' << '\n' ;
    exit(1);
  }
  else{
      string line;
      getline(file,line);
      this->username = line.substr(11,line.length());
      getline(file,line);
      this->password = line.substr(11,line.length());
      file.close();
      if(this->username.empty() || this->password.empty()){
        call_help();
        std::cout << '\n' << "zlý obsah súboru s authentifikačnými údajmi" << '\n';
        exit(1);
      }
    file.close();
  }
  if(this->out_dir.at(this->out_dir.length()-1)!='/')
    this->out_dir+="/";


}

/**
* @brief <function to cout whole arg class structure (used for debugging)
* @return no return value
*/
void arg::print(){
  cout << "server:"  << this->server << '\n';
  cout << "port:"  << this->port << '\n';
  cout << "T:"  << this->T << '\n';
  cout << "S:" << this->S << '\n';
  cout << "D:" << this->D <<'\n';
  cout << "N:" << this->N <<'\n';
  cout << "authentif file:" <<  this->auth_file << '\n';
  cout << "ulozne miesto:" << this->out_dir << '\n';
  cout << "certaddr:" << this->certaddr << '\n';
  cout << "certfile:" << this->certfile << '\n';
  cout << "username:" << this->username << '\n';
  cout << "password:" << this->password << '\n';
}

/**
* @brief < all other functions are public functions for obtaining a set values
           of private variabiles from arg class to every class needed
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/
string arg::get_server(){
  return this->server;
}
int arg::get_port(){
  return this->port;
}
bool arg::get_T(){
  return this->T;
}
bool arg::get_S(){
  return this->S;
}
bool arg::get_D(){
  return this->D;
}
bool arg::get_N(){
  return this->N;
}
string arg::get_auth_file(){
  return this->auth_file;
}
string arg::get_out_dir(){
  return this->out_dir;
}
string arg::get_certaddr(){
  return this->certaddr;
}
string arg::get_certfile(){
  return this->certfile;
}
string arg::get_username(){
  return this->username;
}
string arg::get_password(){
  return this->password;
}
int arg::get_id_msg(){
  return this->id_msg;
}

/**
* @brief funkcia vypíše help pri zlých parametroch
*/
void arg::call_help(){
  std::cout << '\n' << "Popis vstupných parametrov príkazového riadka:" << '\n';
  std::cout << '\n' << "    Povinné parametre:" << '\n';
  std::cout << "        [server]                              hostname/IPv4/IPv6" << '\n';
  std::cout << "        -a [authfile]                         súbor s username a passwordom" << '\n';
  std::cout << "        -o [out_dir]                          priečinok na ukladanie stiahnutých správ" << '\n';
  std::cout << '\n' << "    Voliteľné parametre:" << '\n';
  std::cout << "        -p [port]                             číslo portu" << '\n';
  std::cout << '\n';
  std::cout << "        -T < -c / -C> [certfile/certaddr]     nadviaže šifrované spojenie" << '\n';
  std::cout << "        -T                                    bez ďalších parametrov načíta default certifikát" << '\n';
  std::cout << "        -T < -c > [certfile]                  načíta certifikát zo súboru [certfile]" << '\n';
  std::cout << "        -T < -C > [certaddr]                  načíta certifikát z priečinka [certaddr]" << '\n';
  std::cout << '\n';
  std::cout << "        -S < -c / -C> [certfile/certaddr]     nadviaže nešifrované spojenie a prejde do šifrovaného (STLS)" << '\n';
  std::cout << "        -S                                    bez ďalších parametrov načíta default certifikát" << '\n';
  std::cout << "        -S < -c > [certfile]                  načíta certifikát zo súboru [certfile]" << '\n';
  std::cout << "        -S < -C > [certaddr]                  načíta certifikát z priečinka [certaddr]" << '\n';
  std::cout << '\n';
  std::cout << "        -T -S !!!!! NEPOVOLENÁ KOMBINÁCIA" << '\n';
  std::cout << '\n' << '\n' << '\n' << "Nasledujúci výpis predvádza údaje čo ste vložili" << '\n' << '\n' ;
  print();
}
void arg::destroy(){
  delete this;
}
