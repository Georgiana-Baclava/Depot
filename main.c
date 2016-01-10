#include "train.h"

typedef struct
{
	int nrVag, tipVag;
} Train;//se retine numarul vagoanelor si tipul lor


int Rastoarna(void *ad, void *as)//rastoarna stiva sursa in stiva destinatie
{	int rez,count=0;
	if(isEmpty(as)) return -1;
	TLG aux=VF(as);
	while(aux)
	{
		rez=Push(ad,aux->info);
		if(rez==1) count++;
		aux=aux->urm;
	}
	return count;
}

void AfisareD(TDepou *d)//afisaza configuratia depoului
{	
	int rez,i,count=0,nr=0;
	Train tr;
	TLG aux;
	ASt st;
	for(i=0;i<d->L;i++)
	{	//pentru fiecare stiva din depou, ii afisaza indicele si numarul de elemente din stiva
		count=0;
		printf("%d: %d",i,d->v[i]->max);
		tr.nrVag=1;
		st=InitS(sizeof(TLG));
		if(!st) return;
		Rastoarna(st,d->v[i]);//rastoarna stiva curenta in stiva st
		aux=VF(st);
		if(aux) tr.tipVag=*(int *)aux->info;
		while(aux)
		{
			//cat timp exista elemente in stiva noua
				if(!aux->urm)
				{	/*daca nu exista element urmator afiseaza numarul vagoanelor si tipul reprezentat de informatia curenta*/
					tr.tipVag=*(int *)aux->info;
					printf(" %d %d",tr.nrVag,tr.tipVag);
					break;
				}
			if(tr.tipVag==*(int *)aux->urm->info)
			{	
				/* Daca informatia coincide cu cea din celula urmatoare numarul vagoanelor de acel tip creste cu 1 */
				tr.nrVag++;tr.tipVag=*(int *)aux->info;
	
			}
			
			else if(tr.tipVag!=*(int *)aux->urm->info)
			{	
				/* in caz contrar, se afiseaza numarul vagoanelor de acelasi tip si tipul lor gasite pana la acel moment, numarul vagoanelor devine 1 si tipul vagonului se initializeaza cu informatia gasita in celula urmatoare	*/		
				printf(" %d %d",tr.nrVag,tr.tipVag);
				tr.nrVag=1;
				tr.tipVag=*(int *)aux->urm->info;
			}
		
			aux=aux->urm;
		}
		printf("\n");
		/* la sfarsit se distruge stiva rasturnata */
		DistrS(&st);
	}
	printf("\n");
}


int Verifica(TDepou *d, int N, int M, int lgmax)
{
/* verifica daca exista spatiu in depou pentru acomodarea unui tren. Returneaza 1 in caz afirmativ, 0 in caz contrar. */
    int nr=0, i;
    TLG aux;
    for(i=0;i<N;i++)
    {
        aux=VF(d->v[i]);
        while(aux!=NULL)
        {
            nr++;
            aux=aux->urm;
        }
    }
    if(N*M-nr>=lgmax)
        return 1;
    return 0;
}


TDepou * Acomodare(TDepou *d, void *tr, int M)
{	
	//descarcarea trenului in depou
	int i,rez,rez1,elem;
	
	i=0;
	while(isEmpty(tr)!=1)
	{	
		/* de la prima linie a depoului se cauta spatiu de descarcare a fiecarui vagon */
		for(;;)
		{	
			if(d->v[i]->max==M) break;
			/* Daca linia curenta nu este plina se extrage un element din stiva tren si se insereaza pe linia respectiva */ 
			
			rez=Pop(tr,&elem); 
			if(rez==0) return NULL;
			rez1=Push(d->v[i],&elem); 
			if(rez1==0) return NULL;
			d->v[i]->max++;
			/* daca se ajunge la ultima linie se insereaza daca nu este plina si se revine la prima linie. Altfel creste indicele liniei */
			break;
		}
		i++;
		if(i==d->L) i=0;
	}

	return d;//returneaza adresa depoului
}

int Cauta(ASt s,int *x)
{
	/* returneaza 1 daca vagonul cautat se afla pe linia respectiva, 0 in caz contrar */
	TLG aux;
	for(aux=s->vf;aux!=NULL;aux=aux->urm)
	{
		if(*(int *)aux->info==*x)
			return 1;
	}
	
	return 0;
}


int CautaVagoaneS(ASt s,int *x)
{
	//returneaza numarul vagoanelor de pe o anumita linie
	TLG aux;
	int nr=0;
	for(aux=s->vf;aux!=NULL;aux=aux->urm)
	{
		if(*(int *)aux->info==*x)
			nr++;
	}
	
	return nr;
}


