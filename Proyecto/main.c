#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define LINE_SIZE 256
#define COMMAND "sudo stap trace.stp >> syscalls.log"

// Estructura para almacenar los datos procesados
typedef struct {
    char call[50];
    char pid[10];
    char name[100];
    char length[20];
    char timestamp[100];
} SyscallData;

// Variable global para controlar la ejecución del programa
volatile sig_atomic_t keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

// Función para ejecutar el comando en un hilo separado
void* run_command(void* arg) {
    system(COMMAND);
    return NULL;
}

// Función para leer y procesar el archivo de registro
void* read_log(void* arg) {
    FILE *file;
    char line[LINE_SIZE];
    char *token;
    SyscallData data;

    while (keep_running) {
        file = fopen("syscalls.log", "r");
        if (file == NULL) {
            perror("Error opening file");
            return NULL;
        }

        // Mover al final del archivo
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        // Leer el archivo línea por línea
        while (fgets(line, LINE_SIZE, file) != NULL) {
            // Saltar si la línea está vacía
            if (strlen(line) < 2) continue;

            // Primer split por ';'
            token = strtok(line, ";");
            while (token != NULL) {
                // Segundo split por '-'
                char *subtoken = strchr(token, '-');
                if (subtoken) {
                    subtoken++; // Saltar el '-'
                    if (strncmp(token, "Call", 4) == 0) strcpy(data.call, subtoken);
                    if (strncmp(token, "PID", 3) == 0) strcpy(data.pid, subtoken);
                    if (strncmp(token, "Name", 4) == 0) strcpy(data.name, subtoken);
                    if (strncmp(token, "Length", 6) == 0) strcpy(data.length, subtoken);
                    if (strncmp(token, "Timestamp", 9) == 0) strcpy(data.timestamp, subtoken);
                }
                token = strtok(NULL, ";");
            }

            // Imprimir los datos procesados
            printf("Call: %s\nPID: %s\nName: %s\nLength: %s\nTimestamp: %s\n",
                   data.call, data.pid, data.name, data.length, data.timestamp);
        }

        fclose(file);

        // Esperar un poco antes de leer nuevamente
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t command_thread, log_thread;

    // Manejar la señal SIGINT (Ctrl+C)
    signal(SIGINT, int_handler);

    // Crear y ejecutar el hilo para el comando
    if (pthread_create(&command_thread, NULL, run_command, NULL) != 0) {
        perror("Failed to create command thread");
        return 1;
    }

    // Crear y ejecutar el hilo para leer el archivo de registro
    if (pthread_create(&log_thread, NULL, read_log, NULL) != 0) {
        perror("Failed to create log thread");
        return 1;
    }

    // Esperar a que los hilos terminen
    pthread_join(command_thread, NULL);
    pthread_join(log_thread, NULL);

    return 0;
}
