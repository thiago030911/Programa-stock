#include <string.h>
#include <stdlib.h>

/*Función:  alta_talonario(pasar por valor el vector con los nombre de archivo, pasar por valor el codigo
            correspondiente al alta de talonario)
  Acción:   Crea un nuevo talonario.*/

void alta_talonario(archivo a[],int item)
{
    talonario t;
    menuopciones sm[]={"","FV-A","ND-A","NC-A","FV-B","ND-B","NC-B"};
    menuopciones sm1[]={"","GUARDAR","CANCELAR"};
    int i=0,cant_reg,rango=sizeof(sm)/27,rango1=sizeof(sm1)/27,menu;
    FILE *tal;
    tal=fopen(a[3].fichero,"r+b");
    if (tal!=NULL)
    {
        fseek(tal,0,SEEK_END);
        cant_reg=ftell(tal)/sizeof(t);
        fseek(tal,0,SEEK_SET);
        cls();
        leyendaModuloTalonarios(item);
        printf("\n");
        linea(Largo);
        do
        {
            submenu(&menu,sm,rango,5);
            switch (menu)
            {
                case 0: t.codigo=0;
                        break;
                case 1: t.codigo=1;
                        break;
                case 2: t.codigo=2;
                        break;
                case 3: t.codigo=3;
                        break;
                case 4: t.codigo=6;
                        break;
                case 5: t.codigo=7;
                        break;
                case 6: t.codigo=8;
                        break;
            }
        } while (menu!=0 && (menu<1 || menu>6));
        if (t.codigo>=1 && t.codigo<=8)
        {
            do
            {
                gotoxy(0,7);
                printf("N%c Sucursal: ",167);
                gotoxy(13,7);
                t.sucursal=capturaCaracter(5);
            } while (t.sucursal==0);
            if (verificaTalonario(tal,t,1)==0)
            {
                do
                {
                    gotoxy(0,8);
                    printf("N%c Inicio:   ",167);
                    gotoxy(13,8);
                    t.pag_inicio=capturaCaracter(8);
                } while (t.pag_inicio==0);
                if (verificaTalonario(tal,t,2)==0)
                {
                    do
                    {
                        gotoxy(0,9);
                        printf("N%c Final:    ",167);
                        gotoxy(13,9);
                        t.pag_final=capturaCaracter(8);
                    } while (t.pag_final==0);
                    if (t.pag_inicio<t.pag_final)
                    {
                        gotoxy(0,10);
                        printf("N%c Emisi%cn:  %d\n",167,162,t.pag_actual=t.pag_inicio);
                        cargarFecha(&t.venc_dia,&t.venc_mes,&t.venc_anio,44,11,"Vencimiento Talonario");
                        printf("CAI: %s\n",strcpy(t.cai,obtenerId()));
                        strcpy(t.id_talonario,obtenerId());
                        linea(Largo);
                        printf("Id_Talonario => %-16s\n",t.id_talonario);
                        t.estado=1;
                        t.eliminado=0;
                        linea(Largo);
                        gotoxy(0,16);
                        linea(Largo);
                        do
                        {
                            submenu(&menu,sm1,rango1,17);
                            switch (menu)
                            {
                                case 1: fseek(tal,0,SEEK_END);
                                        fwrite(&t,sizeof(t),1,tal);
                                        completo();
                                        break;
                                case 2: cancela();
                                        break;
                            }
                        } while (menu==0);
                    }
                    else
                    {
                        gotoxy(0,10);
                        linea(Largo);
                        Color(RED,WHITE);
                        printf("El COMPROBANTE INICIAL debe ser menor que el FINAL\n");
                        Color(BLACK,WHITE);
                        linea(Largo);
                    }
                }
                else
                {
                    gotoxy(0,10);
                    linea(Largo);
                    Color(RED,WHITE);
                    printf("El COMPROBANTE INICIAL NO es CORRELATIVO con el FINAL\n");
                    Color(BLACK,WHITE);
                    linea(Largo);
                }
            }
            else
            {
                gotoxy(0,10);
                registroDuplicado();
            }
        }
        else
            cancela();
    }
    else
        archivoInvalido();
    pausa();
    fclose(tal);
}

/*Función:  verficaTalnario(pasar por referencia el talonario, pasar por valor la estructura talonario, pasar por
            valor el item de ingreso)
  Acción:   Retorna 1 si el valor ingresado está bien cargado, 0 si está mal cargado. */

