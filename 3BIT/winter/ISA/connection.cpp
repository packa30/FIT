#include "connection.h"

/**
* @brief <function to open connection over socket and and wrap it under BIO structure or SSL
          and determines whether to
          to set it to non secure connection on port 110 (or STLS over port 110)
          or change it to secure connection over port 995
          if a param -p was used properly connection is opened on used port not implicit ones(110/995)
* @param [arg] <arg> <object class arg aka. parsed arguments from line
* @return no return value just a preset params  BIO *bio_sock;
                                                SSL_CTX *ctx;
                                                SSL *ssl;
          private variabiles in object of class connection
*/
void connection::connection_open(arg arg){
  BIO *bio_sock;
  SSL_CTX *ctx;
  SSL *ssl;
  int sock;
  string result;
  int result2;
  struct hostent *hent;
  char buf[2048];
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;


  if((arg.get_server().find(":")) != string::npos){
    struct sockaddr_in6 serv_addr;
    if ((hent = gethostbyname2(arg.get_server().data(),AF_INET6)) == NULL){err("Neexistuje zadaný host IPv6");}
    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if ( sock < 0){err("Chyba vytvárania socketu IPv6");}

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_flowinfo = 0;
    serv_addr.sin6_family = AF_INET6;
    memmove((char *) &serv_addr.sin6_addr.s6_addr, (char *) hent->h_addr, hent->h_length);
    serv_addr.sin6_port = htons(arg.get_port());

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {err("Nepodarilo sa nastaviť timeout na socket IPv6");}
    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {err("Nepodarilo sa nastaviť timeout na socket IPv6");}
    if ((result2 = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0){err("Nepodarilo sa pripojiť na server IPv6");}
  }
  else{
    struct sockaddr_in addr;
    if((hent = gethostbyname(arg.get_server().data()))==NULL){err("Neexistuje zadaný host IPv4");}
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){err("Chyba vytvárania socketu IPv4");}

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    bcopy(hent->h_addr,&addr.sin_addr.s_addr,hent->h_length);
    addr.sin_port = htons(arg.get_port());

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {err("Nepodarilo sa nastaviť timeout na socket IPv4");}
    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {err("Nepodarilo sa nastaviť timeout na socket IPv4");}
    if((connect(sock, (struct sockaddr *)&addr, sizeof(addr))) < 0){err("Nepodarilo sa pripojiť na server IPv4");}
  }

  this->sock=sock;

  if(arg.get_T() != true){
    if((result2 = read(sock, buf, 2048)) < 0){err("Server connection timed out");}
    isok(buf,"Server nepovolil connection");
    bio_sock = BIO_new_socket(get_sock(),true);
    this->bio_sock=bio_sock;

    if(arg.get_S() == true){
      Write("STLS\r\n");
      result = get_line();
      isok(result,"prechod z nesifrovaneho spojenia na sifrovane pomocou STLS");
      ctx=certfile_control(arg);
      SSL_CTX_set_options(ctx, SSL_OP_ALL);
      ssl=SSL_new(ctx);

      SSL_set_bio(ssl,bio_sock,bio_sock);
      SSL_set_connect_state(ssl);
      SSL_set_verify(ssl, SSL_VERIFY_PEER,NULL);
      if( SSL_connect(ssl) <=0 ){err("nepodareny handshake po prikaze STLS // zrejme nedoverihodny certifikat alebo port");}
      this->STLS=true;
      this->ssl=ssl;
      this->ctx=ctx;
    }
  }
  else{
      ctx = certfile_control(arg);
      ssl = SSL_new(ctx);
      SSL_set_fd(ssl, sock);
      SSL_set_verify(ssl, SSL_VERIFY_PEER,NULL);
      if( SSL_connect(ssl) <=0 ){err("nepodareny handshake pri secure connection // zrejme nedoverihodny certifikat alebo port");}
      this->ssl=ssl;
      this->STLS=true;
      this->ctx=ctx;
      result = get_line();
      isok(result,"chyba pripojenia na daný server cez daný port");
  }

  /*std::cout << BIO_get_conn_hostname(bio_sock) << '\n';
  std::cout << (const unsigned char*)BIO_get_conn_hostname(bio_sock) << '\n';
  if(arg.get_T()==true || arg.get_S()==true){
    cert = SSL_get_peer_certificate(ssl);
    if (cert == NULL)
     err("chyba pri nacitani certifikatu");
    if(X509_check_host(cert,BIO_get_conn_hostname(bio_sock),0,NULL,NULL)!=1){
      if(X509_check_ip(cert,(const unsigned char*)BIO_get_conn_hostname(bio_sock),0,NULL)==1)
      err("hostname sa nezhoduje so subjectom v certifikate (unthrusted certificate)");
     }
  }*/

}

