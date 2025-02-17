#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

volatile sig_atomic_t terminate = 0; 

#define MAX_BUF_SIZE 100

void handle_sigint(int sig) {
    printf("Proceso hijo %d recibió la señal SIGINT\n", getpid());
    terminate = 1;
}

void finish_proces() {
    printf("\n===========================================\n");
    FILE *file;
    char line[MAX_BUF_SIZE];
    int write_count = 0, read_count = 0, open_count = 0;

    file = fopen("/tmp/practica/syscalls.log", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_BUF_SIZE, file) != NULL) {
        if (strstr(line, "write") != NULL) {
            write_count++;
        } else if (strstr(line, "read") != NULL) {
            read_count++;
        } else if (strstr(line, "open") != NULL) {
            open_count++;
        }
    }

    fclose(file);

    printf("Conteo de write: %d\n", write_count);
    printf("Conteo de read: %d\n", read_count);
    printf("Conteo de open: %d\n", open_count);
    printf("Total de procesos: %d\n", write_count + read_count + open_count);
    printf("\n===========================================\n");
}

int main() {

    signal(SIGINT, handle_sigint);

    printf("CONFIGURACION INICIAL COMENZADA.\n\n");

    // Ruta del archivo en la carpeta /tmp/practica
    const char* dirpath = "/tmp/practica";
    const char* filepath_txt = "/tmp/practica/practica1.txt";
    const char* filepath_log = "/tmp/practica/syscalls.log";

    // Verificar si la carpeta existe, si no crearla
    struct stat st = {0};
    if (stat(dirpath, &st) == -1) {
        if (mkdir(dirpath, 0777) == -1) {
            perror("Error creando la carpeta /tmp/practica.\n");
            exit(1);
        }
        printf("Carpeta /tmp/practica creada exitosamente.\n");
    } else {
        printf("La carpeta /tmp/practica ya existe.\n");
    }

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
            perror("Error creando el archivo /tmp/practica/practica1.txt\n");
            exit(1);
        } else {
            printf("Archivo creado exitosamente.\n");
            close(fd);
        }
    }

    // Verificar si el archivo existe
    int fd2 = open(filepath_log, O_RDONLY);
    if (fd2 != -1) {
        printf("El archivo ya existe. Limpiando datos...\n");
        close(fd2);

        // Vaciar el archivo
        fd2 = open(filepath_log, O_WRONLY | O_TRUNC);
        if (fd2 == -1) {
            perror("Error vaciando el archivo.\n");
            exit(1);
        } else {
            printf("Archivo limpio.\n");
            close(fd2);
        }
    } else {
        // Crear el archivo
        fd2 = open(filepath_log, O_CREAT | O_WRONLY, 0666);
        if (fd2 == -1) {
            perror("Error creando el archivo /tmp/practica/syscalls.log\n");
            exit(1);
        } else {
            printf("Archivo creado exitosamente.\n");
            close(fd2);
        }
    }

    printf("CONFIGURACION INICIAL TERMINADA.\n\n");

    pid_t pid1, pid2;
    pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0) {
        // Proceso hijo
        printf("------------------------------------\n");
        printf("Iniciando el proceso Hijo 1, Mi PID es: %d\n", getpid());
        printf("------------------------------------\n\n");
        char* arg_Ptr1[3];
        arg_Ptr1[0] = "child.bin";
        char pid_str1[10];
        sprintf(pid_str1, "%d", getppid()); // Convertir el PID del padre a string
        arg_Ptr1[1] = pid_str1;
        arg_Ptr1[2] = NULL;
        //execv("/home/henry/Sopes2/SO2_GRUPO27/Practica1/child.bin", arg_Ptr1);
        execv("/home/henrry201314439/Documents/SOPES2/SO2_GRUPO27/Practica1/child.bin", arg_Ptr1);

    } else {
        printf("El PID del proceso Hijo 1 es: %d\n", pid1);
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(1);
        }
                
        if (pid2 == 0) {
            // Proceso hijo 2
            printf("------------------------------------\n");
            printf("Iniciando el proceso Hijo 2, Mi PID es: %d\n", getpid());
            printf("------------------------------------\n\n");
            char* arg_Ptr2[3];
            arg_Ptr2[0] = "child.bin";
            char pid_str2[10];
            sprintf(pid_str2, "%d", getppid()); // Convertir el PID del padre a string
            arg_Ptr2[1] = pid_str2;
            arg_Ptr2[2] = NULL;
            //Nota en este el child.bin no hay como necesidad de crear otro archivo child.bin ya que son como independientes cuando se llama
            //osea no tenes necesidad de crear child2.bin ya que son como copias las que se hacen cuando se llama por eso te lo puse como child.bin
            //execv("/home/henry/Sopes2/SO2_GRUPO27/Practica1/child.bin", arg_Ptr2);
            execv("/home/henrry201314439/Documents/SOPES2/SO2_GRUPO27/Practica1/child.bin", arg_Ptr2);

        } else {
            // Proceso padre
            printf("El PID del proceso Hijo 2 es: %d\n", pid2);
            printf("-------------------------------\n");
            printf("Soy el proceso padre, Mi PID es: %d\n", getpid());
            printf("-------------------------------\n\n");

            // Llamar al script de SystemTap y pasar los PID como argumentos                
            //char command[100];
            //sprintf(command, "%s %d %d %s", "sudo stap trace.stp ", pid1, pid2, " >> syscalls.log");
            char command[150];
            sprintf(command, "%s %d %d %s", "sudo stap //home//henrry201314439//Documents//SOPES2//SO2_GRUPO27//Practica1//trace.stp ", pid1, pid2, " >> //tmp/practica//syscalls.log");
            system(command);
        }
    }

    pause();

    // Esperar a que los hijos terminen
    while (!terminate) {
        sleep(1);
    }

    // Enviar señal de terminación a los hijos
    kill(0, SIGINT);

    // Esperar a que todos los hijos terminen
    for (int i = 0; i < 2; ++i) {
        wait(NULL);
    }

    finish_proces();

    printf("Terminando el proceso padre con hijos\n");
    return 0;
}
