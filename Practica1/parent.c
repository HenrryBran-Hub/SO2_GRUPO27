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

    // Ruta del archivo en la carpeta /tmp/practica
    const char* dirpath = "/tmp/practica";
    const char* filepath = "/tmp/practica/practica1.txt";

    // Verificar si la carpeta existe, si no crearla
    struct stat st = {0};
    if (stat(dirpath, &st) == -1) {
        if (mkdir(dirpath, 0777) == -1) {
            perror("Error creando la carpeta /tmp/practica");
            exit(1);
        }
        printf("Carpeta /tmp/practica creada exitosamente.\n");
    } else {
        printf("La carpeta /tmp/practica ya existe.\n");
    }

    // Verificar si el archivo existe
    int fd = open(filepath, O_RDONLY);
    if (fd != -1) {
        printf("El archivo ya existe. Vaciándolo...\n");
        close(fd);

        // Vaciar el archivo
        fd = open(filepath, O_WRONLY | O_TRUNC);
        if (fd == -1) {
            perror("Error vaciando el archivo");
            exit(1);
        } else {
            printf("Archivo vaciado exitosamente.\n");
            close(fd);
        }
    } else {
        // Crear el archivo
        fd = open(filepath, O_CREAT | O_WRONLY, 0666);
        if (fd == -1) {
            perror("Error creando el archivo");
            exit(1);
        } else {
            printf("Archivo creado exitosamente.\n");
            close(fd);
        }
    }

    int pipes[2][2];
    for (int i = 0; i < 2; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid) {
            // Proceso padre
            printf("Soy el proceso padre - %d\n", pid);
            printf("Mi PID es: %d\n\n", getpid());
            close(pipes[i][1]); // Cerrar el extremo de escritura en el padre
        } else {
            // Proceso hijo
            close(pipes[i][0]); // Cerrar el extremo de lectura en el hijo
            char* arg_Ptr[4];
            arg_Ptr[0] = "child.bin";
            char pid_str[10];
            sprintf(pid_str, "%d", getppid()); // Convertir el PID del padre a string
            char pipe_fd_str[10];
            sprintf(pipe_fd_str, "%d", pipes[i][1]); // Convertir el descriptor de pipe a string
            arg_Ptr[1] = pid_str;
            arg_Ptr[2] = pipe_fd_str;
            arg_Ptr[3] = NULL;

            execv("/home/henrry201314439/Documents/SOPES2/SO2_GRUPO27/Practica1/child.bin", arg_Ptr);
        }
    }

    int total_counter = 0;
    int total_read_counter = 0;
    int total_write_counter = 0;

    for (int i = 0; i < 2; ++i) {
        int counter, read_counter, write_counter;
        read(pipes[i][0], &counter, sizeof(int));
        read(pipes[i][0], &read_counter, sizeof(int));
        read(pipes[i][0], &write_counter, sizeof(int));
        total_counter += counter;
        total_read_counter += read_counter;
        total_write_counter += write_counter;
        printf("Contador del hijo %d: %d\n", i + 1, counter);
        printf("Lecturas del hijo %d: %d\n", i + 1, read_counter);
        printf("Escrituras del hijo %d: %d\n", i + 1, write_counter);
        close(pipes[i][0]);
    }

    printf("Contador total: %d\n", total_counter);
    printf("Total lecturas: %d\n", total_read_counter);
    printf("Total escrituras: %d\n", total_write_counter);

    printf("Terminando el proceso padre\n");
    return 0;
}
