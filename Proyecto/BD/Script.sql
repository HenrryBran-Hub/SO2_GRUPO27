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

DELIMITER //

CREATE PROCEDURE GetTopMemoryProcesses()
BEGIN
    -- Crear una tabla temporal para almacenar los resultados
    CREATE TEMPORARY TABLE IF NOT EXISTS TempResults (
        PID VARCHAR(50),
        Nombre VARCHAR(100),
        Memoria DECIMAL(20, 5),
        Porcentaje DECIMAL(20, 5)
    );

    -- Insertar los top 9 procesos en la tabla temporal
    INSERT INTO TempResults (PID, Nombre, Memoria, Porcentaje)
    SELECT PID, Nombre,
           CAST(Memoria / 1024 / 1024 AS DECIMAL(20, 5)) AS Memoria,
           Porcentaje
    FROM Proceso
    ORDER BY Memoria DESC
    LIMIT 9;

    -- Insertar la suma de memoria y porcentaje de los procesos restantes
    INSERT INTO TempResults (PID, Nombre, Memoria, Porcentaje)
    SELECT 'Todos' AS PID, 'Todos' AS Nombre, 
           CAST(SUM(Memoria) / 1024 / 1024 AS DECIMAL(20, 5)) AS Memoria,
           SUM(Porcentaje) AS Porcentaje
    FROM (
        SELECT PID, Nombre, Memoria, Porcentaje,
               ROW_NUMBER() OVER (ORDER BY Memoria DESC) AS row_num
        FROM Proceso
    ) AS ranked
    WHERE row_num > 9;

    -- Seleccionar todos los resultados de la tabla temporal
    SELECT * FROM TempResults;

    -- Eliminar la tabla temporal
    DROP TEMPORARY TABLE TempResults;
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
