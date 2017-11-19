
/* c402.c: ********************************************************************}
{* TĂŠma: NerekurzivnĂ­ implementace operacĂ­ nad BVS 
**                                     Implementace: Petr PĹikryl, prosinec 1994
**                                           Ăpravy: Petr PĹikryl, listopad 1997
**                                                     Petr PĹikryl, kvÄten 1998
**			  	                        PĹevod do jazyka C: Martin TuÄek, srpen 2005
**                                         Ăpravy: Bohuslav KĹena, listopad 2009
**                                         Ăpravy: Karel MasaĹĂ­k, ĹĂ­jen 2013
**                                         Ăpravy: Radek HranickĂ˝, ĹĂ­jen 2014
**                                         Ăpravy: Radek HranickĂ˝, listopad 2015
**                                         Ăpravy: Radek HranickĂ˝, ĹĂ­jen 2016
**
** S vyuĹžitĂ­m dynamickĂŠho pĹidÄlovĂĄnĂ­ pamÄti, implementujte NEREKURZIVNÄ
** nĂĄsledujĂ­cĂ­ operace nad binĂĄrnĂ­m vyhledĂĄvacĂ­m stromem (pĹedpona BT znamenĂĄ
** Binary Tree a je u identifikĂĄtorĹŻ uvedena kvĹŻli moĹžnĂŠ kolizi s ostatnĂ­mi
** pĹĂ­klady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivnĂ­ vloĹženĂ­ novĂŠho uzlu do stromu
**     BTPreorder ...... nerekurzivnĂ­ prĹŻchod typu pre-order
**     BTInorder ....... nerekurzivnĂ­ prĹŻchod typu in-order
**     BTPostorder ..... nerekurzivnĂ­ prĹŻchod typu post-order
**     BTDisposeTree ... zruĹĄ vĹĄechny uzly stromu
**
** U vĹĄech funkcĂ­, kterĂŠ vyuĹžĂ­vajĂ­ nÄkterĂ˝ z prĹŻchodĹŻ stromem, implementujte
** pomocnou funkci pro nalezenĂ­ nejlevÄjĹĄĂ­ho uzlu v podstromu.
**
** PĹesnĂŠ definice typĹŻ naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na nÄj je typu tBTNodePtr. Jeden uzel obsahuje poloĹžku int Cont,
** kterĂĄ souÄasnÄ slouĹžĂ­ jako uĹžiteÄnĂ˝ obsah i jako vyhledĂĄvacĂ­ klĂ­Ä 
** a ukazatele na levĂ˝ a pravĂ˝ podstrom (LPtr a RPtr).
**
** PĹĂ­klad slouĹžĂ­ zejmĂŠna k procviÄenĂ­ nerekurzivnĂ­ch zĂĄpisĹŻ algoritmĹŻ
** nad stromy. NeĹž zaÄnete tento pĹĂ­klad ĹeĹĄit, prostudujte si dĹŻkladnÄ
** principy pĹevodu rekurzivnĂ­ch algoritmĹŻ na nerekurzivnĂ­. ProgramovĂĄnĂ­
** je pĹedevĹĄĂ­m inĹženĂ˝rskĂĄ disciplĂ­na, kde opÄtnĂŠ objevovĂĄnĂ­ Ameriky nemĂĄ
** mĂ­sto. Pokud se VĂĄm zdĂĄ, Ĺže by nÄco ĹĄlo zapsat optimĂĄlnÄji, promyslete
** si vĹĄechny detaily VaĹĄeho ĹeĹĄenĂ­. PovĹĄimnÄte si typickĂŠho umĂ­stÄnĂ­ akcĂ­
** pro rĹŻznĂŠ typy prĹŻchodĹŻ. Zamyslete se nad modifikacĂ­ ĹeĹĄenĂ˝ch algoritmĹŻ
** napĹĂ­klad pro vĂ˝poÄet poÄtu uzlĹŻ stromu, poÄtu listĹŻ stromu, vĂ˝ĹĄky stromu
** nebo pro vytvoĹenĂ­ zrcadlovĂŠho obrazu stromu (pouze popĹehazovĂĄnĂ­ ukazatelĹŻ
** bez vytvĂĄĹenĂ­ novĂ˝ch uzlĹŻ a ruĹĄenĂ­ starĂ˝ch).
**
** PĹi prĹŻchodech stromem pouĹžijte ke zpracovĂĄnĂ­ uzlu funkci BTWorkOut().
** Pro zjednoduĹĄenĂ­ prĂĄce mĂĄte pĹedem pĹipraveny zĂĄsobnĂ­ky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro prĂĄci
** s pomocnĂ˝mi zĂĄsobnĂ­ky neupravujte 
** Pozor! Je tĹeba sprĂĄvnÄ rozliĹĄovat, kdy pouĹžĂ­t dereferenÄnĂ­ operĂĄtor *
** (typicky pĹi modifikaci) a kdy budeme pracovat pouze se samotnĂ˝m ukazatelem 
** (napĹ. pĹi vyhledĂĄvĂĄnĂ­). V tomto pĹĂ­kladu vĂĄm napovĂ­ prototypy funkcĂ­.
** Pokud pracujeme s ukazatelem na ukazatel, pouĹžijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** PomocnĂĄ funkce, kterou budete volat pĹi prĹŻchodech stromem pro zpracovĂĄnĂ­
** uzlu urÄenĂŠho ukazatelem Ptr. Tuto funkci neupravujte.
**/
			
	if (Ptr==NULL) 
    printf("Chyba: Funkce BTWorkOut byla volĂĄna s NULL argumentem!\n");
  else 
    printf("VĂ˝pis hodnoty danĂŠho uzlu> %d\n",Ptr->Cont);
}
	
