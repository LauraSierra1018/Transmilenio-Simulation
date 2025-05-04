import express from 'express';
import cors from 'cors';
import path from 'path';
import { fileURLToPath } from 'url';
import { ejecutarCore } from './utils/coreHandler.js';
import rateLimit from 'express-rate-limit';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();

// Configuración
const PORT = process.env.PORT || 3000;
const API_REQUEST_LIMIT = process.env.API_REQUEST_LIMIT || 100;

// Middlewares
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, '../public')));

// Rate limiting
const limiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutos
  max: API_REQUEST_LIMIT
});
app.use('/api/', limiter);

// Rutas API
app.get('/api/estaciones', async (req, res) => {
  try {
    const data = await ejecutarCore(['listar-estaciones']);
    res.json(JSON.parse(data));
  } catch (error) {
    console.error('Error:', error);
    res.status(500).json({ error: 'Error al obtener estaciones' });
  }
});

app.post('/api/ruta', async (req, res) => {
  try {
    const { origen, destino } = req.body;
    
    if (!origen || !destino) {
      return res.status(400).json({ error: 'Origen y destino son requeridos' });
    }

    const data = await ejecutarCore(['calcular-ruta', origen, destino]);
    res.json(JSON.parse(data));
  } catch (error) {
    console.error('Error:', error);
    res.status(500).json({ error: 'Error al calcular ruta' });
  }
});

// Manejo de errores
app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(500).send('Algo salió mal!');
});

// Iniciar servidor
app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});