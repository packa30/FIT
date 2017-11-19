
/* c016.c: **********************************************************}
{* TĂŠma:  Tabulka s RozptĂ˝lenĂ˝mi PoloĹžkami
**                      PrvnĂ­ implementace: Petr PĹikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Ăpravy: Karel MasaĹĂ­k, ĹĂ­jen 2014
**                              Radek HranickĂ˝, ĹĂ­jen 2014
**                              Radek HranickĂ˝, listopad 2015
**                              Radek HranickĂ˝, ĹĂ­jen 2016
**
** VytvoĹete abstraktnĂ­ datovĂ˝ typ
** TRP (Tabulka s RozptĂ˝lenĂ˝mi PoloĹžkami = Hash table)
** s explicitnÄ ĹetÄzenĂ˝mi synonymy. Tabulka je implementovĂĄna polem
** lineĂĄrnĂ­ch seznamĹŻ synonym.
**
** Implementujte nĂĄsledujĂ­cĂ­ procedury a funkce.
**
**  HTInit ....... inicializuje tabulku pĹed prvnĂ­m pouĹžitĂ­m
**  HTInsert ..... vloĹženĂ­ prvku
**  HTSearch ..... zjiĹĄtÄnĂ­ pĹĂ­tomnosti prvku v tabulce
**  HTDelete ..... zruĹĄenĂ­ prvku
**  HTRead ....... pĹeÄtenĂ­ hodnoty prvku
**  HTClearAll ... zruĹĄenĂ­ obsahu celĂŠ tabulky (inicializace tabulky
**                 potĂŠ, co jiĹž byla pouĹžita)
**
** Definici typĹŻ naleznete v souboru c016.h.
**
** Tabulka je reprezentovĂĄna datovou strukturou typu tHTable,
** kterĂĄ se sklĂĄdĂĄ z ukazatelĹŻ na poloĹžky, jeĹž obsahujĂ­ sloĹžky
** klĂ­Äe 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dalĹĄĂ­ synonymum 'ptrnext'. PĹi implementaci funkcĂ­
** uvaĹžujte maximĂĄlnĂ­ rozmÄr pole HTSIZE.
**
** U vĹĄech procedur vyuĹžĂ­vejte rozptylovou funkci hashCode.  PovĹĄimnÄte si
** zpĹŻsobu pĹedĂĄvĂĄnĂ­ parametrĹŻ a zamyslete se nad tĂ­m, zda je moĹžnĂŠ parametry
** pĹedĂĄvat jinĂ˝m zpĹŻsobem (hodnotou/odkazem) a v pĹĂ­padÄ, Ĺže jsou obÄ
** moĹžnosti funkÄnÄ pĹĂ­pustnĂŠ, jakĂŠ jsou vĂ˝hody Äi nevĂ˝hody toho Äi onoho
** zpĹŻsobu.
**
** V pĹĂ­kladech jsou pouĹžity poloĹžky, kde klĂ­Äem je ĹetÄzec, ke kterĂŠmu
** je pĹidĂĄn obsah - reĂĄlnĂŠ ÄĂ­slo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** RozptylovacĂ­ funkce - jejĂ­m Ăşkolem je zpracovat zadanĂ˝ klĂ­Ä a pĹidÄlit
** mu index v rozmezĂ­ 0..HTSize-1.  V ideĂĄlnĂ­m pĹĂ­padÄ by mÄlo dojĂ­t
** k rovnomÄrnĂŠmu rozptĂ˝lenĂ­ tÄchto klĂ­ÄĹŻ po celĂŠ tabulce.  V rĂĄmci
** pokusĹŻ se mĹŻĹžete zamyslet nad kvalitou tĂŠto funkce.  (Funkce nebyla
** volena s ohledem na maximĂĄlnĂ­ kvalitu vĂ˝sledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitnÄ zĹetÄzenĂ˝mi synonymy.  Tato procedura
** se volĂĄ pouze pĹed prvnĂ­m pouĹžitĂ­m tabulky.
*/

void htInit ( tHTable* ptrht ) {

	if((*ptrht)!=NULL)
	{
		int i=0;
		while(i<=HTSIZE-1)
		{
			(*ptrht)[i]=NULL;
			i++;
		}
	}
}

