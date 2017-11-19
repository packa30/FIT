/*
*       Subor : tail.cc
*       Riešenie IJC-DU2, priklad 1
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>

void Warning(const char *fmt, ...);
void Er(const char *fmt, ...);
unsigned long int Lines(const char *string,std::fstream& file,unsigned long int a,unsigned long int *r,bool open);

int main (int argc, char *argv[])
{
	std::ios::sync_with_stdio(false);//zo zadania,urychli vypocet no sposobuje memory leaks
	std::fstream file;
	unsigned long int rows=10,r=0;
	bool open=0;
	switch(argc)
	{
		case 1:
			rows=Lines(argv[2],file,rows,&r,open);
			break;
		case 2:
			file.open(argv[1],std::fstream::in);
			if(!file.is_open())
			{
				std::cerr << "Subor '" << argv[1] << "' sa nepodarilo otvorit\n";
				exit(1);
			}
			open=1;
			rows=Lines(argv[1],file,rows,&r,open);
			break;
		case 3:
			if(strcmp("-n",argv[1])==0)
			{
				rows=Lines(argv[2],file,0,&r,open);
			}
			else
			{
				std::cerr << "Zly vstupny argument\n";
				exit(1);
			}
			break;
		case 4:
			file.open(argv[3],std::fstream::in);
                        if(!file.is_open())
                        {
                                std::cerr << "Subor '" << argv[1] << "' sa nepodarilo otvorit\n";
                                exit(1);
                        }
			open=1;

			 if(strcmp("-n",argv[1])==0)
                        {
                                rows=Lines(argv[2],file,0,&r,open);
                        }
			else
			{
				std::cerr << "Zly vstupny argument\n";
                                exit(1);
			}
			break;
		default:
			std::cerr << "Zle zadane argumenty\n";
                                exit(1);
			break;
	}

	file.clear();
	file.seekg(0, std::ios::beg);
	std::cin.clear();
	std::cin.seekg(0, std::ios::beg);

	if(r==rows)
	{
		std::string line;
		while(std::getline((open ? file : std::cin),line))
		{
			std::cout << line << "\n";

		}
	}
	else
	{
		std::string line;
		unsigned long int l=0;
		while(std::getline((open ? file : std::cin),line))
		{
			if (l>=rows && l<=r)
			{
				std::cout << line << "\n";
			}
		l++;
		}
	}

	if(file.is_open())
		file.close();

return 0;
}
unsigned long int Lines(const char *string,std::fstream& file,unsigned long int a,unsigned long int *r,bool open)
{
	unsigned long int i=0,vypis;
	if (a==0)
	{

	for (;i < (unsigned long int)(strlen(string));i++)
	{

		if (string[i] > '9' || string[i] < '0')
		{
			std::cerr << "Zly vstupny argument\n";
                                exit(1);
		}
	}

	vypis=atoi(string);
		if(vypis==0)
		{
			std::cerr << "Zly vstupny argument\n";
                                exit(1);
                }

	}
	else
	vypis=a;

	std::string line;
	while(std::getline((open ? file : std::cin),line))
	{
			++*r;
	}
	if(*r>vypis)
		return (*r-vypis);
	else
		return *r;

}
