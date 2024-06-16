#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cJSON.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define user_size_const 200


// Prototipos de funciones
void menu_main();
void upload_user();
void backup_user();
void create_user();
void informe_menu();
//void stats();
void read_and_print_file_by_line(const char *path);
void read_json_file(char* filename, int thread_id, int* record_count);

// Comprobaciones
int is_number_real(float number);
int no_cuenta_exists(float id);
float two_dots(float numero);
void write_msg_error(int fd, char *field);
void *load_data_thread(void *arg);

void clean_stdin();
void operaciones_menu();
void retiro();
void deposito();
void transferencia();
void consultar_cuenta();

//structura de usuario
struct user_struct {
    int no_cuenta;
    char nombre[75];
    float saldo;
};
struct user_struct users_list[user_size_const];
int user_size = 0;
void init_user_list(struct user_struct list[]);
pthread_mutex_t user_list_mutex;

typedef struct {
    int thread_id;
    int* record_count;
} thread_arg_t;

int main() {
    printf("----------------------------------------\n");
    printf("CARGANDO EL SISTEMA.\n");
    // Verificar si la carpeta exisiste para nuestros logs
    //path de directorio de logs
    const char* dirpath_up = "upload";
    struct stat st_up = {0};
    if (stat(dirpath_up, &st_up) == -1) {
        if (mkdir(dirpath_up, 0777) == -1) {
            perror("Error creando la carpeta upload.\n");
            exit(1);
        }
        printf("Carpeta upload creada exitosamente.\n");
    } else {
        printf("La carpeta upload ya existe.\n");
    }

    // Verificar si la carpeta exisiste para nuestros logs
    //path de directorio de logs
    const char* dirpath_log = "logs";
    struct stat st_log = {0};
    if (stat(dirpath_log, &st_log) == -1) {
        if (mkdir(dirpath_log, 0777) == -1) {
            perror("Error creando la carpeta logs.\n");
            exit(1);
        }
        printf("Carpeta logs creada exitosamente.\n");
    } else {
        printf("La carpeta logs ya existe.\n");
    }

    // Verificar si la carpeta exisiste para backups
    //path de directorio de archivos guardados
    const char* dirpath_bck = "backups";
    struct stat st_bck = {0};
    if (stat(dirpath_bck, &st_bck) == -1) {
        if (mkdir(dirpath_bck, 0777) == -1) {
            perror("Error creando la carpeta backup.\n");
            exit(1);
        }
        printf("Carpeta bakcup creada exitosamente.\n");
    } else {
        printf("La carpeta backup ya existe.\n");
    }

    init_user_list(users_list);
    pthread_mutex_init(&user_list_mutex, NULL);

    printf("SISTEMA CARGADO CON EXITO.\n");
    printf("----------------------------------------\n\n");
    menu_main();

    pthread_mutex_destroy(&user_list_mutex);
    return 0;
}

