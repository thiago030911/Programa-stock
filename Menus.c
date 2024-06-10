void cli_pro(archivo a[])
{
    mo m[]={"    CLIENTE/PROVEEDOR    ","ALTA                     ","BAJA                     ","MODIFICACION             ","LISTAR                   ","MOSTAR CLIENTE           ","MOSTRAR PROVEEDOR        ","CUENTA CORRIENTE         ","REGRESAR A MENU PRINCIPAL"};
    int opcion,rango=sizeof(m)/27;
    do
    {
        menu(&opcion,m,rango);
        switch (opcion)
        {
            case 1: altaCP(a,opcion);
                    break;
            case 2: bajaCP(a,opcion);
                    break;
            case 3: modificarCP(a,opcion);
                    break;
            case 4: listarCP(a,opcion);
                    break;
            case 5: mostrarCliente(a,opcion);
                    break;
            case 6: mostrarProveedor(a,opcion);
                    break;
            case 7: mostrarCuentaCorriente(a,opcion);
                    break;
        }
    } while (opcion!=0);
}

void con(archivo a[])
{
    mo m[]={"      CONTABILIDAD       ","GANANCIAS                ","SUBDIARIO IVA VENTAS     ","SUBIDARIO IVA COMPRAS    ","REGRESAR A MENU PRINCIPAL"};
    int opcion,rango=sizeof(m)/27;
    do
    {
        menu(&opcion,m,rango);
        switch (opcion)
        {
            case 1: ganancias(a,opcion);
                    break;
            case 2: ivaVentas(a,opcion);
                    break;
            case 3: ivaCompras(a,opcion);
                    break;
        }
    } while (opcion!=0);
}

void fac(archivo a[])
{
    mo m[]={"       FACTURACION       ","EMITIR COMPROBANTE       ","ANULAR/IMPUTAR CON NC    ","LISTAR COMPROBANTES      ","VER COMPROBANTE          ","PAGAR COMPROBANTE        ","REGRESAR A MENU PRINCIPAL"};
    int opcion,rango=sizeof(m)/27;
    do
    {
        menu(&opcion,m,rango);
        switch (opcion)
        {
            case 1: emitirComprobante(a,opcion);
                    break;
            case 2: anularComprobante(a,opcion);
                    break;
            case 3: listarComprobante(a,opcion);
                    break;
            case 4: verComprobante(a,opcion);
                    break;
            case 5: pagarComprobante(a,opcion);
                    break;
        }
    } while (opcion!=0);
}

void sto(archivo a[])
{
    mo m[]={"          STOCK          ","ALTA                     ","BAJA                     ","MODIFICACION             ","LISTAR                   ","BUSCAR                   ","FALTANTE                 ","REGRESAR A MENU PRINCIPAL"};
    int opcion,rango=sizeof(m)/27;
    do
    {
        menu(&opcion,m,rango);
        switch (opcion)
        {
            case 1: altaStock(a,opcion);
                    break;
            case 2: bajaStock(a,opcion);
                    break;
            case 3: modificaStock(a,opcion);
                    break;
            case 4: listarStock(a,opcion);
                    break;
            case 5: buscarStock(a,opcion);
                    break;
            case 6: faltante(a,opcion);
                    break;
        }
    } while (opcion!=0);
}

void tal(archivo a[])
{
    mo m[]={"        TALONARIO        ","ALTA                     ","BAJA                     ","MODIFICACION             ","LISTAR                   ","REGRESAR A MENU PRINCIPAL"};
    int opcion,rango=sizeof(m)/27;
    do
    {
        menu(&opcion,m,rango);
        switch (opcion)
        {
            case 1: alta_talonario(a,opcion);
                    break;
            case 2: baja_talonario(a,opcion);
                    pausa();
                    break;
            case 3: modifica_talonario(a,opcion);
                    break;
            case 4: lista_talonario(a,opcion);
                    pausa();
                    break;
        }
    } while (opcion!=0);
}

void com(archivo a[])
{
    mo m[]={"         COMPRAS         ","CARGAR                   ","LISTAR                   ","REGRESAR A MENU PRINCIPAL"};
    int opcion,rango=sizeof(m)/27;
    do
    {
        menu(&opcion,m,rango);
        switch (opcion)
        {
            case 1: cargarCompra(a,opcion);
                    break;
            case 2: listarCompra(a,opcion);
                    break;
        }
    } while (opcion!=0);
}
