#include <stdio.h>
#include <iostream>
#include <graphics.h>
#include <time.h>


#define TAM 45 //45tamaño del nodo de la Malla
#define NREN 14   //NUMERO DE NODOS QUE CONTENDRA LA MALLA (controla los ciclos for que crean la malla)
#define NCOL 23
#define NUMBOTONES 4
#define PRE  0
#define NORMAL  1
#define NR 20

typedef	struct
{
 int x, y,num;
 int tipo,tamx,tamy; 
 char texto[50];
}TBoton;


typedef struct nodo
{
  char info;
  int existe;
  int cor_x;
  int cor_y;
  struct nodo *sig,*ant;
  struct nodo *aba,*arr;
  struct jug  *jugador;
  struct enemi *enemigo;
}*tpnodo;

typedef struct regbala
{
 int x;
 int y;
 int direccion;
 tpnodo nodo_malla;
 struct regbala *ant;
 struct regbala *sig;
}*BALA;

typedef struct reg_list_doble_bala
{
 BALA inicio;
 BALA final;
}*MUNICION;



typedef struct jug
{
   int direccion;
   tpnodo nod_malla;
   int nivel;
   char nombre[20];
   int muertos;
   int vida;
   int puntos;
   MUNICION balas;
}JUGADOR;



//ESTRUCTURAS PARA LOS ENEMIGOS
typedef struct enemi
{
  int x,y;
  tpnodo nodo_malla;
  struct enemi *ant,*sig;
  int band_dir;
}*ENEMIGO;

        
typedef struct malos
{
  ENEMIGO inicio;
  ENEMIGO final;
}*LISTA_ENEMIGOS;


typedef struct
{
   int dimx,dimy;
   int **img;
}imagen;//estructura de imagen guarda las dimensiones x,y de cada imagen
                     
                     
typedef struct
{ 
   imagen maton;
   imagen maloso;
   imagen muro2,maton_iz,maton_aba,maton_up;
   imagen bala_der,bala_iz,bala_aba,bala_arr;
   imagen explosion;
   imagen muerto;
}imagenes;//Estructura que contiene las imagenes usadas para el juego


typedef struct
{
   imagenes enem;
   LISTA_ENEMIGOS E;
}ENEMIGOS;
//************************************************
typedef struct
{
 int num_heridos;
 int num_enemigos;
}DATOS_NIVEL;







//FUNCIONES

void portada();
void game_over(char nombre[], int pts);
void guardaRegistro(char nomarch[],JUGADOR r);
void leeRegistros(char nomarch[], JUGADOR registros[NR]);


void info_jug(JUGADOR jugador);
void libera_mem(tpnodo *cab);
void crea_nodo_malla(tpnodo *nodo,int x, int y);
void crea_malla(tpnodo *cab,int x,int y);
void dibuja_malla(tpnodo lista);
char tecla();
void inicializar_jugador(JUGADOR *jugador);
void inicializar_enemigo(ENEMIGOS *Ens);
void crea_enemigo(ENEMIGOS *Ens, tpnodo cab);
int crea_nodo_ENEMIGO(ENEMIGO *nodo,int x, int y);
void libera_lista_malosos(LISTA_ENEMIGOS list);

int  juego(tpnodo *malla,int x,int y, JUGADOR *jugador,ENEMIGOS *enemigo,imagenes pics,int *InoPag);
//FUNCIONES INCOLUCRADAS CON EL FUNCIONAMIENTO DEL JUEGO
void movimientos_jugador(JUGADOR *jug,tpnodo *aux,tpnodo *ant,tpnodo sig,int dir,int *bandera_salida,imagenes pic,tpnodo *cabecera);
void dibuja_jugador(int op,imagenes im,tpnodo borra,tpnodo juga);
int crea_bala(MUNICION parke,tpnodo nodo,int dir);
int crea_nodo_BALA(BALA *nodo,int x,int y);

void movimiento_disparo_fase1(clock_t *ini,clock_t fin,JUGADOR *jugador,ENEMIGOS E,imagenes pics,int *cont_muertes);
//void movimiento_disparo_fase1(JUGADOR *jugador,ENEMIGOS E,imagenes pics);
void movimiento_disparo_fase2(tpnodo sig,tpnodo *ant_malla,BALA *auxiliar_disparo,JUGADOR *jugador,ENEMIGOS E,int dir,imagenes pic,int *cont_muertes);
int elimina_bala(MUNICION parke,BALA nod);
void dibuja_disparo(int direccion,imagenes im,tpnodo bala_anterior,int x,int y);
void muere(tpnodo *aux,tpnodo *ant, JUGADOR *ju, tpnodo *cabecera,int *tecla,imagenes pic);
int elimina_maloso(LISTA_ENEMIGOS enem,ENEMIGO nodo);
void MUEVE(JUGADOR *jugador,tpnodo *aux,tpnodo *aux_ant,tpnodo sig,imagenes pics,int n);
void moves(JUGADOR *jugador,tpnodo *aux,tpnodo *aux_ant,char tecla);

int movimientos_enemigos_fase2(ENEMIGO *aux_maloso, tpnodo *ant_enemigo,tpnodo sig,ENEMIGOS *maloson);
int movimientos_enemigo_fase1(clock_t *inicio,clock_t final,ENEMIGOS *maloso);
///**********************************************************************

//FUNCION QUE CARGA LOS ESCENARIOS
//void ini_escenario(tpnodo main_dor,imagenes pic,int nivel,ENEMIGOS maloso);
void ini_escenario(tpnodo main_dor,imagenes pic,int nivel,ENEMIGOS maloso,int *cont_enems,int *cont_heridos);

//FUNCIIONES INVOLUCRADAS EN LA CREACION DE LAS IMAGENES
void cargaImagenes(imagenes *img);
void carga_Imagen_ENEMIGO(ENEMIGOS *maloso);
void creaicono(char cad[],imagen *imag);
int dibujaicono(imagen imag,int x,int y,int tam,int dir);

void menu();
//void leeRegistros(char nomarch[], TJugador registros[NR]);
void ayuda(char nom_arch[]);
void pedir_nombre(char nombre[]);
void descartar_input();
void llenabotones(TBoton z[]);
void llenaboton(int num, int x, int y, char texto[], TBoton *z);
void dibujabotones(TBoton z[], int w);
void dibujaboton(TBoton z);
int checaboton(TBoton z[], int xm, int ym);
int pintamenu(TBoton Bot[]);   
void portada1();
void intextxy( int x, int y, char texto[], int colf, int collet);
int menu2();




//***********************************************************************************

int main()
{
  initwindow( getmaxwidth(),getmaxheight(), "ZAMARRIPA RESCUE");
  tpnodo inicio=NULL;//Inicializacion del la malla
  srand(time(NULL));
  int cor_x=100;
  int cor_y=0;
  int res,i;
  int InoPag=1;
  int band=1;
  int op;
  char nom[20];
  char record[100];
  imagenes imags;
  JUGADOR jug;
  JUGADOR registros[NR];
  ENEMIGOS E;
  portada();
  inicializar_jugador(&jug);//LISTO 
  cleardevice();
  do{
 
     op=menu2();
     ///FUNCION PRINCIPAL
    switch(op)//parametro switch//OP
      {
        case 0:
                cleardevice();
                descartar_input();
                pedir_nombre(nom);
               // pedir_nombre(jug.nombre);
               // strcpy(jug.nombre,nom);
                do
                  {
                     crea_malla(&inicio,cor_x,cor_y);
                     inicializar_enemigo(&E);//CHECAR FUNCION
                     carga_Imagen_ENEMIGO(&E);//LISTO 
                     cargaImagenes(&imags);
                     setactivepage(InoPag);
                     cleardevice();
                     jug.muertos=0;
                    res=juego(&inicio,cor_x,cor_y,&jug,&E,imags,&InoPag);
                    if(res)
                      {
                        if(jug.nivel==4)
                          {
                            band=1;
                          }
                        else
                            band=0;
                      }
                    else
                       band=1;
                      cleardevice();
                      if(InoPag)
                         InoPag=0;
                      else
                         InoPag=1;
                  }while(band!=1 && jug.vida>0);
                game_over(nom, jug.puntos);  
                jug.puntos=0;
                jug.nivel=1;
                jug.vida=2;
                
        break;
        case 1:
                descartar_input();
                ayuda("ayuda.txt");
                getch();
                cleardevice();
        break;
        case 2:
              descartar_input();
              cleardevice();
              leeRegistros("records", registros);
              moveto(200, 60);
              do{
                   for(i=0; i<5; i++)
                      {
                        sprintf(record, "%d) %s %d pts\n", i+1, registros[i].nombre, registros[i].puntos);
                        settextstyle(1,0,4);
                        setcolor(WHITE);
                        outtextxy(((getmaxx()/2*0)+100),(50)*i+10,record);
                        settextstyle(1,0,4);
                        setcolor(RED);
                        outtextxy(((getmaxx()/2*0)+100),(50)*i+10,record);
                        delay(100);
                      }
               }while(!kbhit());
            getch();
            cleardevice();
        break;
      }
 }while(op!=3);
 closegraph( );
 return( 0 );
}


