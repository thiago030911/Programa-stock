/*Función:  emitirComprobante(pasar por valor la estructura con los nombres de los archivos, pasar por valor la
            ubicacion dentro del menú facturacion)
  Acción:   Emite un comprobante A/B de factura o nota de débito, incrementando la factura actual en el talonario
            correspondiente, restando la cantidad de productos en el stock y generando añadiendo a ítems la cantidad
            de productos vendidos.*/

void emitirComprobante(archivo a[],int it)
{
    cliente_proveedor cp;
    talonario t,t1;
    factura f;
    items i;
    nodo *item;
    item=NULL;
    float subtotal=0;
    time_t date;
    struct tm *tm;
    date=time(NULL);
    tm=localtime(&date);
    menuopciones sm[]={"","GUARDAR","CANCELAR"};
    menuopciones sm1[]={"FECHA  =>","ACTUAL","MANUAL"};
    menuopciones sm2[]={"","BUSCAR","ELEGIR"};
    int menu,flag=0,rango=sizeof(sm)/27,rango1=sizeof(sm1)/27,rango2=sizeof(sm2)/27;
    FILE *c_p,*fac,*tal,*ite,*sto;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"ab");
    tal=fopen(a[3].fichero,"r+b");
    ite=fopen(a[4].fichero,"ab");
    sto=fopen(a[5].fichero,"r+b");
    cls();
    leyendaModuloFacturas(it);
    printf("SELECCIONAR CLIENTE\n");
    linea(Largo);
    do
    {
        submenu(&menu,sm2,rango2,5);
        switch (menu)
        {
            case 1: cp=obtenerClienteProveedor(c_p);
                    break;
            case 2: cp=encontrarClienteProveedor(c_p,1);
                    break;
        }
    } while (menu==0);
    if (strcmp(cp.id_empresa,"")!=0)
    {
        if (strcmp(cp.id_empresa,"?")!=0)
        {
            cls();
            leyendaModuloFacturas(it);
            mostrarCP(cp,3,0);
            t=obtenerTalonario(tal,cp.cond_iva);
            if (strcmp(t.id_talonario,"")!=0)
            {
                strcpy(f.id_factura,obtenerId());
                strcpy(f.id_empresa,cp.id_empresa);
                strcpy(f.id_talonario,t.id_talonario);
                f.num_cliente=cp.num_cliente;
                gotoxy(0,10);
                linea(Largo);
                do
                {
                    submenu(&menu,sm1,rango1,11);
                    switch (menu)
                    {
                        case 1: gotoxy(0,13);
                                printf("Fecha del Comprobante [DD/MM/AAAA] => %02d/%02d/%04d\n",tm->tm_mday,tm->tm_mon+1,1900+tm->tm_year);
                                f.emision_dia=tm->tm_mday;
                                f.emision_mes=tm->tm_mon+1;
                                f.emision_anio=1900+tm->tm_year;
                                break;
                        case 2: cargarFecha(&t.venc_dia,&t.venc_mes,&t.venc_anio,38,12,"del Comprobante");
                                break;
                    }
                } while (menu==0);
                linea(Largo);
                mostrarTalonario(t,15);
                pausa();
                f.numero_sucursal=t.sucursal;
                f.numero_factura=t.pag_actual;
                t.pag_actual++;
                obtenerStock(sto,&item,f.id_factura,&subtotal,1);
                pausa();
                cls();
                leyendaModuloFacturas(it);
                linea(Largo);
                printf("PRODUCTOS A FACTURAR\n");
                linea(Largo);
                verLista(item);
                linea(Largo);
                f.subtotal=subtotal;
                f.iva21=f.subtotal*.21;
                f.total=f.subtotal*1.21;
                f.estado=0;
                pausa();
                cls();
                mostrarComprobante(a,cp,f,t,item,it);
                gotoxy(0,28);
                linea(Largo);
                do
                {
                    submenu(&menu,sm,rango,29);
                    switch (menu)
                    {
                        case 1: fwrite(&f,sizeof(factura),1,fac);
                                fseek(tal,ftell(tal)-sizeof(talonario),SEEK_SET);
                                fwrite(&t,sizeof(talonario),1,tal);
                                cargaModificacionStock(sto,item,0);
                                cargarArchivo(ite,item);
                                generarComprobante(a,cp,f,t,item);
                                gotoxy(0,28);
                                archivoGeneraComprobante(29);
                                break;
                        case 2: cancela();
                                break;
                    }
                } while (menu==0);
            }
            else
                noEncontrado();
        }
        else
            noEncontrado();
    }
    else
        cancela();
    gotoxy(0,31);
    pausa();
    eliminar(&item);
    fclose(c_p);
    fclose(fac);
    fclose(tal);
    fclose(ite);
    fclose(sto);
}

/*Función:  enularComprobante(pasar por valor la estructura con los nombres de los archivos, pasar por valor la
            ubicacion dentro del menú facturacion)
  Acción:   Genera una NC A/B de una factura o nota de débito, se agrega al stock la cantidad de productos que se
            reintegran, incrementa el número comprobante en el talonario y se agregan los ítems del comprobante.*/

