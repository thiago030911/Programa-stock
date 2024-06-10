void altaStock(archivo a[],int opcion)
{
    stock s;
    FILE *sto;
    menuopciones sm[]={"","GUARDAR","CANCELAR"};
    int menu,rango=sizeof(sm)/27;
    sto=fopen(a[5].fichero,"r+b");
    if (sto!=NULL)
    {
        fseek(sto,0,SEEK_END);
        s.id_stock=(ftell(sto)/sizeof(stock))+1;
        fseek(sto,0,SEEK_SET);
        cls();
        leyendaModuloStock(opcion);
        fflush(stdin);
        printf("Producto:     ");
        gets(s.nombre);
        if (existeProducto(sto,s)==1)
        {
            fflush(stdin);
            printf("Descripci%cn:  ",162);
            gets(s.descripcion);
            do
            {
                gotoxy(0,5);
                printf("Cantidad:     ");
                gotoxy(14,5);
                s.cantidad=capturaCaracter(8);
            } while (s.cantidad==0);
            gotoxy(0,6);
            do
            {
                printf("Venta:        $ ");
                scanf("%f",&s.precio_venta);
            } while (s.precio_venta<=0);
            do
            {
                printf("Compra:       $ ");
                scanf("%f",&s.precio_compra);
            } while (s.precio_compra<=0 && s.precio_compra>s.precio_venta);
            s.eliminado=0;
            linea(Largo);
            gotoxy(0,17);
            linea(Largo);
            do
            {
                submenu(&menu,sm,rango,18);
                switch (menu)
                {
                    case 1: fseek(sto,0,SEEK_END);
                            fwrite(&s,sizeof(stock),1,sto);
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
                registroDuplicado();
            }
    }
    else
        archivoInvalido();
    pausa();
    fclose(sto);
}

/*Función:  bajaStock(pasar por referencia la estructura con los nombres de archivo, pasar por valor la opción del menú)
  Acción:   Marca el producto seleccionado poniendolo como un producto discontinuo.*/

void bajaStock(archivo a[],int opcion)
{
    stock s;
    FILE *sto;
    menuopciones sm[]={"","ACEPTAR","CANCELAR"};
    int menu,rango=sizeof(sm)/27;
    sto=fopen(a[5].fichero,"r+b");
    if (sto!=NULL)
    {
        cls();
        s=obtenerProducto(sto);
        if (strcmp(s.nombre,"")!=0)
        {
            gotoxy(0,10);
            linea(Largo);
            Color(RED,WHITE);
            printf("PRODUCTO PARA DISCONTINUAR                                                      \n");
            Color(BLACK,WHITE);
            linea(Largo);
            printf("Id. Stock:   %04d\n",s.id_stock);
            printf("Producto:    %-20s\n",s.nombre);
            printf("Descripci%cn: %-30s\n",162,s.descripcion);
            printf("Cantidad:    %d\n",s.cantidad);
            printf("Venta:       $ %-13.2f\n",s.precio_venta);
            printf("Compra:      $ %-13.2f\n",s.precio_compra);
            linea(Largo);
            gotoxy (0,20);
            linea(Largo);
            do
            {
                submenu(&menu,sm,rango,21);
                switch (menu)
                {
                    case 1: s.eliminado=1;
                            fseek(sto,ftell(sto)-sizeof(stock),SEEK_SET);
                            fwrite(&s,sizeof(stock),1,sto);
                            fseek(sto,0,SEEK_END);
                            completo();
                            break;
                    case 2: cancela();
                            break;
                }
            } while (menu==0);
        }
        else
        {
            leyendaModuloStock(opcion);
            noEncontrado();
        }
    }
    pausa();
    fclose(sto);
}

/*Función:  modificaStock(pasar por referencia la estructura con los nombres de archivo, pasar por valor la opción del menú)
  Acción:   Realiza cambios en la descripción, precio de venta o compra del producto seleccionado.*/

void modificaStock(archivo a[],int opcion)
{
    stock s;
    FILE *sto;
    menuopciones sm[]={"","ACEPTAR","CANCELAR"};
    int menu,rango=sizeof(sm)/27,edicion;
    sto=fopen(a[5].fichero,"r+b");
    if (sto!=NULL)
    {
        cls();
        s=obtenerProducto(sto);
        if (strcmp(s.nombre,"")!=0)
        {
            do
            {
                cls();
                leyendaModuloStock(opcion);
                Color(RED,WHITE);
                printf("PRODUCTO PARA DISCONTINUAR                                                      \n");
                Color(BLACK,WHITE);
                linea(Largo);
                printf("    Id. Stock:   %04d\n",s.id_stock);
                printf("    Producto:    %-20s\n",s.nombre);
                Color(YELLOW,BLUE);
                printf("<1>");
                Color(BLUE,YELLOW);
                printf(" Descripci%cn: ",162);
                Color(BLACK,WHITE);
                printf("%-30s\n",s.descripcion);
                printf("    Cantidad:    %d\n",s.cantidad);
                Color(YELLOW,BLUE);
                printf("<2>");
                Color(BLUE,YELLOW);
                printf(" Venta:       ");
                Color(BLACK,WHITE);
                printf("$ %-13.2f\n",s.precio_venta);
                Color(YELLOW,BLUE);
                printf("<3>");
                Color(BLUE,YELLOW);
                printf(" Compra:      ");
                Color(BLACK,WHITE);
                printf("$ %-13.2f\n",s.precio_compra);
                Color(YELLOW,BLUE);
                printf("<0>");
                Color(BLUE,YELLOW);
                printf(" SALIR\n");
                Color(BLACK,WHITE);
                linea(Largo);
                printf("Opci%cn ==>",162);
                gotoxy(12,13);
                edicion=capturaCaracter(1);
                if (edicion>0 && edicion<=3)
                {
                    gotoxy(0,15);
                    linea(Largo);
                    gotoxy(0,17);
                    linea(Largo);
                    gotoxy(0,16);
                }
                switch (edicion)
                {
                    case 1: fflush(stdin);
                            printf("Descripci%cn:  ",162);
                            gets(s.descripcion);
                            break;
                    case 2: do
                            {
                                printf("Venta:  $ ");
                                scanf("%f",&s.precio_venta);
                            } while (s.precio_venta<=0);
                            break;
                    case 3: do
                            {
                                printf("Compra: $ ");
                                scanf("%f",&s.precio_compra);
                            } while (s.precio_compra<=0 && s.precio_compra>s.precio_venta);
                            break;
                }
            } while (edicion!=0);
            gotoxy(0,20);
            linea(Largo);
            do
            {
                submenu(&menu,sm,rango,21);
                switch (menu)
                {
                    case 1: fseek(sto,ftell(sto)-sizeof(stock),SEEK_SET);
                            fwrite(&s,sizeof(stock),1,sto);
                            fseek(sto,0,SEEK_END);
                            completo();
                            break;
                    case 2: cancela();
                            break;
                }
            } while (menu==0);
        }
        else
        {
            leyendaModuloStock(opcion);
            noEncontrado();
        }
    }
    pausa();
    fclose(sto);
}

/*Función:  listarStock(pasar por referencia la estructura nombres de archivo, pasar por valor la opción del menú)
  Acción:   Muestra por pantalla la lista de productos, poniendo en verde si la cantidad está normal y rojo si esta bajo.*/

void listarStock(archivo a[],int opcion)
{
    stock s;
    FILE *sto;
    sto=fopen(a[5].fichero,"r+b");
    cls();
    leyendaModuloStock(opcion);
    if (sto!=NULL)
    {
        fread(&s,sizeof(stock),1,sto);
        printf("Id.   C%cdigo               Descripcion                    Cantidad\n",162);
        linea(Largo);
        while (!feof(sto))
        {
            if (s.eliminado==0)
            {
                printf("%05d %-20.20s %-30.30s ",s.id_stock,s.nombre,s.descripcion);
                if (s.cantidad<10)
                    Color(RED,WHITE);
                else
                    Color(GREEN,WHITE);
                printf("%9d\n",s.cantidad);
                Color(BLACK,WHITE);
            }
            fread(&s,sizeof(stock),1,sto);
        }
    }
    linea(Largo);
    completo();
    pausa();
    fclose(sto);
}

/*Función:  buscarStock(pasar por referencia la estructura con los nombres de archivo, pasar por valor la opción del menú)
  Acción:   Muestra por pantalla la lista de productos, permite seleccionarlos de una lista desplegable y muestra los datos
            del ítem seleccionado por pantalla, luego nos permite seguir buscando o salir al menú stock.*/

void buscarStock(archivo a[],int opcion)
{
    stock s;
    FILE *sto;
    menuopciones sm[]={"","BUSCAR","SALIR"};
    int menu,rango=sizeof(sm)/27,continua=0;
    sto=fopen(a[5].fichero,"rb");
    if (sto!=NULL)
    {
        do
        {
            continua=0;
            cls();
            s=obtenerProducto(sto);
            if (strcmp(s.nombre,"")!=0)
            {
                gotoxy(0,10);
                linea(Largo);
                Color(RED,WHITE);
                printf("PRODUCTO SELECCIONADO                                                           \n");
                Color(BLACK,WHITE);
                linea(Largo);
                printf("Id. Stock:   %04d\n",s.id_stock);
                printf("Producto:    %-20s\n",s.nombre);
                printf("Descripci%cn: %-30s\n",162,s.descripcion);
                printf("Cantidad:    %d\n",s.cantidad);
                printf("Venta:       $ %-13.2f\n",s.precio_venta);
                printf("Compra:      $ %-13.2f\n",s.precio_compra);
                linea(Largo);
                gotoxy(0,20);
                linea(Largo);
                do
                {
                    submenu(&menu,sm,rango,21);
                    switch (menu)
                    {
                        case 1: continua=1;
                                break;
                        case 2: continua=2;
                                break;
                    }
                } while (menu==0);
            }
            else
            {
                leyendaModuloStock(opcion);
                noEncontrado();
            }
        } while (continua!=2);
    }
    pausa();
    fclose(sto);
}

/*Función:  faltante(pasar por referencia la estructura nombres de archivo, pasar por valor la opción del menú)
  Acción:   Muestra por pantalla la lista de productos que están por debajo del stock mínimo requerido.*/

void faltante(archivo a[],int opcion)
{
    stock s;
    FILE *sto;
    int cant_productos=0;
    sto=fopen(a[5].fichero,"rb");
    cls();
    leyendaModuloStock(opcion);
    if (sto!=NULL)
    {
        fread(&s,sizeof(stock),1,sto);
        printf("Id.   Código               Descripcion                    Cantidad\n");
        linea(Largo);
        while (!feof(sto))
        {
            if (s.eliminado==0 && s.cantidad<10)
            {
                printf("%05d %-20.20s %-30.30s ",s.id_stock,s.nombre,s.descripcion);
                if (s.cantidad<100)
                    Color(RED,WHITE);
                else
                    Color(GREEN,WHITE);
                printf("%9d\n",s.cantidad);
                Color(BLACK,WHITE);
                cant_productos++;
            }
            fread(&s,sizeof(stock),1,sto);
        }
    }
    linea(Largo);
    if (cant_productos>0)
    {
        Color(RED,WHITE);
        printf("REALIZAR EL PEDIDO CORRESPONDIENTE DE ESTOS PRODUCTOS                           \n");
        Color(BLACK,WHITE);
        linea(Largo);
    }
    else
    {
        Color(GREEN,WHITE);
        printf("NO HAY FALTANTE DE PRODUCTOS                                                    \n");
        Color(BLACK,WHITE);
        linea(Largo);
    }
    pausa();
    fclose(sto);
}

/*Función:  leyendaModuloStock(pasar por valor el código del tipo de acción a realizar)
  Acción:   Muestra por pantalla el encabezado del stock del menú a ejecutar.*/

void leyendaModuloStock(int leyenda)
{
    linea(Largo);
    Color(WHITE,RED);
    switch (leyenda)
    {
        case 1: printf("MODULO STOCK       <<< ALTA >>>                                                 \n");
                break;
        case 2: printf("MODULO STOCK       <<< BAJA >>>                                                 \n");
                break;
        case 3: printf("MODULO STOCK       <<< MODIFICA >>>                                             \n");
                break;
        case 4: printf("MODULO STOCK       <<< LISTA >>>                                                \n");
                break;
        case 5: printf("MODULO STOCK       <<< BUSCAR >>>                                               \n");
                break;
        case 6: printf("MODULO STOCK       <<< FALTANTE >>>                                             \n");
                break;
    }
    Color(BLACK,WHITE);
    linea(Largo);
}

/*Función:  existeProducto(pasar por referencia el archivo stock, pasar por valor la estructura stock)
  Acción:   Verifica si el producto pedido existe.*/

int existeProducto(FILE *sto,stock s)
{
    stock se;
    fseek(sto,0,SEEK_SET);
    fread(&se,sizeof(stock),1,sto);
    while(!feof(sto))
    {
        if (strcmp(s.nombre,se.nombre)==0)
            return 0;
        fread(&se,sizeof(stock),1,sto);
    }
    return 1;
}

/*Función:  obtenerProducto(pasar por referencia el archivo stock)
  Acción:   Devuelve el producto seleccionado de la lista desplegable.*/

stock obtenerProducto(FILE *sto)
{
    stock s;
    strcpy(s.nombre,"");
    int j=0,k,flag=0,rango=0,ubicacion,posicion;
    char dato[4];
    fseek(sto,0,SEEK_SET);
    lista_Desplegable *registro;
    registro=(lista_Desplegable*)malloc(RANGO*sizeof(lista_Desplegable));
    cls();
    fread(&s,sizeof(stock),1,sto);
    while (!feof(sto))
    {
        if (s.eliminado==0)
        {
            sprintf(registro[rango++].registro,"%04d %-25.25s %-30.30s",s.id_stock,s.nombre,s.descripcion);
            flag=1;
        }
        fread(&s,sizeof(stock),1,sto);
    }
    if (flag==1)
    {
        posicion=listaDesplegable(registro,3,rango);
        for (k=0;k<4;k++)
            dato[k]=registro[posicion].registro[k];
        ubicacion=atoi(dato);
        fseek(sto,(ubicacion-1)*sizeof(stock),SEEK_SET);
        printf("\n");
        fread(&s,sizeof(stock),1,sto);
    }
    else
        strcpy(s.nombre,"");
    free(registro);
    return s;
}