//FUNCION PERRONA**********************************************************************


int juego(tpnodo *malla,int x,int y, JUGADOR *jugador,ENEMIGOS *enemigo,imagenes pics,int *InoPag)
{
 int salida=0;
 int bala;   
 int cont_en=0;
 int con_herido=0;
 int cont_enems_archivo=0;
 int cont_herido_archivo=0;
 int tecla;
 clock_t inicio,iniDis;
 clock_t fin;
 tpnodo aux=*malla;
 tpnodo aux_ant=*malla;
 ENEMIGOS *Aux_En=enemigo;//AUXILIAR ENEMIGOS
 jugador->nod_malla=*malla;//SE APUNTA EL JUGADOR A LA MALLA
 jugador->nod_malla->jugador=jugador;
 //ini_escenario(*malla,pics,3,*enemigo,&cont_enems_archivo,&cont_herido_archivo);//SE DIBUJA EL ESCENARIO.
 ini_escenario(*malla,pics,jugador->nivel,*enemigo,&cont_enems_archivo,&cont_herido_archivo);//SE DIBUJA EL ESCENARIO.
 printf("contador enemigos= %d\n",cont_enems_archivo);
 printf("contador heridos= %d\n",cont_herido_archivo);
 getch();
 dibujaicono(pics.maton,jugador->nod_malla->cor_x,jugador->nod_malla->cor_y,1,0);
 setvisualpage(*InoPag);
 inicio=clock();//inicializacion de las varibles temporizadoras
 iniDis=clock();
 
 do//CICLO PRINCIPAL
   {
     
     fflush(stdin);
     if(kbhit())
       {
         tecla=getch();
         if(tecla==0)
           {
             tecla=getch();
           }
         switch(tecla)
         {
          case 72://ARRIBA
             //checa_y_mueve(Ju,&aux,&aux_ant,aux->arr,2,Es,list,&ban_sal,&tecla);
                   movimientos_jugador(jugador,&aux,&aux_ant,aux->arr,1,&tecla,pics,malla);
          break;
          case 80: //ABAJO
                  movimientos_jugador(jugador,&aux,&aux_ant,aux->aba,2,&tecla,pics,malla);
          break; 
          case 75://IZQUIERDA
                  movimientos_jugador(jugador,&aux,&aux_ant,aux->ant,3,&tecla,pics,malla);
          break;
          case 77://DERECHA
                   movimientos_jugador(jugador,&aux,&aux_ant,aux->sig,4,&tecla,pics,malla); 
          break;
          case 32://DISPARO JUGADOR
                  
                  if(jugador->direccion==4 && aux->sig!=NULL &&  aux->sig->info!='m' )//DERECHA
                    {
                       bala=crea_bala(jugador->balas,aux->sig,6);
                       if(bala)
                          {
                            dibujaicono(pics.bala_der,jugador->balas->final->nodo_malla->cor_x,jugador->balas->final->nodo_malla->cor_y,1,0);//PENDIENTE

                          }
                    }
                  if(jugador->direccion==3 && aux->ant!=NULL &&  aux->ant->info!='m'  )//IZQUIERDA
                    {
                       bala=crea_bala(jugador->balas,aux->ant,7);
                       if(bala)
                         {
                           dibujaicono(pics.bala_iz,jugador->balas->final->nodo_malla->cor_x,jugador->balas->final->nodo_malla->cor_y,1,0);//PENDIENTE

                         }
                          
                    }
                  
                  if(jugador->direccion==1 && aux->arr!=NULL &&  aux->arr->info!='m')//ARRIBA
                    {
                       bala=crea_bala(jugador->balas,aux->arr,8);
                       if(bala)
                         {
                            dibujaicono(pics.bala_aba,jugador->balas->final->nodo_malla->cor_x,jugador->balas->final->nodo_malla->cor_y,1,0);//PENDIENTE

                         }
                    }
                  if(jugador->direccion==2 && aux->aba!=NULL &&  aux->aba->info!='m')//ABAJO
                    {
                       bala=crea_bala(jugador->balas,aux->aba,9);
                       if(bala)
                         {
                           dibujaicono(pics.bala_arr,jugador->balas->final->nodo_malla->cor_x,jugador->balas->final->nodo_malla->cor_y,1,0);//PENDIENTE
                    
                         }
                    }
          break;
          printf("contador de muertes=%d\n",cont_en);
         }
        info_jug(*jugador);
       }
       
    fin=clock();   
    if(Aux_En->E->inicio != NULL)
        {
         if(movimientos_enemigo_fase1(&inicio,fin,Aux_En))
           {
             //PENDIENTE FUNCION MUERE  
             muere(&aux,&aux_ant,jugador,malla,&tecla,pics);
           }
        }
  
    movimiento_disparo_fase1(&iniDis,fin,jugador,*enemigo,pics,&cont_en); 
    if(cont_en==cont_enems_archivo && cont_herido_archivo==jugador->muertos)
      {
        salida=1;
      }
       
    delay(12);
   }while(tecla!= 27 && salida==0);
 
 libera_lista_malosos(enemigo->E);
 libera_mem(malla);
 if(jugador->vida>0 && salida==1)
   {
     jugador->nivel++;
     return 1;
    }
 return 0;
}


//**************CICLO PRINCIPAL DEL JUEGO






//movimientos_jugador_fase1(jugador,&aux,&aux_ant,aux->sig,&salida);
void movimientos_jugador(JUGADOR *jug,tpnodo *aux,tpnodo *ant,tpnodo sig,int dir,int *bandera_salida,imagenes pic,tpnodo *cabecera)
{
//if(sig!=NULL && sig->obstaculo!=1 && sig->enemigo==NULL && sig->puerta==NULL)
if(sig!=NULL && sig->info != 'm' && sig->enemigo==NULL)//añadir Condicion de Salida 
 {
  *ant=*aux;
  (*aux)->jugador=NULL;
  *aux=sig;
  (*aux)->jugador=jug;
  jug->nod_malla=*aux;
  jug->direccion=dir;
  dibuja_jugador(dir,pic,*ant,jug->nod_malla);
  if((*aux)->info =='j')
    {
      jug->muertos+=1;
      printf("MUERTOS=%d",jug->muertos);
      (*aux)->info='0' ;
    }
  switch(dir)
    {
      case 1: //ARRIBA
              if(sig !=NULL && sig->arr !=NULL && sig->arr->enemigo !=NULL)
                  muere(aux,ant,jug,cabecera,bandera_salida,pic);              
      //void muere(tpnodo *aux,tpnodo *ant, JUGADOR *jugador, tpnodo *cabecera, int *tecla,imagenes pic)
      break;
      case 2://ABAJO 
              if(sig !=NULL && sig->aba !=NULL && sig->aba->enemigo !=NULL)
                   muere(aux,ant,jug,cabecera,bandera_salida,pic);
      break;
      case 3://IZQUIERDA
              if(sig !=NULL && sig->ant !=NULL && sig->ant->enemigo !=NULL)
                    muere(aux,ant,jug,cabecera,bandera_salida,pic);
      break;
      case 4://DERECHA
              if(sig !=NULL && sig->sig !=NULL && sig->sig->enemigo !=NULL)
                   muere(aux,ant,jug,cabecera,bandera_salida,pic);
      break;
      
    }
 }
 //FALTA SEGUNDA FASE DEL MOVIMIENTO CUANDO EL JUGADOR TOCA A UN ENEMIGO
}