void anularComprobante(archivo a[],int item)
{
    time_t date;
    struct tm *tm;
    date=time(NULL);
    tm=localtime(&date);
    cliente_proveedor cp;
    factura f,c;
    talonario t;
    items i;
    stock s;
    nodo *l_items;
    l_items=NULL;
    menuopciones sm[]={"<ESC>","CLIENTE","FACTURA"};
    menuopciones sm1[]={"","GUARDAR","CANCELAR"};
    menuopciones sm2[]={"","BUSCAR","ELEGIR"};
    int rango=sizeof(sm)/27,rango1=sizeof(sm1)/27,rango2=sizeof(sm2)/27,menu,cortar=1;
    char auxiliar[15];
    FILE *c_p,*fac,*tal,*ite,*sto;
    c_p=fopen(a[1].fichero,"r+b");
    fac=fopen(a[2].fichero,"r+b");
    tal=fopen(a[3].fichero,"r+b");
    ite=fopen(a[4].fichero,"r+b");
    sto=fopen(a[5].fichero,"r+b");
    cls();
    strcpy(cp.id_empresa,"");
    f.numero_sucursal=0;
    f.numero_factura=0;
    leyendaModuloFacturas(item);
    do
    {
        submenu(&menu,sm,rango,3);
        switch (menu)
        {
            case 0: cortar=0;
                    break;
            case 1: do
                    {
                        submenu(&menu,sm2,rango2,5);
                        switch (menu)
                        {
                            case 1: cp=obtenerClienteProveedor(c_p);
                                    break;
                            case 2: cp=encontrarClienteProveedor(c_p,1);
                                    break;
                        }
                    } while (menu==0);
                    cortar=2;
                    break;
            case 2: gotoxy(0,5);
                    printf("N%c Factura: _____-________",167);
                    gotoxy(12,5);
                    f.numero_sucursal=capturaCaracter(5);
                    cp=obtenerClienteId(c_p,f);
                    gotoxy(18,5);
                    break;
        }
    } while (menu!=0 && (menu<1 || menu>2));
    if (cortar!=0)
    {
        f=obtenerComprobante(fac,f,cp);
        if (strcmp(f.id_factura,"")!=0)
        {
            if (cp.cond_iva==1)
                t=obtenerFToNCPorSucursal(tal,f,3);
            else
                t=obtenerFToNCPorSucursal(tal,f,8);
            if (strcmp(f.id_talonario,"")!=0)
            {
                cls();
                memcpy(&c,&f,sizeof(factura));
                f.estado=2;
                guardarFactura(fac,f);
                strcpy(c.id_factura,obtenerId());
                obtenerItems(ite,c,f.id_factura,&l_items,1);
                strcpy(c.id_talonario,t.id_talonario);
                strcpy(c.id_items,obtenerId());
                c.estado=2;
                c.emision_dia=tm->tm_mday;
                c.emision_mes=tm->tm_mon+1;
                c.emision_anio=1900+tm->tm_year;
                c.numero_sucursal=t.sucursal;
                c.numero_factura=t.pag_actual;
                t.pag_actual++;
                mostrarComprobante(a,cp,c,t,l_items,item);
                gotoxy(0,28);
                linea(Largo);
                do
                {
                    submenu(&menu,sm1,rango1,29);
                    switch (menu)
                    {
                        case 1: fseek(fac,0,SEEK_END);
                                fwrite(&c,sizeof(factura),1,fac);
                                fseek(tal,ftell(tal)-sizeof(talonario),SEEK_SET);
                                fwrite(&t,sizeof(talonario),1,tal);
                                cargaModificacionStock(sto,l_items,1);
                                generarComprobante(a,cp,c,t,l_items);
                                gotoxy(0,28);
                                archivoGeneraComprobante(29);
                                break;
                        case 2: cancela();
                                break;
                    }
                } while (menu==0);
            }
            else
                noEncontrado();
        }
        else
            noEncontrado();
    }
    else
        cancela();
    gotoxy(0,31);
    pausa();
    eliminar(&l_items);
    fclose(c_p);
    fclose(fac);
    fclose(tal);
    fclose(ite);
    fclose(sto);
}

/*Función:  listarComprobante(pasar por valor la estructura con los nombres de archivos, pasar por valor la posición
            del menu)
  Acción:   Lista los comprobantes por cliente o rango de fecha.*/