// Definición de la función del menú principal
void menu_main() {
    int opcion;
    int continuar = 1;

    while (continuar) {
        // Mostrar el menú principal
        printf("---- Menu Principal ----\n");
        printf("1. Cargar datos de usuarios.\n");
        printf("2. Almacenamiento datos de usuarios.\n");
        printf("3. Cargar operaciones de usuarios.\n");
        printf("4. Almacenamiento operaciones de usuarios.\n");
        printf("5. Crear usuario.\n");
        printf("6. Crear operacion.\n");
        printf("7. Informes.\n");
        printf("8. Salir\n");
        printf("Ingrese una opcion: ");
        
        // Leer la opción ingresada por el usuario
        scanf("%d", &opcion);

        // Evaluar la opción ingresada usando switch
        switch(opcion) {
            case 1:
                upload_user();
                break;
            case 2:
                backup_user();
                break;
            case 3:
                //new_user();
                break;
            case 4:
                //new_trans();
                break;
            case 5:
                create_user();
                break;
            case 6:
                operaciones_menu();
                break;
            case 7:
                informe_menu();
                break;
            case 8:
                printf("\nSaliendo...\n");
                continuar = 0; // Cambiar la condición para salir del bucle
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
        
        printf("\n");
    }
}

void init_user_list(struct user_struct list[]) {
    for (int i = 0; i < user_size_const; i++) {
        list[i].no_cuenta = -1;
        list[i].saldo = -1.0;
        strcpy(list[i].nombre, "vacio");
    }
}

// funcion para cargar el archivo este 
void upload_user() {
    pthread_t threads[3];
    int records[3] = {0, 0, 0};

    for (int i = 0; i < 3; i++) {
        thread_arg_t *thread_arg = malloc(sizeof(thread_arg_t));
        thread_arg->thread_id = i;
        thread_arg->record_count = &records[i];
        pthread_create(&threads[i], NULL, load_data_thread, (void*)thread_arg);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    int total_records = records[0] + records[1] + records[2];

    printf("\n-------------------- Carga de usuarios --------------------\n");
    printf("Fecha y Hora: %s\n", asctime(localtime(&(time_t){time(NULL)})));
    printf("Usuarios cargados:\n");
    printf("Hilo 1: %d\n", records[0]);
    printf("Hilo 2: %d\n", records[1]);
    printf("Hilo 3: %d\n", records[2]);
    printf("Total: %d\n", total_records);
    printf("Errores:\n");
    user_size = total_records;
    const char* filepath_txt = "logs/user_upload_stats_error.log";
    read_and_print_file_by_line(filepath_txt);
    printf("\n-----------------------------------------------------------\n");

}

// función de hilo para cargar datos
void *load_data_thread(void *arg) {
    thread_arg_t *thread_arg = (thread_arg_t*)arg;
    int thread_id = thread_arg->thread_id;
    int* record_count = thread_arg->record_count;
    char* filename = "upload/usuarios.json";
    read_json_file(filename, thread_id, record_count);
    free(arg);
    return NULL;
}

//lectura del archivo de errores
void read_and_print_file_by_line(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 1;

    while ((read = getline(&line, &len, file)) != -1) {
        printf("\t-Linea %d: %s", line_number, line);
        line_number++;
    }

    free(line);
    fclose(file);
}

//lectura de archivo json
void read_json_file(char* filename, int thread_id, int* record_count) {

    const char* filepath_txt = "logs/user_upload_stats_error.log";
    // Verificar si el archivo existe
    int fd = open(filepath_txt, O_RDONLY);
    if (fd != -1) {
        printf("El archivo ya existe. Limpiando datos...\n");
        close(fd);

        // Vaciar el archivo
        fd = open(filepath_txt, O_WRONLY | O_TRUNC);
        if (fd == -1) {
            perror("Error vaciando el archivo.\n");
            exit(1);
        } else {
            printf("Archivo limpio.\n");
            close(fd);
        }
    } else {
        // Crear el archivo
        fd = open(filepath_txt, O_CREAT | O_WRONLY, 0666);
        if (fd == -1) {
            perror("Error creando el archivo logs/user_upload_stats_error.log\n");
            exit(1);
        } else {
            printf("Archivo creado exitosamente, logs/user_upload_stats_error.log.\n");
            close(fd);
        }
    }


    // Open the JSON file for reading
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("Error al abrir el archivo usuarios.json");
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the entire file into a buffer
    char *buffer = (char*)malloc(filesize + 1);
    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';

    // Close the file
    fclose(file);

    // Parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    if(json == NULL){
        perror("Error en el parse del json de usuarios");
        return;
    }

    // Check if it's a valid JSON array
    if(!cJSON_IsArray(json)){
        perror("El json ingresado no es de tipo arreglo");
        return;
    }

    int number_objects_safe = 0;
    int number_objects_all = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json){
         if (number_objects_all % 3 != thread_id) {
            number_objects_all++;
            continue;
        }
        if(cJSON_IsObject(item)){
            cJSON *no_cuenta = cJSON_GetObjectItem(item, "no_cuenta");
            cJSON *nombre = cJSON_GetObjectItem(item, "nombre");
            cJSON *saldo = cJSON_GetObjectItem(item, "saldo");  
            // comprobamos si el numero de cuenta es un numero entero positivo
            // comprobamos si el saldo es un numero positivo mayor o igual a 0
            // comprobamos si ya existe el usuario           
            if(cJSON_IsNumber(no_cuenta) && cJSON_IsString(nombre) 
                && cJSON_IsNumber(saldo) && (no_cuenta -> valueint >= 0) && (saldo -> valuedouble >= 0)){
                    
                    if (is_number_real(no_cuenta -> valueint)){
                        pthread_mutex_lock(&user_list_mutex);
                        if (!no_cuenta_exists(no_cuenta -> valueint)) {
                            for(int i =0; i< user_size_const;i++){
                                if (users_list[i].no_cuenta == -1) {
                                    users_list[i].no_cuenta = no_cuenta -> valueint;
                                    sprintf(users_list[i].nombre , "%s", nombre -> valuestring);
                                    users_list[i].saldo = two_dots(saldo -> valuedouble);
                                    number_objects_safe++;
                                    break;
                                }
                            }                            
                        } else {
                            int fd = open("logs/user_upload_stats_error.log", O_RDWR);
                            if (fd == -1) {
                                perror("Error abriendo el archivo de logs de errores de informe");
                                exit(1);
                            }
                            char msg[200];
                            sprintf(msg,"Registro #%d: registros duplicados, el no_cuenta(%d) ya esta siendo utilizada",number_objects_all,no_cuenta -> valueint);
                            write_msg_error(fd,msg);
                            close(fd);
                        }
                        pthread_mutex_unlock(&user_list_mutex);
                    }else{
                        int fd = open("logs/user_upload_stats_error.log", O_RDWR);
                            if (fd == -1) {
                                perror("Error abriendo el archivo de logs de errores de informe");
                                exit(1);
                            }
                            char msg[200];
                            sprintf(msg,"Registro #%d: no_cuenta, el no_cuenta(%d) no es un numero entero",number_objects_all,no_cuenta -> valueint);
                            write_msg_error(fd,msg);
                            close(fd);
                    }                
            }else{
                int fd = open("logs/user_upload_stats_error.log", O_RDWR);
                if (fd == -1) {
                    perror("Error abriendo el archivo de logs de errores de informe");
                    exit(1);
                }

                if (!cJSON_IsNumber(no_cuenta)){
                    //si el numero de cuenta no es un numero 
                    char msg[200];
                    sprintf(msg,"Registro #%d: no_cuenta, el no_cuenta(%s) no es un numero",number_objects_all,no_cuenta -> valuestring);
                    write_msg_error(fd,msg);
                }

                if (cJSON_IsNumber(no_cuenta) && no_cuenta -> valueint < 0){
                    // si el numero de cuenta no es un numero entero positivio
                    char msg[200];
                    sprintf(msg,"Registro #%d: no_cuenta, el no_cuenta(%d) es un numero negativo",number_objects_all,no_cuenta -> valueint);
                    write_msg_error(fd,msg);
                   
                }

                if (!cJSON_IsString(nombre)){
                    // si el nombre de usuario no es una cadena
                    char msg[200];
                    sprintf(msg,"Registro #%d: nombre, el nombre(%s) no es de tipo cadena",number_objects_all,nombre -> valuestring);
                    write_msg_error(fd,msg);
                }

                if (!cJSON_IsNumber(saldo)){
                    //si el saldo no es un numero
                    char msg[200];
                    sprintf(msg,"Registro #%d: saldo, el saldo(%s) no es un numero",number_objects_all,saldo -> valuestring);
                    write_msg_error(fd,msg);

                }

                if (cJSON_IsNumber(saldo) && saldo -> valuedouble < 0){
                    // si el saldo no es un numero positivio
                    char msg[200];
                    sprintf(msg,"Registro #%d: saldo, el saldo(%f) es un numero negativo",number_objects_all,saldo -> valuedouble);
                    write_msg_error(fd,msg);
                }
                
                close(fd);
            }
            number_objects_all++;
        }
    }
    *record_count = number_objects_safe;
    // Clean up
    cJSON_Delete(json);
    free(buffer);

}

