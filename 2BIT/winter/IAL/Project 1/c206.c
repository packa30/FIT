	
/* c206.c **********************************************************}
{* TĂŠma: DvousmÄrnÄ vĂĄzanĂ˝ lineĂĄrnĂ­ seznam
**
**                   NĂĄvrh a referenÄnĂ­ implementace: Bohuslav KĹena, ĹĂ­jen 2001
**                            PĹepracovanĂŠ do jazyka C: Martin TuÄek, ĹĂ­jen 2004
**                                            Ăpravy: Bohuslav KĹena, ĹĂ­jen 2016
**
** Implementujte abstraktnĂ­ datovĂ˝ typ dvousmÄrnÄ vĂĄzanĂ˝ lineĂĄrnĂ­ seznam.
** UĹžiteÄnĂ˝m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datovĂĄ abstrakce reprezentovĂĄn promÄnnou
** typu tDLList (DL znamenĂĄ Double-Linked a slouĹžĂ­ pro odliĹĄenĂ­
** jmen konstant, typĹŻ a funkcĂ­ od jmen u jednosmÄrnÄ vĂĄzanĂŠho lineĂĄrnĂ­ho
** seznamu). Definici konstant a typĹŻ naleznete v hlaviÄkovĂŠm souboru c206.h.
**
** VaĹĄĂ­m Ăşkolem je implementovat nĂĄsledujĂ­cĂ­ operace, kterĂŠ spolu
** s vĂ˝ĹĄe uvedenou datovou ÄĂĄstĂ­ abstrakce tvoĹĂ­ abstraktnĂ­ datovĂ˝ typ
** obousmÄrnÄ vĂĄzanĂ˝ lineĂĄrnĂ­ seznam:
**
**      DLInitList ...... inicializace seznamu pĹed prvnĂ­m pouĹžitĂ­m,
**      DLDisposeList ... zruĹĄenĂ­ vĹĄech prvkĹŻ seznamu,
**      DLInsertFirst ... vloĹženĂ­ prvku na zaÄĂĄtek seznamu,
**      DLInsertLast .... vloĹženĂ­ prvku na konec seznamu, 
**      DLFirst ......... nastavenĂ­ aktivity na prvnĂ­ prvek,
**      DLLast .......... nastavenĂ­ aktivity na poslednĂ­ prvek, 
**      DLCopyFirst ..... vracĂ­ hodnotu prvnĂ­ho prvku,
**      DLCopyLast ...... vracĂ­ hodnotu poslednĂ­ho prvku, 
**      DLDeleteFirst ... zruĹĄĂ­ prvnĂ­ prvek seznamu,
**      DLDeleteLast .... zruĹĄĂ­ poslednĂ­ prvek seznamu, 
**      DLPostDelete .... ruĹĄĂ­ prvek za aktivnĂ­m prvkem,
**      DLPreDelete ..... ruĹĄĂ­ prvek pĹed aktivnĂ­m prvkem, 
**      DLPostInsert .... vloĹžĂ­ novĂ˝ prvek za aktivnĂ­ prvek seznamu,
**      DLPreInsert ..... vloĹžĂ­ novĂ˝ prvek pĹed aktivnĂ­ prvek seznamu,
**      DLCopy .......... vracĂ­ hodnotu aktivnĂ­ho prvku,
**      DLActualize ..... pĹepĂ­ĹĄe obsah aktivnĂ­ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dalĹĄĂ­ prvek seznamu,
**      DLPred .......... posune aktivitu na pĹedchozĂ­ prvek seznamu, 
**      DLActive ........ zjiĹĄĹĽuje aktivitu seznamu.
**
** PĹi implementaci jednotlivĂ˝ch funkcĂ­ nevolejte ĹžĂĄdnou z funkcĂ­
** implementovanĂ˝ch v rĂĄmci tohoto pĹĂ­kladu, nenĂ­-li u funkce
** explicitnÄ uvedeno nÄco jinĂŠho.
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

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornÄnĂ­ na to, Ĺže doĹĄlo k chybÄ.
** Tato funkce bude volĂĄna z nÄkterĂ˝ch dĂĄle implementovanĂ˝ch operacĂ­.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globĂĄlnĂ­ promÄnnĂĄ -- pĹĂ­znak oĹĄetĹenĂ­ chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pĹed jeho prvnĂ­m pouĹžitĂ­m (tzn. ĹžĂĄdnĂĄ
** z nĂĄsledujĂ­cĂ­ch funkcĂ­ nebude volĂĄna nad neinicializovanĂ˝m seznamem).
** Tato inicializace se nikdy nebude provĂĄdÄt nad jiĹž inicializovanĂ˝m
** seznamem, a proto tuto moĹžnost neoĹĄetĹujte. VĹždy pĹedpoklĂĄdejte,
** Ĺže neinicializovanĂŠ promÄnnĂŠ majĂ­ nedefinovanou hodnotu.
**/

	L->First=NULL;
	L->Act=NULL;
	L->Last=NULL;

}