/* -------------------------------------------------------------------------- */
/*
** Funkce pro zĂĄsobnĂ­k hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)  
/*   ------
** Inicializace zĂĄsobnĂ­ku.
**/
{
	S->top = 0;  
}	

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** VloĹžĂ­ hodnotu na vrchol zĂĄsobnĂ­ku.
**/
{ 
                 /* PĹi implementaci v poli mĹŻĹže dojĂ­t k pĹeteÄenĂ­ zĂĄsobnĂ­ku. */
  if (S->top==MAXSTACK) 
    printf("Chyba: DoĹĄlo k pĹeteÄenĂ­ zĂĄsobnĂ­ku s ukazateli!\n");
  else {  
		S->top++;  
		S->a[S->top]=ptr;
	}
}	

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** OdstranĂ­ prvek z vrcholu zĂĄsobnĂ­ku a souÄasnÄ vrĂĄtĂ­ jeho hodnotu.
**/
{
                            /* Operace nad prĂĄzdnĂ˝m zĂĄsobnĂ­kem zpĹŻsobĂ­ chybu. */
	if (S->top==0)  {
		printf("Chyba: DoĹĄlo k podteÄenĂ­ zĂĄsobnĂ­ku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top--]);
	}	
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zĂĄsobnĂ­k prĂĄzdnĂ˝, vrĂĄtĂ­ hodnotu true.
**/
{
  return(S->top==0);
}	

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zĂĄsobnĂ­k hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zĂĄsobnĂ­ku.
**/

	S->top = 0;  
}	

void SPushB (tStackB *S,bool val) {
/*   ------
** VloĹžĂ­ hodnotu na vrchol zĂĄsobnĂ­ku.
**/
                 /* PĹi implementaci v poli mĹŻĹže dojĂ­t k pĹeteÄenĂ­ zĂĄsobnĂ­ku. */
	if (S->top==MAXSTACK) 
		printf("Chyba: DoĹĄlo k pĹeteÄenĂ­ zĂĄsobnĂ­ku pro boolean!\n");
	else {
		S->top++;  
		S->a[S->top]=val;
	}	
}

bool STopPopB (tStackB *S) {
/*   --------
** OdstranĂ­ prvek z vrcholu zĂĄsobnĂ­ku a souÄasnÄ vrĂĄtĂ­ jeho hodnotu.
**/
                            /* Operace nad prĂĄzdnĂ˝m zĂĄsobnĂ­kem zpĹŻsobĂ­ chybu. */
	if (S->top==0) {
		printf("Chyba: DoĹĄlo k podteÄenĂ­ zĂĄsobnĂ­ku pro boolean!\n");
		return(NULL);	
	}	
	else {  
		return(S->a[S->top--]); 
	}	
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zĂĄsobnĂ­k prĂĄzdnĂ˝, vrĂĄtĂ­ hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** NĂĄsleduje jĂĄdro domĂĄcĂ­ Ăşlohy - funkce, kterĂŠ mĂĄte implementovat. 
*/

void BTInit (tBTNodePtr *RootPtr)	{

	*RootPtr=NULL;
/*   ------
** Provede inicializaci binĂĄrnĂ­ho vyhledĂĄvacĂ­ho stromu.
**
** Inicializaci smĂ­ programĂĄtor volat pouze pĹed prvnĂ­m pouĹžitĂ­m binĂĄrnĂ­ho
** stromu, protoĹže neuvolĹuje uzly neprĂĄzdnĂŠho stromu (a ani to dÄlat nemĹŻĹže,
** protoĹže pĹed inicializacĂ­ jsou hodnoty nedefinovanĂŠ, tedy libovolnĂŠ).
** Ke zruĹĄenĂ­ binĂĄrnĂ­ho stromu slouĹžĂ­ procedura BTDisposeTree.
**	
** VĹĄimnÄte si, Ĺže zde se poprvĂŠ v hlaviÄce objevuje typ ukazatel na ukazatel,	
** proto je tĹeba pĹi prĂĄci s RootPtr pouĹžĂ­t dereferenÄnĂ­ operĂĄtor *.
**/
	

}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** VloĹžĂ­ do stromu novĂ˝ uzel s hodnotou Content.
**
** Z pohledu vklĂĄdĂĄnĂ­ chĂĄpejte vytvĂĄĹenĂ˝ strom jako binĂĄrnĂ­ vyhledĂĄvacĂ­ strom,
** kde uzly s hodnotou menĹĄĂ­ neĹž mĂĄ otec leĹžĂ­ v levĂŠm podstromu a uzly vÄtĹĄĂ­
** leĹžĂ­ vpravo. Pokud vklĂĄdanĂ˝ uzel jiĹž existuje, neprovĂĄdĂ­ se nic (danĂĄ hodnota
** se ve stromu mĹŻĹže vyskytnout nejvĂ˝ĹĄe jednou). Pokud se vytvĂĄĹĂ­ novĂ˝ uzel,
** vznikĂĄ vĹždy jako list stromu. Funkci implementujte nerekurzivnÄ.
**/
	
		tBTNodePtr help=*RootPtr;
		tBTNodePtr father=NULL;

		
			while(help!=NULL)
			{
				father=help;

				if(help->Cont==Content)
					return;

				if(help->Cont>Content)
					help=help->LPtr;
				else
					help=help->RPtr;
	
			}

			help=malloc(sizeof(struct tBTNode));
			help->Cont=Content;
			help->RPtr=NULL;
			help->LPtr=NULL;

			if((*RootPtr)!=NULL)
			{
				if(father->Cont>Content)
					father->LPtr=help;
				else
					father->RPtr=help;
			}
			else
				*RootPtr=help;
		
		
	
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levÄ vÄtvi podstromu, dokud nenarazĂ­ na jeho nejlevÄjĹĄĂ­ uzel.
**
** PĹi prĹŻchodu Preorder navĹĄtĂ­venĂŠ uzly zpracujeme volĂĄnĂ­m funkce BTWorkOut()
** a ukazatele na nÄ is uloĹžĂ­me do zĂĄsobnĂ­ku.
**/
	while(ptr!=NULL)
	{
		BTWorkOut(ptr);
		SPushP(Stack,ptr);
		ptr=ptr->LPtr;
	}
	
	
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** PrĹŻchod stromem typu preorder implementovanĂ˝ nerekurzivnÄ s vyuĹžitĂ­m funkce
** Leftmost_Preorder a zĂĄsobnĂ­ku ukazatelĹŻ. ZpracovĂĄnĂ­ jednoho uzlu stromu
** realizujte jako volĂĄnĂ­ funkce BTWorkOut(). 
**/

	tStackP SStack;
	SInitP(&SStack);
	tBTNodePtr help;
	Leftmost_Preorder(RootPtr,&SStack);
	while(!SEmptyP(&SStack))
	{
		help=STopPopP(&SStack);
		Leftmost_Preorder(help->RPtr,&SStack);
	}	
	
}


/*                                  INORDER                                   */ 

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levÄ vÄtvi podstromu, dokud nenarazĂ­ na jeho nejlevÄjĹĄĂ­ uzel.
**
** PĹi prĹŻchodu Inorder uklĂĄdĂĄme ukazatele na vĹĄechny navĹĄtĂ­venĂŠ uzly do
** zĂĄsobnĂ­ku. 
**/
	while(ptr!=NULL)
	{
		SPushP(Stack,ptr);
		ptr=ptr->LPtr;
	}
	
	
	
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** PrĹŻchod stromem typu inorder implementovanĂ˝ nerekurzivnÄ s vyuĹžitĂ­m funkce
** Leftmost_Inorder a zĂĄsobnĂ­ku ukazatelĹŻ. ZpracovĂĄnĂ­ jednoho uzlu stromu
** realizujte jako volĂĄnĂ­ funkce BTWorkOut(). 
**/
	tStackP SStack;
	SInitP(&SStack);
	tBTNodePtr help;
	Leftmost_Inorder(RootPtr,&SStack);
	while(!SEmptyP(&SStack))
	{
		help=STopPopP(&SStack);
		BTWorkOut(help);
		Leftmost_Inorder(help->RPtr,&SStack);
	}	
	
	
}

