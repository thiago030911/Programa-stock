/*Función:  cargarCompra(pasar por valor la estructura con los nombres de los archivos, pasar por valor la
            ubicacion dentro del menú facturacion)
  Acción:   Carga un comprobante A/B/C de factura o nota de débito, incrementando la cantidad de productos
            en el stock y generando añadiendo a ítems la cantidad de productos adquiridos.*/

void cargarCompra(archivo a[],int opcion)
{
    cliente_proveedor cp;
    compra c;
    items i;
    nodo *item;
    item=NULL;
    float subtotal=0;
    time_t date;
    struct tm *tm;
    date=time(NULL);
    tm=localtime(&date);
    menuopciones sm[]={"","GUARDAR","CANCELAR"};
    menuopciones sm0[]={"FECHA  =>","ACTUAL","MANUAL"};
    menuopciones sm1[]={"","FV-A","ND-A","FV-B","ND-B"};
    menuopciones sm2[]={"","FV-C","ND-C"};
    menuopciones sm3[]={"","BUSCAR","ELEGIR"};
    int menu,flag=0,rango=sizeof(sm)/27,rango0=sizeof(sm0)/27,rango1=sizeof(sm1)/27,rango2=sizeof(sm2)/27,rango3=sizeof(sm3)/27;
    FILE *c_p,*ite,*sto,*com;
    c_p=fopen(a[1].fichero,"rb");
    ite=fopen(a[4].fichero,"ab");
    sto=fopen(a[5].fichero,"r+b");
    com=fopen(a[6].fichero,"ab");
    cls();
    leyendaModuloCompras(opcion);
    printf("SELECCIONAR PROVEEDOR\n");
    linea(Largo);
    do
    {
        submenu(&menu,sm3,rango3,5);
        switch (menu)
        {
            case 1: cp=obtenerClienteProveedor(c_p);
                    break;
            case 2: cp=encontrarClienteProveedor(c_p,2);
                    break;
        }
    } while (menu==0);
    if (strcmp(cp.id_empresa,"")!=0)
    {
        if (strcmp(cp.id_empresa,"?")!=0)
        {
            if (strcmp(cp.tipo_cliente,"P")==0)
            {
                cls();
                leyendaModuloCompras(opcion);
                mostrarCP(cp,3,0);
                strcpy(c.id_factura,obtenerId());
                strcpy(c.id_empresa,cp.id_empresa);
                c.num_cliente=cp.num_cliente;
                gotoxy(0,10);
                linea(Largo);
                do
                {
                    submenu(&menu,sm0,rango0,11);
                    switch (menu)
                    {
                        case 1: gotoxy(0,13);
                                printf("Fecha del Comprobante [DD/MM/AAAA] => %02d/%02d/%04d\n",tm->tm_mday,tm->tm_mon+1,1900+tm->tm_year);
                                c.emision_dia=tm->tm_mday;
                                c.emision_mes=tm->tm_mon+1;
                                c.emision_anio=1900+tm->tm_year;
                                break;
                        case 2: cargarFecha(&c.emision_dia,&c.emision_mes,&c.emision_anio,38,12,"del Comprobante");
                                break;
                    }
                } while (menu==0);
                linea(Largo);
                if (cp.tipo_documento==1)
                {
                    do
                    {
                        submenu(&menu,sm1,rango1,15);
                        switch (menu)
                        {
                            case 0: c.codigo_talonario=0;
                                    break;
                            case 1: c.codigo_talonario=1;
                                    break;
                            case 2: c.codigo_talonario=2;
                                    break;
                            case 3: c.codigo_talonario=6;
                                    break;
                            case 4: c.codigo_talonario=7;
                                    break;
                        }
                    } while (menu!=0 && (menu<1 || menu>4));
                }
                else
                {
                    do
                    {
                        submenu(&menu,sm2,rango2,15);
                        switch (menu)
                        {
                            case 0: c.codigo_talonario=0;
                                    break;
                            case 1: c.codigo_talonario=11;
                                    break;
                            case 2: c.codigo_talonario=12;
                                    break;
                        }
                    } while (menu!=0 && (menu<1 || menu>2));
                }
                do
                {
                    gotoxy(0,17);
                    printf("N%c Sucursal: ",167);
                    gotoxy(13,17);
                    c.numero_sucursal=capturaCaracter(5);
                } while (c.numero_sucursal==0);
                do
                {
                    gotoxy(0,18);
                    printf("N%c Compra:   ",167);
                    gotoxy(13,18);
                    c.numero_factura=capturaCaracter(8);
                } while (c.numero_factura==0);
                obtenerStock(sto,&item,c.id_factura,&subtotal,2);
                pausa();
                cls();
                leyendaModuloCompras(opcion);
                linea(Largo);
                printf("PRODUCTOS COMPRADOS\n");
                linea(Largo);
                verLista(item);
                linea(Largo);
                c.subtotal=subtotal;
                c.iva21=c.subtotal*.21;
                c.total=c.subtotal*1.21;
                c.estado=0;
                pausa();
                cls();
                mostrarCompra(a,cp,c,item,opcion);
                gotoxy(0,28);
                linea(Largo);
                do
                {
                    submenu(&menu,sm,rango,29);
                    switch (menu)
                    {
                        case 1: fwrite(&c,sizeof(compra),1,com);
                                cargaModificacionStock(sto,item,1);
                                cargarArchivo(ite,item);
                                gotoxy(0,28);
                                archivoGeneraComprobante(29);
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
                printf("IMPOSIBLE, SOLO SE PUEDEN INGRESAR COMPRAS DE UN PROVEEDOR                      \n");
                Color(BLACK,WHITE);
                linea(Largo);
            }
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
    fclose(ite);
    fclose(sto);
    fclose(com);
}

/*Función:  listarCompra(pasar por referencia la estructura con el nombre de archivo, pasar por valor la opción del menú
            anterior)
  Acción:   Muestra la lista de los comproantes de compra según el criterio elegido.*/

void listarCompra(archivo a[],int opcion)
{
    cliente_proveedor cp;
    compra f,fd,fh;
    menuopciones sm[]={"<ESC>","CLIENTE","RANGO","TODOS"};
    menuopciones sm1[]={"","BUSCAR","LISTA"};
    int rango=sizeof(sm)/27,rango1=sizeof(sm1)/27,menu,menu1,cortar=1;
    FILE *c_p,*com;
    c_p=fopen(a[1].fichero,"rb");
    com=fopen(a[6].fichero,"rb");
    cls();
    leyendaModuloCompras(opcion);
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
                            case 2: cp=encontrarClienteProveedor(c_p,2);
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
        leyendaModuloCompras(opcion);
        if (cortar==2)
            printf("LISTA COMPROBANTES PROVEEDOR: %-25s   %s: %s\n",cp.nombre_empresa,nombreDocumento(cp.tipo_documento),cp.documento);
        else
            if (cortar!=3)
                printf("LISTA COMPRAS REALIZADAS DESDE %02d/%02d/%04d HASTA %02d/%02d/%04d\n",fd.emision_dia,fd.emision_mes,fd.emision_anio,fh.emision_dia,fh.emision_mes,fh.emision_anio);
        linea(Largo);
        leerCompra(com,cp,fd,fh,menu);
        linea(Largo);
        completo();
    }
    else
        cancela();
    pausa();
    fclose(c_p);
    fclose(com);
}

/*Función:  leerCompra(pasar por referencia archivo compras, pasar por valor estructura cliente_proveedor, pasar por valor
            la estructura compras con la fecha desde-hasta y la opción del menú anterior)
  Acción    Imprime por pantalla el listado de compras correspondientes al proveedor solicitado.*/

void  leerCompra(FILE *com,cliente_proveedor cp,compra fd,compra fh,int opcion)
{
    compra f;
    int i;
    fseek(com,0,SEEK_SET);
    if (com!=NULL)
    {
        fread(&f,sizeof(compra),1,com);
        printf("Fecha      C. L N%c Comprobante Subtotal      IVA 21%%       Total\n",167);
        linea(Largo);
        while (!feof(com))
        {
            switch (opcion)
            {
                case 1: if (strcmp(cp.id_empresa,f.id_empresa)==0 && f.estado==0)
                            printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",f.emision_dia,f.emision_mes,f.emision_anio,nombre_comprobante(f.codigo_talonario,'c'),letra_comprobante(f.codigo_talonario),f.numero_sucursal,f.numero_factura,f.subtotal,f.iva21,f.total);
                        break;
                case 2: if (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)>=0 && compararFechas(fh.emision_dia,fh.emision_mes,fh.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)<=0)
                            printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",f.emision_dia,f.emision_mes,f.emision_anio,nombre_comprobante(f.codigo_talonario,'c'),letra_comprobante(f.codigo_talonario),f.numero_sucursal,f.numero_factura,f.subtotal,f.iva21,f.total);
                        break;
                case 3: printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",f.emision_dia,f.emision_mes,f.emision_anio,nombre_comprobante(f.codigo_talonario,'c'),letra_comprobante(f.codigo_talonario),f.numero_sucursal,f.numero_factura,f.subtotal,f.iva21,f.total);
                        break;
            }
            fread(&f,sizeof(compra),1,com);
        }
    }
    else
        noEncontrado;
}

/*Función:  leyendaModuloCompras(pasar por valor el código del tipo de acción a realizar)
  Acción:   Muestra por pantalla el encabezado del ítem del menú a ejecutar.*/

void leyendaModuloCompras(int leyenda)
{
    linea(Largo);
    Color(WHITE,RED);
    switch (leyenda)
    {
        case 1: printf("MODULO COMPRAS   <<< CARGAR COMPROBANTES >>>                                    \n");
                break;
        case 2: printf("MODULO COMPRAS   <<< ANULAR COMPROBANTES >>>                                    \n");
                break;
        case 3: printf("MODULO COMPRAS   <<< LISTAR COMPROBANTES >>>                                    \n");
                break;
    }
    Color(BLACK,WHITE);
    linea(Largo);
}

/*Función:  mostrarComprobante(pasar por valor los registros cliente_proveedor, factura, talonario y lista items)
  Acción:   Muestra por pantalla los datos del comprobante.*/

void mostrarCompra(archivo a[],cliente_proveedor cp,compra f,nodo *l_item,int opcion)
{
    cliente_proveedor m;
    m=empresaMaestra(a);
    leyendaModuloCompras(opcion);
    printf("   %-30.30s               %-16.16s              \n",cp.nombre_empresa,nombre_comprobante(f.codigo_talonario,'l'));
    printf("                                    %s           N%c %05d-%08d\n",letra_comprobante(f.codigo_talonario),167,f.numero_sucursal,f.numero_factura);
    printf("   %-22.22s        Cod. %02d        FECHA  %02d/%02d/%04d\n",tipoIVA(cp.cond_iva),f.codigo_talonario,f.emision_dia,f.emision_mes,f.emision_anio);
    linea(Largo);
    printf("  Raz%cn Social: %-30.30s                                \n",162,m.nombre_empresa);
    printf("  Direcci%cn:    %-20.20s Localidad %-20.20s - CP(%04d)\n",162,m.direccion,m.localidad,m.cpost);
    printf("  Tipo IVA: %-22.22s             DNI: %-11s\n",tipoIVA(m.cond_iva),m.documento);
    linea(Largo);
    printf("    C%cdigo   Cantidad          Descripci%cn             Unitario       Total     \n",162,162);
    while (l_item)
    {
        printf("  %-10s %8d %-30s $%11.2f $%11.2f  \n",l_item->reg.nombre,l_item->reg.cantidad,l_item->reg.descripcion,l_item->reg.unitario,l_item->reg.cantidad*l_item->reg.unitario);
        l_item=l_item->sig;
    }
    linea(Largo);
    if (f.codigo_talonario==1)
        printf("       SubTotal          Neto sin IVA          IVA 21%%             Total\n");
    else
        printf("       SubTotal                                                    Total\n");
    linea(Largo);
    if (f.codigo_talonario==1 || f.codigo_talonario==2 || f.codigo_talonario==3)
        printf("      $%13.2f      $%13.2f    $%13.2f      $%13.2f\n",f.subtotal,f.subtotal,f.iva21,f.total);
    else
        printf("      $%13.2f                                          $%13.2f\n",f.subtotal,f.subtotal,f.iva21,f.total);
    linea(Largo);
    printf("                                 O R I G I N A L\n");
}
