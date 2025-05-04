import React, { useState, useEffect } from 'react';
import './App.css';
import MapaRuta from './components/MapaRuta';

function App() {
  const [estaciones, setEstaciones] = useState([]);
  const [origen, setOrigen] = useState(null);
  const [destino, setDestino] = useState(null);
  const [ruta, setRuta] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  // Cargar estaciones al inicio
  useEffect(() => {
    const cargarEstaciones = async () => {
      try {
        const res = await fetch('/api/estaciones');
        if (!res.ok) throw new Error('Error al cargar estaciones');
        const data = await res.json();
        
        // Mapear a formato {id, nombre}
        const estacionesFormateadas = data.map((nombre, id) => ({ id, nombre }));
        setEstaciones(estacionesFormateadas);
      } catch (err) {
        setError(err.message);
      }
    };
    cargarEstaciones();
  }, []);

  const buscarRuta = async () => {
    if (!origen || !destino) return;
    
    setLoading(true);
    setError(null);
    try {
      const res = await fetch('/api/ruta', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ 
          origen: origen.id, 
          destino: destino.id 
        })
      });
      
      if (!res.ok) throw new Error('Error al calcular ruta');
      const data = await res.json();
      setRuta(data);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="app-container">
      <h1>Planificador de Rutas - TransMilenio</h1>
      
      <div className="search-panel">
        <div className="select-group">
          <label>Origen:</label>
          <select 
            value={origen?.id || ''}
            onChange={(e) => {
              const est = estaciones.find(est => est.id == e.target.value);
              setOrigen(est);
            }}
          >
            <option value="">Seleccione origen</option>
            {estaciones.map(est => (
              <option key={`origen-${est.id}`} value={est.id}>
                {est.nombre}
              </option>
            ))}
          </select>
        </div>

        <div className="select-group">
          <label>Destino:</label>
          <select 
            value={destino?.id || ''}
            onChange={(e) => {
              const est = estaciones.find(est => est.id == e.target.value);
              setDestino(est);
            }}
          >
            <option value="">Seleccione destino</option>
            {estaciones.map(est => (
              <option key={`destino-${est.id}`} value={est.id}>
                {est.nombre}
              </option>
            ))}
          </select>
        </div>

        <button 
          onClick={buscarRuta} 
          disabled={!origen || !destino || loading}
        >
          {loading ? 'Calculando...' : 'Buscar Ruta'}
        </button>
      </div>

      {error && <div className="error-message">{error}</div>}

      {ruta && (
        <div className="route-result">
          <h3>Ruta recomendada ({ruta.tiempo} minutos)</h3>
          <ol>
            {ruta.estaciones.map((est, idx) => (
              <li key={idx}>
                {est.nombre}
                {est.transbordo && <span className="transfer-badge">Transbordo</span>}
              </li>
            ))}
          </ol>
        </div>
      )}

      <MapaRuta estaciones={estaciones} ruta={ruta} />
    </div>
  );
}

export default App;