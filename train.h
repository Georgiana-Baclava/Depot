#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _DEPOT_MANAGER_
#define _DEPOT_MANAGER_


typedef struct celst
{
	struct celst *urm;
	void *info;
} TCelulaG, *TLG, **ALG;//structura pentru lista

typedef struct stiva
{
	size_t dime, max;
	TLG vf;
} TStiva, *ASt;//structura pentru stiva

typedef struct depou
{
	size_t L;
	ASt *v;
} TDepou;//structura pentru depou

#define VF(a) (((ASt)(a))->vf)//varful stivei
#define DIME(a) (((ASt)(a))->dime)//dimensiunea unui element din stiva


ASt InitS(size_t d);/* initializeaza stiva cu elemente de dimensiune d */

TDepou *InitD(size_t Lin);/* initializeaza depoul cu Lin linii si M capacitatea maxima */

int Push(void* a, void* ae);

int Pop (void* a, void* ae);

int Top (void* a, void* ae);

int isEmpty(void *s);

void DistrS(ASt *s);//elibereaza stiva

void ElibD(TDepou **ad);//elibereaza depou


#endif
