
/* c201.c *********************************************************************}
{* TĂŠma: JednosmÄrnĂ˝ lineĂĄrnĂ­ seznam
**
**                     NĂĄvrh a referenÄnĂ­ implementace: Petr PĹikryl, ĹĂ­jen 1994
**                                          Ăpravy: Andrea NÄmcovĂĄ listopad 1996
**                                                   Petr PĹikryl, listopad 1997
**                                PĹepracovanĂŠ zadĂĄnĂ­: Petr PĹikryl, bĹezen 1998
**                                  PĹepis do jazyka C: Martin TuÄek, ĹĂ­jen 2004
**                                            Ăpravy: Bohuslav KĹena, ĹĂ­jen 2016
**
** Implementujte abstraktnĂ­ datovĂ˝ typ jednosmÄrnĂ˝ lineĂĄrnĂ­ seznam.
** UĹžiteÄnĂ˝m obsahem prvku seznamu je celĂŠ ÄĂ­slo typu int.
** Seznam bude jako datovĂĄ abstrakce reprezentovĂĄn promÄnnou typu tList.
** Definici konstant a typĹŻ naleznete v hlaviÄkovĂŠm souboru c201.h.
** 
** VaĹĄĂ­m Ăşkolem je implementovat nĂĄsledujĂ­cĂ­ operace, kterĂŠ spolu s vĂ˝ĹĄe
** uvedenou datovou ÄĂĄstĂ­ abstrakce tvoĹĂ­ abstraktnĂ­ datovĂ˝ typ tList:
**
**      InitList ...... inicializace seznamu pĹed prvnĂ­m pouĹžitĂ­m,
**      DisposeList ... zruĹĄenĂ­ vĹĄech prvkĹŻ seznamu,
**      InsertFirst ... vloĹženĂ­ prvku na zaÄĂĄtek seznamu,
**      First ......... nastavenĂ­ aktivity na prvnĂ­ prvek,
**      CopyFirst ..... vracĂ­ hodnotu prvnĂ­ho prvku,
**      DeleteFirst ... zruĹĄĂ­ prvnĂ­ prvek seznamu,
**      PostDelete .... ruĹĄĂ­ prvek za aktivnĂ­m prvkem,
**      PostInsert .... vloĹžĂ­ novĂ˝ prvek za aktivnĂ­ prvek seznamu,
**      Copy .......... vracĂ­ hodnotu aktivnĂ­ho prvku,
**      Actualize ..... pĹepĂ­ĹĄe obsah aktivnĂ­ho prvku novou hodnotou,
**      Succ .......... posune aktivitu na dalĹĄĂ­ prvek seznamu,
**      Active ........ zjiĹĄĹĽuje aktivitu seznamu.
**
** PĹi implementaci funkcĂ­ nevolejte ĹžĂĄdnou z funkcĂ­ implementovanĂ˝ch v rĂĄmci
** tohoto pĹĂ­kladu, nenĂ­-li u danĂŠ funkce explicitnÄ uvedeno nÄco jinĂŠho.
**
** NemusĂ­te oĹĄetĹovat situaci, kdy mĂ­sto legĂĄlnĂ­ho ukazatele na seznam 
** pĹedĂĄ nÄkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnÄ komentujte!
**
** TerminologickĂĄ poznĂĄmka: Jazyk C nepouĹžĂ­vĂĄ pojem procedura.
** Proto zde pouĹžĂ­vĂĄme pojem funkce i pro operace, kterĂŠ by byly
** v algoritmickĂŠm jazyce PascalovskĂŠho typu implemenovĂĄny jako
** procedury (v jazyce C procedurĂĄm odpovĂ­dajĂ­ funkce vracejĂ­cĂ­ typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornÄnĂ­ na to, Ĺže doĹĄlo k chybÄ.
** Tato funkce bude volĂĄna z nÄkterĂ˝ch dĂĄle implementovanĂ˝ch operacĂ­.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globĂĄlnĂ­ promÄnnĂĄ -- pĹĂ­znak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L pĹed jeho prvnĂ­m pouĹžitĂ­m (tzn. ĹžĂĄdnĂĄ
** z nĂĄsledujĂ­cĂ­ch funkcĂ­ nebude volĂĄna nad neinicializovanĂ˝m seznamem).
** Tato inicializace se nikdy nebude provĂĄdÄt nad jiĹž inicializovanĂ˝m
** seznamem, a proto tuto moĹžnost neoĹĄetĹujte. VĹždy pĹedpoklĂĄdejte,
** Ĺže neinicializovanĂŠ promÄnnĂŠ majĂ­ nedefinovanou hodnotu.
**/
	L->Act=NULL;
	L->First=NULL;

}

void DisposeList (tList *L) {
/*
** ZruĹĄĂ­ vĹĄechny prvky seznamu L a uvede seznam L do stavu, v jakĂŠm se nachĂĄzel
** po inicializaci. VeĹĄkerĂĄ pamÄĹĽ pouĹžĂ­vanĂĄ prvky seznamu L bude korektnÄ
** uvolnÄna volĂĄnĂ­m operace free.
***/
/*do pomocnej premennej ulozime prvok ktory ideme mazat a zaradom vymazeme vsetky*/
	while(L->First != NULL)
	{
		tElemPtr help;
		help=L->First;
		L->First=help->ptr;
		free(help);
	}
	L->Act=NULL;

}