void dibuja_jugador(int op,imagenes im,tpnodo borra,tpnodo juga)
{
 switch(op)
   {
      case 1://ARRIBA 
             dibujaicono(im.maton_up,juga->cor_x,juga->cor_y,1,0);
             setfillstyle(1,BLACK);
             bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
             floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
      break;
      case 2://ABAJO
             dibujaicono(im.maton_aba,juga->cor_x,juga->cor_y,1,0);
             setfillstyle(1,BLACK);
             bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
             floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
      break;
      case 3://IZQUIERDA
             dibujaicono(im.maton_iz,juga->cor_x,juga->cor_y,1,0);
             setfillstyle(1,BLACK);
             bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
             floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
      break;
      case 4://DERECHA
             dibujaicono(im.maton,juga->cor_x,juga->cor_y,1,0);
             setfillstyle(1,BLACK);
             bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
             floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
      break;
   }
}

///*******************DISPAROS***************************************************
//********************************************************************************
//********************************************************************************
/*
typedef struct nodo
{
  char info;
  int existe;
  int cor_x;
  int cor_y;
  struct nodo *sig,*ant;
  struct nodo *aba,*arr;
  struct jug  *jugador;
  struct enemi *enemigo;
}*tpnodo;

typedef struct regbala
{
 int x;
 int y;
 int direccion;
 tpnodo nodo_malla;
 struct regbala *ant;
 struct regbala *sig;
}*BALA;

typedef struct reg_list_doble_bala
{
 BALA inicio;
 BALA final;
}*MUNICION;



typedef struct jug
{
   int direccion;
   tpnodo nod_malla;
   int nivel;
   char nom[20];
   int muertos;
   int parque;
   int puntos;
   MUNICION balas;
}JUGADOR;



//ESTRUCTURAS PARA LOS ENEMIGOS
typedef struct enemi
{
  int x,y;
  tpnodo nodo_malla;
  struct enemi *ant,*sig;
  int band_dir;
}*ENEMIGO;

        
typedef struct malos
{
  ENEMIGO inicio;
  ENEMIGO final;
}*LISTA_ENEMIGOS;


typedef struct
{
   int dimx,dimy;
   int **img;
}imagen;//estructura de imagen guarda las dimensiones x,y de cada imagen
                     
                     
typedef struct
{ 
   imagen maton,maton2,maton3;
   imagen maloso;
   imagen muro2,maton_iz,maton_aba,maton_up;
   imagen bala_der,bala_iz,bala_aba,bala_arr;
}imagenes;//Estructura que contiene las imagenes usadas para el juego


typedef struct
{
   imagenes enem;
   LISTA_ENEMIGOS E;
}ENEMIGOS;

*/


int crea_bala(MUNICION parke,tpnodo nodo,int dir)
{
 int res;
 BALA nuevo;
 res=crea_nodo_BALA(&nuevo,nodo->cor_x,nodo->cor_y);
 if(res)
   {
    if(parke->inicio==NULL)
      {
        parke->inicio=nuevo;
        parke->final=nuevo;
        parke->inicio->nodo_malla=nodo;
        parke->inicio->direccion=dir;
      }
    else
      {
       parke->final->sig=nuevo;
       nuevo->ant=parke->final;
       parke->final=nuevo;
       parke->final->nodo_malla=nodo;
       parke->final->direccion=dir;
      
      }
   }
 return(res);
}



int crea_nodo_BALA(BALA *nodo,int x,int y)
{
 int band=0;
 *nodo=(BALA)malloc(sizeof(struct regbala));
 if(*nodo)
  {
   (*nodo)->x=x;
   (*nodo)->y=y;
   (*nodo)->ant=NULL;
   (*nodo)->sig=NULL;
   (*nodo)->direccion=6;
   (*nodo)->nodo_malla=(tpnodo)malloc(sizeof(struct nodo));
   if((*nodo)->nodo_malla) 
         band=1;
  }
 return(band);
}

void movimiento_disparo_fase1(clock_t *ini,clock_t fin,JUGADOR *jugador,ENEMIGOS E,imagenes pics,int *cont_muertes)
{
 int seg;
 seg=(fin-(*ini))%CLK_TCK;
 BALA auxiliar_balas=jugador->balas->inicio;
 tpnodo ant;
 if(seg>100)
 {
   while(auxiliar_balas !=NULL)
     { 
       if(auxiliar_balas->direccion ==6)//DERECHA
         {
           movimiento_disparo_fase2(auxiliar_balas->nodo_malla->sig,&ant,&auxiliar_balas,jugador,E,6,pics,cont_muertes);
         }
       if(auxiliar_balas->direccion ==7)//IZQUIERDA
         {
           movimiento_disparo_fase2(auxiliar_balas->nodo_malla->ant,&ant,&auxiliar_balas,jugador,E,7,pics,cont_muertes);
         }
       if(auxiliar_balas->direccion ==8)//ARRIBA
         {
          movimiento_disparo_fase2(auxiliar_balas->nodo_malla->arr,&ant,&auxiliar_balas,jugador,E,8,pics,cont_muertes);
         }
       if(auxiliar_balas->direccion ==9)//ABAJO
         {
           movimiento_disparo_fase2(auxiliar_balas->nodo_malla->aba,&ant,&auxiliar_balas,jugador,E,9,pics,cont_muertes);
         }
      auxiliar_balas=auxiliar_balas->sig;//Siguiente bala
   }
    *ini=clock();
 }
}



void movimiento_disparo_fase2(tpnodo sig,tpnodo *ant_malla,BALA *auxiliar_disparo,JUGADOR *jugador,ENEMIGOS En,int dir,imagenes pic,int *cont_muertes)
{

 if(sig!=NULL && sig->info != 'm' &&sig->info !='j' && (*auxiliar_disparo)->nodo_malla->enemigo ==NULL) 
   {
     *ant_malla=(*auxiliar_disparo)->nodo_malla;
     (*auxiliar_disparo)->nodo_malla=sig;//apunta la bala al siguiente nodo de la malla
     dibuja_disparo(dir,pic,*ant_malla,sig->cor_x,sig->cor_y); 
   }
 else 
   {

     if((*auxiliar_disparo)->nodo_malla->enemigo!=NULL)//SI ES ENEMIGO
       {
         setfillstyle(1,BLACK);
         bar((*auxiliar_disparo)->nodo_malla->cor_x,(*auxiliar_disparo)->nodo_malla->cor_y,(*auxiliar_disparo)->nodo_malla->cor_x+TAM,(*auxiliar_disparo)->nodo_malla->cor_y+TAM);
         floodfill((*auxiliar_disparo)->nodo_malla->cor_x+2,(*auxiliar_disparo)->nodo_malla->cor_y+2,BLACK);
         dibujaicono(pic.explosion,(*auxiliar_disparo)->nodo_malla->cor_x,(*auxiliar_disparo)->nodo_malla->cor_y,1,0);
         delay(300);
         elimina_maloso(En.E,(*auxiliar_disparo)->nodo_malla->enemigo);
         (*auxiliar_disparo)->nodo_malla->enemigo=NULL;
         jugador->puntos+=5;
         *cont_muertes=*cont_muertes+1;
         printf("CONTADOR MUERTES %d\n",*cont_muertes);
         //LIBERA ENEMIGO
       }
     setfillstyle(1,BLACK);
     bar((*auxiliar_disparo)->nodo_malla->cor_x,(*auxiliar_disparo)->nodo_malla->cor_y,(*auxiliar_disparo)->nodo_malla->cor_x+TAM,(*auxiliar_disparo)->nodo_malla->cor_y+TAM);
     floodfill((*auxiliar_disparo)->nodo_malla->cor_x+2,(*auxiliar_disparo)->nodo_malla->cor_y+2,BLACK);
     elimina_bala(jugador->balas,*auxiliar_disparo);//LIBERAR MEMORIA DISPARO
   }
}


