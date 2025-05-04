// En tu componente React
import { MapContainer, TileLayer, Marker, Popup, Polyline } from 'react-leaflet';

function MapaRuta({ estaciones }) {
  return (
    <MapContainer center={[4.6097, -74.0817]} zoom={12}>
      <TileLayer
        url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
      />
      {estaciones.map(est => (
        <Marker key={est.id} position={[est.lat, est.lng]}>
          <Popup>{est.nombre}</Popup>
        </Marker>
      ))}
    </MapContainer>
  );
}