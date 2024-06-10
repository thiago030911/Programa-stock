#include <direct.h>
#include <windows.h>

void inicio(archivo a[],int cant)
{
    FILE *pArchivo;
    int i,cont=0;
    char path[MAX_PATH]={""},aux[MAX_PATH]={""};
    verifica(pArchivo,a,0);
    strcat(path,crearpath(pArchivo,a));
    linea(Largo);
    printf("ARCHIVOS DATOS SISTEMA\n");
    linea(Largo);
    for (i=1;i<cant;i++)
    {
        strcpy(aux,"");
        strcpy(aux,path);
        strcat(aux,a[i].fichero);
        strcpy(a[i].fichero,aux);
        printf("\n=> %s",a[i].fichero);
    }
    printf("\n\n");
    linea(Largo);
    pausa();
    if (strcmp(path,"NULL")!=0)
        setup(a,cant);
}

void verifica(FILE *arch,ar a[],int pos)
{
    arch=fopen(a[pos].fichero,"r");
    if (arch==NULL)
    {
        arch=fopen(a[pos].fichero,"w");
        fprintf(arch,"NULL");
    }
    fclose(arch);
}

char *crearpath(FILE *arch,ar a[])
{
    mo sm[]={"    ","DEFECTO","EDITAR"};
    char aux[1],camino[MAX_PATH]={""},path[MAX_PATH]={""};
    int unidad,i,flag=0,change,rango=sizeof(sm)/27;
    FILE *archbin;
    static char ruta[MAX_PATH];
    arch=fopen(a[0].fichero,"r+");
    if (arch!=NULL)
    {
        fgets(ruta,MAX_PATH,arch);
        if (strcmp(ruta,"NULL")==0)
        {
            strcpy(ruta,"");
            unidad=_getdrive()+'A'-1;
            camino[0]=unidad;
            strcat(camino,":\\EMPRESA");
            linea(Largo);
            printf("RUTA DE ACCESO POR [DEFECTO]\n");
            linea(Largo);
            printf("Unidad:  %c\n",unidad);
            printf("Path:    %s\n",camino);
            linea(Largo);
            gotoxy(0,14);
            printf("PATH");
            submenu(&change,sm,rango,14);
            switch (change)
            {
                case 0: case 1: strcpy(ruta,camino);
                                break;
                case 2: printf("Ingrese la nueva ruta:  ");
                        fflush(stdin);
                        gets(ruta);
                        break;
            }
            for (i=0;i<=strlen(ruta);i++)
                if (ruta[i]!='\\')
                    path[i]=ruta[i];
                else
                    {
                        if (path[i]=='\0')
                            path[i]='\\';
                        CreateDirectory(path,NULL);
                        path[i]='\\';
                    }
            strcat(path,"\\");
            CreateDirectory(path,NULL);
            strcat(ruta,"\\DATOS\\");
            CreateDirectory(ruta,NULL);
            strcat(path,"\\COMPROBANTES\\");
            CreateDirectory(path,NULL);
            printf("=> Path: %s\n",ruta);
            linea(Largo);
            gotoxy(0,18);
            rewind(arch);
            fprintf(arch,"%s",ruta);
        }
    }
    fclose(arch);
    return ruta;
}

void setup(archivo a[],int cantidad)
{
    int i;
    char opcion=' ',path[MAX_PATH],unidad[1];
    FILE *aempresa;
    cls();
    linea(Largo);
    gotoxy(0,1);
    Color(WHITE,RED);
    printf("                                                                                ");
    gotoxy(0,1);
    printf("EXISTENCIA DE ARCHIVOS DATOS");
    Color(BLACK,WHITE);
    gotoxy(0,2);
    linea(Largo);
    for(i=1;i<cantidad;i++)
    {
        aempresa=fopen(a[i].fichero,"rb");
        if (aempresa==NULL)
        {
            aempresa=fopen(a[i].fichero,"wb");
            fclose(aempresa);
            printf("\n=>%-70s ",a[i].fichero);
            Color(RED,WHITE);
            printf("CREADO");
            Color(BLACK,WHITE);
        }
        else
        {
            printf("\n=>%-70s ",a[i].fichero);
            Color(GREEN,WHITE);
            printf("OK");
            Color(BLACK,WHITE);
        }
    }
    printf("\n\n");
    linea(Largo);
    pausa();
}
