char *obtenerId();
int bisiesto(int);
char *completarCeros(int,int);
void cls();
void pausa();
void linea(int);
void invalido();
void bienvenida();
void finaliza();
void gotoxy(int,int);
void completo();
int verifica_fecha(int,int,int);
int verifica_numero_rango(int,int,int);
char *letra_comprobante(int);
char *nombre_comprobante(int,char);
void recuadro(int,int);
void menu(int*,menuopciones[],int);
void submenu(int*,menuopciones[],int,int);
void noEncontrado();
void cancela();
int verificaCUIT(char[]);
char *nombreDocumento(int);
int capturaCaracter(int);
char *tipoIVA(int);
void archivoInvalido();
void archivoGeneraComprobante();
void cargarFecha(int*,int*,int*,int,int,char[]);
int compararFechas(int,int,int,int,int,int);
int listaDesplegable(lista_Desplegable[],int,int);
void leyendaRegistro(int);
void mostrarRegistroLeyenda(int,int);
void mostrarRegistro(lista_Desplegable[],int,int,int);
void Color(int,int);
void registroDuplicado();