TDepou * Construire(TDepou *d, ASt tr,int *x,int M)
{	
	//insereaza in stiva trenului care trebuie construit, vagonul dat ca parametru
	int rez,indice,r=0,i,elem;
	TLG aux;

	for(i=0;i<d->L;i++)
	{	
		r=Cauta(d->v[i],x);
		if(r==1)  break;
	}
	
	/* cand se gaseste vagonul in depou se obtine valoarea elementului din stiva pe care s-a gasit vagonul */
	
	rez=Top(d->v[i],&elem);
	if(rez==0) return NULL;
	indice=i;
	while(elem!=*x)
	{	

		/* daca valoare obtinuta de Top nu coincide cu vagonul cautat, se muta pe linia urmatoare daca nu este plina. Circularitatea se pastreaza ca la acomodare */
		indice++;
		
		if(indice==d->L) indice=0;
		
		if(indice!=i && d->v[indice]->max!=M)
		{
			rez=Pop(d->v[i],&elem);
			if(rez==0) return NULL;
			d->v[i]->max--;//numarul elementelor din stiva curenta a depoului scade
			rez=Push(d->v[indice],&elem);
			if(rez==0) return NULL;
			d->v[indice]->max++;//creste numarul elementelor din stiva depoului pe care se muta elementul
			rez=Top(d->v[i],&elem);
			if(rez==0) return NULL;
			
			
		}
	}
	/* Cand valoarea elementul din varful stivei coincide cu vagonul cautat se extrage si se insereaza in stiva trenului. */
	rez=Pop(d->v[i],&elem);
	if(rez==0) return NULL;
	d->v[i]->max--;//numarul elementelor din stiva curenta a depoului scade
	rez=Push(tr,&elem);
	if(rez==0) return NULL;
	tr->max++;//numarul elementelor din stiva trenului de construit creste
	return d;
	

}


int CautaVagoaneD(TDepou *d, int *x)//intoarce numarul de vagoane de un anumit tip din depou
{
	int i,rez=0;
	for(i=0;i<d->L;i++)
		rez+=CautaVagoaneS(d->v[i],x);
	return rez;
}

int CheckVagoaneD(ASt s,TDepou *d)
{
/* verifica daca numarul de vagoane de un anumit tip, cerut pentru construirea unui tren este mai mare decat numarul acestora in depou. In acest caz nu se poate forma trenul si intoarce -1. In caz contrar, returneza 1. */
	TLG aux;
	int n,m;
	for(aux=s->vf;aux!=NULL;aux=aux->urm)
	{
		n=CautaVagoaneS(s,(int *)aux->info);
		m=CautaVagoaneD(d,(int *)aux->info);
		if(n>m) return -1;	
	}
	return 1;
}



