nodo *nuevonodo(items reg)
{
    nodo *nuevo;
    nuevo=(nodo*) malloc(sizeof(nodo));
    memcpy(nuevo,&reg,sizeof(items));
    nuevo->sig=NULL;
}

void insertar(nodo **lista,items reg)
{
    nodo *nuevo;
    if (*lista==NULL)
    {
        nuevo=nuevonodo(reg);
        nuevo->sig=*lista;
        (*lista)=nuevo;
    }
    else
        insertar(&(*lista)->sig,reg);
}

void verLista(nodo *lista)
{
    if (lista)
    {
        printf("% 10d %04d %-25s $ %13.2f $ %13.2f\n",lista->reg.cantidad,lista->reg.id_stock,lista->reg.descripcion,lista->reg.unitario,lista->reg.cantidad*lista->reg.unitario);
        verLista(lista->sig);
    }
}

void cargarArchivo(FILE *ite,nodo *lista)
{
    items i;
    if (lista!=NULL)
    {
        memcpy(&i,lista,sizeof(items));
        fwrite(&i,sizeof(items),1,ite);
        cargarArchivo(ite,lista->sig);
    }
}

void modificarItems(factura c,nodo **lista)
{
    if (*lista)
    {
        strcpy((*lista)->reg.id_items,obtenerId());
        strcpy((*lista)->reg.id_factura,c.id_factura);
        modificarItems(c,&(*lista)->sig);
    }
}

void cargaModificacionStock(FILE *sto,nodo *lista,int accion)
{
    stock s;
    if (lista!=NULL)
    {
        s=traeStock(sto,lista->reg.id_stock);
        if (accion==0)
            s.cantidad=s.cantidad-lista->reg.cantidad;
        else
            s.cantidad=s.cantidad+lista->reg.cantidad;
        fseek(sto,ftell(sto)-sizeof(stock),SEEK_SET);
        fwrite(&s,sizeof(stock),1,sto);
        cargaModificacionStock(sto,lista->sig,0);
    }
}

stock traeStock(FILE *sto,int id_stock)
{
    stock s;
    fseek(sto,0,SEEK_SET);
    fread(&s,sizeof(stock),1,sto);
    while (!feof(sto))
    {
        if (id_stock==s.id_stock)
            return s;
        fread(&s,sizeof(stock),1,sto);
    }
    s.id_stock=0;
    return s;
}

void eliminar(nodo **lista)
{
    nodo *aux;
    if (*lista)
    {
        aux=*lista;
        (*lista)=aux->sig;
        free(aux);
        eliminar(lista);
    }
}
