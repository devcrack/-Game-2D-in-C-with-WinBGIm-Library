#include <stdio.h>
#include <iostream>
#include <graphics.h>

typedef struct malla
{
 int info;
 struct malla *prev;
 struct malla *next;
 struct malla *up;
 struct malla *down;
};

typedef struct malla *MALLA;



int main()
{
 initwindow( 640 , 480 , "WinBGIm" );
 outtextxy( 0 ,0 , "Hello WinBGIm!" );
 bgiout << "Hello WinBGIm-Stream!" ;
 outstreamxy( 0 , 15 ); 
 printf( "Hello Console!\n" );
 cout << "Hello Console-Stream!\n" ;
 while( !kbhit() ); 
 closegraph( );
 return( 0 );
}

int crea_malla()
{
 int i,j;
 for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      {
       //CASOS
       if(i==0)
       if(i==n-1)
       if(j==0)
       if(j==n-1)
       
       
       
       res=crea_nodo()
       aux=aux->sig;
      }
}


int crea_nodo_malla(MALLA *nodo,int n)
{
 int band=0;
 *nodo=(MALLA)malloc(sizeof(struct malla));
 if(*nodo)
  {
   (*nodo)->info=n;
   (*nodo)->up=NULL;
   (*nodo)->down=NULL;
   (*nodo)->prev=NULL;
   (*nodo)->next=NULL;
   band=1;
  }
}