void listarComprobante(archivo a[],int item)
{
    cliente_proveedor cp;
    factura f,fd,fh;
    talonario t;
    menuopciones sm[]={"<ESC>","CLIENTE","RANGO","TODOS"};
    menuopciones sm1[]={"","BUSCAR","LISTA"};
    int rango=sizeof(sm)/27,rango1=sizeof(sm1)/27,menu,menu1,cortar=1;
    FILE *c_p,*fac,*tal;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"rb");
    tal=fopen(a[3].fichero,"rb");
    cls();
    leyendaModuloFacturas(item);
    do
    {
        submenu(&menu,sm,rango,3);
        switch (menu)
        {
            case 0: cortar=0;
                    break;
            case 1: do
                    {
                        submenu(&menu1,sm1,rango1,5);
                        switch (menu1)
                        {
                            case 1: cp=obtenerClienteProveedor(c_p);
                                    break;
                            case 2: cp=encontrarClienteProveedor(c_p,1);
                                    break;
                        }
                    } while (menu1==0);
                    cortar=2;
                    break;
            case 2: do
                    {
                        cargarFecha(&fd.emision_dia,&fd.emision_mes,&fd.emision_anio,28,4,"DESDE");
                        cargarFecha(&fh.emision_dia,&fh.emision_mes,&fh.emision_anio,28,5,"HASTA");
                        gotoxy(0,6);
                        printf("El RANGO [DESDE]>=[HASTA]");
                    } while (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,fh.emision_dia,fh.emision_mes,fh.emision_anio)==-1);
                    break;
            case 3: cortar=3;
                    break;
        }
    } while (menu!=0 && (menu<1 || menu>3));
    if (cortar!=0)
    {
        cls();
        leyendaModuloFacturas(item);
        if (cortar==2)
            printf("LISTA COMPROBANTES CLIENTE: %-25s   %s: %s\n",cp.nombre_empresa,nombreDocumento(cp.tipo_documento),cp.documento);
        else
            if (cortar!=3)
                printf("LISTA COMPROBANTES DESDE %02d/%02d/%04d HASTA %02d/%02d/%04d\n",fd.emision_dia,fd.emision_mes,fd.emision_anio,fh.emision_dia,fh.emision_mes,fh.emision_anio);
        linea(Largo);
        leerFactura(fac,tal,cp,fd,fh,menu);
        linea(Largo);
        completo();
    }
    else
        cancela();
    pausa();
    fclose(c_p);
    fclose(fac);
    fclose(tal);
}

/*Función:  verComprobante(pasar por valor la estructura con los nombres de archivos, pasar por valor la posición
            del menu)
  Acción:   Permite seleccionar de una Lista los comprobantes por cliente o rango de fecha, luego muestra por pantalla
            los datos del comprobantes seleccionado.*/

void verComprobante(archivo a[],int item)
{
    cliente_proveedor cp;
    factura f,fd,fh;
    talonario t;
    items i;
    nodo *l_items;
    menuopciones sm[]={"<ESC>","CLIENTE","LISTA"};
    int rango=sizeof(sm)/27,menu,cortar=1;
    FILE *c_p,*fac,*tal,*ite;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"rb");
    tal=fopen(a[3].fichero,"rb");
    ite=fopen(a[4].fichero,"rb");
    l_items=NULL;
    cls();
    leyendaModuloFacturas(item);
    do
    {
        submenu(&menu,sm,rango,3);
        switch (menu)
        {
            case 0: cortar=0;
                    break;
            case 1: cp=obtenerClienteProveedor(c_p);
                    cortar=2;
                    break;
            case 2: cp=encontrarClienteProveedor(c_p,1);
                    cortar=2;
                    break;
        }
    } while (menu!=0 && (menu<1 || menu>2));
    if (cortar!=0)
    {
        f=obtenerComprobante(fac,f,cp);
        if (strcmp(f.id_factura,"")!=0)
        {
            t=traerTalonario(tal,f);
            if (strcmp(t.id_talonario,"")!=0)
            {
                obtenerItems(ite,f,f.id_factura,&l_items,0);
                cls();
                mostrarComprobante(a,cp,f,t,l_items,item);
                linea(Largo);
                completo();
            }
            else
                noEncontrado();
        }
        else
            noEncontrado();
    }
    else
        cancela();
    pausa();
    eliminar(&l_items);
    fclose(c_p);
    fclose(fac);
    fclose(tal);
    fclose(ite);
}

/*Función:  pagarComprobante(pasar por valor la estructura con los nombres de archivos, pasar por valor la posición
            del menu)
  Acción:   Selecciona el comprobante por datos del cliente y lo cancela en el sistema.*/

