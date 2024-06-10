/*Funciones del menú cliente_proveedor*/
void altaCP(archivo[],int);
void bajaCP(archivo[],int);
void modificarCP(archivo[],int);
void leerMovimiento(FILE *,FILE *,FILE *,cliente_proveedor,int);
void listarCP(archivo[],int);
void mostrarCliente(archivo[],int);
void mostrarProveedor(archivo[],int);
void mostrarCuentaCorriente(archivo[],int);

/*Funciones secundarias*/
void listaCP(FILE *,int);
cliente_proveedor obtenerClienteProveedor(FILE *);
cliente_proveedor buscarClienteProveedor(FILE *,cliente_proveedor);
void mostrarCP(cliente_proveedor,int,int);
cliente_proveedor encontrarClienteProveedor(FILE *,int);
cliente_proveedor obtenerClienteId(FILE *,factura);
void leyendaModuloClienteProveedor(int);
char *PalabraClienteProveedor(char[]);
cliente_proveedor empresaMaestra(archivo[]);