/* TRP s explicitnÄ zĹetÄzenĂ˝mi synonymy.
** VyhledĂĄnĂ­ prvku v TRP ptrht podle zadanĂŠho klĂ­Äe key.  Pokud je
** danĂ˝ prvek nalezen, vracĂ­ se ukazatel na danĂ˝ prvek. Pokud prvek nalezen nenĂ­, 
** vracĂ­ se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	if((*ptrht)==NULL)
	{
		return NULL;
	}
	else
	{
		int hkey=hashCode(key);
		tHTItem *help=(*ptrht)[hkey];
		while(help!=NULL)
		{
			if(help->key!=key)
				help=help->ptrnext;
			else
				return help;
		}
		return NULL;
	}
}

/* 
** TRP s explicitnÄ zĹetÄzenĂ˝mi synonymy.
** Tato procedura vklĂĄdĂĄ do tabulky ptrht poloĹžku s klĂ­Äem key a s daty
** data.  ProtoĹže jde o vyhledĂĄvacĂ­ tabulku, nemĹŻĹže bĂ˝t prvek se stejnĂ˝m
** klĂ­Äem uloĹžen v tabulce vĂ­ce neĹž jedenkrĂĄt.  Pokud se vklĂĄdĂĄ prvek,
** jehoĹž klĂ­Ä se jiĹž v tabulce nachĂĄzĂ­, aktualizujte jeho datovou ÄĂĄst.
**
** VyuĹžijte dĹĂ­ve vytvoĹenou funkci htSearch.  PĹi vklĂĄdĂĄnĂ­ novĂŠho
** prvku do seznamu synonym pouĹžijte co nejefektivnÄjĹĄĂ­ zpĹŻsob,
** tedy proveÄte.vloĹženĂ­ prvku na zaÄĂĄtek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	if((*ptrht)!=NULL)
	{
		tHTItem *help=htSearch(ptrht,key);
		if(help==NULL)
		{
			int hkey=hashCode(key);
			tHTItem *inew=malloc(sizeof(struct tHTItem));
			inew->key=key;
			inew->data=data;
			inew->ptrnext=(*ptrht)[hkey];
			(*ptrht)[hkey]=inew;
		}
		else
		{
			help->data=data;
		}
	}
}

/*
** TRP s explicitnÄ zĹetÄzenĂ˝mi synonymy.
** Tato funkce zjiĹĄĹĽuje hodnotu datovĂŠ ÄĂĄsti poloĹžky zadanĂŠ klĂ­Äem.
** Pokud je poloĹžka nalezena, vracĂ­ funkce ukazatel na poloĹžku
** Pokud poloĹžka nalezena nebyla, vracĂ­ se funkÄnĂ­ hodnota NULL
**
** VyuĹžijte dĹĂ­ve vytvoĹenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	if((*ptrht)==NULL)
	{
		return NULL;
	}
	else
	{
		tHTItem *help=htSearch(ptrht,key);
		if(help==NULL)
			return NULL;
		else
		{
			tData *idata=&(help->data);
			return idata;
		}
	}
}

/*
** TRP s explicitnÄ zĹetÄzenĂ˝mi synonymy.
** Tato procedura vyjme poloĹžku s klĂ­Äem key z tabulky
** ptrht.  UvolnÄnou poloĹžku korektnÄ zruĹĄte.  Pokud poloĹžka s uvedenĂ˝m
** klĂ­Äem neexistuje, dÄlejte, jako kdyby se nic nestalo (tj. nedÄlejte
** nic).
**
** V tomto pĹĂ­padÄ NEVYUĹ˝ĂVEJTE dĹĂ­ve vytvoĹenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	if((*ptrht)!=NULL)
	{
		int hkey=hashCode(key);
		tHTItem *help1=(*ptrht)[hkey];
		tHTItem *help2=(*ptrht)[hkey];
		while(help1!=NULL)
		{
			if(help1->key==key)
			{
				if(help1==(*ptrht)[hkey])
					(*ptrht)[hkey]=help1->ptrnext;
				else
					help2->ptrnext=help1->ptrnext;
				free(help1);
			}
			
			help2=help1;
			help1=help1->ptrnext;
		}
	}
}

/* TRP s explicitnÄ zĹetÄzenĂ˝mi synonymy.
** Tato procedura zruĹĄĂ­ vĹĄechny poloĹžky tabulky, korektnÄ uvolnĂ­ prostor,
** kterĂ˝ tyto poloĹžky zabĂ­raly, a uvede tabulku do poÄĂĄteÄnĂ­ho stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	if((*ptrht)!=NULL)
	{
		tHTItem *help=NULL;
		int i=0;
		while(i<=HTSIZE-1)
		{
			while((*ptrht)[i]!=NULL)
			{
				help=(*ptrht)[i];
				(*ptrht)[i]=(*ptrht)[i]->ptrnext;
				free(help);
			}
			(*ptrht)[i]=NULL;
			i++;
		}
	}
}