int main(int argc, char **argv)
{	
	TLG aux;
	int sum,nrvag=0,i,ok=0,r,j,N,M,nOp,rez,tip,x,lgmax,op;
	TDepou *d;
	ASt train,s,rs;
	Train tr;
	FILE *in;
	in=fopen(argv[1],"r");//deschidere fisier input
	if(!in) return -1;
	fscanf(in,"%d",&N);//citire numar linii din depou
	fscanf(in,"%d",&M);//citire numar capatitate maxima a unei linii
	fscanf(in,"%d",&nOp);//citire numar operatii
	d=InitD(N);//initializare depou
	if(!d) return -1;
	for(i=0;i<d->L;i++)
	{
		d->v[i]=InitS(sizeof(TLG));
		if(!d->v[i]) 
		{
			for(j=0;j<i;j++)
				DistrS(&d->v[j]);
			ElibD(&d);
			return -1;
		}
	}	
	while(nOp>0)
	{	
		/* cat timp nu am terminat de realizat toate operatiile, aflu numarul de vagoane din depou la momentul respectiv, citesc tipul operatiei si lungimea maxima a trenului */
		for(i=0;i<d->L;i++)
			nrvag+=d->v[i]->max;

		fscanf(in,"%d",&op);
		fscanf(in,"%d",&lgmax);
	
		/* daca trebuie acomodat un tren cu o lungime mai mare decat capacitatea maxima a depoului N*M, afisez un mesaj de eroare si trec la linia urmatoare a fisierului */
		if(op==0 && Verifica(d,N,M,lgmax)==0)
		{	printf("Dock error!\n");
			sum=0;
			while(sum<lgmax)
			{
				fscanf(in,"%d",&tr.nrVag);
				fscanf(in,"%d",&tr.tipVag);
				sum+=tr.nrVag;
			}
		}
	

		if(op==0 && Verifica(d,N,M,lgmax)==1)
		{	
			/* daca trebuie acomodat un tren, iar capacitatea depoului este mai mare sau egala cu lungimea trenului, initializez stiva train */
			train=InitS(sizeof(TLG));
			if(!train) 
			{
				ElibD(&d);
				return -1;
			}
			sum=0;
			while(sum<lgmax)
			{
				/* cat timp nu am terminat de citit vagoanele si tipul lor, citesc numarul vagoanelor tr.nrVag si tipul tr.tipVag si inserez in stiva train tr.nrVag vagoane de tipul respectiv */
				fscanf(in,"%d",&tr.nrVag);
				fscanf(in,"%d",&tr.tipVag);
				sum+=tr.nrVag;
				for(j=0;j<tr.nrVag;j++)
					Push(train,&tr.tipVag);
			}
			Acomodare(d,train,M);//acomodare tren
			DistrS(&train);//distrugere stiva train
			
		}
		
		if(op==1)
		{

				/* daca trebuie construit un tren, iar numarul vagoanelor din depou la momentul respectiv este mai mic decat lungimea maxima a trenului, afisez un mesaj de eroare si trec la linia urmatoare a fisierului */
			if(nrvag<lgmax)
			{	printf("Train error!\n");
				sum=0;
				while(sum<lgmax)
				{
					fscanf(in,"%d",&tr.nrVag);
					fscanf(in,"%d",&tr.tipVag);
					sum+=tr.nrVag;
				}		
			}
			/* daca trebuie construit un tren si numarul vagoanelor din depou este mai mare sau egal cu lungimea maxima a trenului initializez stiva in care construiesc trenul, stiva in care retin trenul la citire si stiva resturnata a stivei de la citire */
			else
			{
				train=InitS(sizeof(TLG));
				if(!train) 
				{
					ElibD(&d);
					return -1;
				}
				s=InitS(sizeof(TLG));
				if(!s) 
				{
					DistrS(&train);
					ElibD(&d);
					return -1;
				}
				rs=InitS(sizeof(TLG));
				if(!rs) 
				{
					DistrS(&s);
					DistrS(&train);
					ElibD(&d);
					return -1;
				}
				sum=0; ok=0; r=0;
				while(sum<lgmax)
				{
					//cat timp nu am terminat de citit vagoanele si tipul lor
					fscanf(in,"%d",&tr.nrVag);//citire numar vagoane
					fscanf(in,"%d",&tr.tipVag);//citire tip vagoane
				
					/* verific daca numarul de vagoane cerut pentru construirea trenului este mai mic sau egal cu numarul vagoanelor de acelasi tip din depou */
					rez=0;
					for(i=0;i<d->L;i++)
						rez+=CautaVagoaneS(d->v[i],&tr.tipVag);
					
					if(rez<tr.nrVag)
					{
						/* daca numarul de vagoane cerut este mai mare decat numarul lor in depou, afisez un mesaj de eroare si distrug stivele initializate */
						printf("Train error!\n");
						sum+=tr.nrVag;
						while(sum<lgmax)
						{
							fscanf(in,"%d",&tr.nrVag);
							fscanf(in,"%d",&tr.tipVag);
							sum+=tr.nrVag;
						}
					
							/*DistrS(&s);
							DistrS(&rs);
							DistrS(&train);*/
						ok=-1;					
						break;
					}
					sum+=tr.nrVag;
					// construiesc stiva trenului citit
					for(j=0;j<tr.nrVag;j++)
						Push(s,&tr.tipVag);
				}
				r=CheckVagoaneD(s,d);//printf("ro=%d,ok=%d\n",ro,ok);
				if(ok!=-1 && r==-1) printf("Train error!\n");
					
				if(ok!=-1 && r!=-1)
				{
					Rastoarna(rs,s);//rastorn stiva s in rs
					while(isEmpty(rs)!=1)
					{
						/* cat timp rs nu este vida extrag cate un element din stiva rs si apelez functia Construire cu acel element ca parametru */
						rez=Pop(rs,&x);
						if(rez==0) 
						{
							DistrS(&rs);
							DistrS(&s);
							DistrS(&train);
							ElibD(&d);
							return -1;
						}
						rs->max--;
						Construire(d,train,&x,M);
					}	
				}

				//La sfarsit distrug stivele initializate
				DistrS(&rs);
				DistrS(&train);
				DistrS(&s);
			}
		}
			
		/* Pentru fiecare optiune afisez depoul, numarul vagoanelor se initoalizeaza cu 0, iar numarul optiunilor scade cu 1 */
		AfisareD(d);
		nrvag=0;
		nOp--;

	}

	//la sfarsit, inchid fisierul si eliberez depoul
	fclose(in);
	ElibD(&d);
	
	return 0;
}
