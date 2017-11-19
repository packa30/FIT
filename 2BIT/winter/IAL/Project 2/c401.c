
/* c401.c: **********************************************************}
{* TĂŠma: RekurzivnĂ­ implementace operacĂ­ nad BVS
**                                         VytvoĹil: Petr PĹikryl, listopad 1994
**                                         Ăpravy: Andrea NÄmcovĂĄ, prosinec 1995
**                                                      Petr PĹikryl, duben 1996
**                                                   Petr PĹikryl, listopad 1997
**                                  PĹevod do jazyka C: Martin TuÄek, ĹĂ­jen 2005
**                                         Ăpravy: Bohuslav KĹena, listopad 2009
**                                         Ăpravy: Karel MasaĹĂ­k, ĹĂ­jen 2013
**                                         Ăpravy: Radek HranickĂ˝, ĹĂ­jen 2014
**                                         Ăpravy: Radek HranickĂ˝, listopad 2015
**                                         Ăpravy: Radek HranickĂ˝, ĹĂ­jen 2016
**
** Implementujte rekurzivnĂ­m zpĹŻsobem operace nad binĂĄrnĂ­m vyhledĂĄvacĂ­m
** stromem (BVS; v angliÄtinÄ BST - Binary Search Tree).
**
** KlĂ­Äem uzlu stromu je jeden znak (obecnÄ jĂ­m mĹŻĹže bĂ˝t cokoliv, podle
** Äeho se vyhledĂĄvĂĄ). UĹžiteÄnĂ˝m (vyhledĂĄvanĂ˝m) obsahem je zde integer.
** Uzly s menĹĄĂ­m klĂ­Äem leĹžĂ­ vlevo, uzly s vÄtĹĄĂ­m klĂ­Äem leĹžĂ­ ve stromu
** vpravo. VyuĹžijte dynamickĂŠho pĹidÄlovĂĄnĂ­ pamÄti.
** RekurzivnĂ­m zpĹŻsobem implementujte nĂĄsledujĂ­cĂ­ funkce:
**
**   BSTInit ...... inicializace vyhledĂĄvacĂ­ho stromu
**   BSTSearch .... vyhledĂĄvĂĄnĂ­ hodnoty uzlu zadanĂŠho klĂ­Äem
**   BSTInsert .... vklĂĄdĂĄnĂ­ novĂŠ hodnoty
**   BSTDelete .... zruĹĄenĂ­ uzlu se zadanĂ˝m klĂ­Äem
**   BSTDispose ... zruĹĄenĂ­ celĂŠho stromu
**
** ADT BVS je reprezentovĂĄn koĹenovĂ˝m ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klĂ­Ä (typu char), podle
** kterĂŠho se ve stromu vyhledĂĄvĂĄ, vlastnĂ­ obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levĂ˝ a pravĂ˝ podstrom (LPtr a RPtr). PĹesnou definici typĹŻ 
** naleznete v souboru c401.h.
**
** Pozor! Je tĹeba sprĂĄvnÄ rozliĹĄovat, kdy pouĹžĂ­t dereferenÄnĂ­ operĂĄtor *
** (typicky pĹi modifikaci) a kdy budeme pracovat pouze se samotnĂ˝m ukazatelem 
** (napĹ. pĹi vyhledĂĄvĂĄnĂ­). V tomto pĹĂ­kladu vĂĄm napovĂ­ prototypy funkcĂ­.
** Pokud pracujeme s ukazatelem na ukazatel, pouĹžijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede poÄĂĄteÄnĂ­ inicializaci stromu pĹed jeho prvnĂ­m pouĹžitĂ­m.
**
** OvÄĹit, zda byl jiĹž strom pĹedanĂ˝ pĹes RootPtr inicializovĂĄn, nelze,
** protoĹže pĹed prvnĂ­ inicializacĂ­ mĂĄ ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. ProgramĂĄtor vyuĹžĂ­vajĂ­cĂ­ ADT BVS tedy musĂ­ zajistit, aby inicializace
** byla volĂĄna pouze jednou, a to pĹed vlastnĂ­ pracĂ­ s BVS. ProvedenĂ­
** inicializace nad neprĂĄzdnĂ˝m stromem by totiĹž mohlo vĂŠst ke ztrĂĄtÄ pĹĂ­stupu
** k dynamicky alokovanĂŠ pamÄti (tzv. "memory leak").
**	
** VĹĄimnÄte si, Ĺže se v hlaviÄce objevuje typ ukazatel na ukazatel.	
** Proto je tĹeba pĹi pĹiĹazenĂ­ pĹes RootPtr pouĹžĂ­t dereferenÄnĂ­ operĂĄtor *.
** Ten bude pouĹžit i ve funkcĂ­ch BSTDelete, BSTInsert a BSTDispose.
**/
	*RootPtr=NULL;
	
		
	
}	

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledĂĄ uzel v BVS s klĂ­Äem K.
**
** Pokud je takovĂ˝ nalezen, vracĂ­ funkce hodnotu TRUE a v promÄnnĂŠ Content se
** vracĂ­ obsah pĹĂ­sluĹĄnĂŠho uzlu.Â´Pokud pĹĂ­sluĹĄnĂ˝ uzel nenĂ­ nalezen, vracĂ­ funkce
** hodnotu FALSE a obsah promÄnnĂŠ Content nenĂ­ definovĂĄn (nic do nĂ­ proto
** nepĹiĹazujte).
**
** PĹi vyhledĂĄvĂĄnĂ­ v binĂĄrnĂ­m stromu bychom typicky pouĹžili cyklus ukonÄenĂ˝
** testem dosaĹženĂ­ listu nebo nalezenĂ­ uzlu s klĂ­Äem K. V tomto pĹĂ­padÄ ale
** problĂŠm ĹeĹĄte rekurzivnĂ­m volĂĄnĂ­ tĂŠto funkce, pĹiÄemĹž nedeklarujte ĹžĂĄdnou
** pomocnou funkci.
**/
		if(RootPtr==NULL)
		{
			return FALSE;
		}				
		else
		{
			if(RootPtr->Key!=K)
			{
				if(RootPtr->Key<K)
					return BSTSearch(RootPtr->RPtr,K,Content);
				else
					return BSTSearch(RootPtr->LPtr,K,Content);
			}
			else
			{
				*Content=RootPtr->BSTNodeCont;
				return TRUE;
			}
			return FALSE;
		}		   
	
	
} 


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{	
/*   ---------
** VloĹžĂ­ do stromu RootPtr hodnotu Content s klĂ­Äem K.
**
** Pokud jiĹž uzel se zadanĂ˝m klĂ­Äem ve stromu existuje, bude obsah uzlu
** s klĂ­Äem K nahrazen novou hodnotou. Pokud bude do stromu vloĹžen novĂ˝
** uzel, bude vloĹžen vĹždy jako list stromu.
**
** Funkci implementujte rekurzivnÄ. Nedeklarujte ĹžĂĄdnou pomocnou funkci.
**
** RekurzivnĂ­ implementace je mĂŠnÄ efektivnĂ­, protoĹže se pĹi kaĹždĂŠm
** rekurzivnĂ­m zanoĹenĂ­ uklĂĄdĂĄ na zĂĄsobnĂ­k obsah uzlu (zde integer).
** NerekurzivnĂ­ varianta by v tomto pĹĂ­padÄ byla efektivnÄjĹĄĂ­ jak z hlediska
** rychlosti, tak z hlediska pamÄĹĽovĂ˝ch nĂĄrokĹŻ. Zde jde ale o ĹĄkolnĂ­
** pĹĂ­klad, na kterĂŠm si chceme ukĂĄzat eleganci rekurzivnĂ­ho zĂĄpisu.
**/
		
		if(*RootPtr==NULL)
		{
			*RootPtr=malloc(sizeof(struct tBSTNode));
			(*RootPtr)->Key=K;
			(*RootPtr)->BSTNodeCont=Content;
			(*RootPtr)->RPtr=NULL;
			(*RootPtr)->LPtr=NULL;
		}
		else
		{
			if((*RootPtr)->Key<K)
			{
				BSTInsert((&(*RootPtr)->RPtr),K,Content);
				return;
			}
			
			if((*RootPtr)->Key>K)
			{
				BSTInsert((&(*RootPtr)->LPtr),K,Content);
				return;
			}

			if((*RootPtr)->Key==K)
				(*RootPtr)->BSTNodeCont=Content;
		}
		
	
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** PomocnĂĄ funkce pro vyhledĂĄnĂ­, pĹesun a uvolnÄnĂ­ nejpravÄjĹĄĂ­ho uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterĂŠho bude pĹesunuta hodnota
** nejpravÄjĹĄĂ­ho uzlu v podstromu, kterĂ˝ je urÄen ukazatelem RootPtr.
** PĹedpoklĂĄdĂĄ se, Ĺže hodnota ukazatele RootPtr nebude NULL (zajistÄte to
** testovĂĄnĂ­m pĹed volĂĄnĂ­ tĂŠto funkce). Tuto funkci implementujte rekurzivnÄ. 
**
** Tato pomocnĂĄ funkce bude pouĹžita dĂĄle. NeĹž ji zaÄnete implementovat,
** pĹeÄtÄte si komentĂĄĹ k funkci BSTDelete(). 
**/
	
		if(*RootPtr!=NULL)
		{
			tBSTNodePtr help=NULL;

			if((*RootPtr)->RPtr!=NULL)
				ReplaceByRightmost(PtrReplaced,(&(*RootPtr)->RPtr));
			else
			{
				help=*RootPtr;
				PtrReplaced->BSTNodeCont=help->BSTNodeCont;
				PtrReplaced->Key=help->Key;
				*RootPtr=help->LPtr;
				free(help);
			}
		}
			
	
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** ZruĹĄĂ­ uzel stromu, kterĂ˝ obsahuje klĂ­Ä K.
**
** Pokud uzel se zadanĂ˝m klĂ­Äem neexistuje, nedÄlĂĄ funkce nic. 
** Pokud mĂĄ ruĹĄenĂ˝ uzel jen jeden podstrom, pak jej zdÄdĂ­ otec ruĹĄenĂŠho uzlu.
** Pokud mĂĄ ruĹĄenĂ˝ uzel oba podstromy, pak je ruĹĄenĂ˝ uzel nahrazen nejpravÄjĹĄĂ­m
** uzlem levĂŠho podstromu. Pozor! NejpravÄjĹĄĂ­ uzel nemusĂ­ bĂ˝t listem.
**
** Tuto funkci implementujte rekurzivnÄ s vyuĹžitĂ­m dĹĂ­ve deklarovanĂŠ
** pomocnĂŠ funkce ReplaceByRightmost.
**/
	
		if(*RootPtr!=NULL)
		{
			if((*RootPtr)->Key<K)
				BSTDelete((&(*RootPtr)->RPtr),K);
			if((*RootPtr)->Key>K)
				BSTDelete((&(*RootPtr)->LPtr),K);
			if((*RootPtr)->Key==K)
			{
				tBSTNodePtr help=*RootPtr;
				
				if((*RootPtr)->RPtr==NULL)
				{
					*RootPtr=(*RootPtr)->LPtr;
					free(help);
				}
				else if((*RootPtr)->LPtr==NULL)
				{
					*RootPtr=(*RootPtr)->RPtr;
					free(help);
				}
				else
					ReplaceByRightmost(*RootPtr,(&(*RootPtr)->LPtr));
			
			}
		}
		

} 

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** ZruĹĄĂ­ celĂ˝ binĂĄrnĂ­ vyhledĂĄvacĂ­ strom a korektnÄ uvolnĂ­ pamÄĹĽ.
**
** Po zruĹĄenĂ­ se bude BVS nachĂĄzet ve stejnĂŠm stavu, jako se nachĂĄzel po
** inicializaci. Tuto funkci implementujte rekurzivnÄ bez deklarovĂĄnĂ­ pomocnĂŠ
** funkce.
**/
	
	if(*RootPtr!=NULL)
	{
		BSTDispose(&(*RootPtr)->LPtr);
		BSTDispose(&(*RootPtr)->RPtr);
		free(*RootPtr);
		(*RootPtr)=NULL;	
	}

}

/* konec c401.c */

