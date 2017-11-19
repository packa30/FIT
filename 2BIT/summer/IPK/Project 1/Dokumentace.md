IPK
CLIENT-SERVER
-   aplikacia na prenos súborov pomocou HTTP

Server - ftresd
SYNOPSIS
```sh
	ftresd [-r ROOT-FOLDER] [-p PORT]
```
DESCRTIPTION
-  	Server neustále vyčkáva na pripojenie klienta. Akonáhle obdrží správu volá         funkciu getH.Podľa jej výsledku sa rozhodne v nasledujúcej činnosti.

OPTIONS
- -r
	 - špecifikuje koreňový adresár(defaultne aktuálny)
- -p
	- špecifikuje port, na ktorom bude server naslúchať(defaultne 6677)

CLIENT - ftresd
```sh
	ftrest COMMAND REMOTE-PATH [LOCAL-PATH]
```
DESCRIPTION
-	Aplikácia nadväzuje komunikáciu so serverom,ktorý už musí naslúchať. V HTTP hlavičke posiela žiadosť, ktorý príkaz chce aby bol vykonaný, nad ktorým adresárom/súborom.
	Neskôr analyzuje RESPONSE od servera a v prípade že sa v odpovedi nachádza 200 OK znamená to, že bolo žiadosti vyhovené.

OPTIONS
-	COMMAND
	-	príkaz čo má byť prevedený na servery
	
-   REMOTE-PATH
	-	cesta k súboru alebo adresáru na servery

-   LOCAL-PATH(povinný len pre put)
	-	cesta v lokálnom súborovom systéme(opäť k súboru alebo adresáru)

AUTHOR
``
	Peter Grofčík _ xgrofc00
``