void pagarComprobante(archivo a[],int item)
{
    cliente_proveedor cp;
    factura f;
    menuopciones sm[]={"<ESC>","CLIENTE","LISTA"};
    menuopciones sm1[]={"","CANCELAR","ACEPTAR"};
    int rango=sizeof(sm)/27,rango1=sizeof(sm1)/27,menu,cortar=1;
    FILE *c_p,*fac;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"r+b");
    cls();
    leyendaModuloFacturas(item);
    do
    {
        submenu(&menu,sm,rango,3);
        switch (menu)
        {
            case 0: cortar=0;
                    break;
            case 1: cp=obtenerClienteProveedor(c_p);
                    break;
            case 2: cp=encontrarClienteProveedor(c_p,1);
                    break;
        }
    } while (menu!=0 && (menu<1 || menu>1));
    if (cortar!=0)
    {
        f=obtenerComprobante(fac,f,cp);
        if (strcmp(f.id_factura,"")!=0)
        {
            cls();
            leyendaModuloFacturas(item);
            printf("Cliente:   %-30s\n",f.id_empresa);
            printf("Direcci%cn: %s - %s - CP(%d)\n",162,cp.direccion,cp.localidad,cp.cpost);
            linea(Largo);
            printf("Fecha:     %02d/%02d/%04d\n",f.emision_dia,f.emision_mes,f.emision_anio);
            printf("FACTURA N%c %05d-%08d\n",167,f.numero_sucursal,f.numero_factura);
            printf("Monto:     $ %-13.2f\n",f.total);
            f.estado=1;
            linea(Largo);
            gotoxy(0,14);
            linea(Largo);
            {
                submenu(&menu,sm1,rango1,15);
                switch (menu)
                {
                    case 1: cancela();
                            break;
                    case 2: f.estado=1;
                            guardarFactura(fac,f);
                            completo();
                            break;
                }
            } while (menu==0);
        }
        else
            noEncontrado();
    }
    else
        cancela();
    pausa();
    fclose(c_p);
    fclose(fac);
}

/*Función:  guardaFactura(pasar por referencia el archivo, pasar por valor la estrucutra factura)
  Acción:   Guarda el comprobante en la posición correspondiente del registro.*/

void guardarFactura(FILE *fac,factura f)
{
    factura fb;
    int flag=0;
    fseek(fac,0,SEEK_SET);
    fread(&fb,sizeof(factura),1,fac);
    while (!feof(fac) && flag==0)
    {
        if (strcmp(fb.id_factura,f.id_factura)==0)
        {
            fseek(fac,ftell(fac)-sizeof(factura),SEEK_SET);
            fwrite(&f,sizeof(factura),1,fac);
            fseek(fac,0,SEEK_END);
            flag=1;
        }
        fread(&fb,sizeof(factura),1,fac);
    }
}

/*Función:  obtenerComprobante(pasar la condicion de iva "cond_iva 1 RI, 2 RM, 3 CF, 4 EX, 5 SC")
  Acción:   Devuelve el código del comprobante en base a la condición de IVA y si el comprobante es una factura,
            nota de credito o nota de débito.*/

int obtenerCodigoComprobante(int cond_iva)
{
    menuopciones sm[]={"","FACTURA","NOTA DEBITO"};
    int codigo_comprobante=0,menu,rango=sizeof(sm)/27;
    do
    {
        submenu(&menu,sm,rango,10);
        switch (menu)
        {
            case 1: if (cond_iva==1)
                        codigo_comprobante=1;
                    else
                        codigo_comprobante=6;
                    break;
            case 2: if (cond_iva==1)
                        codigo_comprobante=2;
                    else
                        codigo_comprobante=7;
                    break;
            case 3: if (cond_iva==1)
                        codigo_comprobante=3;
                    else
                        codigo_comprobante=8;
                    break;
        }
    } while (menu==0);
    return codigo_comprobante;
}

/*Función:  obtenerComprobante(pasar por referencia el archivo fac, pasa por valor la estructura de la facutra,
            pasa por valor la estructura de cliente_proveedor)
  Acción:   Retorna los datos del comprobante seleccionado de la lista desplegable.*/

factura obtenerComprobante(FILE *fac,factura f,cliente_proveedor cp)
{
    int j=0,k,flag=0,rango=0,ubicacion,posicion;
    char dato[4];
    factura f_dato;
    lista_Desplegable *registro;
    registro=(lista_Desplegable*)malloc(RANGO*sizeof(lista_Desplegable));
    fseek(fac,0,SEEK_SET);
    fread(&f_dato,sizeof(factura),1,fac);
    while (!feof(fac))
    {
        if ((strcmp(cp.id_empresa,f_dato.id_empresa)==0 || (f.numero_sucursal==f_dato.numero_sucursal && f.numero_factura==f_dato.numero_factura)) && f_dato.estado==0)
        {
            sprintf(registro[rango++].registro,"%04d %02d/%02d/%04d %05d-%08d $%13.2f",j+1,f_dato.emision_dia,f_dato.emision_mes,f_dato.emision_anio,f_dato.numero_sucursal,f_dato.numero_factura,f_dato.total);
            flag=1;
        }
        fread(&f_dato,sizeof(factura),1,fac);
        j++;
    }
    if (flag==1)
    {
        posicion=listaDesplegable(registro,3,rango);
        for (k=0;k<4;k++)
            dato[k]=registro[posicion].registro[k];
        ubicacion=atoi(dato);
        fseek(fac,(ubicacion-1)*sizeof(factura),SEEK_SET);
        printf("\n");
        fread(&f_dato,sizeof(factura),1,fac);
    }
    else
        strcpy(f_dato.id_factura,"");
    linea(Largo);
    free(registro);
    return f_dato;
}

