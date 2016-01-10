#include "train.h"

ASt InitS(size_t d)/* initializeaza stiva cu elemente de dimensiune d */
{
	ASt s;
	s=(ASt)malloc(sizeof(TStiva));
	if(!s)	return NULL;
	s->max=0;
	s->dime=d;
	s->vf=NULL;
	return s;
}

TDepou *InitD(size_t Lin)// initializeaza depoul cu Lin linii 
{
	TDepou *d;
	d=(TDepou *)malloc(sizeof(TDepou));
	if(!d) return NULL;
	d->L=Lin;
	d->v=malloc(Lin*sizeof(ASt));
	if(!d->v)
	{
		free(d);printf("maria");
		return NULL;
	}
	return d;
}	

int isEmpty(void *s)/* returneaza 1 daca stiva s este vida, 0 in caz contrar */
{
	if(VF(s)==NULL) return 1;
	return 0;
}

int Push(void *s, void *ae)//insereaza element in stiva
{
	int *aelem;
	TLG aux;
	aelem=malloc(sizeof(int));
	if(!aelem) return 0;
	//creaza celula de lista
	aux=(TLG)malloc(sizeof(TCelulaG));
	if(!aux) 
	{
		free(aelem);
		return 0;
	}
	aux->info=malloc(sizeof(int));
	if(!aux->info)
	{
		free(aux);
		free(aelem);
		return 0;
	}
	memcpy(aelem,ae,sizeof(int));
	*(int *)(aux->info)=*aelem;//valoarea informatiei din celula devine valoarea informatiei de la adresa aelem
	aux->urm=VF(s);
	VF(s)=aux;
	free(aelem);
	return 1;
}

int Pop(void *s,void *ae)//extrage element din stiva
{
	int *aelem;
	TLG aux;
	if (isEmpty(s)==1) return 0;
	aelem=malloc(sizeof(int));
	if(!aelem) return 0;
	aux=VF(s);
	memcpy(aelem,(int *)aux->info,sizeof(int));
	*(int *)ae=*aelem;//valoarea informatiei de la adresa ae devine valoarea informatiei de la adresa aelem
	VF(s)=aux->urm;
	free(aux->info);
	free(aux);
	free(aelem);
	return 1;
}

int Top(void *s,void *ae)//obtine valoarea elementului din varful stivei
{
	if (isEmpty(s)==1) return 0;
	int *aelem;
	aelem=malloc(sizeof(int));
	if(!aelem) return 0;
	memcpy(aelem,(int *)VF(s)->info,sizeof(int));
	*(int *)ae=*aelem;//valoarea informatiei de la adresa ae devine valoarea informatiei de la adresa aelem
	free(aelem);
	return 1;
}


void DistrS(ASt *s)//eliberare spatiu alocat stiva
{
	TLG aux;
	for(;(*s)->vf!=NULL;)
	{
		aux=(*s)->vf;
		(*s)->vf=aux->urm;
		free(aux->info);
		free(aux);
	}
	free((*s)->vf);
	free(*s);
	
}


void ElibD(TDepou **ad)//eliberare spatiu alocat depou
{
	int i;
	for(i=0;i<(*ad)->L;i++)/*se parcurge depoul pentru eliberarea spatiului de memorie ocupat*/
	{
		DistrS(&(*ad)->v[i]);
	}
	free((*ad)->v);
	free(*ad);
}