int no_cuenta_exists(float id) {
    for (int i = 0; i < user_size_const; i++) {
        if (users_list[i].no_cuenta == id) {
            return 1;
        }
    }
    return 0;
}

int is_number_real(float number) {
    int real_number = (int)number;
    return number == real_number;
}

void write_msg_error(int fd, char *field) {
    char full_message[256]; 
    snprintf(full_message, sizeof(full_message), "Error:%s.\n",field);
    lseek(fd, 0, SEEK_END);
    write(fd, full_message, strlen(full_message));
}

float two_dots(float numero) {
    return roundf(numero * 100) / 100;
}

char* struct_to_json_users(){

    // Create an empty JSON Array
    cJSON *json_array = cJSON_CreateArray();

    // Iterate through the structs
    for(int i = 0; i < user_size; i++){
        // Create an emty JSON Object
        cJSON *item = cJSON_CreateObject();

        // Add attributes to the object
        cJSON_AddNumberToObject(item, "no_cuenta", users_list[i].no_cuenta);
        cJSON_AddStringToObject(item, "nombre", users_list[i].nombre);
        cJSON_AddNumberToObject(item, "saldo", users_list[i].saldo);

        // Add the opbject to the Array
        cJSON_AddItemToArray(json_array, item);
    }

    // Format the JSON Array 
    char* formatted = cJSON_Print(json_array);
    return formatted;
}