/*Función:  leerFactura(pasar por referencia archivo facturas, pasar por referencia archivo talonario, pasar por valor
            estructura cliente_proveedor y la estructura fecha con la fecha desde-hasta y la opción del menú anterior)
  Acción    Imprime por pantalla el listado de facturas correspondientes al criterio solicitado.*/

void  leerFactura(FILE *fac,FILE *tal,cliente_proveedor cp,factura fd,factura fh,int opcion)
{
    factura f;
    talonario t;
    int i;
    fseek(fac,0,SEEK_SET);
    if (fac!=NULL)
    {
        fread(&f,sizeof(factura),1,fac);
        printf("Fecha      C. L N%c Comprobante Subtotal      IVA 21%%       Total\n",167);
        linea(Largo);
        while (!feof(fac))
        {
            switch (opcion)
            {
                case 1: if (strcmp(cp.id_empresa,f.id_empresa)==0 && f.estado==0)
                        {
                            t=traerTalonario(tal,f);
                            printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",f.emision_dia,f.emision_mes,f.emision_anio,nombre_comprobante(t.codigo,'c'),letra_comprobante(t.codigo),f.numero_sucursal,f.numero_factura,f.subtotal,f.iva21,f.total);
                        }
                        break;
                case 2: if (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)>=0 && compararFechas(fh.emision_dia,fh.emision_mes,fh.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)<=0)
                        {
                            t=traerTalonario(tal,f);
                            printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",f.emision_dia,f.emision_mes,f.emision_anio,nombre_comprobante(t.codigo,'c'),letra_comprobante(t.codigo),f.numero_sucursal,f.numero_factura,f.subtotal,f.iva21,f.total);
                        }
                        break;
                case 3: t=traerTalonario(tal,f);
                        printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",f.emision_dia,f.emision_mes,f.emision_anio,nombre_comprobante(t.codigo,'c'),letra_comprobante(t.codigo),f.numero_sucursal,f.numero_factura,f.subtotal,f.iva21,f.total);
                        break;
            }
            fread(&f,sizeof(factura),1,fac);
        }
    }
    else
        noEncontrado;
}

/*Función:  leyendaModuloFacturas(pasar valor númerico del dato 1: emisión)
  Acción:   Imprime en pantalla la leyenda cuyo valor fue pedido.*/

void leyendaModuloFacturas(int leyenda)
{
    linea(Largo);
    Color(WHITE,RED);
    switch (leyenda)
    {
        case 1: printf("MODULO FACTURACION  <<< EMISION COMPROBANTES >>>                                \n");
                break;
        case 2: printf("MODULO FACTURACION  <<< IMPUTAR COMPROBANTES >>>                                \n");
                break;
        case 3: printf("MODULO FACTURACION  <<< LISTADO COMPROBANTES >>>                                \n");
                break;
        case 4: printf("MODULO FACTURACION  <<< MOSTRAR COMPROBANTES >>>                                \n");
                break;
        case 5: printf("MODULO FACTURACION  <<< PAGAR COMPROBANTES >>>\n");
                break;
    }
    Color(BLACK,WHITE);
    linea(Largo);
}

/*Función:  generarComprobante(pasar el vector con los datos del los archivos, se para por valor los registros
            cliente_proveedor, factura, talonario y la lista de items)
  Acción:   Genera el archivo del comprobante emitido y usa como nombre los el nombre corto del comprobante, la letra
            de la factura, el número de sucursal y el de factura.*/

