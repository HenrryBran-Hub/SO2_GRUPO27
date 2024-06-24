CREATE DATABASE sopes2;
USE sopes2;

-- -----------------------------------------------------
-- CREACION TABLA PACIENTE
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Proceso`(
  `id` INT NOT NULL AUTO_INCREMENT,
  `PID` BIGINT NOT NULL,
  `Nombre` VARCHAR(100) NOT NULL,
  `LLamada` VARCHAR(10) NOT NULL,
  `Memoria` BIGINT NOT NULL,
  `Fecha` VARCHAR(100) NOT NULL,
  `Porcentaje` DECIMAL(10,5) NOT NULL,
  PRIMARY KEY (id)
)ENGINE = InnoDB;

DROP PROCEDURE GetTopMemoryProcesses;

DELIMITER //

CREATE PROCEDURE GetTopMemoryProcesses()
BEGIN
    DECLARE totalMemoria DECIMAL(20, 5);
    
    -- Calcular el total de memoria para calcular los porcentajes después
    SET totalMemoria = (
    SELECT SUM(GREATEST((CASE WHEN LLamada LIKE 'MMAP' THEN Memoria ELSE -Memoria END),0))
    FROM Proceso
	);

    -- Crear una tabla temporal para almacenar los resultados
    CREATE TEMPORARY TABLE IF NOT EXISTS TempResults (
        PID VARCHAR(50),
        Nombre VARCHAR(100),
        Memoria DECIMAL(20, 5),
        Porcentaje DECIMAL(20, 5)
    );

    -- Insertar los top 9 procesos agrupados por PID, Nombre en la tabla temporal
    INSERT INTO TempResults (PID, Nombre, Memoria, Porcentaje)
    SELECT PID, Nombre,
           GREATEST(CAST(SUM(CASE WHEN LLamada LIKE 'MMAP' THEN Memoria ELSE -Memoria END) / 1024 / 1024 AS DECIMAL(20, 5)), 0) AS Memoria,
           GREATEST(SUM(CASE WHEN LLamada LIKE 'MMAP' THEN Memoria ELSE -Memoria END) / totalMemoria * 100, 0) AS Porcentaje
    FROM Proceso 
    GROUP BY PID, Nombre
    ORDER BY SUM(CASE WHEN LLamada LIKE 'MMAP' THEN Memoria ELSE -Memoria END) DESC    
    LIMIT 10;

    -- Emulación de ROW_NUMBER() usando variables de usuario
    SET @row_num := 0;
    CREATE TEMPORARY TABLE ranked (
        PID VARCHAR(50),
        Nombre VARCHAR(100),
        Memoria DECIMAL(20, 5),
        row_num INT
    );

    INSERT INTO ranked (PID, Nombre, Memoria, row_num)
    SELECT PID, Nombre,
           GREATEST(SUM(CASE WHEN LLamada LIKE 'MMAP' THEN Memoria ELSE -Memoria END), 0) AS Memoria,
           @row_num := @row_num + 1 AS row_num
    FROM Proceso
    GROUP BY PID, Nombre
    ORDER BY Memoria DESC;

    -- Insertar la suma de memoria y porcentaje de los procesos restantes bajo "Otros"
    INSERT INTO TempResults (PID, Nombre, Memoria, Porcentaje)
    SELECT 'Todos' AS PID, 'Otros' AS Nombre, 
           GREATEST(CAST(SUM(Memoria) / 1024 / 1024 AS DECIMAL(20, 5)), 0) AS Memoria,
           GREATEST(SUM(Memoria) / totalMemoria * 100, 0) AS Porcentaje
    FROM ranked
    WHERE row_num > 12;

    -- Seleccionar todos los resultados de la tabla temporal
    SELECT * FROM TempResults;

    -- Eliminar las tablas temporales
    DROP TEMPORARY TABLE TempResults;
    DROP TEMPORARY TABLE ranked;
END //

DELIMITER ;


DELIMITER //

CREATE PROCEDURE ConvertirMemoriaToMB()
BEGIN
    SELECT
        id,
        PID, 
        Llamada, 
        CONCAT(
            FORMAT(CONVERT(Memoria / 1024 / 1024, DECIMAL(20, 5)), 5),
            ' MB'
        ) AS Tamaño,
        Fecha
    FROM Proceso
    ORDER BY Fecha DESC;
END //

DELIMITER ;


CALL ConvertirMemoriaToMB();
CALL GetTopMemoryProcesses();
   