void write_file_users(char* filename, char* data){
    //Open the file
    FILE *file = fopen(filename, "wb");
    if(file == NULL){
        perror("Error al abrir el archivo backups/usuarios.json");
        return;
    }

    // Write the data
    fwrite(data, 1, strlen(data), file);
    fclose(file);
}

void backup_user(){
    printf("\n------------------- Backup de usuarios --------------------\n");
    printf("Convirtiendo datos a json...\n");
    char* new_json = struct_to_json_users();
    write_file_users("backups/usuarios.json", new_json);
    sleep(2);
    printf("Listo");
    printf("\n-----------------------------------------------------------\n");
}

void clean_stdin() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void create_user() {
    int no_cuenta;
    char nombre[75];
    float saldo;

    // Ingreso de no_cuenta (entero)
    printf("Ingrese el numero de cuenta (ej. xx): ");
    if (scanf("%d", &no_cuenta) != 1 || no_cuenta < 0) {
        printf("Error: El numero de cuenta debe ser un entero mayor o igual a cero.\n");
        clearerr(stdin); // Limpiar el búfer de entrada
        while (getchar() != '\n'); // Vaciar el búfer de entrada
        return;
    }

    if (no_cuenta < 0){
        return;
    }
    // Ingreso de cadena
    printf("Ingrese una cadena: ");
    scanf(" %99[^\n]", nombre);
    printf("Cadena ingresada: %s\n", nombre);

    // Ingreso de valor_float
    printf("Ingrese el saldo (ej. xxx.xx): ");
    if (scanf("%f", &saldo) != 1 || saldo < 0) {
        printf("Error: El valor del saldo debe ser un numero mayor o igual a cero.\n");
        clearerr(stdin); // Limpiar el búfer de entrada
        while (getchar() != '\n'); // Vaciar el búfer de entrada
        return;
    } else if (saldo == (int)saldo) {
        printf("Error: El valor ingresado es un entero, se requiere un valor xxx.xx .\n");
        clearerr(stdin); // Limpiar el búfer de entrada
        while (getchar() != '\n'); // Vaciar el búfer de entrada
        return;
    }

    for (int i = 0; i < user_size_const; i++) {
        if (users_list[i].no_cuenta == -1) {
            users_list[i].no_cuenta = no_cuenta;
            strcpy(users_list[i].nombre, nombre);
            users_list[i].saldo = two_dots(saldo); // Asumiendo que two_dots ajusta el saldo a dos decimales
            user_size++;
            printf("Usuario creado exitosamente.\n");
            break;
        }
    }
}

void informe_menu() {
    int opcion;
    int continuar = 1;

    while (continuar) {
        printf("\n---- Menu de Informes ----\n");
        printf("1. Mostrar datos de usuarios.\n");
        printf("2. Salir al menu principal.\n");
        printf("Ingrese una opcion: ");

        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                if (user_size == 0){
                    printf("\nNo hay usuarios registrados.\n");
                }else{
                    for (int i = 0; i < user_size_const; i++) {
                        if (users_list[i].no_cuenta != -1) {
                            printf("Usuario %d:\n", i + 1);
                            printf("\tNumero de cuenta: %d\n", users_list[i].no_cuenta);
                            printf("\tNombre: %s\n", users_list[i].nombre);
                            printf("\tSaldo: %.2f\n", users_list[i].saldo);
                        }
                    }
                }                
                break;
            case 2:
                continuar = 0;
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }

        printf("\n");
    }
}

void operaciones_menu() {
    int opcion;
    int continuar = 1;

    while (continuar) {
        printf("\n---- Menu de Operaciones ----\n");
        printf("1. Deposito\n");
        printf("2. Retiro\n");
        printf("3. Transferencia\n");
        printf("4. Consultar cuenta\n");
        printf("5. Salir al menu principal\n");
        printf("Ingrese una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Error: Ingrese un número válido.\n");
            clean_stdin();
            continue;
        }

        switch (opcion) {
            case 1:
                deposito();
                break;
            case 2:
                retiro();
                break;
            case 3:
                transferencia();
                break;
            case 4:
                consultar_cuenta();
                break;
            case 5:
                continuar = 0;
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    }
}