/**
* @brief <function to authentificate user with username and password taken from file
          in arg.username by public function from class arg: get_username() and
          in arg.password by public function from class arg : get_password()
          function is working with helping functions Write() and get_line()
          < those two determines themselves whether to work over BIO or SSL
            depending on private variabiles in class connection
* @param [arg] <arg> <object class arg aka. parsed arguments from line
* @return no return value
*/
void connection::authentif(arg arg){
  string result;
  string to_write;

  to_write="USER "+arg.get_username()+"\r\n";
  Write(to_write);

  result = get_line();
  isok(result,"authentifikacia username/password");

  to_write="PASS "+arg.get_password()+"\r\n";
  Write(to_write);

  result = get_line();
  isok(result,"authentifikacia username/password");
}

/**
* @brief <functiion implicitly downloads all mails from e-mail to a dir in arg.out_dir
          but it also decide whether to download just new mails or all of them
* @param [arg] <arg> <object class arg aka. parsed arguments from line
* @return no return value
*/
void connection::download_all(arg arg){
  //int result;
  string msg;
  int count_of_downloaded=0;
  int count_of_msg=stat(arg);
  bool TOP_implemented = false;
  ifstream file;
  ofstream file2;
  string name;
  if(arg.get_N() == true){
    msg="TOP 1 0\r\n";
    Write(msg);
    msg=get_line();
    //std::cout << msg << '\n';
    if(msg.find("+OK") == string::npos){}
    else{
      msg="";
      while((msg.find("\r\n.\r\n"))==string::npos){
          msg+=get_line();
        }
    TOP_implemented = true;
    }
  }

  mkdir((arg.get_out_dir()+arg.get_server()).data(),0777);
  mkdir((arg.get_out_dir()+arg.get_server()+"/"+arg.get_username()).data(),0777);

  for(int i=1 ;i<=count_of_msg;i++)
  {

    if(TOP_implemented == true){
      msg="TOP " + to_string(i) + " 0" + "\r\n";
      Write(msg);
      msg=get_line();
      isok(msg + " " + to_string(i)," Problem pri funkcnom TOP prikaze // mala pravdepodobnost");

      msg="";
      while((msg.find("\r\n.\r\n"))==string::npos){
          msg+=get_line();
      }
      name = find_name(msg,i);
      file.open(arg.get_out_dir()+arg.get_server()+"/"+arg.get_username()+"/"+name);
    }
    if(file.is_open()){file.close();}
    else{
      msg="RETR " + to_string(i) + "\r\n";
      Write(msg);

      msg=get_line();
      isok(msg+" "+to_string(i),"problem s ID sprav na servery");
      string msg2="";
      while((msg2.find("\r\n.\r\n"))==string::npos){
          msg2+=get_line();
      }
      if(arg.get_N() == true){
          name = find_name(msg2,i);

          file.open(arg.get_out_dir()+arg.get_server()+"/"+arg.get_username()+"/"+name);
          if(!file.is_open()){
            file2.open(arg.get_out_dir()+arg.get_server()+"/"+arg.get_username()+"/"+name);
            file2 << msg2;
            file2.close();
            count_of_downloaded++;
            if(arg.get_D() == true)
              delete_msg(i);
          }
          else{
            file.close();
          }

      }
      else{
        name = find_name(msg2,i);
        file2.open(arg.get_out_dir()+arg.get_server()+"/"+arg.get_username()+"/"+name);
        file2 << msg2;
        file2.close();
        count_of_downloaded++;
        if(arg.get_D() == true)
          delete_msg(i);
      }

    }
  }
  if(arg.get_N() == true)
    cout << "Bolo stiahnutých " << count_of_downloaded << " nových/nová správ/správa." << '\n';
  else
    cout << "Bolo stiahnutých " << count_of_downloaded << " správ/správa." << '\n';
  if(arg.get_D() == true && count_of_downloaded > 0)
    std::cout << "Všetky tieto správy boli aj úspešne odstránené zo servera" << '\n';
}

