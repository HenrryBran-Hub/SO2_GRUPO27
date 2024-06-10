# [BD2]Practica1_G27

## Descripcion 
___
### Practica
[Enunciado de la Practica](./Img/Enunciado%20Practica%201.pdf)

### Indicaciones Basicas :nerd_face:
    * Se utilizo linux mint version 21.2
    * Se instalaron los siguientes paquetes
        - https://community.linuxmint.com/software/view/build-essential
        - https://community.linuxmint.com/software/view/gdb
        - https://community.linuxmint.com/software/view/systemtap
    * Se instalo el visual studio con los paquetes de c/c++ de Microsoft

### Comandos utilizados en C:
```javascript
// Comandos para C
// Para compilar los archivos de C
gcc child.c -o child.bin
gcc child2.c -o child2.bin
gcc parent.c -o parent

// Para Ejecutar el programa 
./parent

// Comando para ver el el numero de proceso de ejecutandose
ps -A | grep firefox

// Para ejecutar el Stapsytem
sudo stap trace.stp <<PID1>> 

// Para ejecutar el Stapsytem y sacar el contenido en el archivo syscalls.log
sudo stap trace.stp <<PID1>>  >> /tmp/practica/syscalls.log
```

### Explicacion basico del codigo

## Archivo parent.c

1. #### Rutas de los archivos creados

![1](./Img/1.png)

2. #### Verificacion de rutas existentes

![2](./Img/2.png)

3. #### Creacion de archivos

![3](./Img/3.png)

4. #### Creacion de procesos hijos o hilos hijos

![4](./Img/4.png)

5. #### Llamado del proceso hijo no. 1

![5](./Img/5.png)

6. #### Llamado del proceso hijo no. 2

![6](./Img/6.png)

7. #### Creacion de SIGNIT (ctrl +c)

![7](./Img/7.png)

8. #### Comprobacion de terminado de procesos

![8](./Img/8.png)

## Archivo child.c

9. #### Generacion de datos alfanumericos

![9](./Img/9.png)

10. #### Funcion de write

![10](./Img/10.png)

11. #### Funcion de read

![11](./Img/11.png)

12. #### Funcion de wait

![12](./Img/12.png)

13. #### Ciclo con las funciones implementadas

![13](./Img/13.png)

## Como funciona el programa

14. #### Creacion de archivos bin y compilado y ejecucion

![14](./Img/14.png)

15. #### Salida del programa

![15](./Img/15.png)

16. #### Archivos creados

![16](./Img/16.png)

17. #### Archivo de practica1.txt

![17](./Img/17.png)

18. #### Archvio de syscalls.log

![18](./Img/18.png)

19. #### SystemTap
SystemTap es una herramienta avanzada diseñada para la instrumentación y el análisis de sistemas Linux. Permite a los desarrolladores y administradores de sistemas inspeccionar y monitorear la actividad del kernel en tiempo real. Con SystemTap, es posible escribir scripts que recopilan datos detallados sobre el funcionamiento del sistema, facilitando la depuración, el monitoreo del rendimiento y la resolución de problemas.

20. #### trace.stp
El archivo trace.stp se utiliza para capturar los procesos del kernel especificados por el archivo parent.c. En este script, se configura la detección de procesos y se monitorean las llamadas al sistema write, read y open. Además, se registra la fecha y hora exactas en que se realizan estas llamadas.

El archivo parent.c se encarga de ejecutar el script trace.stp y almacenar la información recopilada en un archivo llamado syscalls.log.

![19](./Img/19.jpeg)

21. #### syscalls.log
El archivo syscalls.log almacena la información de las llamadas a sistemas detectadas. Cada entrada en este archivo incluye el número del proceso hijo, el tipo de llamada realizada (write, read o open), y la hora exacta en que se ejecutó la llamada.

![20](./Img/20.jpeg)

![21](./Img/21.jpeg)


___
### Integrantes
| Nombre | Carnet | Usuario Git |
|----------|----------|----------|
| Henrry David Bran Velasquez | 201314439 | [Henrry Bran](https://github.com/HenrryBran-Hub) |
| Henry Gabriel Peralta Martinez  | 201712289 | [Henry Peralta](https://github.com/HenryPeralta) |
___

