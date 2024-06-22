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
  `Fecha` DATETIME NOT NULL,
  `Porcentaje` DECIMAL(10,5) NOT NULL,
  PRIMARY KEY (id)
)ENGINE = InnoDB;

-- Insertando datos de ejemplo en la tabla Proceso
INSERT INTO Proceso (PID, Nombre, LLamada, Memoria, Fecha, Porcentaje) VALUES
('466589', 'firefox', 'MAP', '12365478', '2024-06-21 08:00:00', '5'),
('789012', 'chrome', 'UNMAP', '98765432', '2024-06-20 15:30:00', '8'),
('123456', 'edge', 'MAP', '54321098', '2024-06-19 12:45:00', '4'),
('987654', 'notepad', 'UNMAP', '24680135', '2024-06-18 10:00:00', '2'),
('234567', 'vscode', 'MAP', '13579246', '2024-06-17 14:20:00', '6'),
('876543', 'sublime', 'UNMAP', '86420973', '2024-06-16 09:30:00', '3'),
('345678', 'excel', 'MAP', '97531024', '2024-06-15 11:00:00', '7'),
('765432', 'word', 'UNMAP', '35792468', '2024-06-14 16:45:00', '9'),
('456789', 'powerpoint', 'MAP', '46813579', '2024-06-13 13:10:00', '5'),
('654321', 'outlook', 'UNMAP', '79246810', '2024-06-12 08:20:00', '4'),
('567890', 'notepad++', 'MAP', '80246879', '2024-06-11 10:30:00', '6'),
('543210', 'discord', 'UNMAP', '63218745', '2024-06-10 14:15:00', '3'),
('678905', 'skype', 'MAP', '98765123', '2024-06-09 12:00:00', '7'),
('432109', 'zoom', 'UNMAP', '51482097', '2024-06-08 09:45:00', '2'),
('789065', 'slack', 'MAP', '35648210', '2024-06-07 15:20:00', '8'),
('210987', 'teams', 'UNMAP', '71420983', '2024-06-06 11:30:00', '1'),
('543289', 'photoshop', 'MAP', '96543127', '2024-06-05 13:40:00', '9'),
('987654', 'illustrator', 'UNMAP', '75319824', '2024-06-04 10:15:00', '6'),
('123890', 'indesign', 'MAP', '82345671', '2024-06-03 14:50:00', '3'),
('876543', 'premiere', 'UNMAP', '43210987', '2024-06-02 08:25:00', '5'),
('234567', 'aftereffects', 'MAP', '63147892', '2024-06-01 16:00:00', '7'),
('765432', 'lightroom', 'UNMAP', '92017364', '2024-05-31 12:20:00', '4'),
('456789', 'audition', 'MAP', '51928734', '2024-05-30 14:35:00', '8'),
('654321', 'dreamweaver', 'UNMAP', '79283164', '2024-05-29 09:40:00', '2'),
('567890', 'bridge', 'MAP', '80236481', '2024-05-28 11:55:00', '6'),
('543210', 'acrobat', 'UNMAP', '63218759', '2024-05-27 15:10:00', '1'),
('678905', 'reader', 'MAP', '98765123', '2024-05-26 12:30:00', '9'),
('432109', 'chrome', 'UNMAP', '51482097', '2024-05-25 10:45:00', '3'),
('789065', 'firefox', 'MAP', '35648210', '2024-05-24 14:20:00', '7'),
('210987', 'edge', 'UNMAP', '71420983', '2024-05-23 11:00:00', '5'),
('543289', 'vscode', 'MAP', '96543127', '2024-05-22 13:15:00', '8'),
('987654', 'sublime', 'UNMAP', '75319824', '2024-05-21 09:30:00', '5'),
('123890', 'notepad', 'MAP', '82345671', '2024-05-20 14:00:00', '6'),
('876543', 'excel', 'UNMAP', '43210987', '2024-05-19 08:45:00', '2'),
('234567', 'word', 'MAP', '63147892', '2024-05-18 16:10:00', '9'),
('765432', 'powerpoint', 'UNMAP', '92017364', '2024-05-17 12:25:00', '3'),
('456789', 'outlook', 'MAP', '51928734', '2024-05-16 14:40:00', '7'),
('654321', 'notepad++', 'UNMAP', '79283164', '2024-05-15 09:50:00', '1'),
('567890', 'skype', 'MAP', '80236481', '2024-05-14 11:55:00', '8'),
('543210', 'zoom', 'UNMAP', '63218759', '2024-05-13 15:10:00', '6'),
('678905', 'slack', 'MAP', '98765123', '2024-05-12 12:30:00', '4'),
('432109', 'teams', 'UNMAP', '51482097', '2024-05-11 10:45:00', '9'),
('789065', 'discord', 'MAP', '35648210', '2024-05-10 14:20:00', '5'),
('210987', 'skype', 'UNMAP', '71420983', '2024-05-09 11:00:00', '3'),
('543289', 'slack', 'MAP', '96543127', '2024-05-08 13:15:00', '7'),
('987654', 'teams', 'UNMAP', '75319824', '2024-05-07 09:30:00', '2'),
('123890', 'discord', 'MAP', '82345671', '2024-05-06 14:00:00', '8'),
('876543', 'photoshop', 'UNMAP', '43210987', '2024-05-05 08:45:00', '6'),
('234567', 'illustrator', 'MAP', '63147892', '2024-05-04 16:10:00', '1'),
('765432', 'indesign', 'UNMAP', '92017364', '2024-05-03 12:25:00', '9'),
('456789', 'premiere', 'MAP', '51928734', '2024-05-02 14:40:00', '5');

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