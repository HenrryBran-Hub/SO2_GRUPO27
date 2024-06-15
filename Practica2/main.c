#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define user_size_const 100


// Prototipos de funciones
void menu_main();
void upload_user();
void upload_trans();
//void new_user();
//void new_trans();
//void stats();
void read_json_file(char* filename);

// Comprobaciones
int is_number_real(float number);
int no_cuenta_exists(float id);
float two_dots(float numero);
void write_msg_error(int fd, char *field);


//structura de usuario
struct user_struct {
    int no_cuenta;
    char nombre[75];
    float saldo;
};
struct user_struct users_list[user_size_const];
int user_size = 0;
void init_user_list(struct user_struct list[]);

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

    printf("SISTEMA CARGADO CON EXITO.\n");
    printf("----------------------------------------\n");
    menu_main();
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
                //upload_trans;
                break;
            case 3:
                //new_user();
                break;
            case 4:
                //new_trans();
                break;
            case 5:
                //stats();
                break;
            case 6:
                //stats();
                break;
            case 7:
                //stats();
                break;
            case 8:
                printf("Saliendo...\n");
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
    char* filename = "upload/usuarios.json";
    read_json_file(filename);
    for(int i = 0; i < user_size; i++){
        printf("no_cuenta: %d   Nombre: %s   Saldo: %f\n", 
            users_list[i].no_cuenta, users_list[i].nombre, users_list[i].saldo);
    }
}

//lectura de archivo json
void read_json_file(char* filename){

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
                        if (!no_cuenta_exists(no_cuenta -> valueint)) {
                            users_list[number_objects_safe].no_cuenta = no_cuenta -> valueint;
                            sprintf(users_list[number_objects_safe].nombre , "%s", nombre -> valuestring);
                            users_list[number_objects_safe].saldo = two_dots(saldo -> valuedouble);
                            number_objects_safe++;
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
    user_size = number_objects_safe;

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