void generarComprobante(archivo a[],cliente_proveedor cp,factura f,talonario t,nodo *l_item)
{
    cliente_proveedor m;
    m=empresaMaestra(a);
    FILE *com;
    char comprobante[MAX_PATH],ruta[MAX_PATH],*token=NULL;
    int cant_articulos=1,i,flag=0;
    strcpy(ruta,a[1].fichero);
    strcpy(comprobante,"");
    token=strtok(ruta,"\\");
    while(token!=NULL && flag==0)
    {
        if (strcmp(token,"DATOS")!=0)
        {
            strcat(comprobante,token);
            strcat(comprobante,"\\");
        }
        else
            flag=1;
        token=strtok(NULL,"\\");
    }
    strcat(comprobante,"COMPROBANTES\\");
    strcat(comprobante,nombre_comprobante(t.codigo,'c'));
    strcat(comprobante,letra_comprobante(t.codigo));
    strcat(comprobante,"-");
    strcat(comprobante,completarCeros(f.numero_sucursal,5));
    strcat(comprobante,"-");
    strcat(comprobante,completarCeros(f.numero_factura,8));
    strcat(comprobante,".txt");
    com=fopen(comprobante,"w+");
    if (com!=NULL)
    {
        fprintf(com,"┌───────────────────────────────────┬──────────────────────────────────────────┐\n");
        fprintf(com,"│  %-30.30s  ┌┴┐            %-16s             │\n",m.nombre_empresa,nombre_comprobante(t.codigo,'l'));
        fprintf(com,"│  %-20.20s            │%s│            N° %05d-%08d            │\n",m.direccion,letra_comprobante(t.codigo),f.numero_sucursal,f.numero_factura);
        fprintf(com,"│  %-20.20s CP(%04d)   └┬┘                  ┌──────────┐           │\n",m.localidad,m.cpost);
        fprintf(com,"│  %-20.20s          Cod. %02d          FECHA │%02d/%02d/%04d│           │\n",m.tel,t.codigo,f.emision_dia,f.emision_mes,f.emision_anio);
        fprintf(com,"│                                   │                   └──────────┘           │\n",m.tel);
        fprintf(com,"│                                   │             CUIT: %-11.11s            │\n",m.documento);
        fprintf(com,"│                                   │             IIBB: Convenio Multilateral  │\n");
        fprintf(com,"│  %-21.21s            │             Fecha Incio Act.: 01-01-1999 │\n",tipoIVA(m.cond_iva));
        fprintf(com,"└───────────────────────────────────┴──────────────────────────────────────────┘\n");
        fprintf(com,"┌──────────────────────────────────────────────────────────────────────────────┐\n");
        fprintf(com,"│ Razón Social: %-30s             Cliente N°: %07d │\n",cp.nombre_empresa,cp.num_cliente);
        fprintf(com,"│ Dirección: %-20s              %20s - CP(%04d) │\n",cp.direccion,cp.localidad,cp.cpost);
        fprintf(com,"│ Tipo IVA: %-21s                 %15s: %11s │\n",tipoIVA(cp.cond_iva),nombreDocumento(cp.tipo_documento),cp.documento);
        fprintf(com,"└──────────────────────────────────────────────────────────────────────────────┘\n");
        fprintf(com,"┌───────────┬────────┬──────────────────────────────┬────────────┬─────────────┐\n");
        fprintf(com,"│   Código  │Cantidad│         Descripción          │  Unitario  │    Total    │\n");
        fprintf(com,"├───────────┼────────┼──────────────────────────────┼────────────┼─────────────┤\n");
        while (l_item)
        {
            fprintf(com,"│ %-10.10s│%8.8d│%-30.30s│$%11.2f│$%11.2f │\n",l_item->reg.nombre,l_item->reg.cantidad,l_item->reg.descripcion,l_item->reg.unitario,l_item->reg.cantidad*l_item->reg.unitario);
            l_item=l_item->sig;
            cant_articulos++;
        }
        for (i=cant_articulos;i<=15;i++)
            fprintf(com,"│           │        │                              │            │             │\n");
        fprintf(com,"├───────────┴────────┼───────────────────┬──────────┴──────┬─────┴─────────────┤\n");
        if (t.codigo==1 || t.codigo==2 || t.codigo==3)
            fprintf(com,"│      SubTotal      │   Neto sin IVA    │     IVA 21%%     │       Total       │\n");
        else
            fprintf(com,"│      SubTotal      │                   │                 │       Total       │\n");
        fprintf(com,"├────────────────────┼───────────────────┼─────────────────┼───────────────────┤\n");
        if (t.codigo==1 || t.codigo==2 || t.codigo==3)
            fprintf(com,"│     $%13.2f │    $%13.2f │  $%13.2f │    $%13.2f │\n",f.subtotal,f.subtotal,f.iva21,f.total);
        else
            fprintf(com,"│     $%13.2f │                   │                 │    $%13.2f │\n",f.subtotal,f.subtotal,f.iva21,f.total);
        fprintf(com,"├────────────────────┴───────────────────┴─────────────────┴───────────────────┤\n");
        fprintf(com,"│ CAI: %-15s                                 Vencimiento: %02d/%02d/%04d │\n",t.cai,t.venc_dia,t.venc_mes,t.venc_anio);
        fprintf(com,"└──────────────────────────────────────────────────────────────────────────────┘\n");
        fprintf(com,"                                 O R I G I N A L                                \n");
    }
    else
        archivoInvalido();
    fclose(com);
}

/*Función:  mostrarComprobante(pasar por valor los registros cliente_proveedor, factura, talonario y lista items)
  Acción:   Muestra por pantalla los datos del comprobante.*/

