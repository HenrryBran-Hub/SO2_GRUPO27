#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>

volatile sig_atomic_t terminate = 0;

void handle_sigint(int sig) {
    terminate = 1;
}

char generate_random_char() {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[rand() % (sizeof(charset) - 1)];
}

void write_random_text(int fd) {
    char buffer[9];
    for (int i = 0; i < 8; ++i) {
        buffer[i] = generate_random_char();
    }
    buffer[8] = '\n';
    lseek(fd, 0, SEEK_END);
    write(fd, buffer, 9);
}

void read_random_text(int fd) {
    char buffer[10];
    buffer[9] = '\0';
    lseek(fd, -9, SEEK_END);
    read(fd, buffer, 9);
    printf("Leído del archivo: %s\n", buffer);
}

void wait_random_time() {
    int random_time = rand() % 3 + 1;
    sleep(random_time);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <pid_del_padre> <pipe_fd>\n", argv[0]);
        exit(1);
    }

    signal(SIGINT, handle_sigint);

    int pipe_fd = atoi(argv[2]);

    printf("Soy el proceso hijo\n");
    printf("Mi PID es: %d\n", getpid());
    printf("PID del proceso padre: %s\n", argv[1]);

    int counter = 0;
    int read_counter = 0;
    int write_counter = 0;

    // Inicializar la semilla para los números aleatorios
    srand(time(NULL) ^ (getpid() << 16));

    // Entrar en un ciclo infinito hasta que se termine manualmente
    while (!terminate) {
        int fd = open("/tmp/practica/practica1.txt", O_RDWR);
        if (fd == -1) {
            perror("Error abriendo el archivo");
            exit(1);
        }

        int random_number = rand() % 10 + 1;
        if (random_number % 2 == 0) {
            write_random_text(fd);
            write_counter++;
        } else {
            read_random_text(fd);
            read_counter++;
        }

        close(fd);
        
        wait_random_time(); // Esperar un tiempo aleatorio después de la operación de lectura o escritura

        counter++;
    }

    write(pipe_fd, &counter, sizeof(int));
    write(pipe_fd, &read_counter, sizeof(int));
    write(pipe_fd, &write_counter, sizeof(int));
    close(pipe_fd);

    printf("Proceso hijo terminado con contador: %d\n", counter);
    printf("Total lecturas: %d\n", read_counter);
    printf("Total escrituras: %d\n", write_counter);

    return 0;
}
