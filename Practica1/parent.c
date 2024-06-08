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

void handle_sigint(int sig) {
    terminate = 1;
}

int main() {
    signal(SIGINT, handle_sigint);

    printf("CONFIGURACION INICIAL COMENZADA.\n\n");

    // Ruta del archivo en la carpeta /tmp/practica
    const char* dirpath = "/tmp/practica";
    const char* filepath = "/tmp/practica/practica1.txt";

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
    int fd = open(filepath, O_RDONLY);
    if (fd != -1) {
        printf("El archivo ya existe. Limpiando datos...\n");
        close(fd);

        // Vaciar el archivo
        fd = open(filepath, O_WRONLY | O_TRUNC);
        if (fd == -1) {
            perror("Error vaciando el archivo.\n");
            exit(1);
        } else {
            printf("Archivo limpio.\n");
            close(fd);
        }
    } else {
        // Crear el archivo
        fd = open(filepath, O_CREAT | O_WRONLY, 0666);
        if (fd == -1) {
            perror("Error creando el archivo /tmp/practica/practica1.txt\n");
            exit(1);
        } else {
            printf("Archivo creado exitosamente.\n");
            close(fd);
        }
    }

    printf("CONFIGURACION INICIAL TERMINADA.\n\n");

    for (int i = 0; i < 2; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid) {
            // Proceso padre
            printf("-------------------------------\n");
            printf("Soy el proceso padre, Mi PID es: %d\n", getpid());
            printf("-------------------------------\n\n");
        } else {
            // Proceso hijo
            printf("------------------------------------\n");
            printf("Iniciando el proceso Hijo, Mi PID es: %d\n", getpid());
            printf("------------------------------------\n\n");
            char* arg_Ptr[4];
            arg_Ptr[0] = "child.bin";
            char pid_str[10];
            sprintf(pid_str, "%d", getppid()); // Convertir el PID del padre a string
            char pipe_fd_str[10];
            sprintf(pipe_fd_str, "%d", i); // Convertir el descriptor de pipe a string
            arg_Ptr[1] = pid_str;
            arg_Ptr[2] = pipe_fd_str;
            arg_Ptr[3] = NULL;

            execv("/home/henrry201314439/Documents/SOPES2/SO2_GRUPO27/Practica1/child.bin", arg_Ptr);
        }
    }

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

    printf("Terminando el proceso padre con hijos\n");
    return 0;
}
