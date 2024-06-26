# [BD2]Practica2_G27

## Descripcion 
___
### Practica
[Enunciado de la Practica](./Img/SO2_Proyecto.pdf)


### Comandos utilizados en C:
```javascript
// Comandos para C

// Libreria necesaria
sudo apt-get install libmysqlclient-dev

// Para compilar los archivos de C
gcc main.c -o main -lmysqlclient

// Para Ejecutar el programa 
sudo ./main

// paquetes instalados para node js
npm init -y 
npm install express dotenv mysql2 cores morgan

// paquetes instalados para react
npm install react-router-dom
npm install chart.js react-chart.js-2
npm install bootstrap

// para ejecutar las aplicaciones
// node
npm start

// react 
npm start
```

# Explicación basica del codigo

## Arquitectura

![Arquitectura](./Img/Arquitectura.png)

## Base de datos

### Base de datos sopes2 y tabla proces

![DB-Tabla](./Img/DB-Tabla.png)

### Procedure GetTopMemoryProcesses()
- Este proceso calcula el total de memoria para calcular los porcentajes
- Crea una tabla temporal para almacenar los resultados 
- Inserta los top 9 procesos agrupados por PID
- Nombra en la tabla temporal e inserta la suma de memoria y porcentaje de los procesos restantes bajo "Otros".

![Procedure_1](./Img/Procedure_1.png)

![Procedure_1_2](./Img/Procedure_1_2.png)

![Procedure_1_3](./Img/Procedure_1_3.png)

### Procedure ConvertirMemoriaToMB(), este proceso convierte la memoria a MB

![Procedure_3](./Img/Procedure_2.png)

### Modelo entidad relacion

![Diagrama](./Img/Diagrama.png)

## Backend

### Modelo para la conexion a la base de datos

![Back-bd](./Img/Back-bd.png)

### Servidor Node, se configuro el servidor node en el puerto 5000 y este se conecta con BD

![Back-index](./Img/Back-index.png)

### Api end point para obtener los datos de la BD

![Back-index2](./Img/Back-index2.png)

## Front

### Index.html

![Front-html](./Img/Front-html.png)

### App.js

![Front-App](./Img/Front-App.png)

### NavBar.js

![Front-NavBar](./Img/Front-NavBar.png)

### HistoryMonitor.js

![Front-HistoryMonitor](./Img/Front-HistoryMonitor.png)

![Front-HistoryMonitor2](./Img/Front-HistoryMonitor2.png)

### HomePage.js

![Front-Homepage](./Img/Front-Homepage.png)

### RealTimeMonitor.js

![Front-RealMonitor](./Img/Front-RealMonitor.png)

## C

### Estructura para almacenar los datos procesados

![C_1](./Img/C_1.png)

### Variables globales para controlar la ejecución del programa

![C_2](./Img/C_2.png)

### Función para obtener la memoria total del sistema

![C_3](./Img/C_3.png)

### Función para leer y procesar el archivo de registro e insertar en la base de datos

![C_4](./Img/C_4.png)

![C_5](./Img/C_5.png)

![C_6](./Img/C_6.png)

### funcion main()
- Contiene el Manejo de la señal SIGINT (Ctrl+C)
- Crea o trunca el archivo syscalls.log
- Inicializa la conexión MySQL
- Tiene los datos de conexión
- Conecta a la base de datos e Inicia el comando para capturar los registros

![C_7](./Img/C_7.png)

![C_8](./Img/C_8.png)

![C_9](./Img/C_9.png)

# Funcionamiento del programa

### Inicio del programa
![Programa-inicio](./Img/Programa-Inicio.png)

### Grafica y top de procesos
![Programa-grafica](./Img/Programa-grafica.png)

### Tabla de procesos
![Programa-tabla](./Img/Programa-tabla.png)

___
### Integrantes
| Nombre | Carnet | Usuario Git |
|----------|----------|----------|
| Henrry David Bran Velasquez | 201314439 | [Henrry Bran](https://github.com/HenrryBran-Hub) |
| Henry Gabriel Peralta Martinez  | 201712289 | [Henry Peralta](https://github.com/HenryPeralta) |
___
