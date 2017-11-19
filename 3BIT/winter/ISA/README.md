# POP3 client
pop3 client for IPv4 & IPv6 trough socket using BIO and SSL

## Usage:
```
popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>
( ORDER DOES NOT MATTER )
```
## mandatory arguments

#### [server]                         hostname/IPv4/IPv6

#### -a [authfile]                    file with username & password

#### -o [out_dir]                     directory for downloaded e-mails

## optional arguments

#### -p [port]                        port number

### SSL connection:

#### -T [ -c / -C] [certfile/certaddr]    file/directory with certificates

#### -T                                   loads default certificates

### NONSSL connection -> SSL connection

#### -S [ -c / -C] [certfile/certaddr]    file/directory with certificates

#### -S                                   loads default certificates

#### -T -S !!!!! can not be used together

## Example usage:
```
./popcl -a authentiFILE -o /homes/documents/msg/ pop.gmail.com -T       secure connection/download all msg
```
```
./popcl -a authentiFILE -o /homes/documents/msg/ pop.seznam.cz -S       nonsecure to secure (STLS)
```
```
./popcl -a authentiFILE -o /homes/documents/msg/ pop.gmail.com -p 995   nonsecure trough port 995 (gmail wont
respond)
```
## Included files
arguments.cpp

arguments.h

connection.cpp

connection.h

popcl.cpp

popcl.h

Makefile

manual.pdf

README