void deposito() {
    int no_cuenta;
    float monto;

    printf("Ingrese el numero de cuenta: ");
    if (scanf("%d", &no_cuenta) != 1 || no_cuenta < 0) {
        printf("Error: El numero de cuenta debe ser un entero mayor o igual a cero.\n");
        clean_stdin();
        return;
    }

    if (!no_cuenta_exists(no_cuenta)) {
        printf("Error: La cuenta no existe.\n");
        return;
    }

    printf("Ingrese el monto a depositar: ");
    if (scanf("%f", &monto) != 1 || monto <= 0) {
        printf("Error: El monto debe ser un número positivo.\n");
        clean_stdin();
        return;
    }

    monto = two_dots(monto);

    for (int i = 0; i < user_size_const; i++) {
        if (users_list[i].no_cuenta == no_cuenta) {
            users_list[i].saldo += monto;
            printf("Deposito exitoso. Nuevo saldo: %.2f\n", users_list[i].saldo);
            return;
        }
    }
}

void retiro() {
    int no_cuenta;
    float monto;

    printf("Ingrese el numero de cuenta: ");
    if (scanf("%d", &no_cuenta) != 1 || no_cuenta < 0) {
        printf("Error: El numero de cuenta debe ser un entero mayor o igual a cero.\n");
        clean_stdin();
        return;
    }

    if (!no_cuenta_exists(no_cuenta)) {
        printf("Error: La cuenta no existe.\n");
        return;
    }

    printf("Ingrese el monto a retirar: ");
    if (scanf("%f", &monto) != 1 || monto <= 0) {
        printf("Error: El monto debe ser un número positivo.\n");
        clean_stdin();
        return;
    }

    monto = two_dots(monto);

    for (int i = 0; i < user_size_const; i++) {
        if (users_list[i].no_cuenta == no_cuenta) {
            if (users_list[i].saldo >= monto) {
                users_list[i].saldo -= monto;
                printf("Retiro exitoso. Nuevo saldo: %.2f\n", users_list[i].saldo);
            } else {
                printf("Error: Saldo insuficiente.\n");
            }
            return;
        }
    }
}

void transferencia() {
    int no_cuenta_origen, no_cuenta_destino;
    float monto;

    printf("Ingrese el numero de cuenta de origen: ");
    if (scanf("%d", &no_cuenta_origen) != 1 || no_cuenta_origen < 0) {
        printf("Error: El numero de cuenta de origen debe ser un entero mayor o igual a cero.\n");
        clean_stdin();
        return;
    }

    if (!no_cuenta_exists(no_cuenta_origen)) {
        printf("Error: La cuenta de origen no existe.\n");
        return;
    }

    printf("Ingrese el numero de cuenta de destino: ");
    if (scanf("%d", &no_cuenta_destino) != 1 || no_cuenta_destino < 0) {
        printf("Error: El numero de cuenta de destino debe ser un entero mayor o igual a cero.\n");
        clean_stdin();
        return;
    }

    if (!no_cuenta_exists(no_cuenta_destino)) {
        printf("Error: La cuenta de destino no existe.\n");
        return;
    }

    printf("Ingrese el monto a transferir: ");
    if (scanf("%f", &monto) != 1 || monto <= 0) {
        printf("Error: El monto debe ser un número positivo.\n");
        clean_stdin();
        return;
    }

    monto = two_dots(monto);

    for (int i = 0; i < user_size_const; i++) {
        if (users_list[i].no_cuenta == no_cuenta_origen) {
            if (users_list[i].saldo >= monto) {
                users_list[i].saldo -= monto;
                for (int j = 0; j < user_size_const; j++) {
                    if (users_list[j].no_cuenta == no_cuenta_destino) {
                        users_list[j].saldo += monto;
                        printf("Transferencia exitosa.\n");
                        printf("Nuevo saldo de la cuenta de origen: %.2f\n", users_list[i].saldo);
                        printf("Nuevo saldo de la cuenta de destino: %.2f\n", users_list[j].saldo);
                        return;
                    }
                }
            } else {
                printf("Error: Saldo insuficiente en la cuenta de origen.\n");
                return;
            }
        }
    }
}

void consultar_cuenta() {
    int no_cuenta;

    printf("Ingrese el numero de cuenta: ");
    if (scanf("%d", &no_cuenta) != 1 || no_cuenta < 0) {
        printf("Error: El numero de cuenta debe ser un entero mayor o igual a cero.\n");
        clean_stdin();
        return;
    }

    if (!no_cuenta_exists(no_cuenta)) {
        printf("Error: La cuenta no existe.\n");
        return;
    }

    for (int i = 0; i < user_size_const; i++) {
        if (users_list[i].no_cuenta == no_cuenta) {
            printf("Informacion de la cuenta:\n");
            printf("Numero de cuenta: %d\n", users_list[i].no_cuenta);
            printf("Nombre: %s\n", users_list[i].nombre);
            printf("Saldo: %.2f\n", users_list[i].saldo);
            return;
        }
    }
}