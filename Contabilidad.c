void ganancias(archivo a[],int leyenda)
{
    factura f,fd,fh,ftotal;
    talonario t;
    compra c,ctotal;
    FILE *fac,*tal,*com;
    fac=fopen(a[2].fichero,"rb");
    tal=fopen(a[3].fichero,"rb");
    com=fopen(a[6].fichero,"rb");
    cls();
    ftotal.subtotal=0;
    ctotal.subtotal=0;
    leyendaModuloContable(leyenda);
    do
    {
        cargarFecha(&fd.emision_dia,&fd.emision_mes,&fd.emision_anio,28,2,"DESDE");
        cargarFecha(&fh.emision_dia,&fh.emision_mes,&fh.emision_anio,28,3,"HASTA");
    } while (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,fh.emision_dia,fh.emision_mes,fh.emision_anio)==-1);
    linea(Largo);
    fread(&f,sizeof(factura),1,fac);
    while (!feof(fac))
    {
        if (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)>=0 && compararFechas(fh.emision_dia,fh.emision_mes,fh.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)<=0)
        {
            t=traerTalonario(tal,f);
            if (t.codigo==3 || t.codigo==8 || t.codigo==13)
                ftotal.subtotal=ftotal.subtotal-f.subtotal;
            else
                ftotal.subtotal=ftotal.subtotal+f.subtotal;
        }
        fread(&f,sizeof(factura),1,fac);
    }
    fread(&c,sizeof(compra),1,com);
    while (!feof(com))
    {
        if (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,c.emision_dia,c.emision_mes,c.emision_anio)>=0 && compararFechas(fh.emision_dia,fh.emision_mes,fh.emision_anio,c.emision_dia,c.emision_mes,c.emision_anio)<=0)
        {
            if (c.codigo_talonario==3 || c.codigo_talonario==8 || c.codigo_talonario==13)
                ftotal.subtotal=ftotal.subtotal+c.subtotal;
            else
                ftotal.subtotal=ftotal.subtotal-c.subtotal;
        }
        fread(&c,sizeof(compra),1,com);
    }
    linea(Largo);
    Color(BLACK,LGREY);
    printf("Ganancias: %13.2f\n",ftotal.subtotal);
    Color(BLACK,WHITE);
    linea(Largo);
    completo();
    pausa();
    fclose(fac);
    fclose(com);
}

void ivaVentas(archivo a[],int leyenda)
{
    cliente_proveedor cp;
    factura f,fd,fh,ftotal;
    talonario t;
    FILE *c_p,*fac,*tal;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"rb");
    tal=fopen(a[3].fichero,"rb");
    cls();
    ftotal.subtotal=0;
    ftotal.iva21=0;
    ftotal.total=0;
    leyendaModuloContable(leyenda);
    do
    {
        cargarFecha(&fd.emision_dia,&fd.emision_mes,&fd.emision_anio,28,2,"DESDE");
        cargarFecha(&fh.emision_dia,&fh.emision_mes,&fh.emision_anio,28,3,"HASTA");
    } while (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,fh.emision_dia,fh.emision_mes,fh.emision_anio)==-1);
    linea(Largo);
    Color(BLACK,LGREY);
    printf("Emisi%cn  C. Suc. Factura   IVA Documento   Raz%cn Social         Neto sin IVA  \n   IVA 21%%       Total con IVA\n",162,162);
    Color(BLACK,WHITE);
    linea(Largo);
    fread(&f,sizeof(factura),1,fac);
    while (!feof(fac))
    {
        if (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)>=0 && compararFechas(fh.emision_dia,fh.emision_mes,fh.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)<=0)
        {
            t=traerTalonario(tal,f);
            cp=obtenerClienteId(c_p,f);
            printf("\n%04d%02d%02d %02d %05d %08d %02d  %-11s %-20.20s %013.0f\n   %013.0f %013.0f\n",f.emision_anio,f.emision_dia,f.emision_mes,t.codigo,f.numero_sucursal,f.numero_factura,cp.tipo_documento,cp.documento,cp.nombre_empresa,f.subtotal*100,f.iva21*100,f.total*100);
            if (t.codigo==3 || t.codigo==8 || t.codigo==13)
                ftotal.subtotal=ftotal.subtotal-f.subtotal;
            else
                ftotal.subtotal=ftotal.subtotal+f.subtotal;

        }
        fread(&f,sizeof(factura),1,fac);
    }
    linea(Largo);
    Color(BLACK,LGREY);
    printf("Subtotal sin IVA: %13.2f\n",ftotal.subtotal);
    printf("IVA 21%%:          %13.2f\n",ftotal.subtotal*0.21);
    printf("Total con IVA:    %13.2f\n",ftotal.subtotal*1.21);
    Color(BLACK,WHITE);
    linea(Largo);
    completo();
    pausa();
    fclose(c_p);
    fclose(fac);
    fclose(tal);
}