int verificaTalonario(FILE *tal,talonario t,int item)
{
    talonario taux;
    fseek(tal,0,SEEK_SET);
    fread(&taux,sizeof(talonario),1,tal);
    while (!feof(tal))
    {
        switch (item)
        {
            case 1: if (t.codigo==taux.codigo && t.sucursal==taux.sucursal && taux.pag_final==99999999)
                        return 1;
                        break;
            case 2: if (t.codigo==taux.codigo && t.sucursal==taux.sucursal && t.pag_inicio<=taux.pag_final)
                        return 1;
                        break;
        }
        fread(&taux,sizeof(talonario),1,tal);
    }
    return 0;
}

/*Función:  baja_talonario(pasar por valor el vector con los nombre de archivo, pasar por valor el codigo
            correspondiente la baja del talonario)
  Acción:   Realiza una baja lógica de un talonario.*/

void baja_talonario(archivo a[],int item)
{
    talonario t;
    int flag=0,cant_registros,opcion;
    FILE *tal;
    tal=fopen(a[3].fichero,"r+b");
    cls();
    if (tal!=NULL)
    {
        leyendaModuloTalonarios(item);
        printf("TALONARIOS ACTIVOS\n");
        linea(Largo);
        cant_registros=lista_talonario(a,item);
        do
        {
            printf("OPCION => ");
            scanf("%d",&opcion);
        } while (opcion<1 || opcion>cant_registros);
        fseek(tal,(opcion-1)*sizeof(talonario),SEEK_SET);
        fread(&t,sizeof(talonario),1,tal);
        fseek(tal,0,SEEK_END);
        t.estado=0;
        linea(Largo);
        printf("TALONARIO VIGENTE => <ANULADO>\n");
        fseek(tal,ftell(tal)-sizeof(talonario),SEEK_SET);
        fwrite(&t,sizeof(talonario),1,tal);
        fseek(tal,0,SEEK_END);
        completo();
    }
    linea(Largo);
    fclose(tal);
}

/*Función:  modifica_talonario(pasar por valor el vector con los nombre de archivo, pasar por valor el codigo
            correspondiente para la modificación del talonario)
  Acción:   Modifica, la fecha de vencimiento o el estado del talonario.*/

void modifica_talonario(archivo a[],int item)
{
    FILE *tal;
    talonario t;
    menuopciones sm[]={"<ESC>","FECHA","ESTADO"};
    int rango=sizeof(sm)/27,menu,j=0,cant_registros,aborta=0;
    lista_Desplegable *registro;
    tal=fopen(a[3].fichero,"r+b");
    registro=(lista_Desplegable*)malloc(RANGO*sizeof(lista_Desplegable));
    fseek(tal,0,SEEK_END);
    cant_registros=ftell(tal)/sizeof(talonario);
    fseek(tal,0,SEEK_SET);
    cls();
    if (tal!=NULL)
    {
        fread(&t,sizeof(talonario),1,tal);
        while (!feof(tal))
        {
            sprintf(registro[j].registro,"%04d %-2s [%s]/%05d-%08d %04d ",j+1,nombre_comprobante(t.codigo,'s'),letra_comprobante(t.codigo),t.sucursal,t.pag_actual,t.estado);
            strcat(registro[j].registro,nombre_comprobante(t.codigo,'l'));
            j++;
            fread(&t,sizeof(talonario),1,tal);
        }
    }
    fseek(tal,listaDesplegable(registro,1,cant_registros)*sizeof(talonario),SEEK_SET);
    printf("\n");
    fread(&t,sizeof(talonario),1,tal);
    linea(Largo);
    do
    {
        submenu(&menu,sm,rango,11);
        switch (menu)
        {
            case 0: aborta=1;
                    break;
            case 1: gotoxy(0,13);
                    printf("Fecha Vencimiento Actual: %02d/%02d/%04d",t.venc_dia,t.venc_mes,t.venc_anio);
                    cargarFecha(&t.venc_dia,&t.venc_mes,&t.venc_anio,40,13,"Nuevo Vencimiento");
                    gotoxy(0,14);
                    break;
            case 2: do
                    {
                        gotoxy(0,13);
                        printf("ESTADO [0/1] => ");
                        gotoxy(16,13);
                        t.estado=capturaCaracter(1);
                    } while (t.estado<0 || t.estado>1);
                    gotoxy(0,14);
                    break;
        }
    } while (menu!=0 && (menu<1 || menu>2));
    if (aborta!=1)
    {
        fseek(tal,ftell(tal)-sizeof(talonario),SEEK_SET);
        fwrite(&t,sizeof(talonario),1,tal);
        fseek(tal,0,SEEK_END);
        linea(Largo);
        completo();
        pausa();
    }
    else
        cancela();
    free(registro);
    fclose(tal);
}