/*                                 POSTORDER                                  */ 

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levÄ vÄtvi podstromu, dokud nenarazĂ­ na jeho nejlevÄjĹĄĂ­ uzel.
**
** PĹi prĹŻchodu Postorder uklĂĄdĂĄme ukazatele na navĹĄtĂ­venĂŠ uzly do zĂĄsobnĂ­ku
** a souÄasnÄ do zĂĄsobnĂ­ku bool hodnot uklĂĄdĂĄme informaci, zda byl uzel
** navĹĄtĂ­ven poprvĂŠ a Ĺže se tedy jeĹĄtÄ nemĂĄ zpracovĂĄvat. 
**/

	while(ptr!=NULL)
	{
		SPushP(StackP,ptr);
		SPushB(StackB,TRUE);
		ptr=ptr->LPtr;
	}	
	
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** PrĹŻchod stromem typu postorder implementovanĂ˝ nerekurzivnÄ s vyuĹžitĂ­m funkce
** Leftmost_Postorder, zĂĄsobnĂ­ku ukazatelĹŻ a zĂĄsobnĂ­ku hotdnot typu bool.
** ZpracovĂĄnĂ­ jednoho uzlu stromu realizujte jako volĂĄnĂ­ funkce BTWorkOut(). 
**/

	tStackP pstack;
	tStackB bstack;
	SInitP(&pstack);
	SInitB(&bstack);
	tBTNodePtr help;

	Leftmost_Postorder(RootPtr, &pstack, &bstack);
	while(!SEmptyP(&pstack))
	{
		
		if(!STopPopB(&bstack))
		{
			help=STopPopP(&pstack);
			BTWorkOut(help);
		}
		else
		{
			help=STopPopP(&pstack);
			SPushP(&pstack,help);
			
			SPushB(&bstack,FALSE);
			Leftmost_Postorder(help->RPtr,&pstack,&bstack);
		}
	}
			
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** ZruĹĄĂ­ vĹĄechny uzly stromu a korektnÄ uvolnĂ­ jimi zabranou pamÄĹĽ.
**
** Funkci implementujte nerekurzivnÄ s vyuĹžitĂ­m zĂĄsobnĂ­ku ukazatelĹŻ.
**/

	tStackP SStack;
	SInitP(&SStack);
	tBTNodePtr help;

	while(*RootPtr!=NULL || !SEmptyP(&SStack))
	{
		if(*RootPtr==NULL && !SEmptyP(&SStack))
			*RootPtr=STopPopP(&SStack);
		else
		{
			help=*RootPtr;
			*RootPtr=(*RootPtr)->LPtr;
			
			if(help->RPtr!=NULL)
			{
				SPushP(&SStack,help->RPtr);
			}		

			free(help);
		}


	}
	
}

/* konec c402.c */