int elimina_bala(MUNICION parke,BALA nod)
{
    BALA aux=parke->inicio;
    BALA aux_ant=parke->inicio;
    if(aux !=NULL)
       {
         if(aux==nod) 
           {
             if(aux->sig!=NULL) //SI ES EL PRIMERO
               {
                  parke->inicio=aux->sig;
                  parke->inicio->ant=NULL;
                  free(aux);
               }
             else
               {
                 parke->inicio=NULL;
                 parke->final-NULL;
                 free(aux);
               }
             return 1;  
           }
         aux=parke->final;
         if(aux==nod)//SI ES EL ULTIMO
           {
             if(aux->ant!=NULL)
               {
                  parke->final=aux->ant;
                  parke->final->sig=NULL;
                  free(aux);
               }
             else
               {
                  parke->inicio=NULL;
                  parke->final=NULL;
                  free(aux);
               }
             return 1;
           }
         else
           {
              aux=parke->inicio->sig;
              aux_ant=parke->inicio;
              while(aux !=parke->final)
                {
                   if(aux==nod)
                     {
                       aux_ant->sig=aux->sig;
                       aux_ant->sig->ant=aux_ant;
                       free(aux);
                       return 1;
                     }
                   aux=aux->sig;
                   aux_ant=aux_ant->sig;
                }
           }
     }
}


     
int elimina_maloso(LISTA_ENEMIGOS enem,ENEMIGO nodo)
{
 ENEMIGO aux=enem->inicio;
 ENEMIGO aux_ant=enem->inicio;
 if(aux !=NULL)
   {
     if(aux==nodo)
       {
         if(aux->sig!=NULL)
           {
             enem->inicio=aux->sig;
             enem->inicio->ant=NULL;
             free(aux);
           }
         else
           {
             enem->inicio=NULL;
             enem->final=NULL;
             free(aux);
           }
         return 1;
       }
     aux=enem->final;
     if(aux==nodo)
       {
         if(aux->ant !=NULL)
           {
             enem->final=aux->ant;
             enem->final->sig=NULL;
             free(aux);
           }
         else
           {
             enem->inicio=NULL;
             enem->final=NULL;
             free(aux);
           }
         return 1;
       }
     else
       {
         aux=enem->inicio->sig;
         aux_ant=enem->inicio;
         while(aux !=enem->final)
           {
             if(aux==nodo)
               {
                 aux_ant->sig=aux->sig;
                 aux_ant->sig->ant=aux_ant;
                 free(aux);
                 return 1; 
               }
             aux=aux->sig;
             aux_ant=aux_ant->sig;
           }
       }
   }
}


void dibuja_disparo(int direccion,imagenes im,tpnodo borra,int x,int y)
{

 switch(direccion)
 {
  case 6://DERECHA
       //printf("ENTRO FUNCION DIBUJA DISPARO con un%d \n\n",6);
         dibujaicono(im.bala_der,x,y,1,0);
         setfillstyle(1,BLACK);
         bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
         floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
        //dibujaicono(im.maton_up,juga->cor_x,juga->cor_y,1,0);
  break;
  case 7://IZQUIERDA
         dibujaicono(im.bala_iz,x,y,1,0);
         setfillstyle(1,BLACK);
         bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
         floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
  break;
  case 8://ARRIBA
         dibujaicono(im.bala_aba,x,y,1,0);
         setfillstyle(1,BLACK);
         bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
         floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
  break;
  case 9://ABAJO
       dibujaicono(im.bala_arr,x,y,1,0);setfillstyle(1,BLACK);
       bar(borra->cor_x,borra->cor_y,borra->cor_x+TAM,borra->cor_y+TAM);
       floodfill(borra->cor_x+2,borra->cor_y+2,BLACK);
  break;
 }
}

//********************************************************************************
//********************************************************************************
//********************************************************************************







//************MOVIMIENTOS DEL JUGADOR*************************************************
void moves(JUGADOR *jugador,tpnodo *aux,tpnodo *aux_ant,char tecla)
{

  switch(tecla)
   {
    case 77://DERECHA
            if((*aux)->sig->sig!=NULL  && (*aux)->sig->info !='m')
              { 
                *aux_ant=*aux;
                (*aux)->jugador=NULL;
                *aux=(*aux)->sig;
                (*aux)->jugador=jugador;
                jugador->nod_malla=*aux;
                setfillstyle(1,BLACK);                   
                bar(jugador->nod_malla->ant->cor_x,jugador->nod_malla->ant->cor_y,jugador->nod_malla->cor_x+TAM,jugador->nod_malla->ant->cor_y+TAM);      
                floodfill(jugador->nod_malla->ant->cor_x+2,jugador->nod_malla->ant->cor_y+2,BLACK);
             }
             
    break;
    case 75://IZQUIERDA
            if((*aux)->ant!=NULL && (*aux)->ant->info != 'm')
             {    
                 *aux_ant=*aux; 
                 (*aux)->jugador=NULL;
                 *aux=(*aux)->ant;
                 (*aux)->jugador=jugador;
                 jugador->nod_malla=*aux;
                 setfillstyle(1,BLACK);                   
                 bar(jugador->nod_malla->sig->cor_x,jugador->nod_malla->sig->cor_y,jugador->nod_malla->sig->cor_x+TAM,jugador->nod_malla->sig->cor_y+TAM);      
                 floodfill(jugador->nod_malla->sig->cor_x+2,jugador->nod_malla->sig->cor_y+2,BLACK);
             }
             
                 
    break;
    case 80://ABAJO
            if((*aux)->aba!=NULL && (*aux)->aba->info != 'm')
             {    
                 *aux_ant=*aux;
                 (*aux)->jugador=NULL;
                 *aux=(*aux)->aba;
                 (*aux)->jugador=jugador;
                 jugador->nod_malla=*aux;
                 setfillstyle(1,BLACK);                   
                 bar(jugador->nod_malla->arr->cor_x,jugador->nod_malla->arr->cor_y,jugador->nod_malla->arr->cor_x+TAM,jugador->nod_malla->arr->cor_y+TAM);      
                 floodfill(jugador->nod_malla->arr->cor_x+2,jugador->nod_malla->arr->cor_y+2,BLACK);
                 
             }
    break;
    case 72://ARRIBA
             if((*aux)->arr && (*aux)->arr->info != 'm')
              {  
                *aux_ant=*aux;
                (*aux)->jugador=NULL;
                *aux=(*aux)->arr;
                (*aux)->jugador=jugador;
                jugador->nod_malla=*aux;
                setfillstyle(1,BLACK);                   
                bar(jugador->nod_malla->aba->cor_x,jugador->nod_malla->aba->cor_y,jugador->nod_malla->aba->cor_x+TAM,jugador->nod_malla->aba->cor_y+TAM);      
                floodfill(jugador->nod_malla->aba->cor_x+2,jugador->nod_malla->aba->cor_y+2,BLACK);
              }
    break;
   }
}
//************MOVIMIENTOS DEL JUGADOR*************************************************



//************NO TOCAR*************************************************
//************NO TOCAR*************************************************
//************NO TOCAR*************************************************
//************NO TOCAR*************************************************
//************NO TOCAR*************************************************
//************MOVIMIENTOS DEL ENEMIGO*************************************************

