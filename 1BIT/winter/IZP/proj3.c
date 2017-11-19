/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX


#define FALSE 1
#define TRUE 0
/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

	c->size = 0;
	c->capacity = cap;
	c->obj= malloc(cap*sizeof(struct obj_t));
    // TODO

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
	free(c->obj);
        c->size = 0;
        c->capacity = 0;
    // TODO
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */ 
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	if(c->size < c->capacity)
	{
		c->obj[c->size] = obj;
	}
	else
	{
		resize_cluster(c,c->capacity+1);
		c->obj[c->size] = obj;
	}
		c->size++;
    // TODO
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

	int i=0;
	while(i < c2->size)
	{
		append_cluster(c1,c2->obj[i]);
		i++;
	}
	sort_cluster(c1);

    // TODO
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

	clear_cluster(&carr[idx]);
	int i=idx;
	while(i<narr-1)
	{
		carr[i]=carr[i+1];
		i++;
	}
	return narr-1;
    // TODO
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);


	return sqrt( ((o1->x)-(o2->x)) * ((o1->x)-(o2->x)) + ((o1->y)-(o2->y)) * ((o1->y)-(o2->y)) );
    // TODO
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
	float cdistance = 10000;
	int i=0;
	while(i < c1->size)
	{
		int u=0;
		while(u < c2->size)
		{
			float distance = obj_distance(&(c1->obj[i]),&(c2->obj[u]));
			if(distance < cdistance)
				cdistance=distance;
			u++;

		}

	i++;
	}

	return cdistance;

    // TODO
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
	float neighbours = cluster_distance(carr,carr+1);
	float neighbour;
	int i=0;
	while(i<narr)
	{
		int u=i+1;
		while(u<narr)
		{
			neighbour=cluster_distance(carr+i,carr+u);
			if(neighbour<neighbours)
			{
				neighbours=neighbour;
				*c1=i;
				*c2=u;
			}
			u++;
		}

		i++;
	}
    // TODO
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
	FILE *file;
	struct obj_t object;
	char many[20];
	struct cluster_t *clusters;
	if (!(file=fopen(filename,"r")))
	{
		*arr=NULL;
		return FALSE;
	}
	fscanf(file,"%s", many);
	int sum=0,i=0;
	sscanf(many, "count=%d", &sum);

	if(sum<=0)
        {
                *arr=NULL;
		return FALSE;
	}

		else
		{
			if(!(clusters = malloc(sum*sizeof(struct cluster_t))))
			{
				*arr=NULL;
				return FALSE;
			}
			while(i<sum)
			{
				struct cluster_t cluster;
				fscanf(file, "%d %f %f", &object.id, &object.x, &object.y);
				init_cluster(&cluster, CLUSTER_CHUNK);
				append_cluster(&cluster, object);
				clusters[i] = cluster;
				i++;
			}

			*arr = &clusters[0];
			fclose(file);
			return sum;

		}
    // TODO
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{	if(argc>3)						//ak je program spusteny s viac ako 3 argumentmi vypise chybu
	{
		fprintf(stderr,"spatne argumenty\n");
		return FALSE;
	}
    struct cluster_t *clusters;
	int HMclusters=load_clusters(argv[1], &clusters);	//do premennej HMclusters sa nacita navratova hodnota z funkcie load_clusters
	if(clusters==NULL)					//v pripade chyby v load_clusters sa vypise chybova hlaska a program sa ukonci
	{
		fprintf(stderr,"Nepodarilo sa otvorit subor alebo subor obsahoval spatne udaje, pripadne sa nepodarila alokacia pamete\n");
		return FALSE;
	}
	else
	{
		if(argv[2]!=NULL && atoi(argv[2])<INT_MAX && atoi(argv[2])>1)	//ak argument s indexom 2 nie je nulovy a zaroven je vacsi jak jedna
		{								//a mensi jak INT_MAX

			if(atoi(argv[2])>HMclusters)                            //v pripade ze argument s indexom 2 je vacsi ako pocet nacitanych objektov
			{                                                       //program vypise chybovu hlasku a ukonci sa
				fprintf(stderr,"Nie je mozne rozdelit nacitane objekty do zadaneho poctu clusterov\n");
		                return FALSE;
        		}



			int Mclusters=atoi(argv[2]);
			int r_Mclusters = HMclusters-Mclusters;			//program zavola zaradom funkcie find_neighbours, merge_clusters
			int i=0,c1,c2;						//a remove_clusters

				while(i<r_Mclusters)		//cele sa to opakuje pokial premenna i je mesia ako r_Mclusters
				{				//premenna r-Mclusters sa rovna rozdielu nacitanych clusterov - argument s indexom 2
					find_neighbours(clusters,HMclusters,&c1,&c2);
					merge_clusters(&clusters[c1],&clusters[c2]);
					HMclusters=remove_cluster(clusters,HMclusters,c2);
					i++;
				}
			print_clusters(clusters,HMclusters);	//vypisu sa clustre
		}

			else				//v inom pripade a teda ked argument s indexom 2 nebude zadany alebo bude 1
			{				//zname na to ze vysledny pocet clusterov bude 1
				while(clusters[1].size != 0)		//takze pokial size clustera s indexom 1 nebude rovna 0
                                {					//program bude volat funkciu merge_clusters a remove_cluster
                                        merge_clusters(&clusters[0],&clusters[1]);//nemusime volat funkciu find_neighbours
                                        HMclusters=remove_cluster(clusters,HMclusters,1);//pretoze vysledkom su vsetky objekty v jednom clustery
                                }

				print_clusters(clusters, HMclusters);//vypisu sa clustre
			}
		while(HMclusters > 0)
		{
		HMclusters = remove_cluster(clusters, HMclusters, (HMclusters-1));//na konci po vypise clusterov program vymaze clustery
		}

	free(clusters);//a uvolni alocovanu pamet
	}
return TRUE;

// TODO
}
