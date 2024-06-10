/*Función:  altaCP(pasar por referencia la estructura con los nombres de archivo, pasar por valor la posición del menú)
  Acción:   Realiza el alta de un cliente/proveedor*/

void altaCP(archivo a[],int opcion)
{
    cliente_proveedor cp,cp_aux;
    FILE *c_p;
    menuopciones sm0[]={"","CLIENTE","PROVEEDOR"};
    menuopciones sm1[]={"TIPO DOC","CUIT","CUIL","DNI","OTRO"};
    menuopciones sm2[]={"TIPO IVA","RI","RM","CF","EX","SC"};
    menuopciones sm3[]={"","GUARDAR","CANCELAR"};
    int menu,rango0=sizeof(sm0)/27,rango1=sizeof(sm1)/27,rango2=sizeof(sm2)/27,rango3=sizeof(sm3)/27,posicion;
    char reactivar;
    c_p=fopen(a[1].fichero,"r+b");
    fseek(c_p,0,SEEK_END);
    posicion=ftell(c_p)/sizeof(cliente_proveedor);
    fseek(c_p,0,SEEK_SET);
    cls();
    leyendaModuloClienteProveedor(opcion);
    if (c_p!=NULL)
    {
        strcpy(cp.id_empresa,obtenerId());
        if (posicion!=0)
        {
            cp.num_cliente=posicion+1;
            do
            {
                submenu(&menu,sm0,rango0,3);
                switch (menu)
                {
                    case 1: strcpy(cp.tipo_cliente,"C");
                            break;
                    case 2: strcpy(cp.tipo_cliente,"P");
                            break;
                }
            } while (menu==0);
        }
        else
        {
            cp.num_cliente=posicion;
            strcpy(cp.tipo_cliente,"M");
            Color(WHITE,BLACK);
            printf("REGISTRO MAESTRO\n");
            Color(BLACK,WHITE);
            linea(Largo);
        }
        fflush(stdin);
        printf("Raz%cn Social:     ",162);
        gets(cp.nombre_empresa);
        fflush(stdin);
        printf("Nombre Comercial: ");
        gets(cp.nombre_fantasia);
        fflush(stdin);
        printf("Direcci%cn:        ",162);
        gets(cp.direccion);
        fflush(stdin);
        printf("Localidad:        ");
        gets(cp.localidad);
        do
        {
            gotoxy(0,9);
            printf("C%cdigo Postal:    ",162);
            gotoxy(18,9);
            cp.cpost=capturaCaracter(4);
        } while (cp.cpost==0);
        gotoxy(0,10);
        fflush(stdin);
        printf("Tel%cfono:         ",130);
        gets(cp.tel);
        linea(Largo);
        do
        {
            submenu(&menu,sm1,rango1,12);
            switch (menu)
            {
                case 1: cp.tipo_documento=80;
                        break;
                case 2: cp.tipo_documento=86;
                        break;
                case 3: cp.tipo_documento=96;
                        break;
                case 4: cp.tipo_documento=99;
                        break;
            }
        } while (menu==0);
        do
        {
            gotoxy(0,14);
            printf("                                                                                ");
            gotoxy(0,14);
            fflush(stdin);
            printf("Documento: ");
            gets(cp.documento);
        }
        while (verificaCUIT(cp.documento)==0);
        linea(Largo);
        do
        {
            submenu(&menu,sm2,rango2,16);
            switch (menu)
            {
                case 1: cp.cond_iva=1;
                        break;
                case 2: cp.cond_iva=2;
                        break;
                case 3: cp.cond_iva=3;
                        break;
                case 4: cp.cond_iva=4;
                        break;
                case 5: cp.cond_iva=5;
                        break;
            }
        } while (menu==0);
        fflush(stdin);
        printf("Contacto Apellido: ");
        gets(cp.contacto_apellido);
        fflush(stdin);
        printf("Contacto Nombre:   ");
        gets(cp.contacto_nombre);
        fflush(stdin);
        printf("Contacto Cargo:    ");
        gets(cp.contacto_cargo);
        do
        {
            gotoxy(0,21);
            printf("                                                                                ");
            gotoxy(0,21);
            fflush(stdin);
            printf("Documento:         ");
            gets(cp.contacto_dni);
        } while (verificaCUIT(cp.contacto_dni)==0);
        cp.eliminado=0;
        linea(Largo);
        do
        {
            submenu(&menu,sm3,rango3,23);
            switch (menu)
            {
                case 1: cp_aux=buscarClienteProveedor(c_p,cp);
                        if (strcmp(cp_aux.id_empresa,"?")==0)
                        {
                            fseek(c_p,0,SEEK_END);
                            fwrite(&cp,sizeof(cp),1,c_p);
                            completo();
                        }
                        else
                        {
                            registroDuplicado();
                            if (cp.eliminado==1)
                            {
                                do
                                {
                                    Color(GREEN,WHITE);
                                    gotoxy(0,28);
                                    printf("REGISTRO INACTIVO, desea REACTIVARLO <S>/<N> => ");
                                    scanf("%c",&reactivar);
                                    Color(BLACK,WHITE);
                                    gotoxy(0,29);
                                } while (reactivar!='s' || reactivar!='S' || reactivar!='n' || reactivar!='N');
                                linea(Largo);
                            }
                        }
                        break;
                case 2: cancela();
                        break;
            }
        } while (menu==0);
    }
    pausa();
    fclose(c_p);
}