int movimientos_enemigo_fase1(clock_t *inicio,clock_t final,ENEMIGOS *maloso)
{
 int seg;
 ENEMIGO auxiliar_enemigos;
 tpnodo anterior;
 auxiliar_enemigos=maloso->E->inicio;
 seg=(final-(*inicio))%CLK_TCK;
 
 if(seg > 500)
  {
   
    //INICIA RECORRIDO DE LA LISTA ENEMIGOS
    while(auxiliar_enemigos)
        {
        
          switch(auxiliar_enemigos->band_dir)
              {
                case 0: 
                     //void movimientos_enemigos_fase2(ENEMIGO *aux_maloso, tpnodo *ant_enemigo,tpnodo sig,int dire,ENMIGOS *maloson)
                        if(movimientos_enemigos_fase2(&auxiliar_enemigos,&anterior,auxiliar_enemigos->nodo_malla->arr,maloso))
                           return 1;
                break;
                case 1:
                        if(movimientos_enemigos_fase2(&auxiliar_enemigos,&anterior,auxiliar_enemigos->nodo_malla->aba,maloso))
                          return 1;
                        
                break;
                case 2:
                       if(movimientos_enemigos_fase2(&auxiliar_enemigos,&anterior,auxiliar_enemigos->nodo_malla->sig,maloso))
                          return 1;
                break;
                case 3:
                        if(movimientos_enemigos_fase2(&auxiliar_enemigos,&anterior,auxiliar_enemigos->nodo_malla->ant,maloso))
                          return 1;
                break;
              }//Llave switch
          
           //AVANZE EN LA LISTA DE ENEMIGOS AL SIGUIENTE ENEMIGO
          auxiliar_enemigos=auxiliar_enemigos->sig;
      
        }//Llave While
      *inicio=clock();  
  }
 return 0;
}//Llave Funcion


int movimientos_enemigos_fase2(ENEMIGO *aux_maloso, tpnodo *ant_enemigo,tpnodo sig,ENEMIGOS *maloson)
{
 //CONDICIONES PARA QUE SE HAGA EL AVANZE DEL ENEMIGO
  //if(sig!=NULL &&  sig->info != 'm' && sig->info!='j' && sig->enemigo==NULL)//SUPER IFCondicion para que muera solo si toca al jugador
   if(sig!=NULL &&  sig->info != 'm' && sig->info!='j' && sig->jugador==NULL &&  sig->enemigo==NULL)//SUPER IF
   {
     *ant_enemigo=(*aux_maloso)->nodo_malla;//se guarda la posicion anterior del nodo enemigo que apunta a la malla;
     (*aux_maloso)->nodo_malla->enemigo=NULL;//SE PONE EN ANTERIOR EL ENEMIGO ANTERIOR
     (*aux_maloso)->nodo_malla=sig;
     (*aux_maloso)->nodo_malla->enemigo=*aux_maloso;
     (*aux_maloso)->x=(*aux_maloso)->nodo_malla->cor_x;
     (*aux_maloso)->y=(*aux_maloso)->nodo_malla->cor_y;
     dibujaicono(maloson->enem.maloso,(*aux_maloso)->x,(*aux_maloso)->y,1,0);
     setfillstyle(1,BLACK);
     bar((*ant_enemigo)->cor_x,(*ant_enemigo)->cor_y,(*ant_enemigo)->cor_x+TAM,(*ant_enemigo)->cor_y+TAM);
     floodfill((*ant_enemigo)->cor_x+2,(*ant_enemigo)->cor_y+2,BLACK);
    //VALIDAR SI EL JUGADOR ESTA AL LADO
     if(sig->sig!=NULL && sig->sig->jugador!=NULL)
       return 1;
     if(sig->ant!=NULL && sig->ant->jugador!=NULL)
       return 1;
     if(sig->arr!=NULL && sig->arr->jugador!=NULL)
       return 1;
     if(sig->aba!=NULL && sig->aba->jugador!=NULL)
       return 1;
   }
 else
   {
     (*aux_maloso)->band_dir=rand()%4;
   }
 return 0;
}


void inicializar_enemigo(ENEMIGOS *Ens)
{
  Ens->E=(LISTA_ENEMIGOS)malloc(sizeof(struct malos));
  Ens->E->inicio=NULL;
  Ens->E->final=NULL;
}
//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************



//FUNCION QUE CREA UNA LISTA DE ENEMIGOS 
/*PARAMETROS NECESARIOS

LA VARIABLE QUE CONTIENE LA CABECERA A LA LISTA DE ENEMIGOS
EL APUNTADOR QUE CONTENE LA DIRECCION DEL NODO DE LA MALLA DONDE ESTE SE INCERTARA

*/

void crea_enemigo(ENEMIGOS *Ens, tpnodo cab)
{
 int res;
 ENEMIGO  nuevo;
 res=crea_nodo_ENEMIGO(&nuevo,cab->cor_x,cab->cor_y);
 if(res)
  {
    if(!Ens->E->inicio)
     {
        Ens->E->inicio=Ens->E->final=nuevo;
        Ens->E->inicio->nodo_malla=cab;
        cab->enemigo=Ens->E->inicio;
     }
    else
     { 
        Ens->E->final->sig=nuevo;
        nuevo->ant=Ens->E->final;
        Ens->E->final=nuevo;
        Ens->E->final->nodo_malla=cab;
        cab->enemigo=Ens->E->final; 
     }
  }
}
  
  

  

int crea_nodo_ENEMIGO(ENEMIGO *nodo,int x, int y)
{
 int res=0;
 *nodo=(ENEMIGO)malloc(sizeof(struct enemi));
 if(*nodo)
  {
    (*nodo)->x=x;
    (*nodo)->y=y;
    (*nodo)->ant=NULL;
    (*nodo)->sig=NULL;
    (*nodo)->band_dir=rand()%4;
    (*nodo)->nodo_malla=(tpnodo)malloc(sizeof(struct nodo));
    res=1;
  }
 return(res);
}
//**************************************************************************************************************************





void inicializar_jugador(JUGADOR *jugador)
{
 jugador->nod_malla=NULL;
 jugador->nivel=1;
 jugador->direccion=4;
 strcpy(jugador->nombre,"NOIVAZMAR");
 jugador->muertos=0;
 jugador->vida=2;
 jugador->puntos=0;
 jugador->balas=(MUNICION)malloc(sizeof(struct regbala));
 jugador->balas->inicio=NULL;
 jugador->balas->final=NULL;
}


/*

typedef struct jug
{
   int direccion;
   tpnodo nod_malla;
   int nivel;
   char nom[20];
   int muertos;
   int parque;
   MUNICION balas;
}JUGADOR;

typedef struct nodo
{
  char info;
  int existe;
  int cor_x;
  int cor_y;
  struct nodo *sig,*ant;
  struct nodo *aba,*arr;
  struct jug  *jugador;
  struct enemi *enemigo;
}*tpnodo;

typedef struct regbala
{
 int x;
 int y;
 tpnodo nodo_malla;
 struct regbala *ant;
 struct regbala *sig;
}*BALA;

typedef struct reg_list_doble_bala
{
 BALA inicio;
 BALA final;
}*MUNICION;






*/




