const express = require('express');
const db = require('./db');
const cors = require('cors');
const morgan = require('morgan'); // Importa morgan

const app = express();

// Middleware para habilitar CORS (Cross-Origin Resource Sharing)
app.use(cors());

// Middleware de logging con morgan
app.use(morgan('dev'));

// Endpoint para llamar al procedimiento almacenado (GET)
app.get('/grafico', async (req, res) => {
    try {
        const connection = await db.getConnection();
        const [rows] = await connection.execute('CALL GetTopMemoryProcesses();');
        connection.release();
        res.json(rows); // asumiendo que el quinto resultado es el total
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Error al ejecutar el procedimiento almacenado.' });
    }
});

// Endpoint para llamar a la función definida (GET)
app.get('/tabla', async (req, res) => {
    try {
        const connection = await db.getConnection();
        const [rows] = await connection.execute('CALL ConvertirMemoriaToMB();');
        connection.release();
        res.json(rows);
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Error al ejecutar el procedimiento almacenado.' });
    }
});

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
    console.log(`Servidor escuchando en puerto ${PORT}`);
});
