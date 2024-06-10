/** COMPILADOR  GNU GCC COMPILER **/

/**************************************************************************/
/** @brief Proyecto de Estructura y Base de Datos  del turno tarde 2020. **/
/** @version 0.0.1                                                       **/
/**                                                                      **/
/**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#include <time.h>

#define Largo 80
#define RANGO 100

/** INICIO DE ESTRUCTURA DE LAS TABLAS **/

typedef struct
{
    char id_factura[15];
    char id_empresa[15];
    char id_talonario[15];
    int num_cliente;
    int emision_dia;
    int emision_mes;
    int emision_anio;
    int numero_sucursal;
    int numero_factura;
    char id_items[15];
    float subtotal;
    float iva21;
    float total;
    int estado;             //0 Activa, 1 Paga, 2 Anulada
    unsigned int eliminado;
} factura;

typedef struct
{
    char id_talonario[15];
    int codigo;
    int sucursal;
    int pag_inicio;
    int pag_final;
    int pag_actual;
    int venc_dia;
    int venc_mes;
    int venc_anio;
    char cai[15];
    int estado;
    unsigned int eliminado;
} talonario;

typedef struct
{
    char id_items[15];
    char id_factura[15];
    int id_stock;
    int cantidad;
    char nombre[20];
    char descripcion[30];
    float unitario;
    unsigned int eliminado;
} items;

typedef struct
{
    int id_stock;
    char nombre[20];
    char descripcion[30];
    int cantidad;
    float precio_venta;
    float precio_compra;
    unsigned int eliminado;
} stock;

typedef struct
{
    char id_empresa[15];
    char nombre_empresa[30];
    char nombre_fantasia[30];
    char tel[20];
    char direccion[20];
    char localidad[20];
    int cpost;
    int tipo_documento;         //80 CUIT, 86 CUIL, 96 DNI, 99 SIN IDENTIFICAR
    char documento[11];
    int cond_iva;               //1 RI, 2 RM, 3 CF, 4 EX, 5 SC
    int num_cliente;
    char tipo_cliente[2];
    char contacto_nombre[30];
    char contacto_apellido[30];
    char contacto_dni[11];
    char contacto_cargo[40];
    unsigned int eliminado;
} cliente_proveedor;

typedef struct
{
    char id_factura[15];
    char id_empresa[15];
    char id_items[15];
    int num_cliente;
    int emision_dia;
    int emision_mes;
    int emision_anio;
    int codigo_talonario;
    int numero_sucursal;
    int numero_factura;
    float subtotal;
    float iva21;
    float total;
    int estado;             //0 Activa, 1 Paga, 2 Anulada
    unsigned int eliminado;
} compra;

/** FIN DE ESTRUCTURAS DE LAS TABLAS**/

/** ESTRUCTURAS AUXILIARES **/

/*Estructura que almaecena las variables con el código de color*/
enum Colors {BLACK=0,BLUE=1,GREEN=2,CYAN=3,RED=4,MAGENTA=5,BROWN=6,LGREY=7,DGREY=8,LBLUE=9,LGREEN=10,LCYAN=11,LRED=12,LMAGENTA=13,YELLOW=14,WHITE=15};

/*Estructura para manejar la lista de items*/
typedef struct registro
{
    items reg;
    struct registro *sig;
} nodo;

/*Estructura para manejar los menús y submenús*/
typedef struct
{
    char menu[27];
} menuopciones;

/*Estructura para guardar el path de los archivos*/
typedef struct
{
    char fichero[MAX_PATH];
} archivo;

/*Estructura para manejar la lista desplegable*/
typedef struct
{
    char registro[66];
} lista_Desplegable;

typedef menuopciones mo;
typedef archivo ar;

/** CABECERAS EXTERNAS DE FUNCIONES**/

#include "FuncionesGral.h"
#include "FuncionesGral.c"
#include "Archivos.h"
#include "Archivos.c"
#include "Lista.h"
#include "Lista.c"
#include "Talonarios.h"
#include "Talonarios.c"
#include "ClienteProveedor.h"
#include "ClienteProveedor.c"
#include "Facturacion.h"
#include "Facturacion.c"
#include "Compras.h"
#include "Compras.c"
#include "Stock.h"
#include "Stock.c"
#include "Contabilidad.h"
#include "Contabilidad.c"
#include "Menus.h"
#include "Menus.c"

int main()
{
    mo m[]={"     MENU  PRINCIPAL     ","CLIENTE/PROVEEDOR        ","FACTURACION              ","STOCK                    ","CONTABILIDAD             ","TALONARIO                ","COMPRAS                  ","FINALIZAR PROGRAMA       "};
    ar a[]={"empresa.cfg","cliente_proveedor.dat","factura.dat","talonario.dat","items.dat","stock.dat","compras.dat"};
    int cant=sizeof(a)/MAX_PATH,opcion,rango=sizeof(m)/27;
    char path[MAX_PATH];
    bienvenida();               /*Mensaje de bienvenida*/
    inicio(a,cant);             /*Configura los archivos del sistema*/
    vencimientoTalonario(a);    /*Verifica que los talonarios estén vigentes al iniciar, sino les da la baja*/
    do
    {
        menu(&opcion,m,rango);
        switch (opcion)
        {
            case 1: cli_pro(a); /*Módulo cliente-proveedor*/
                    break;
            case 2: fac(a);     /*Módulo facturación*/
                    break;
            case 3: sto(a);     /*Módulo stock*/
                    break;
            case 4: con(a);     /*Módulo contabilidad*/
                    break;
            case 5: tal(a);     /*Módulo talonarios*/
                    break;
            case 6: com(a);     /*Módulo compras*/
                    break;
        }
    } while (opcion!=0);
    finaliza();                 /*Mensaje de salida*/
    return 0;
}