void crea_malla(tpnodo *cab,int x,int y)
{
 int i,j,reinicio_x=x;
 tpnodo  nuevo=NULL;
 tpnodo  auxren=NULL,auxcol=NULL;
 //AUXCOL auxiliar usado para situarse en las columnas de la lista 
 //AUXREN auxliar usado para situarse en los renglones de la lista
 for(i=0;i<NREN;i++)//FOR QUE CREA LAS FILAS DE LA MALLA(RENGLONES)
 {
   crea_nodo_malla(&nuevo,x,y);
   if(!*cab)//CHECA SI LA LISTA ESTA VACIA
          *cab=auxren=auxcol=nuevo;//SI LA LISTA ESTA VACIA TODO APUNTA AL PRIMER NODO ES DECIR EL NODO RECIEN CREADO  
    else
        {
          auxcol=auxren;//VUELVE A SITUAR A AUXCOL AL INICIO DEL RENGLON LO REGRESA DEL ULTIMO NODO AL PRIMERO
          auxcol->aba=nuevo;
          auxren=auxren->aba;//**
          auxren->arr=auxcol;//INDICA QUE YA HAY UNA FILA
          auxcol=auxren;//***ESTA INSTRUCCION DA INICIO A LA FILA SIGUIENTE DE LA MALLA(<<SIGUIENTE RENGLON>>)
        }
    for(j=0;j<NCOL;j++)//FOR QUE CREA LAS COLUMNAS DE LA MALLA
         {
           x+=TAM;
           crea_nodo_malla(&nuevo,x,y);
           auxcol->sig=nuevo;//
           nuevo->ant=auxcol;//NUEVO APUNTA->ANTERIOR APUNTA A LO QUE AUXCOL ESTA APUNTADO 
           if(auxren->arr)
             {
               auxcol->sig->arr=auxcol->arr->sig;
               auxcol->arr->sig->aba=auxcol->sig;
             }
            auxcol=auxcol->sig;
         }
     y+=TAM;//AUMENTA COORDENADAS EN Y para generar nuevas coordenadas
     x=reinicio_x;//reinicia las coordenadas en X para iniciar con la nueva FILA
 }
}





void crea_nodo_malla(tpnodo *nodo,int x, int y)
{
  *nodo=(tpnodo)malloc(sizeof(struct nodo));
  if(*nodo)
  {
    (*nodo)->info='0';//Se inicializa los valores para la posterior utilizacion del Nodo         
    (*nodo)->existe=0;//Se inicializa los valores para la posterior utilizacion del Nodo         
    (*nodo)->cor_x=x;
    (*nodo)->cor_y=y;
    (*nodo)->arr=NULL;
    (*nodo)->aba=NULL;
    (*nodo)->ant=NULL;
    (*nodo)->sig=NULL;
    (*nodo)->jugador=NULL;
    (*nodo)->enemigo=NULL;
  }
}






void libera_mem(tpnodo *cab)
{
  tpnodo auxren,auxcol;
  auxren=auxcol=*cab;
  while(auxren)      
  {
    auxren=auxren->aba;
    while(auxcol)
         {
            free(auxcol);
            auxcol=auxcol->sig;
           
         }
    auxcol=auxren;
  }
  *cab=NULL;
}





void dibuja_malla(tpnodo lista)
{
  int i,j;
  tpnodo aux_ren=lista;
  for(i=0;i<NREN;i++)
    {
      for(j=0;j<NCOL+1;j++)
        {
          switch(lista->existe)
               {
                 //void bar( int left, int top, int right, int bottom );
                 case 0: //setcolor(WHITE);
                     //    rectangle(lista->cor_x,lista->cor_y,lista->cor_x+TAM,lista->cor_y+TAM);
                 break;
               }
          lista=lista->sig;
          
        }
     
     aux_ren=aux_ren->aba;
     lista=aux_ren;
   }
} 





char tecla()
{
 char c;
 c=getch();
 if(c==0)
   {
    c=getch();
   }
 return(c);
}

/*
ypedef struct
{ 
   imagen maton,maton2,maton3;
   imagen maloso;
   imagen muro2,maton_iz,maton_aba,maton_up;
}imagenes;//Estructura que contiene las imagenes usadas para el juego


typedef struct
{
   imagenes enem;
   LISTA_ENEMIGOS E;
}ENEMIGOS;

*/

void carga_Imagen_ENEMIGO(ENEMIGOS *enems)
{
 creaicono("maloso.edb",&enems->enem.maloso);
 
}

void cargaImagenes(imagenes *img)//carga todas las imagenes a la estructura
{
 creaicono("maton.edb",&img->maton);
 creaicono("matonizquierda.edb",&img->maton_iz);
 creaicono("mariod.edb",&img->maton_aba);
 creaicono("matonup.edb",&img->maton_up);
 creaicono("pared2.edb",&img->muro2);
 creaicono("maloso.edb",&img->maloso);
 creaicono("bala2_der.edb",&img->bala_der);
 creaicono("bala_izquierda2.edb",&img->bala_iz);
 creaicono("bala_aba.edb",&img->bala_aba);
 creaicono("bala_arr.edb",&img->bala_arr);
 creaicono("explosion.edb",&img->explosion);
 creaicono("muerto.edb",&img->muerto);
}


void creaicono(char cad[],imagen *imag)//funcion que crea iconos leyendolo de un archivo y guardandolos en una variable por referencia
{
    int i,j,x;
    FILE *fp;
    
    fp = fopen(cad,"rb");//se abre el archivo segun la cadena cad[]
          
    fread(&imag->dimy,sizeof(int),1,fp);//se guardan las dimensiones de la imagen
    fread(&imag->dimx,sizeof(int),1,fp);
    
    imag->img = (int**)malloc(sizeof(int*)*imag->dimy);//se reserva memoria para la imagen
    
    for(i=0;i<imag->dimy;i++)//reserva memoria
         *((imag->img)+i) = (int*)malloc(sizeof(int)*imag->dimx);
    
    for(i=0;i<imag->dimy;i++)//se guarda cada pixel en un arreglo
         for(j=0;j<imag->dimx;j++)
         {
                            fread(&x,sizeof(int),1,fp);
                            imag->img[i][j] = x;
         }
    fclose(fp);//se cierra el archivo    
}




int dibujaicono(imagen imag,int x,int y,int tam,int dir)//funcion que dibuja un icono en las coordenadas x y,tam: tamaño del icono, dir: rotacion del icono
{
    int i,j;
    float o;
    o=0.5;
    for(i=0;i<imag.dimy;i++)//ciclo para leer cada color de pixel del arreglo de la imagen
         for(j=0;j<imag.dimx;j++)
         {
                                 if(imag.img[i][j] != 67043583)//si el color es diferente a ROSA se imprime en pantalla
                                 {
                                                   setfillstyle(1,imag.img[i][j]);
                                                   switch(dir)
                                                   {
                                                              case 0://0 grados
                                                                   
                                                                  
                                                                   bar(x+(j*tam),y+(i*tam),x+(j*tam)+tam,y+(i*tam)+tam);
                                                                   break;
                                                              case 1://90 derecha
                                                                   bar(x+imag.dimy-(i*tam),y+(j*tam),x+imag.dimy-((i*tam)+tam),y+(j*tam)+tam);
                                                                   break;
                                                              case 2://180 derecha
                                                                   bar(imag.dimx+imag.dimx/2+(x-(j*tam)),imag.dimy+(y-(i*tam)),imag.dimx+imag.dimx/2+(x-(j*tam)+tam),imag.dimy+(y-(i*tam)+tam));
                                                                   break;
                                                              case 3://270 derecha
                                                                   bar(x+(i*tam),y+imag.dimx+imag.dimx/2-(j*tam),x+((i*tam)+tam),y+imag.dimx+imag.dimx/2-((j*tam)+tam));
                                                                   break;
                                                   }
                                                   
                                 }
         }
    
    
}






void ini_escenario(tpnodo main_dor,imagenes pic,int nivel,ENEMIGOS maloso,int *cont_enems,int *cont_heridos)
{
 tpnodo aux;//*****AUXILIAR PARA EL RECORRIDO DE LA MALLA
 aux=main_dor;
 char dat;
 FILE *f;
 nivel==1 ? f=fopen("nivel1.txt","r") : nivel==2 ? f=fopen("nivel2.txt","r") : f=fopen("nivel3.txt","r");
 //f=fopen("nivel1.txt","r");
 while(!feof(f))  //feof(f)==NULL es igual a !feof(f)  feof(f)!=NULL es igual a feof(f)
    {
       dat=fgetc(f);
       if(dat=='1')
         {  
            dibujaicono(pic.muro2,aux->cor_x,aux->cor_y,1,0);
            aux->info='m';
         }
       if(dat=='4')
         {  
            dibujaicono(pic.muro2,aux->cor_x,aux->cor_y,1,0);
            aux->info='m';
         }
      if(dat=='2')
         {
            crea_enemigo(&maloso,aux);
            aux->enemigo=maloso.E->final;
            dibujaicono(maloso.enem.maloso,aux->enemigo->x,aux->enemigo->y,1,0);
            aux->info='E';
            *cont_enems=*cont_enems+1;
            printf("CONTADOR ENEMIGOS=%d\n",*cont_enems);
         }
      if(dat=='3')
       {
        dibujaicono(pic.muerto,aux->cor_x,aux->cor_y,1,0);
        aux->info='j';
        *cont_heridos=*cont_heridos+1;
        printf("\nCONTADOR HERIDOS=%d\n",*cont_heridos);
       }
      if(!feof(f))
       {
         aux=aux->sig;
         if(!aux)
         {
           main_dor=main_dor->aba;
           aux=main_dor;
         }
       }
    }
 fclose(f);
// *cont_enems=cont;
}