void mostrarComprobante(archivo a[],cliente_proveedor cp,factura f,talonario t,nodo *l_item,int item)
{
    cliente_proveedor m;
    m=empresaMaestra(a);
    leyendaModuloFacturas(item);
    printf("   %-30.30s               %-16s              \n",m.nombre_empresa,nombre_comprobante(t.codigo,'l'));
    printf("                                    %s           N%c %05d-%08d\n",letra_comprobante(t.codigo),167,f.numero_sucursal,f.numero_factura);
    printf("   %-22.22s        Cod. %02d        FECHA  %02d/%02d/%04d\n",tipoIVA(m.cond_iva),t.codigo,f.emision_dia,f.emision_mes,f.emision_anio);
    linea(Largo);
    printf("  Raz%cn Social: %-30.30s             Cliente N%c: %07d\n",162,cp.nombre_empresa,167,cp.num_cliente);
    printf("  Direcci%cn: %-20.20s              %20.20s - CP(%04d)\n",162,cp.direccion,cp.localidad,cp.cpost);
    printf("  Tipo IVA: %-21.21s                 %15.15s: %11s\n",tipoIVA(cp.cond_iva),nombreDocumento(cp.tipo_documento),cp.documento);
    linea(Largo);
    printf("    C%cdigo   Cantidad          Descripci%cn             Unitario       Total     \n",162,162);
    while (l_item)
    {
        printf("  %-10.10s %8d %-30.30s $%11.2f $%11.2f  \n",l_item->reg.nombre,l_item->reg.cantidad,l_item->reg.descripcion,l_item->reg.unitario,l_item->reg.cantidad*l_item->reg.unitario);
        l_item=l_item->sig;
    }
    linea(Largo);
    if (t.codigo==1)
        printf("       SubTotal          Neto sin IVA          IVA 21%%             Total\n");
    else
        printf("       SubTotal                                                    Total\n");
    linea(Largo);
    if (t.codigo==1 || t.codigo==2 || t.codigo==3)
        printf("      $%13.2f      $%13.2f    $%13.2f      $%13.2f\n",f.subtotal,f.subtotal,f.iva21,f.total);
    else
        printf("      $%13.2f                                          $%13.2f\n",f.subtotal,f.subtotal,f.iva21,f.total);
    linea(Largo);
    printf("  CAI: %-15s                                 Vencimiento: %02d/%02d/%04d\n",t.cai,t.venc_dia,t.venc_mes,t.venc_anio);
    linea(Largo);
    printf("                                 O R I G I N A L\n");
}

/*Función:  obtenerItems(pasar por referencia el archivo, pasar por valor la estructura de factura, pasar por valor
            el id_factura, pasar por referencia la lista de ítems, pasar por valor el número de opción)
  Acción:   Devuelve la lista de ítems de la factura original para traspasarlos a la lista según la opción pedida.*/

void obtenerItems(FILE *ite,factura c,char id_factura[],nodo** lista,int opcion)
{
    items i;
    fseek(ite,0,SEEK_SET);
    fread(&i,sizeof(items),1,ite);
    while (!feof(ite))
    {
        if (strcmp(id_factura,i.id_factura)==0)
        {
            if (opcion==1)
            {
                strcpy(i.id_items,obtenerId());
                strcpy(i.id_factura,c.id_factura);
            }
            insertar(&(*lista),i);
        }
        fread(&i,sizeof(items),1,ite);
    }
}

/*Función:  obtenerStock(pasar por referencia el archivo stock, pasar por referencia una lista de items, pasar por
            valor el id_factura, pasar por referencia el subtotal como valor float)
  Acción:   Muestra una cuadro de lista donde elegimos los productos, nos movemos con <arriba>, <abajo> y <enter>,
            una vez que los seleccionamos, cargamos la cantidad de unidades que nos piden comprar, al final tenemos
            tres opciones, cancelar la opearación, agregar el producto a la lista o finalizar la operación para
            regresar.*/

void obtenerStock(FILE *sto,nodo** item,char idFactura[15],float *subtotal,int cli_pro)
{
    menuopciones sm[]={"","CANCELA","AGREGA","TERMINA"};
    int j=0,cant_productos,opcion,cant_articulos=1,menu,rango=sizeof(sm)/27,falta_stock=1;
    lista_Desplegable *registro;
    registro=(lista_Desplegable*)malloc(RANGO*sizeof(lista_Desplegable));
    stock s;
    items i;
    cls();
    fseek(sto,0,SEEK_END);
    cant_productos=ftell(sto)/sizeof(stock);
    fseek(sto,0,SEEK_SET);
    fread(&s,sizeof(stock),1,sto);
    while (!feof(sto))
    {
        sprintf(registro[j].registro,"%04d %-25.25s %-30.30s",s.id_stock,s.nombre,s.descripcion);
        fread(&s,sizeof(stock),1,sto);
        j++;
    }
    do
    {
        do
        {
            fseek(sto,listaDesplegable(registro,2,cant_productos)*sizeof(stock),SEEK_SET);
            printf("\n");
            fread(&s,sizeof(stock),1,sto);
            if (s.eliminado==0)
            {
                strcpy(i.id_factura,idFactura);
                strcpy(i.id_items,obtenerId());
                linea(Largo);
                printf("%03d %-20.20s %-30.30s $%12.2f\n",s.id_stock,s.nombre,s.descripcion,s.precio_venta);
                linea(Largo);
                do
                {
                    printf("CANTIDAD => ");
                    scanf("%d",&i.cantidad);
                } while (verifica_numero_rango(i.cantidad,1,99999999)==0);
                if (i.cantidad>s.cantidad && cli_pro==1)
                {
                    linea(Largo);
                    Color(RED,WHITE);
                    printf("STOCK INSUFICIENTE                                                              \n");
                    Color(BLACK,WHITE);
                    linea(Largo);
                    pausa();
                }
            }
            else
            {
                linea(Largo);
                Color(RED,WHITE);
                printf("PRODUCTO DISCONTINUO                                                            \n");
                Color(BLACK,WHITE);
                linea(Largo);
                pausa();
            }
            linea(Largo);
        } while (i.cantidad>s.cantidad && cli_pro==1);
        if (cli_pro==1)
        {
            s.cantidad=s.cantidad-i.cantidad;
            i.unitario=s.precio_venta;
        }
        else
        {
            s.cantidad=s.cantidad+i.cantidad;
            i.unitario=s.precio_compra;
        }
        strcpy(i.nombre,s.nombre);
        strcpy(i.descripcion,s.descripcion);
        i.id_stock=s.id_stock;
        *subtotal=*subtotal+(i.cantidad*s.precio_venta);
        gotoxy(0,17);
        linea(Largo);
        do
        {
            submenu(&menu,sm,rango,18);
            switch (menu)
            {
                case 1: opcion=1;
                        break;
                case 2: insertar(&(*item),i);
                        opcion=1;
                        break;
                case 3: insertar(&(*item),i);
                        opcion=2;
                        break;
            }
        } while (menu==0);
        cant_articulos++;
        if (cant_articulos-1==10)
        {
            linea(Largo);
            printf("ALCANZO EL LIMITE DE 10 ARTICULOS\n");
            linea(Largo);
        }
    } while (opcion==1 && cant_articulos<=10);
    free(registro);
}