/*Función:  lista_talonario(pasar por valor el vector con los nombre de archivo, pasar por valor el codigo
            correspondiente listar los talonarios)
  Acción:   Lista los talonarios que están de alta.*/

int lista_talonario(archivo a[],int item)
{
    talonario t;
    int cant_reg=1;
    FILE *tal;
    tal=fopen(a[3].fichero,"rb");
    cls();
    leyendaModuloTalonarios(item);
    if (tal!=NULL)
    {
        fread(&t,sizeof(talonario),1,tal);
        printf("N%c   Tipo             L Suc.  Inicio   Final    Actual   F. Venc.\n",167);
        linea(Largo);
        while (!feof(tal))
        {
            if (t.estado==1)
                printf("[%02d]  %-15.15s %s %05d %08d %08d %08d %02d-%02d-%04d\n",cant_reg++,nombre_comprobante(t.codigo,'l'),letra_comprobante(t.codigo),t.sucursal,t.pag_inicio,t.pag_final,t.pag_actual,t.venc_dia,t.venc_mes,t.venc_anio);
            fread(&t,sizeof(talonario),1,tal);
        }
        linea(Largo);
    }
    completo();
    fclose(tal);
    return cant_reg;
}

/*Función:  leyendaModuloTalonarios(pasar por valor el código del tipo de acción a realizar)
  Acción:   Muestra por pantalla el encabezado del ítem del menú a ejecutar.*/

void leyendaModuloTalonarios(int leyenda)
{
    linea(Largo);
    Color(WHITE,RED);
    switch (leyenda)
    {
        case 1: printf("MODULO TALONARIOS  <<< ALTA COMPROBANTES >>>                                    \n");
                break;
        case 2: printf("MODULO TALONARIOS  <<< BAJA COMPROBANTES >>>                                    \n");
                break;
        case 3: printf("MODULO TALONARIOS  <<< MODIFICA COMPROBANTES >>>                                \n");
                break;
        case 4: printf("MODULO TALONARIOS  <<< LISTA COMPROBANTES >>>                                   \n");
                break;
    }
    Color(BLACK,WHITE);
    linea(Largo);
}

/*Función:  vencimientoTalonario(pasa por valor la estrucutra con la ruta de los archivos)
  Acción:   Cambia el estado del talonario poniendolo de baja si la fecha actual supera la fecha de vencimiento.*/

void vencimientoTalonario(archivo a[])
{
    time_t date;
    struct tm *tm;
    date=time(NULL);
    tm=localtime(&date);
    talonario t;
    FILE *tal;
    tal=fopen(a[3].fichero,"r+b");
    if (tal!=NULL)
    {
        fread(&t,sizeof(talonario),1,tal);
        while (!feof(tal))
        {
            if (compararFechas(1900+tm->tm_year,tm->tm_mon+1,tm->tm_mday,t.venc_anio,t.venc_mes,t.venc_dia)==-1)
            {
                t.estado=0;
                fseek(tal,ftell(tal)-sizeof(talonario),SEEK_SET);
                fwrite(&t,sizeof(talonario),1,tal);
            }
            fread(&t,sizeof(talonario),1,tal);
        }
    }
    fclose(tal);
}

/*Función:  traerTalonario(pasar por referencia el archivo, pasar por valor la factura)
  Acción:   Devuelve el talonario asociado a la factura ingresada.*/

talonario traerTalonario(FILE *tal,factura f)
{
    talonario t;
    fseek(tal,0,SEEK_SET);
    if (tal!=NULL)
    {
        fread(&t,sizeof(talonario),1,tal);
        while (!feof(tal))
        {
            if (strcmp(f.id_talonario,t.id_talonario)==0)
                return t;
            fread(&t,sizeof(talonario),1,tal);
        }
        strcpy(t.id_talonario,"");
        return t;
    }
}
