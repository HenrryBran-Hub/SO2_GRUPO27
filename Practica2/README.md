# [BD2]Practica2_G27

## Descripcion 
___
### Practica
[Enunciado de la Practica](./Img/Enunciado%20Practica%202%20-%20Junio.pdf)


### Comandos utilizados en C:
```javascript
// Comandos para C
// Para compilar los archivos de C
gcc main.c cJSON.c -o main -lm

// Para Ejecutar el programa 
./main
```

# Explicación basica del codigo

## Archivo main.c

1. #### Librerias utilizadas 

![1](./Img/1.png)

2. #### Funciones utilizadas para el procesamiento de datos

![2](./Img/2.png)

3. #### Estructuras utilizda para crear al usuario y su lista

![3](./Img/3.png)

4. #### Hilo utlizado para la carga de datos en el sistema, tambien su estructura

![4](./Img/4.png)

5. #### Se tiene la funcion main donde cargamos al sistema los elementos imporatentes, creacion de carpetas upload

![5](./Img/5.png)

6. #### Creacion de la carpeta logs

![6](./Img/6.png)

7. #### Creacion de la carpeta backups

![7](./Img/7.png)

8. #### inicializacion de la lista de usuarios para limpiarla, tambien se inicia el hilo de los usuaiors, asi como la inicializacion del mutex, llamada del menu principal y por ultimo la destruccion del mutex

![8](./Img/8.png)

9. #### Menu principal de la funcion en el main

![9](./Img/9.png)

10. #### Limpiamos la lista de usuarios ponemos marcadores negativos

![10](./Img/10.png)

11. #### Funcion para la creacion de los hilos, se crean 3 hilos y se crea un recods de tamaño 3 para el conteo de los usuarios cargados por cada hilo, se crean los tres hilos con sus argumentos donde se tendran el numero de hilo y los usuarios cargados al sistema, tambien desde un inicio se almacena una pedazo de memoria con malloc para los argumentos, por ultimo se tiene la ejecucion de los hilos y su espera hasta que terminen

![11](./Img/11.png)

12. #### Mostramos el contenido de la carga de los hilos y el reporte en general de la carga de usuarios

![12](./Img/12.png)

13. #### Esta funcion es donde mandamos a llamar el proceso para leer el json y cargar los datos al sistema, en este casteamos el argumento, se pasan los datos que se tiene en el parametro se manda a llamar la funcion para leer el json y se libera la memria de los argumentos pasados

![13](./Img/13.png)

14. #### Esta funcion basicamente lee el archivo log de erores para los usuarios y se imprimi en pantalla

![14](./Img/14.png)

15. #### Esta funcion es donde leemos el json, primero creamos el archivo de logs de errores, 

![15](./Img/15.png)

16. #### Se verifica que se cree bien el archivo

![16](./Img/16.png)

17. #### En esta parte leemos el archivo json y lo pasamos a un buffer parseamos el json

![17](./Img/17.png)

18. #### Se reccore el arreglo que nos da el json, declaramos unos contadores y se hace un residuo entre tres por los tres hilos y si cumple con el id del hilo se ejecuta en caso contrario suma y salta

![18](./Img/18.png)

19. #### Se obtiene los objetos del json

![19](./Img/19.png)

20. #### En esta parte se hacen las comprobaciones generales para el usuario y si cumple todas se ejecuta si no salta a los errores

![20](./Img/20.png)

21. #### Buscamos cuales fueron los errores para el objeto

![21](./Img/21.png)

22. #### Funcines generales para la comprobacion

![22](./Img/22.png)

23. #### Se crea un json apartir de la structura del usuario

![23](./Img/23.png)

24. #### Funcin para escribiri el archivo de usuarios

![24](./Img/24.png)

25. #### Funcion donde llamamos la escritura de nuestro archivo usuarios.json

![25](./Img/25.png)

26. #### Funcion para limpiar el area antes de ingresar un valor

![26](./Img/26.png)

27. #### Funcion para crear un usuario con sus condiciones

![27](./Img/27.png)

28. #### Menu secundario para el informe

![28](./Img/28.png)

29. #### Menu para las operaciones con el usuario

![29](./Img/29.png)

30. #### Funcion para el depositio en solitario

![30](./Img/30.png)

31. #### Funcion para el retiro en solitario

![31](./Img/31.png)

32. #### Fucion para las tranferencias en solitario

![32](./Img/32.png)

33. #### Parte de la funcion de transferencia, con comprobacions

![33](./Img/33.png)

34. #### Funcion para la consulta de una cuenta

![34](./Img/34.png)

# Funcionamiento del programa

35. #### Menu principal

![35](./Img/35.png)

36. #### Carpetas creadas por el sistema

![36](./Img/36.png)

37. #### Opcion de carga de usuarios con su reporte

![37](./Img/37.png)

38. #### Opcion de creacion de backup archivo usuarios.json

![38](./Img/38.png)

39. #### Salida del archivo usuarios.json

![39](./Img/39.png)

40. #### Archivo subido al sistema 

![40](./Img/40.png)

41. #### Opcion de crear una nueva cuenta la opcion 5

![41](./Img/41.png)

42. #### Opcion de deposito la opcion 1 de este menu

![42](./Img/42.png)

43. #### Opcion de retiro la opcion 2 de este menu

![43](./Img/43.png)

44. #### Opcion de transferencia la opcion 3 de este menu

![44](./Img/44.png)

45. #### Consulta general de los datos de usuario

![45](./Img/45.png)

46. #### Salida del programa con opcion 8

![46](./Img/46.png)
___
### Integrantes
| Nombre | Carnet | Usuario Git |
|----------|----------|----------|
| Henrry David Bran Velasquez | 201314439 | [Henrry Bran](https://github.com/HenrryBran-Hub) |
| Henry Gabriel Peralta Martinez  | 201712289 | [Henry Peralta](https://github.com/HenryPeralta) |
___

