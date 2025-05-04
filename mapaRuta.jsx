import React from 'react';
import { MapContainer, TileLayer, Marker, Popup, Polyline } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';

// Fix para iconos
delete L.Icon.Default.prototype._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-shadow.png',
});

// Coordenadas aproximadas de Bogotá para centrar el mapa
const CENTRO_BOGOTA = [4.6097, -74.0817];
const ZOOM_INICIAL = 12;

// Simulamos coordenadas (en un sistema real deberías tenerlas en tus datos)
const generarCoordenadas = (id, total) => {
  const lat = CENTRO_BOGOTA[0] + (Math.random() * 0.1 - 0.05);
  const lng = CENTRO_BOGOTA[1] + (Math.random() * 0.1 - 0.05);
  return [lat, lng];
};

export default function MapaRuta({ estaciones, ruta }) {
  // Asignamos coordenadas simuladas a cada estación
  const estacionesConCoords = estaciones.map(est => ({
    ...est,
    coords: generarCoordenadas(est.id, estaciones.length)
  }));

  // Generar línea para la ruta
  const lineaRuta = ruta?.estaciones.map(est => {
    const estacion = estacionesConCoords.find(e => e.id === est.id);
    return estacion?.coords || CENTRO_BOGOTA;
  });

  return (
    <div className="mapa-container">
      <MapContainer center={CENTRO_BOGOTA} zoom={ZOOM_INICIAL} scrollWheelZoom={true}>
        <TileLayer
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
          attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
        />

        {estacionesConCoords.map(est => (
          <Marker key={`marker-${est.id}`} position={est.coords}>
            <Popup>
              <strong>{est.nombre}</strong>
              <br/>
              ID: {est.id}
            </Popup>
          </Marker>
        ))}

        {lineaRuta && (
          <Polyline 
            positions={lineaRuta}
            color="blue"
            weight={5}
            opacity={0.7}
          />
        )}
      </MapContainer>
    </div>
  );
}