/**
* @brief <function sends a command DELE with a id of mail to be deleted form mail
* @param [int] <id> <id of mail to be deteled from mail
* @return no return value
*/
void connection::delete_msg(int id){
  string msg="DELE "+to_string(id)+"\r\n";
  Write(msg);
  msg=get_line();
  isok(msg,"problem pri odstranovani spravy podla ID");

}

/**
* @brief <function takes string/response from server to a STAT command and return
          a int value of how many mails are there in mail
* @param [string] <buf> <string/buffer with a answer on command STAT from server
* @return <integer vale of mails on mail
*/
int connection::count_msg(string buf){
  return atoi(buf.erase(0,4).substr(0,buf.find_first_of('\0')).data());
}

/**
* @brief <function sends a STAT command to server and obtain response
          and returns a call of function count_msg so that a function download_all
          know how manny mail are there to download
* @param [arg] <arg> <object class arg aka. parsed arguments from line
* @return <returns a call of function count_msg so that a function download_all
*/
int connection::stat(arg arg){
  string result;
  Write("STAT\r\n");
  result = get_line();
  isok(result,"chyba prikazu STAT");
  return count_msg(result);

}

/**
* @brief <function obtains a line from response from server over BIO or SSL
* @return <returns a string (line from response)
*/
string connection::get_line(){
  string msg;
  int result;
  char buf;
  if(get_STLS()!=true){
    while((result = BIO_read(get_bio(), &buf,1 ))>0){
      msg+=buf;
      if(buf=='\n' || result == 0)
        break;
    }
  }
  else{
    while((result = SSL_read(get_ssl(), &buf,1 ))>0){
      msg+=buf;
      if(buf=='\n' || result == 0)
        break;
    }
  }
  if(msg.empty())
    err("chyba pri citani zo servera");

  return msg;
}

/**
* @brief <function for error ends of program it exits the program and stdouts
          a param where
* @param [string] <where> <string containing a reason for end of the program
* @return no return value
*/
void connection::err(string where) {
    std::cout << where << '\n';
    exit(1);
}

/**
* @brief <function compares a response from server to find out whether the server is:
          +OK with the command obtained from client
          or
          -ERR with it (wrong command/can not do it)
* @param [string] <buf> <response from server
* @param [string] <why> <string containing a reason for err in case server responded
                         with -ERR
* @return no return value
*/
void connection::isok(string buf,string why){
  //std::cout << buf << '\n';
  if(buf.find("+OK") == string::npos)
  {
    close_all();
    string errs = "odmietnutie zo strany servera z důvodu:"+why;
    err(errs);
  }
}

/**
* @brief <private function for class connection to obtain a preset bio_sock
* @return < returns a BIO structure
*/
BIO *connection::get_bio(){
  return this->bio_sock;
}

/**
* @brief <private function for class connection to obtain a preset ssl
* @return < returns a SSL structure
*/
SSL *connection::get_ssl(){
  return this->ssl;
}

/**
* @brief < function clears the buffer
* @param [char *] <buffer> < buffer
* @return no return value
* @details < I dont need to use this anymore
*/
void connection::clear_buffer(char *buffer){
  for(int i=0;i<2048;i++){buffer[i]='\0';}
}