void DLDisposeList (tDLList *L) {
/*
** ZruĹĄĂ­ vĹĄechny prvky seznamu L a uvede seznam do stavu, v jakĂŠm
** se nachĂĄzel po inicializaci. RuĹĄenĂŠ prvky seznamu budou korektnÄ
** uvolnÄny volĂĄnĂ­m operace free. 
**/

/* odstanim vzdy prvy prvok zoznamu a na konci inicializujem hodnoty zoznamu na NULL co je  stav zoznamu po inicializacii*/
	while(L->First!=NULL)
	{
		tDLElemPtr help;
		help=L->First;
		L->First=L->First->rptr;
		free(help);
	}

	L->Act=NULL;
	L->Last=NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** VloĹžĂ­ novĂ˝ prvek na zaÄĂĄtek seznamu L.
** V pĹĂ­padÄ, Ĺže nenĂ­ dostatek pamÄti pro novĂ˝ prvek pĹi operaci malloc,
** volĂĄ funkci DLError().
**/

/*v pripade ze je dost pamati pre novy prvok vlozim ho na zaciatok zoznamu a previazem ako First prvok zoznamu*/
	struct tDLElem *Elnew;
	if((Elnew=malloc(sizeof(struct tDLElem)))!=NULL)
	{
		Elnew->data=val;
		Elnew->lptr=NULL;
		Elnew->rptr=L->First;

		if(Elnew->rptr!=NULL)
			Elnew->rptr->lptr=Elnew;
		else
			L->Last=Elnew;
		L->First=Elnew;
	}
	else
	{
		DLError();
		return;
	}
	
}

void DLInsertLast(tDLList *L, int val) {
/*
** VloĹžĂ­ novĂ˝ prvek na konec seznamu L (symetrickĂĄ operace k DLInsertFirst).
** V pĹĂ­padÄ, Ĺže nenĂ­ dostatek pamÄti pro novĂ˝ prvek pĹi operaci malloc,
** volĂĄ funkci DLError().
**/ 	
	
/*vkladany prvok previazem so zoznamom ako Last */
	struct tDLElem *Elnew;
	if((Elnew=malloc(sizeof(struct tDLElem)))!=NULL)
	{
		Elnew->data=val;
		Elnew->rptr=NULL;
		Elnew->lptr=L->Last;

		if(Elnew->lptr!=NULL)
			Elnew->lptr->rptr=Elnew;
		else
			L->First=Elnew;
		L->Last=Elnew;
	}
	else
	{
		DLError();
		return;
	}

}

void DLFirst (tDLList *L) {
/*
** NastavĂ­ aktivitu na prvnĂ­ prvek seznamu L.
** Funkci implementujte jako jedinĂ˝ pĹĂ­kaz (nepoÄĂ­tĂĄme-li return),
** aniĹž byste testovali, zda je seznam L prĂĄzdnĂ˝.
**/
	L->Act=L->First;

}

void DLLast (tDLList *L) {
/*
** NastavĂ­ aktivitu na poslednĂ­ prvek seznamu L.
** Funkci implementujte jako jedinĂ˝ pĹĂ­kaz (nepoÄĂ­tĂĄme-li return),
** aniĹž byste testovali, zda je seznam L prĂĄzdnĂ˝.
**/
	L->Act=L->Last;
	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** ProstĹednictvĂ­m parametru val vrĂĄtĂ­ hodnotu prvnĂ­ho prvku seznamu L.
** Pokud je seznam L prĂĄzdnĂ˝, volĂĄ funkci DLError().
**/

	if(L->First!=NULL)
	{
		*val=L->First->data;
	}
	else
		DLError();

}

void DLCopyLast (tDLList *L, int *val) {
/*
** ProstĹednictvĂ­m parametru val vrĂĄtĂ­ hodnotu poslednĂ­ho prvku seznamu L.
** Pokud je seznam L prĂĄzdnĂ˝, volĂĄ funkci DLError().
**/
	if(L->First!=NULL)
	{
		*val=L->Last->data;
	}
	else
		DLError();

}

void DLDeleteFirst (tDLList *L) {
/*
** ZruĹĄĂ­ prvnĂ­ prvek seznamu L. Pokud byl prvnĂ­ prvek aktivnĂ­, aktivita 
** se ztrĂĄcĂ­. Pokud byl seznam L prĂĄzdnĂ˝, nic se nedÄje.
**/
	if(L->First!=NULL)
	{
		if(L->First==L->Act)
			L->Act=NULL;
		tDLElemPtr help;
		help=L->First;
		L->First=L->First->rptr;
		L->First->lptr=NULL;
		free(help);
	}

	
}	

void DLDeleteLast (tDLList *L) {
/*
** ZruĹĄĂ­ poslednĂ­ prvek seznamu L. Pokud byl poslednĂ­ prvek aktivnĂ­,
** aktivita seznamu se ztrĂĄcĂ­. Pokud byl seznam L prĂĄzdnĂ˝, nic se nedÄje.
**/ 

	if(L->First!=NULL)
	{
		if(L->Last==L->Act)
			L->Act=NULL;
		tDLElemPtr help;
		help=L->Last;
		L->Last=L->Last->lptr;
		L->Last->rptr=NULL;
		free(help);
	}
	
}