void ivaCompras(archivo a[],int leyenda)
{
    cliente_proveedor cp;
    compra f,fd,fh,ftotal;
    FILE *c_p,*com;
    c_p=fopen(a[1].fichero,"rb");
    com=fopen(a[6].fichero,"rb");
    cls();
    ftotal.subtotal=0;
    ftotal.iva21=0;
    ftotal.total=0;
    leyendaModuloContable(leyenda);
    do
    {
        cargarFecha(&fd.emision_dia,&fd.emision_mes,&fd.emision_anio,28,2,"DESDE");
        cargarFecha(&fh.emision_dia,&fh.emision_mes,&fh.emision_anio,28,3,"HASTA");
    } while (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,fh.emision_dia,fh.emision_mes,fh.emision_anio)==-1);
    linea(Largo);
    Color(BLACK,LGREY);
    printf("Emisi%cn  C. Suc. Factura   IVA Documento   Raz%cn Social         Neto sin IVA  \n   IVA 21%%       Total con IVA\n",162,162);
    Color(BLACK,WHITE);
    linea(Largo);
    fread(&f,sizeof(compra),1,com);
    while (!feof(com))
    {
        if (compararFechas(fd.emision_dia,fd.emision_mes,fd.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)>=0 && compararFechas(fh.emision_dia,fh.emision_mes,fh.emision_anio,f.emision_dia,f.emision_mes,f.emision_anio)<=0)
        {
            cp=obtenerProveedorId(c_p,f);
            printf("\n%04d%02d%02d %02d %05d %08d %02d  %-11s %-20.20s %013.0f\n   %013.0f %013.0f\n",f.emision_anio,f.emision_dia,f.emision_mes,f.codigo_talonario,f.numero_sucursal,f.numero_factura,cp.tipo_documento,cp.documento,cp.nombre_empresa,f.subtotal*100,f.iva21*100,f.total*100);
            if (f.codigo_talonario==3 || f.codigo_talonario==8 || f.codigo_talonario==13)
                ftotal.subtotal=ftotal.subtotal-f.subtotal;
            else
                ftotal.subtotal=ftotal.subtotal+f.subtotal;
        }
        fread(&f,sizeof(compra),1,com);
    }
    linea(Largo);
    Color(BLACK,LGREY);
    printf("Subtotal sin IVA: %13.2f\n",ftotal.subtotal);
    printf("IVA 21%%:          %13.2f\n",ftotal.subtotal*0.21);
    printf("Total con IVA:    %13.2f\n",ftotal.subtotal*1.21);
    Color(BLACK,WHITE);
    linea(Largo);
    completo();
    pausa();
    fclose(c_p);
    fclose(com);
}

/*Función:  leyendaModuloTalonarios(pasar por valor el código del tipo de acción a realizar)
  Acción:   Muestra por pantalla el encabezado del ítem del menú a ejecutar.*/

void leyendaModuloContable(int leyenda)
{
    linea(Largo);
    Color(WHITE,RED);
    switch (leyenda)
    {
        case 1: printf("MODULO CONTABLE  <<< GANANCIAS >>>                                              \n");
                break;
        case 2: printf("MODULO CONTABLE  <<< SUBDIARIO IVA VENTAS >>>                                   \n");
                break;
        case 3: printf("MODULO CONTABLE  <<< SUBDIARIO IVA COMPRAS >>>                                  \n");
                break;
    }
    Color(BLACK,WHITE);
    linea(Largo);
}

/*Función:  obtenerProveedorId(pasar por referencia el archivo, pasar por valor la estructura compra)
  Acción:   Retorna los datos del proveedor por el iD de la factura.*/

cliente_proveedor obtenerProveedorId(FILE *c_p,compra f)
{
    cliente_proveedor cp;
    fseek(c_p,0,SEEK_SET);
    fread(&cp,sizeof(cliente_proveedor),1,c_p);
    while (!feof(c_p))
    {
        if (strcmp(cp.id_empresa,f.id_empresa)==0)
            return cp;
        fread(&cp,sizeof(cliente_proveedor),1,c_p);
    }
    strcpy(cp.id_empresa,"");
    return cp;
}
