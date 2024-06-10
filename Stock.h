/*Funciones del menú Stock*/
void altaStock(archivo[],int);
void bajaStock(archivo[],int);
void modificaStock(archivo[],int);
void listarStock(archivo[],int);
void buscarStock(archivo[],int);
void faltante(archivo[],int);

/*Funciones secundarias*/
void leyendaModuloStock(int);
int existeProducto(FILE *,stock);
stock obtenerProducto(FILE *);