/**
* @brief <function decides whether to send a command over BIO or SSL and send it
* @param [string] <what> <string command to be sent to a server
* @return <no return value
*/
void connection::Write(string what){
  if(get_STLS()!=true)
    BIO_write(get_bio(), what.data(), what.length() );
  else
    SSL_write(get_ssl(), what.data(), what.length() );

}

/**
* @brief <private function for class connection to obtain a preset param STLS value
* @return < returns bool value from STLS param
*/
bool connection::get_STLS(){
  return this->STLS;
}
/**
* @brief <private function for class connection to obtain a preset param sock value
* @return < returns int sock param
*/
int connection::get_sock(){
  return this->sock;
}
/**
* @brief <private function to properly close opened connection
* @return < no return value
*/
void connection::close_all(){
  Write("QUIT\r\n");
  string result = get_line();
  isok(result,"can not properly close the connection to server");
  free_all();
}
/**
* @brief <private function to properly free all created connection structures
          socket has been set to close automatic after closing BIO or SSL
          so theres no need to close it implicitly
* @return < no return value
*/
void connection::free_all(){
  if(get_STLS() != false){SSL_free(get_ssl());}
  if(get_ctx() != NULL){SSL_CTX_free(get_ctx());}
  BIO_free_all(get_bio());
  close(get_sock());
}
/**
* @brief <function to determine a name of downloaded or download to be message
* @param [string] <msg> <input is header of msg or the msg whole
* @param [int] <k> < UID of msg
* @return < returns name determined from msg input or if failed to find one it returns name like "mail - k"
            where k is input param / UID of msg
*/
string connection::find_name(string msg,int k){
  string to_do;
  string to_do2;
  string name;
  int reti;
  regex_t regex;
  reti = regcomp(&regex, "<.*>", 0);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
  }
  to_do = msg;
  while(!to_do.empty()){
    if(strcmp(to_do.substr(0,5).data(),"From:") == 0){
      reti = regexec(&regex, to_do.data(), 0, NULL, 0);
      if(!reti){
        name = to_do.substr(to_do.find("<"),to_do.find(">")-to_do.find("<")+1);
        break;
      }
    }
    to_do = to_do.substr(to_do.find('\n')+1,string::npos);
  }
  to_do = msg;
  string name2;
  while(!to_do.empty()){
    if(strcmp(to_do.substr(0,5).data(),"Date:") == 0){
      name2 = to_do.substr(6,to_do.find('\n')-7);
      break;
    }
    to_do = to_do.substr(to_do.find('\n')+1,string::npos);
  }
  if(name.empty() && name2.empty()){
    name = "mail - " + to_string(k);
  }
  else if(name.empty()){
    name = "mail - "+name2;
  }
  else if(name2.empty()){
    name = name + to_string(k);
  }
  else
    name = name + name2;

  return name;
}
/**
* @brief <function to load certificate to connect or load default path depending on input params
* @param [arg] <arg> <object of class arg
* @return <returns ctx structure with loaded certificate/s
*/
SSL_CTX *connection::certfile_control(arg arg){
  SSL_CTX *ctx;
  ctx = SSL_CTX_new(SSLv23_client_method());
  if(arg.get_certfile() != ""){
    //std::cout << "certfile good" << '\n';
    if(!SSL_CTX_load_verify_locations(ctx, NULL, arg.get_certfile().c_str()))
      err("problém s certifikatom predanym suborom");
  }
  if(arg.get_certaddr() != ""){
    //std::cout << "adresar sedi a funguje" << '\n';
    if(!SSL_CTX_load_verify_locations(ctx, NULL, arg.get_certaddr().c_str()))
      err("problem s certifikatmi v priecinku");
  }
  if(arg.get_certfile().empty() && arg.get_certaddr().empty()){
    if(!SSL_CTX_set_default_verify_paths(ctx)){err("nejde load default certifikaty");};
  }
  return ctx;
}
SSL_CTX *connection::get_ctx(){
  return this->ctx;
}
