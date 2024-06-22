#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <mysql/mysql.h>
#include <sys/stat.h>

#define LINE_SIZE 256

// Estructura para almacenar los datos procesados
typedef struct {
    char call[50];
    char pid[10];
    char name[100];
    char length[20];
    char timestamp[100];
    double length_percentage;
} SyscallData;

// Variables globales para controlar la ejecución del programa
volatile sig_atomic_t keep_running = 1;
pid_t child_pid = 0;
pid_t stap_pid = 0;

void int_handler(int sig) {
    keep_running = 0;
    if (child_pid != 0) {
        kill(child_pid, SIGTERM);
    }
    if (stap_pid != 0) {
        kill(stap_pid, SIGTERM);
    }
    exit(0);
}

// Función para obtener la memoria total del sistema
unsigned long obtener_memoria_total() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Error al abrir /proc/meminfo");
        return 0;
    }

    char buffer[256];
    unsigned long mem_total = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (sscanf(buffer, "MemTotal: %lu kB", &mem_total) == 1) {
            break;
        }
    }

    fclose(fp);
    return mem_total;
}

// Función para leer y procesar el archivo de registro e insertar en la base de datos
void read_log(MYSQL *conn) {
    FILE *file;
    char line[LINE_SIZE];
    char *token;
    SyscallData data;
    long last_pos = 0;

    // Obtener la memoria total del sistema
    unsigned long mem_total = obtener_memoria_total();

    if (mem_total == 0) {
        fprintf(stderr, "No se pudo obtener la memoria total del sistema.\n");
        return;
    }

    while (keep_running) {
        file = fopen("syscalls.log", "r");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        // Mover a la última posición conocida
        fseek(file, last_pos, SEEK_SET);

        // Leer la última línea del archivo
        while (fgets(line, LINE_SIZE, file) != NULL && keep_running) {
            last_pos = ftell(file);  // Actualizar la última posición conocida

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
                    if (strncmp(token, "Length", 6) == 0) {
                        strcpy(data.length, subtoken);
                        // Calcular el porcentaje de la longitud respecto a la memoria total
                        unsigned long length = strtoul(data.length, NULL, 10);
                        data.length_percentage = ((double)length / (1024*mem_total)) * 100;
                    }
                    if (strncmp(token, "Timestamp", 9) == 0) strcpy(data.timestamp, subtoken);
                }
                token = strtok(NULL, ";");
            }

            // Convertir timestamp a formato DATETIME compatible con MySQL
            char datetime[100];
            snprintf(datetime, sizeof(datetime), "%s", data.timestamp);

            // Construir la consulta de inserción
            char query[1024];
            snprintf(query, sizeof(query),
                     "INSERT INTO Proceso (PID, Nombre, Llamada, Memoria, Fecha, Porcentaje) "
                     "VALUES (%s, '%s', '%s', %s, '%s', %.5f);",
                     data.pid, data.name, data.call, data.length, datetime, data.length_percentage);

            // Ejecutar la consulta
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Error al insertar datos: %s\n", mysql_error(conn));
            } else {
                printf("Datos insertados correctamente: PID=%s, Nombre=%s, Llamada=%s, Memoria=%s, Fecha=%s, Porcentaje=%.5f\n",
                       data.pid, data.name, data.call, data.length, datetime, data.length_percentage);
            }
        }

        fclose(file);

        // Verificar si debemos seguir ejecutando
        if (!keep_running) break;
        sleep(1);
    }
}

int main() {
    // Manejar la señal SIGINT (Ctrl+C)
    signal(SIGINT, int_handler);

    // Crear o truncar el archivo syscalls.log
    int fd = open("syscalls.log", O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error creando o truncando syscalls.log");
        return 1;
    }
    close(fd);

    // Inicializar la conexión MySQL
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    // Datos de conexión
    char *server = "db-sopes2.c70q6g40ywg4.us-east-1.rds.amazonaws.com";
    char *user = "admin";
    char *password = "P_Sopes2.2024*";
    char *database = "sopes2";

    // Conectar a la base de datos
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return 1;
    }

    // Iniciar el comando para capturar los registros
    stap_pid = fork();
    if (stap_pid == 0) {
        // Proceso hijo para ejecutar stap
        // Ejecutar el comando completo
        execlp("sudo", "sudo", "sh", "-c", "stap trace.stp >> syscalls.log", NULL);
        perror("execlp"); // Si llegamos aquí, hubo un error
        exit(1);
    } else if (stap_pid < 0) {
        perror("fork para stap");
        exit(1);
    }


    // Crear proceso hijo para leer y procesar el archivo de registro
    child_pid = fork();
    if (child_pid == -1) {
        perror("Error al hacer fork");
        mysql_close(conn);
        return 1;
    }

    if (child_pid == 0) {
        // Proceso hijo: Leer el archivo de registro e insertar en la base de datos
        read_log(conn);
        exit(0);
    } else {
        // Proceso padre: Esperar a que los hijos terminen o manejar señales
        int status;
        pid_t wpid;
        while ((wpid = wait(&status)) > 0) {
            if (wpid == child_pid) {
                printf("Proceso de lectura terminado\n");
            } else if (wpid == stap_pid) {
                printf("Proceso stap terminado\n");
            }
        }

        // Asegurarse de que ambos procesos hayan terminado
        kill(child_pid, SIGTERM);
        kill(stap_pid, SIGTERM);

        // Cerrar la conexión MySQL
        mysql_close(conn);
    }

    return 0;
}