void DLPostDelete (tDLList *L) {
/*
** ZruĹĄĂ­ prvek seznamu L za aktivnĂ­m prvkem.
** Pokud je seznam L neaktivnĂ­ nebo pokud je aktivnĂ­ prvek
** poslednĂ­m prvkem seznamu, nic se nedÄje.
**/
	if(L->Act!=NULL && (L->Act!=L->Last))
	{
		tDLElemPtr help;
		help=L->Act->rptr;

		if(help->rptr!=NULL)
		{
			L->Act->rptr=help->rptr;
			help->rptr->lptr=L->Act;
		}
		else
		{
			L->Act->rptr=NULL;
			L->Last=L->Act;
		}
		free(help);
	}

}

void DLPreDelete (tDLList *L) {
/*
** ZruĹĄĂ­ prvek pĹed aktivnĂ­m prvkem seznamu L .
** Pokud je seznam L neaktivnĂ­ nebo pokud je aktivnĂ­ prvek
** prvnĂ­m prvkem seznamu, nic se nedÄje.
**/

	if(L->Act!=NULL && (L->Act!=L->First))
        {
                tDLElemPtr help;
                help=L->Act->lptr;

                if(help->lptr!=NULL)
                {
                        L->Act->lptr=help->lptr;
                        help->lptr->rptr=L->Act;
                }
                else
                {
                        L->Act->lptr=NULL;
                        L->First=L->Act;
                }
                free(help);
        }

}

void DLPostInsert (tDLList *L, int val) {
/*
** VloĹžĂ­ prvek za aktivnĂ­ prvek seznamu L.
** Pokud nebyl seznam L aktivnĂ­, nic se nedÄje.
** V pĹĂ­padÄ, Ĺže nenĂ­ dostatek pamÄti pro novĂ˝ prvek pĹi operaci malloc,
** volĂĄ funkci DLError().
**/
	if(L->Act!=NULL)
	{
		struct tDLElem *help;
		if((help=malloc(sizeof(struct tDLElem)))!=NULL)
		{
			help->data=val;
			if(L->Act->rptr!=NULL)
			{
				L->Act->rptr->lptr=help;
				help->rptr=L->Act->rptr;
			}
			else
			{
				help->rptr=NULL;
				L->Last=help;
			}
			L->Act->rptr=help;
			help->lptr=L->Act;
		}
		else
			DLError();
	}
	
}

void DLPreInsert (tDLList *L, int val) {
/*
** VloĹžĂ­ prvek pĹed aktivnĂ­ prvek seznamu L.
** Pokud nebyl seznam L aktivnĂ­, nic se nedÄje.
** V pĹĂ­padÄ, Ĺže nenĂ­ dostatek pamÄti pro novĂ˝ prvek pĹi operaci malloc,
** volĂĄ funkci DLError().
**/
	if(L->Act!=NULL)
        {
                struct tDLElem *help;
                if((help=malloc(sizeof(struct tDLElem)))!=NULL)
                {
                        help->data=val;
                        if(L->Act->lptr!=NULL)
                        {
                                L->Act->lptr->rptr=help;
                                help->lptr=L->Act->lptr;
                        }
                        else
                        {
                                help->lptr=NULL;
                                L->First=help;
                        }
                        L->Act->lptr=help;
                        help->rptr=L->Act;
                }
                else
                        DLError();
        }


}

void DLCopy (tDLList *L, int *val) {
/*
** ProstĹednictvĂ­m parametru val vrĂĄtĂ­ hodnotu aktivnĂ­ho prvku seznamu L.
** Pokud seznam L nenĂ­ aktivnĂ­, volĂĄ funkci DLError ().
**/

	if(L->Act!=NULL)
	{
		*val=L->Act->data;
	}
	else
	{
		DLError();
	}
}

void DLActualize (tDLList *L, int val) {
/*
** PĹepĂ­ĹĄe obsah aktivnĂ­ho prvku seznamu L.
** Pokud seznam L nenĂ­ aktivnĂ­, nedÄlĂĄ nic.
**/
	if(L->Act!=NULL)
	{
		L->Act->data=val;
	}

}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na nĂĄsledujĂ­cĂ­ prvek seznamu L.
** NenĂ­-li seznam aktivnĂ­, nedÄlĂĄ nic.
** VĹĄimnÄte si, Ĺže pĹi aktivitÄ na poslednĂ­m prvku se seznam stane neaktivnĂ­m.
**/

	if(L->Act!=NULL)
	{
		L->Act=L->Act->rptr;
	}

}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pĹedchozĂ­ prvek seznamu L.
** NenĂ­-li seznam aktivnĂ­, nedÄlĂĄ nic.
** VĹĄimnÄte si, Ĺže pĹi aktivitÄ na prvnĂ­m prvku se seznam stane neaktivnĂ­m.
**/

	if(L->Act!=NULL)
	{
		L->Act=L->Act->lptr;
	}

}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivnĂ­, vracĂ­ nenulovou hodnotu, jinak vracĂ­ 0.
** Funkci je vhodnĂŠ implementovat jednĂ­m pĹĂ­kazem return.
**/

	return(L->Act!=NULL);
}

/* Konec c206.c*/