/*Función:  bajaCP(pasar por referencia la estructura con los nombres de archivo, pasar por valor la posición del menú)
  Acción:   Realiza la baja de un cliente/proveedor*/

void bajaCP(archivo a[],int opcion)
{
    cliente_proveedor cp;
    FILE *c_p;
    menuopciones sm[]={"","BAJA","CANCELAR"};
    menuopciones sm1[]={"","BUSCAR","ELEGIR"};
    int menu,rango=sizeof(sm)/27,rango1=sizeof(sm1)/27;
    c_p=fopen(a[1].fichero,"r+b");
    cls();
    leyendaModuloClienteProveedor(opcion);
    if (c_p!=NULL)
    {
        gotoxy(0,4);
        linea(Largo);
        do
        {
            submenu(&menu,sm1,rango1,5);
            switch (menu)
            {
                case 1: cp=obtenerClienteProveedor(c_p);
                        break;
                case 2: cp=encontrarClienteProveedor(c_p,3);
                        break;
            }
        } while (menu==0);
        if (strcmp(cp.id_empresa,"")!=0)
        {
            if (strcmp(cp.id_empresa,"?")!=0)
            {
                gotoxy(0,8);
                linea(Largo);
                mostrarCP(cp,9,0);
                gotoxy(0,19);
                linea(Largo);
                do
                {
                    submenu(&menu,sm,rango,20);
                    switch (menu)
                    {
                        case 1: cp.eliminado=1;
                                fseek(c_p,ftell(c_p)-sizeof(cliente_proveedor),SEEK_SET);
                                fwrite(&cp,sizeof(cp),1,c_p);
                                fseek(c_p,0,SEEK_END);
                                completo();
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
            cancela();
    }
    pausa();
    fclose(c_p);
}

/*Función:  modificarsCP(pasar por referencia la estructura con los nombres de archivo, pasar por valor la posición del menú)
  Acción:   Realiza la modificación de un cliente/proveedor menos los datos fiscales.*/

void modificarCP(archivo a[],int opcion)
{
    cliente_proveedor cp;
    FILE *c_p;
    menuopciones sm[]={"","GUARDAR","CANCELAR"};
    int menu,rango=sizeof(sm)/27,edicion;
    c_p=fopen(a[1].fichero,"r+b");
    cls();
    leyendaModuloClienteProveedor(opcion);
    if (c_p!=NULL)
    {
        gotoxy(0,4);
        linea(Largo);
        cp=encontrarClienteProveedor(c_p,3);
        if (strcmp(cp.id_empresa,"?")!=0)
        {
            do
            {
                cls();
                leyendaModuloClienteProveedor(opcion);
                printf("    Id.: %-15s",cp.id_empresa);
                gotoxy(55,3);
                printf("N%c %-9s: %07d\n",167,PalabraClienteProveedor(cp.tipo_cliente),cp.num_cliente);
                linea(Largo);
                printf("    Raz%cn Social:   %-30s     ""%-5s: %s\n",162,cp.nombre_empresa,nombreDocumento(cp.tipo_documento),cp.documento);
                printf("    Direcci%cn:      %s - %s - CP(%04d)\n",162,cp.direccion,cp.localidad,cp.cpost);
                printf("    Condici%cn IVA:  %-15s\n",162,tipoIVA(cp.cond_iva));
                Color(YELLOW,BLUE);
                printf("<1>");
                Color(BLUE,YELLOW);
                printf(" Nombre Com.:    ");
                Color(BLACK,WHITE);
                printf("%-30s\n",cp.nombre_fantasia);
                Color(YELLOW,BLUE);
                printf("<2>");
                Color(BLUE,YELLOW);
                printf(" Apellido:       ");
                Color(BLACK,WHITE);
                printf("%-30s\n",cp.contacto_apellido);
                Color(YELLOW,BLUE);
                printf("<3>");
                Color(BLUE,YELLOW);
                printf(" Nombre:         ");
                Color(BLACK,WHITE);
                printf("%-30s\n",cp.contacto_nombre);
                Color(YELLOW,BLUE);
                printf("<4>");
                Color(BLUE,YELLOW);
                printf(" Contacto Cargo: ");
                Color(BLACK,WHITE);
                printf("%-40s\n",cp.contacto_cargo);
                Color(YELLOW,BLUE);
                printf("<5>");
                Color(BLUE,YELLOW);
                printf(" Contacto DNI:   ");
                Color(BLACK,WHITE);
                printf("%-11s\n",cp.contacto_dni);
                Color(YELLOW,BLUE);
                printf("<6>");
                Color(BLUE,YELLOW);
                printf(" Tel%cfono:       ",130);
                Color(BLACK,WHITE);
                printf("%-20s\n",cp.tel);
                Color(YELLOW,BLUE);
                printf("<0>");
                Color(BLUE,YELLOW);
                printf(" SALIR           \n");
                Color(BLACK,WHITE);
                linea(Largo);
                printf("Opci%cn ==>",162);
                gotoxy(12,16);
                edicion=capturaCaracter(1);
                if (edicion>0 && edicion<=6)
                {
                    gotoxy(0,17);
                    linea(Largo);
                    gotoxy(0,19);
                    linea(Largo);
                    gotoxy(0,18);
                }
                switch (edicion)
                {
                    case 1: fflush(stdin);
                            printf("Nombre Comercial:  ");
                            gets(cp.nombre_fantasia);
                            break;
                    case 2: fflush(stdin);
                            printf("Contacto Apellido: ");
                            gets(cp.contacto_apellido);
                            break;
                    case 3: fflush(stdin);
                            printf("Contacto Nombre:   ");
                            gets(cp.contacto_nombre);
                            break;
                    case 4: fflush(stdin);
                            printf("Contacto Cargo:    ");
                            gets(cp.contacto_cargo);
                            break;
                    case 5: fflush(stdin);
                            printf("Contacto DNI:      ");
                            gets(cp.contacto_dni);
                            break;
                    case 6: fflush(stdin);
                            printf("Tel%cfono:  ",130);
                            gets(cp.tel);
                            break;
                }
            } while (edicion!=0);
            gotoxy(0,19);
            linea(Largo);
            do
            {
                submenu(&menu,sm,rango,20);
                switch (menu)
                {
                    case 1: fseek(c_p,ftell(c_p)-sizeof(cliente_proveedor),SEEK_SET);
                            fwrite(&cp,sizeof(cp),1,c_p);
                            fseek(c_p,0,SEEK_END);
                            completo();
                            break;
                    case 2: cancela();
                            break;
                }
            } while (menu==0);
        }
        else
            noEncontrado();
    }
    pausa();
    fclose(c_p);
}

/*Función:  listarCP(pasar por referencia la estructura con los nombres de archivo, pasar por valor la posición del menú)
  Acción:   Muestra por pantalla el listado de clientes, proveedores o ambos según la opción elegiga.*/

void listarCP(archivo a[],int opcion)
{
    menuopciones sm[]={"<ESC>","CLIENTE","PROVEEDOR","TODOS"};
    int menu,rango=sizeof(sm)/27,escape=0;
    FILE *c_p;
    c_p=fopen(a[1].fichero,"rb");
    cls();
    leyendaModuloClienteProveedor(opcion);
    if (c_p!=NULL)
    {
        do
        {
            submenu(&menu,sm,rango,3);
            switch (menu)
            {
                case 0: escape=1;
                        break;
                case 1: printf("LISTA CLIENTES\n");
                        linea(Largo);
                        listaCP(c_p,1);
                        break;
                case 2: printf("LISTA PROVEEDORES\n");
                        linea(Largo);
                        listaCP(c_p,2);
                        break;
                case 3: printf("LISTA CLIENTES/PROVEEDORES\n");
                        linea(Largo);
                        listaCP(c_p,3);
                        break;
            }
        } while (menu!=0 && (menu<1 || menu>3));
    }
    linea(Largo);
    if (escape==1)
        cancela();
    else
        completo();
    fclose(c_p);
    pausa();
}

/*Función:  mostrarCliente(pasar por referencia la estructura con los nombres de archivo, pasar por valor la posición del menú)
  Acción:   Muestra por pantalla al cliente seleccionado.*/

void mostrarCliente(archivo a[],int opcion)
{
    cliente_proveedor cp;
    FILE *c_p,*fac,*tal,*com;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"rb");
    tal=fopen(a[3].fichero,"rb");
    com=fopen(a[6].fichero,"rb");
    if (c_p!=NULL && fac!=NULL)
    {
        cp=encontrarClienteProveedor(c_p,1);
        cls();
        leyendaModuloClienteProveedor(opcion);
        mostrarCP(cp,3,1);
        leerMovimiento(fac,com,tal,cp,1);
        completo();
    }
    pausa();
    fclose(c_p);
    fclose(fac);
    fclose(tal);
    fclose(com);
}

/*Función:  mostrarProveedor(pasar por referencia la estructura con los nombres de archivo, pasar por valor la posición del menú)
  Acción:   Muestra por pantalla al proveedor seleccionado.*/

void mostrarProveedor(archivo a[],int opcion)
{
    cliente_proveedor cp;
    FILE *c_p,*fac,*tal,*com;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"rb");
    tal=fopen(a[3].fichero,"rb");
    com=fopen(a[6].fichero,"rb");
    if (c_p!=NULL && fac!=NULL)
    {
        cp=encontrarClienteProveedor(c_p,2);
        cls();
        leyendaModuloClienteProveedor(opcion);
        mostrarCP(cp,3,1);
        leerMovimiento(fac,com,tal,cp,2);
        completo();
    }
    pausa();
    fclose(c_p);
    fclose(fac);
    fclose(tal);
    fclose(com);
}

/*Función:  mostrarCuentaCorriente(pasar por referencia la estructura con los nombres de archivo, pasar por valor la posición del menú)
  Acción:   Muestra por pantalla la cuenta corriente del cliente/proveedor seleccionado.*/

void mostrarCuentaCorriente(archivo a[],int opcion)
{
    cliente_proveedor cp;
    FILE *c_p,*fac,*tal,*com;
    c_p=fopen(a[1].fichero,"rb");
    fac=fopen(a[2].fichero,"rb");
    tal=fopen(a[3].fichero,"rb");
    com=fopen(a[6].fichero,"rb");
    if (c_p!=NULL && fac!=NULL)
    {
        cp=encontrarClienteProveedor(c_p,3);
        cls();
        leyendaModuloClienteProveedor(opcion);
        mostrarCP(cp,3,0);
        leerMovimiento(fac,com,tal,cp,1);
        completo();
    }
    pausa();
    fclose(c_p);
    fclose(fac);
    fclose(tal);
    fclose(com);
}

/*Función:  leerMovimiento(pasar por referencia los archivos fac, com, tal, pasar por valor estructura cliente_proveedor,
            pasar por valor la opción correspondiente del menú)
  Acción:   Muestra por pantalla al proveedor seleccionado.*/

void  leerMovimiento(FILE *fac,FILE *com,FILE *tal,cliente_proveedor cp,int opcion)
{
    factura f;
    compra c;
    talonario t;
    int subtotal=0,flag=0;
    fseek(fac,0,SEEK_SET);
    fseek(com,0,SEEK_SET);
    if (opcion==1)
        fread(&f,sizeof(factura),1,fac);
    else
        fread(&c,sizeof(compra),1,com);
    printf("Fecha      C. L N%c Comprobante Subtotal      IVA 21%%       Total\n",167);
    linea(Largo);
    while ((!feof(fac) && opcion==1) || (!feof(com) && opcion==2))
    {
        switch (opcion)
        {
            case 1: if (strcmp(cp.id_empresa,f.id_empresa)==0 && f.estado==0)
                    {
                        t=traerTalonario(tal,f);
                        printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",f.emision_dia,f.emision_mes,f.emision_anio,nombre_comprobante(t.codigo,'c'),letra_comprobante(t.codigo),f.numero_sucursal,f.numero_factura,f.subtotal,f.iva21,f.total);
                        if (t.codigo==1 || t.codigo==6 || t.codigo==11)
                            subtotal=subtotal+f.subtotal;
                        else
                            subtotal=subtotal-f.subtotal;
                    }
                    break;
            case 2: if (strcmp(cp.id_empresa,c.id_empresa)==0 && f.estado==0)
                    {
                        printf("%02d/%02d/%04d %s %s %05d-%08d $%12.2f $%12.2f $%12.2f\n",c.emision_dia,c.emision_mes,c.emision_anio,nombre_comprobante(c.codigo_talonario,'c'),letra_comprobante(c.codigo_talonario),c.numero_sucursal,c.numero_factura,c.subtotal,c.iva21,c.total);
                        if (c.codigo_talonario==1 || c.codigo_talonario==6 || c.codigo_talonario==11)
                            subtotal=subtotal+f.subtotal;
                        else
                            subtotal=subtotal-f.subtotal;
                    }
                    break;
        }
        if (opcion==1)
            fread(&f,sizeof(factura),1,fac);
        else
            fread(&c,sizeof(compra),1,com);
    }
    linea(Largo);
    Color(BLACK,LGREY);
    printf("Saldo Deudor: $ %13.2f\n",subtotal*1.21);
    Color(BLACK,WHITE);
    linea(Largo);
}

/*Función:  listaCP(pasar por referencia el archivo cliente_proveedor, pasar por valor la opcion a listar)
  Acción:   Devuelve la lista cliente_proveedor según la opción pedida desde listarCP().*/

void listaCP(FILE *c_p,int opcion)
{
    cliente_proveedor cp;
    fseek(c_p,0,SEEK_SET);
    printf("N%c     Raz%cn Social          T. Documento   Direcci%cn           Localidad  CP   \n",167,162,162);
    fread(&cp,sizeof(cliente_proveedor),1,c_p);
    linea(Largo);
    while (!feof(c_p))
    {
        switch (opcion)
        {
            case 1: if (strcmp(cp.tipo_cliente,"C")==0 && cp.eliminado==0)
                        printf("%06d %-21.21s %02d %11s %-19.19s %-10s.10 %04d\n",cp.num_cliente,cp.nombre_empresa,cp.tipo_documento,cp.documento,cp.direccion,cp.localidad,cp.cpost);
                    break;
            case 2: if (strcmp(cp.tipo_cliente,"P")==0 && cp.eliminado==0)
                        printf("%06d %-21.21s %02d %11s %-19.19s %-10s.10 %04d\n",cp.num_cliente,cp.nombre_empresa,cp.tipo_documento,cp.documento,cp.direccion,cp.localidad,cp.cpost);
                    break;
            case 3: if (strcmp(cp.tipo_cliente,"M")!=0 && cp.eliminado==0)
                        printf("%06d %-21.21s %02d %11s %-19.19s %-10.10s %04d\n",cp.num_cliente,cp.nombre_empresa,cp.tipo_documento,cp.documento,cp.direccion,cp.localidad,cp.cpost);
                    break;
        }
        fread(&cp,sizeof(cliente_proveedor),1,c_p);
    }
}

/*Función:  obtenerClienteProveedor(pasar por referencia el archivo cliente_proveedor)
  Acción:   Nos permite seleccionar de un menú las opciones para buscar al cliente o cancelar la operación presionando
            la tecla <ESC> escape, lo que devuelve es el registro buscado, en caso de no encontrarlo devuelve el mismo
            registro ingresado con el campo cp.id_empresa en blanco.*/

cliente_proveedor obtenerClienteProveedor(FILE *c_p)
{
    cliente_proveedor cp;
    menuopciones sm[]={"<ESC>","DOCUMENTO","RAZON SOC","EMPRESA","CONTACTO"};
    int rango=sizeof(sm)/27,menu;
    do
    {
        strcpy(cp.id_empresa,"");
        submenu(&menu,sm,rango,5);
        switch (menu)
        {
            case 1: printf("CUIT/CUIL/DNI: ");
                    gets(cp.documento);
                    break;
            case 2: printf("Raz%cn Social:  ",162);
                    gets(cp.nombre_empresa);
                    break;
            case 3: printf("Nombre Fantasia: ");
                    gets(cp.nombre_fantasia);
                    break;
            case 4: printf("Apellido:      ");
                    gets(cp.contacto_apellido);
                    printf("Nombre:        ");
                    gets(cp.contacto_nombre);
                    break;
        }
        if (menu!=0)
        {
            cp=buscarClienteProveedor(c_p,cp);
            menu=0;
        }
    } while (menu!=0);
    return cp;
}

/*Función:  buscarClienteProveedor(pasar por referencia el archivo cliente_proveedor, pasar por valor el registro
            cliente_proveedor)
  Acción:   Devuelve el registro cliente_proveedor si éste existe, caso contrario devuelve un registro con el campo
            cp.id_Empresa en blanco.*/

cliente_proveedor buscarClienteProveedor(FILE *c_p,cliente_proveedor cp)
{
    cliente_proveedor cli_pro;
    fseek(c_p,0,SEEK_SET);
    fread(&cli_pro,sizeof(cliente_proveedor),1,c_p);
    while (!feof(c_p))
    {
        if (strcmp(cp.documento,cli_pro.documento)==0 || strcmp(cp.nombre_empresa,cli_pro.nombre_empresa)==0 || strcmp(cp.nombre_fantasia,cli_pro.nombre_fantasia)==0 || (strcmp(cp.contacto_apellido,cli_pro.contacto_apellido)==0 && strcmp(cp.contacto_nombre,cli_pro.contacto_nombre)==0))
            return cli_pro;
        fread(&cli_pro,sizeof(cliente_proveedor),1,c_p);
    }
    strcpy(cp.id_empresa,"?");
    return cp;
}

/*Función:  mostrarCP(pasar los datos guardados en la estructura cliente_proveedor, pasar la posicion vertical 'y')
  Acción:   Imprime por pantalla los datos básicos del cliente/proveedor*/

void mostrarCP(cliente_proveedor cp,int y,int completo)
{
    gotoxy(0,y);
    Color(WHITE,GREEN);
    printf("DATOS %-10s                                                                \n",PalabraClienteProveedor(cp.tipo_cliente));
    gotoxy(60,y);
    Color(GREEN,WHITE);
    printf("N%c Cliente: %07d\n",167,cp.num_cliente);
    Color(BLACK,WHITE);
    linea(Largo);
    printf("Raz%cn Social: %-30s \n",162,cp.nombre_empresa);
    printf("Documento:    %s N%c %-11s\n",nombreDocumento(cp.tipo_documento),167,cp.documento);
    printf("Direcci%cn:    %s - %s - CP(%d)\n",162,cp.direccion,cp.localidad,cp.cpost);
    printf("Tipo IVA:     %s\n",tipoIVA(cp.cond_iva));
    linea(Largo);
    if (completo==1)
    {
        printf("Comercial:    %s\n",cp.nombre_fantasia);
        printf("Contacto:     %s, %s\n",cp.contacto_apellido,cp.contacto_nombre);
        printf("Cargo:        %s\n",cp.contacto_cargo);
        printf("Documento:    %s\n",cp.contacto_dni);
        printf("Tel%cfono:     %s\n",130,cp.tel);
        linea(Largo);
    }
}

cliente_proveedor encontrarClienteProveedor(FILE *c_p,int opcion)
{
    int j=0,k,flag=0,rango=0,ubicacion,posicion;
    char dato[4];
    cliente_proveedor cp;
    lista_Desplegable *registro;
    registro=(lista_Desplegable*)malloc(RANGO*sizeof(lista_Desplegable));
    fseek(c_p,0,SEEK_SET);
    fread(&cp,sizeof(cliente_proveedor),1,c_p);
    while (!feof(c_p))
    {
        switch (opcion)
        {
            case 1: if (strcmp(cp.tipo_cliente,"C")==0 && cp.eliminado==0)
                    {
                        sprintf(registro[rango++].registro,"%04d %-20.20s %-11s %-20.20s",j+1,cp.nombre_empresa,cp.documento,cp.nombre_fantasia);
                        flag=1;
                    }
                    break;
            case 2: if (strcmp(cp.tipo_cliente,"P")==0 &&  cp.eliminado==0)
                    {
                        sprintf(registro[rango++].registro,"%04d %-20.20s %-11s %-20.20s",j+1,cp.nombre_empresa,cp.documento,cp.nombre_fantasia);
                        flag=1;
                    }
                    break;
            case 3: if (strcmp(cp.tipo_cliente,"M")!=0 && cp.eliminado==0)
                    {
                        sprintf(registro[rango++].registro,"%04d %-20.20s %-11s %-20.20s",j+1,cp.nombre_empresa,cp.documento,cp.nombre_fantasia);
                        flag=1;
                    }
                    break;
        }
        fread(&cp,sizeof(cliente_proveedor),1,c_p);
        j++;
    }
    if (flag==1)
    {
        posicion=listaDesplegable(registro,4,rango);
        for (k=0;k<4;k++)
            dato[k]=registro[posicion].registro[k];
        ubicacion=atoi(dato);
        fseek(c_p,(ubicacion-1)*sizeof(cliente_proveedor),SEEK_SET);
        printf("\n");
        fread(&cp,sizeof(cliente_proveedor),1,c_p);
    }
    else
        strcpy(cp.id_empresa,"");
    linea(Largo);
    free(registro);
    return cp;
}

/*Función:  obtenerClienteId(pasar por referencia el archivo, pasar por valor la estructura factura)
  Acción:   Retorna los datos del cliente por el iD de la factura.*/

cliente_proveedor obtenerClienteId(FILE *c_p,factura f)
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

void leyendaModuloClienteProveedor(int leyenda)
{
    linea(Largo);
    Color(WHITE,RED);
    switch (leyenda)
    {
        case 1: printf("MODULO CLIENTE/PROVEEDOR  <<< ALTA >>>                                          \n");
                break;
        case 2: printf("MODULO CLIENTE/PROVEEDOR  <<< BAJA >>>                                          \n");
                break;
        case 3: printf("MODULO CLIENTE/PROVEEDOR  <<< MODIFICAR >>>                                     \n");
                break;
        case 4: printf("MODULO CLIENTE/PROVEEDOR  <<< LISTAR >>>                                        \n");
                break;
        case 5: printf("MODULO CLIENTE/PROVEEDOR  <<< MOSTRAR CLIENTE >>>                               \n");
                break;
        case 6: printf("MODULO CLIENTE/PROVEEDOR  <<< MOSTRAR PROVEEDOR >>>                             \n");
                break;
        case 7: printf("MODULO CLIENTE/PROVEEDOR  <<< CUENTA CORRIENTE >>>                              \n");
                break;
    }
    Color(BLACK,WHITE);
    linea(Largo);
}

/*Función:  PalabraClienteProveedor(pasar por valor el caracter que representa cliente, proveedor o maestro)
  Acción:   Retorna la leyenca Cliente, Proveedor o Maestro.*/

char *PalabraClienteProveedor(char cp[])
{
    switch (cp[0])
    {
        case 'C':   return "CLIENTE";
                    break;
        case 'P':   return "PROVEEDOR";
                    break;
        case 'M':   return "MAESTRO";
                    break;
    }
    return "INDEFINIDO";
}

/*Función:  empresaMaestra(pasar por valor la estructura con nombres de archivo)
  Acción:   Devuelve el registro 0 de cliente_proveedor que contiene el registro maestro del sistema.*/

cliente_proveedor empresaMaestra(archivo a[])
{
    cliente_proveedor m;
    FILE *c_p;
    c_p=fopen(a[1].fichero,"rb");
    fseek(c_p,0,SEEK_SET);
    fread(&m,sizeof(cliente_proveedor),1,c_p);
    fclose(c_p);
    return m;
}