void portada()
{
         int display_x,display_y;
     display_x=getmaxx(); 
     display_y=getmaxy();
     setcolor(4);
     settextstyle(1,0,5); 
     outtextxy(display_x*0+100 ,display_y*0+25, "Universidad Autonoma de San Luis Potosi" );
     setcolor(1);
     outtextxy(display_x*0+100 ,display_y*0+75, "Facultad de Ingenieria" );
     outtextxy(display_x*0+100 ,display_y*0+125, "Area de Computacion e Informatica" );
     outtextxy(display_x*0+100 ,display_y*0+175, "Estrucuturas de Datos y Algoritmos B");
     outtextxy(display_x*0+200 ,display_y-100, "Elaborado por: Jesus Aurelio Hernandez Aguilar");
     outtextxy(display_x*0+200 ,display_y-200, "Profesor Miguel Angel de la Cruz Lopez");
     settextstyle(0,0,8);
     setcolor(4);
     outtextxy(display_x*0+200 ,display_y/2-50, "ZAMARRIPA RESCUE");
     getch();
}

void info_jug(JUGADOR jugador)
{
 char vidas[2];
 char puntos[5];
 char heridos[2];
 int x=50;
 int y=10;

 setcolor(LIGHTBLUE);
 settextstyle(3,0,2);///2
 outtextxy(x-45,y,"PUNTOS");
 sprintf(puntos,"%d",jugador.puntos);
 outtextxy(x-30,y+30,puntos);
 settextstyle(3,0,2);///2
 setcolor(LIGHTBLUE);
 outtextxy(x-45,y+60,"HERIDOS");
 sprintf(heridos,"%d",jugador.muertos);
 outtextxy(x-30,y+90,heridos);
  setcolor(LIGHTBLUE);
 outtextxy(x-45,y+120,"VIDAS");
 sprintf(vidas,"%d",jugador.vida);
 setcolor(RED);
 outtextxy(x-30,y+150,vidas);
  
  
 /*
 
  outtextxy(getmaxx()-280+x,10+y,ju.nombre);
     sprintf(puntos,"%d", ju.puntos);
     outtextxy(getmaxx()-80+x,40+y,puntos);          
     outtextxy(getmaxx()-280+x,40+y,"Puntos:");  
 */
 
}
void muere(tpnodo *aux,tpnodo *ant, JUGADOR *jugador, tpnodo *cabecera, int *tecla,imagenes pic)
{
 *ant=*aux;
 (*aux)->jugador=NULL;
 jugador->nod_malla=*aux;
 delay(500);
 jugador->vida--;
 setfillstyle(1,BLACK);
 bar(jugador->nod_malla->cor_x,jugador->nod_malla->cor_y,jugador->nod_malla->cor_x+TAM,jugador->nod_malla->cor_y+TAM);
 floodfill(jugador->nod_malla->cor_x+2,jugador->nod_malla->cor_y+2,BLACK);
 dibujaicono(pic.explosion,jugador->nod_malla->cor_x,jugador->nod_malla->cor_y,1,0);
 delay(450);
 setfillstyle(1,BLACK);
 bar(jugador->nod_malla->cor_x,jugador->nod_malla->cor_y,jugador->nod_malla->cor_x+TAM,jugador->nod_malla->cor_y+TAM);
 floodfill(jugador->nod_malla->cor_x+2,jugador->nod_malla->cor_y+2,BLACK);
 if(jugador->vida==0)
   {
     *tecla=27;
   }
 else
   {
     delay(1000);
     jugador->nod_malla=*cabecera;
     dibujaicono(pic.maton,jugador->nod_malla->cor_x,jugador->nod_malla->cor_y,1,0);
     *aux=*cabecera;
     (*aux)->jugador=jugador;
     *ant=*cabecera;
     jugador->direccion=4;    
   }
}

void libera_lista_malosos(LISTA_ENEMIGOS list)
{
  ENEMIGO aux_elimina;
  ENEMIGO aux;
  if(list !=NULL)
    {
      if(list->final==list->inicio)
        {
          free(list);
        } 
      else 
        {
           while(list->final==list->inicio)
             {
               aux=list->final->ant;
               aux_elimina=list->final;
               list->final=aux;
               free(aux_elimina);
             }
           free(list);
        }
    }
}




int menu2()
{
 TBoton botones[NUMBOTONES];
 int x,y,bot;
    
 portada1();   
 pintamenu(botones);
 clearmouseclick(WM_LBUTTONDOWN);
 while (!ismouseclick(WM_LBUTTONDOWN))
 delay(50);
 getmouseclick(WM_LBUTTONDOWN, x, y);
 bot=checaboton(botones,x,y);
 botones[bot].tipo=PRE;  
 dibujaboton(botones[bot]);   
 delay(100);
 botones[bot].tipo=NORMAL; 
 dibujaboton(botones[bot]);    
return(bot);
}






void menu()
{
 TBoton botones[NUMBOTONES];
 int x, y, bot;
 do
    {
        portada1();   
        pintamenu(botones);
        clearmouseclick(WM_LBUTTONDOWN);
        while (!ismouseclick(WM_LBUTTONDOWN))
        delay(50);
        getmouseclick(WM_LBUTTONDOWN, x, y);
        bot=checaboton(botones,x,y);
        botones[bot].tipo=PRE;  
        dibujaboton(botones[bot]);   
        delay(100);
        botones[bot].tipo=NORMAL; 
        dibujaboton(botones[bot]);    
        char nom[30];
       // TJugador registros[NR];
        int i;
        char record[100];
        switch(bot)
        {
            case 0:
                   descartar_input();
                   pedir_nombre(nom);
//                   juego(nom);
            break;
            
            case 1:
                 descartar_input();
                 ayuda("ayuda.txt");
                 getch();
                 
            break;
            
         /*   case 2:
                   descartar_input();
                   cleardevice();
                              leeRegistros("records", registros);
                              moveto(200, 60);
                           do{
                              for(i=0; i<5; i++)
                               {
                                  //sprintf(record, "%d) %s %d pts\n", i+1, registros[i].nombre, registros[i].puntos);
                                  
                                       sprintf(record, "%d) %s %d pts\n", i+1, registros[i].nombre, registros[i].puntos);
                                       settextstyle(1,0,4);
                                       setcolor(WHITE);
                                       outtextxy(((getmaxx()/2*0)+100),(50)*i+10,record);
                                       settextstyle(1,0,4);
                                       setcolor(RED);
                                       outtextxy(((getmaxx()/2*0)+100),(50)*i+10,record);
                                       delay(100);
                               }
                             }while(!kbhit());
                              getch();
            break;*/
        }
        cleardevice();      
    } while (bot!=3);
    exit(1);
   }
   


