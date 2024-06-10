/*Funciones del menú Facturación*/
void emitirComprobante(archivo[],int);
void anularComprobante(archivo[],int);
void listarComprobante(archivo[],int);
void verComprobante(archivo[],int);
void pagarComprobante(archivo[],int);

/*Funciones secuendarias*/
void guardarFactura(FILE *,factura);
int obtenerCodigoComprobante(int);
factura obtenerComprobante(FILE *,factura,cliente_proveedor);
void leerFactura(FILE *,FILE *,cliente_proveedor,factura,factura,int);
void leyendaModuloFacturas(int);
void generarComprobante(archivo[],cliente_proveedor,factura,talonario,nodo*);
void mostrarComprobante(archivo[],cliente_proveedor,factura,talonario,nodo*,int);

/*Funciones items*/
void obtenerItems(FILE *,factura,char[],nodo**,int);

/*Funciones stock*/
void obtenerStock(FILE *,nodo**,char[],float*,int);

/*Funciones talonario*/
talonario obtenerTalonario(FILE *,int);
talonario obtenerFToNCPorSucursal(FILE *,factura,int);
void mostrarTalonario(talonario,int);