/*Función:  obeteneraTalonario(pasar por referencia el archivo talonario, pasar por valor la condición de IVA del
            cliente proveedor)
  Acción:   Retorna el talonario elegido de un cuadro de lista desplegable, el cual seleccionamos con las teclas <arriba>,
            <abajo> y <enter>.*/

talonario obtenerTalonario(FILE *tal,int cond_iva)
{
    int j=0,codigo_comprobante=0,flag=0,rango=0,k,posicion,ubicacion;
    char dato[4];
    talonario t;
    lista_Desplegable *registro;
    registro=(lista_Desplegable*)malloc(RANGO*sizeof(lista_Desplegable));
    codigo_comprobante=obtenerCodigoComprobante(cond_iva);
    fseek(tal,0,SEEK_SET);
    fread(&t,sizeof(talonario),1,tal);
    while (!feof(tal))
    {
        if (t.codigo==codigo_comprobante && t.estado==1)
        {
            sprintf(registro[rango].registro,"%04d %-2s [%s]/%05d-%08d %04d ",j+1,nombre_comprobante(t.codigo,'s'),letra_comprobante(t.codigo),t.sucursal,t.pag_actual,t.estado);
            strcat(registro[rango].registro,nombre_comprobante(t.codigo,'l'));
            flag=1;
            rango++;
        }
        fread(&t,sizeof(talonario),1,tal);
        j++;
    }
    if (flag==1)
    {
        posicion=listaDesplegable(registro,1,rango);
        for (k=0;k<4;k++)
            dato[k]=registro[posicion].registro[k];
        ubicacion=atoi(dato);
        fseek(tal,(ubicacion-1)*sizeof(talonario),SEEK_SET);
        printf("\n");
        fread(&t,sizeof(talonario),1,tal);
    }
    else
        strcpy(t.id_talonario,"");
    linea(Largo);
    free(registro);
    return t;
}

talonario obtenerFToNCPorSucursal(FILE *tal,factura f,int codigo)
{
    talonario t;
    fseek(tal,0,SEEK_SET);
    fread(&t,sizeof(talonario),1,tal);
    while (!feof(tal))
    {
        if (f.numero_sucursal==t.sucursal && t.codigo==codigo && t.estado==1)
            return t;
        fread(&t,sizeof(talonario),1,tal);
    }
    strcpy(t.id_talonario,"");
    return t;
}

/*Función:  mostrarTalonario(pasar los datos guardados en la estructura talonario, pasar la posicion vertical 'y')
  Devuelve: Imprime por pantalla los datos básicos del talonario seleccionado*/

void mostrarTalonario(talonario t,int y)
{
    gotoxy(0,y);
    Color(WHITE,BLACK);
    printf("                                                                                \n");
    printf("                                                                                \n");
    gotoxy(0,y);
    printf("Comprobante:  %-15s ",nombre_comprobante(t.codigo,'l'));
    printf("[%s] ",letra_comprobante(t.codigo));
    printf("N%c%05d-%08d\n",167,t.sucursal,t.pag_actual);
    printf("CAI: %s - Vencimiento: %02d/%02d/%04d\n",t.cai,t.venc_dia,t.venc_mes,t.venc_anio);
    Color(BLACK,WHITE);
    linea(Largo);
}