void portada1()
{
     int display_x,display_y;
     display_x=getmaxx(); 
     display_y=getmaxy();
     setcolor(LIGHTBLUE);
     line(display_x*0+5,display_y*0+5,display_x-5,display_y*0+5);
     line(display_x*0+5,display_y*0+5,display_x*0+5,display_y-5);
     line(display_x*0+5,display_y-5,display_x-5,display_y-5);
     line(display_x-5,display_y-5,display_x-5,display_y*0+5);
     line(display_x*0+10,display_y*0+10,display_x-10,display_y*0+10);
     line(display_x*0+10,display_y*0+10,display_x*0+10,display_y-10);
     line(display_x*0+10,display_y-10,display_x-10,display_y-10);
     line(display_x-10,display_y-10,display_x-10,display_y*0+10);
     line(display_x*0+15,display_y*0+15,display_x-15,display_y*0+15);
     line(display_x*0+15,display_y*0+15,display_x*0+15,display_y-15);
     line(display_x*0+15,display_y-15,display_x-15,display_y-15);
     line(display_x-15,display_y-15,display_x-15,display_y*0+15);
     setcolor(2);
     settextstyle(1,0,5); 
     outtextxy(display_x*0+200 ,display_y-100, "infocomp");
     outtextxy(display_x*0+200 ,display_y-200, "Facultad de Ingeniera");
     outtextxy(display_x*0+200 ,display_y-300, "UASLP");
     settextstyle(0,0,8);
     setcolor(2);
     outtextxy(display_x*0+20 ,display_y/2-50, "ZAMARRIPA AL RESCATE");
     settextstyle(0,0,4);
     outtextxy(display_x-400 ,display_y-50, "E.D.A 'B'");
}



int pintamenu(TBoton Bot[])
{
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	llenabotones(Bot);  
	dibujabotones(Bot,4);
}



int checaboton(TBoton z[], int xm, int ym)
{
	int i;
	int reg=-1,x1,y1,x2,y2;
	for(i=0;i<NUMBOTONES && reg==-1;i++)
	{
		x1=z[i].x;
		x2=z[i].x+z[i].tamx;
		y1=z[i].y;
		y2=z[i].y+z[i].tamy;
		if(xm>=x1&& xm<=x2&&ym>y1&&ym<=y2)
	    reg=i;
	 }
    return(reg);
}



void dibujaboton(TBoton z)
{
	int color;
	color=z.tipo==NORMAL?LIGHTGRAY:DARKGRAY;
	setfillstyle(SOLID_FILL, 2);
	bar(z.x,z.y,z.x+z.tamx,z.y+z.tamy);
	setcolor(color);
	line(z.x,z.y,z.x+z.tamx,z.y);
	line(z.x,z.y,z.x,z.y+z.tamy);
	color=z.tipo==NORMAL?DARKGRAY:LIGHTGRAY;
	setcolor(color);
	line(z.x,z.y+z.tamy,z.x+z.tamx,z.y+z.tamy);
	line(z.x+z.tamx,z.y,z.x+z.tamx,z.y+z.tamy);
	setcolor(15);
	//settextjustify(1,1);
	outtextxy(z.x+z.tamx/2,z.y+z.tamy/2,z.texto);
}


void dibujabotones(TBoton z[], int w)
{
	int i;
	   for(i=0;i<w;i++)
	      dibujaboton(z[i]);
}



void llenaboton(int num, int x, int y, char texto[], TBoton *z)
{
	z->num=num;
	z->x=x;
	z->y=y;
	strcpy(z->texto, texto);
	z->tipo=NORMAL;
	z->tamx=textwidth(texto)*2;
	z->tamy=textheight(texto)*2;
}


void llenabotones(TBoton z[])
{
	llenaboton(1,190,80,"JUEGO", &z[0]);
	llenaboton(2,190,120,"AYUDAS", &z[1]);
	llenaboton(3,190,160,"RECORDS", &z[2]);
	llenaboton(4,190,200,"SALIR", &z[3]);
}



void descartar_input()
{
     while( kbhit() )
            getch();
}



void pedir_nombre(char nombre[])
{
     //descartar_input();
     cleardevice();
     
     int x = getmaxx()/2;
     int y = getmaxy()/2;
     
     settextjustify(LEFT_TEXT, TOP_TEXT);
     //settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
     settextstyle(0, HORIZ_DIR, 4);
     setcolor(WHITE);
      
     outtextxy(x-textwidth("nombre:"), y, "nombre:");
     intextxy(x, y, nombre, BLACK, WHITE);   
}



void ayuda(char nom_arch[])
{
     FILE *f;
     int x=0,y=0;
     char texto[100];
     f=fopen(nom_arch,"r");
     if(f==NULL)
     {
       outtextxy(400,600,"Archivo Inexistente");
       getch();
     }
     else
     {
         cleardevice();
         setcolor(15);
         settextstyle(0,0,3);
         while(!feof(f))
         {
         fgets(texto,100,f);
         outtextxy(x,y,texto);
         y+=textheight(texto);
         //if(y>=450)
           //{
             //outtextxy(400,y,"Presiona ENTER para continuar");
              //cleardevice();
             //getch();
             //y=100;
            
             
           //}
         }
     }
}

void leeRegistros(char nomarch[], JUGADOR registros[NR])
{
     int i;
     FILE *f;
     
     for(i=0;i<NR;i++)
     {  strcpy(registros[i].nombre," ");
        registros[i].puntos=0;
     }
     
     f=fopen(nomarch,"rb+");
     if(f==NULL)
     { 
        f=fopen(nomarch,"wb");
        fwrite(registros,sizeof(JUGADOR),NR,f);
     }
     else
         fread(registros,sizeof(JUGADOR),NR,f);
     fclose(f);
}


void intextxy( int x, int y, char texto[], int colf, int collet)
{
     char car[2];
     int i=0;
     
     do{
         while(!kbhit())
         {
                        setcolor(collet);
                        outtextxy(x,y,"_");
                        delay(50);
                        setcolor(colf);
                        outtextxy(x,y,"_");
                        delay(50);
         }
         car[0]=getch();
         car[1]='\0';
                  
         if(car[0]!='\b' && car[0]!='\r')
         {
                         setcolor(collet);
                         outtextxy(x,y,car);
                         x+=textwidth(car);
                         texto[i++]=car[0];
         }
         else if(car[0]=='\b' && i>0)
         {             
              car[0]=texto[--i];
              car[1]='\0';
              x-=textwidth(car);
              setcolor(colf);
              outtextxy(x,y,car);  
         }
     } while(car[0]!='\r');
     texto[i]='\0';
}


void game_over(char nombre[], int pts)
{
     JUGADOR jugador;
     strcpy(jugador.nombre,nombre);
     jugador.puntos = pts;
     
     //setactivepage(0);
     //setvisualpage(0);
     cleardevice();
     do{
                       setcolor(RED);
                       settextstyle(4,0,50);
                       outtextxy(getmaxx()/2-250,getmaxy()/2,"GAME OVER ");
                       setcolor(WHITE);
                       outtextxy(getmaxx()/2-250,getmaxy()/2,"GAME OVER ");
                       setcolor(12);
                       delay(85);
       }while(!kbhit());
     guardaRegistro("records",jugador);
     
     //guardaRegistro("records", jugador);
     delay(800);
}


void guardaRegistro(char nomarch[],JUGADOR r)
{
     JUGADOR registros[NR];
     int i;
     FILE *f;
     printf("jugador GAME %s",r.nombre);
     printf("PUNTOS%d",r.puntos);
     
     for(i=0;i<NR;i++)
     {  strcpy(registros[i].nombre," ");
        registros[i].puntos=0;
     }
     
     f=fopen(nomarch,"rb+");
     if(f==NULL)
     {  printf("No se encontro el archivo");
        registros[0]=r;
        f=fopen(nomarch,"wb");
        fwrite(registros,sizeof(JUGADOR),NR,f);
        fclose(f);
     }
     else
     {
         fread(registros,sizeof(JUGADOR),NR,f);
        
         for(i=NR-1;i>=0;i--)
         {  if(registros[i].puntos<=r.puntos && i+1<NR)
               registros[i+1]=registros[i];
            else 
               if(registros[i].puntos>=r.puntos && i+1<NR)
               {  registros[i+1]=r;
                  i=-1;
               }
         }
         if(registros[0].puntos<=r.puntos)
            registros[0]=r;
         rewind(f);
         fwrite(registros,sizeof(JUGADOR),NR,f);
         fclose(f);
     }
}