void InsertFirst (tList *L, int val) {
/*
** VloĹžĂ­ prvek s hodnotou val na zaÄĂĄtek seznamu L.
** V pĹĂ­padÄ, Ĺže nenĂ­ dostatek pamÄti pro novĂ˝ prvek pĹi operaci malloc,
** volĂĄ funkci Error().
**/
    	struct tElem *Lnew;
	if((Lnew=malloc(sizeof(struct tElem)))!=NULL)
	{
		Lnew->data=val;
		Lnew->ptr=L->First;
		L->First=Lnew;
	}
	else
	{
		Error();
		return;
	}

}

void First (tList *L) {
/*
** NastavĂ­ aktivitu seznamu L na jeho prvnĂ­ prvek.
** Funkci implementujte jako jedinĂ˝ pĹĂ­kaz, aniĹž byste testovali,
** zda je seznam L prĂĄzdnĂ˝.
**/
	L->Act=L->First;

}

void CopyFirst (tList *L, int *val) {
/*
** ProstĹednictvĂ­m parametru val vrĂĄtĂ­ hodnotu prvnĂ­ho prvku seznamu L.
** Pokud je seznam L prĂĄzdnĂ˝, volĂĄ funkci Error().
**/
	if(L->First!=NULL)
	{
		*val=L->First->data;
	}
	else
	{
		Error();
	}

}

void DeleteFirst (tList *L) {
/*
** ZruĹĄĂ­ prvnĂ­ prvek seznamu L a uvolnĂ­ jĂ­m pouĹžĂ­vanou pamÄĹĽ.
** Pokud byl ruĹĄenĂ˝ prvek aktivnĂ­, aktivita seznamu se ztrĂĄcĂ­.
** Pokud byl seznam L prĂĄzdnĂ˝, nic se nedÄje.
**/
	if(L->First!=NULL)
	{
		if(L->First==L->Act)
			L->Act=NULL;
		tElemPtr help;
		help=L->First;
		L->First=help->ptr;
		free(help);
	}

}

void PostDelete (tList *L) {
/* 
** ZruĹĄĂ­ prvek seznamu L za aktivnĂ­m prvkem a uvolnĂ­ jĂ­m pouĹžĂ­vanou pamÄĹĽ.
** Pokud nenĂ­ seznam L aktivnĂ­ nebo pokud je aktivnĂ­ poslednĂ­ prvek seznamu L,
** nic se nedÄje.
**/

/*prvok za aktivnym ulozim do pomocnej premennej aby bolo mozne previazat zoznam bez neho*/
	if(L->Act!=NULL && L->Act->ptr!=NULL)
	{
		tElemPtr help;
		help=L->Act->ptr;
		L->Act->ptr=help->ptr;
		free(help);
	}

}

void PostInsert (tList *L, int val) {
/*
** VloĹžĂ­ prvek s hodnotou val za aktivnĂ­ prvek seznamu L.
** Pokud nebyl seznam L aktivnĂ­, nic se nedÄje!
** V pĹĂ­padÄ, Ĺže nenĂ­ dostatek pamÄti pro novĂ˝ prvek pĹi operaci malloc,
** zavolĂĄ funkci Error().
**/
	if(L->Act!=NULL)
	{
		struct tElem *help;
		if((help=malloc(sizeof(struct tElem)))!=NULL)
		{
			help->data=val;
			help->ptr=L->Act->ptr;
			L->Act->ptr=help;
		}
		else
		{
			Error();
			return;
		}
	}

}

void Copy (tList *L, int *val) {
/*
** ProstĹednictvĂ­m parametru val vrĂĄtĂ­ hodnotu aktivnĂ­ho prvku seznamu L.
** Pokud seznam nenĂ­ aktivnĂ­, zavolĂĄ funkci Error().
**/
	if(L->Act!=NULL)
	{
		*val=L->Act->data;
	}
	else
	{
		Error();
		return;
	}

}

void Actualize (tList *L, int val) {
/*
** PĹepĂ­ĹĄe data aktivnĂ­ho prvku seznamu L hodnotou val.
** Pokud seznam L nenĂ­ aktivnĂ­, nedÄlĂĄ nic!
**/
	if(L->Act!=NULL)
	{
		L->Act->data=val;
	}

}

void Succ (tList *L) {
/*
** Posune aktivitu na nĂĄsledujĂ­cĂ­ prvek seznamu L.
** VĹĄimnÄte si, Ĺže touto operacĂ­ se mĹŻĹže aktivnĂ­ seznam stĂĄt neaktivnĂ­m.
** Pokud nenĂ­ pĹedanĂ˝ seznam L aktivnĂ­, nedÄlĂĄ funkce nic.
**/
	if(L->Act!=NULL)
	{
		L->Act=L->Act->ptr;
	}

}

int Active (tList *L) {		
/*
** Je-li seznam L aktivnĂ­, vracĂ­ nenulovou hodnotu, jinak vracĂ­ 0.
** Tuto funkci je vhodnĂŠ implementovat jednĂ­m pĹĂ­kazem return. 
**/
	return L->Act!=NULL;

}

/* Konec c201.c */
