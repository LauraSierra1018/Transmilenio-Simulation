// Usando react-select
import Select from 'react-select';

function BuscadorEstaciones({ estaciones, onChange }) {
  const opciones = estaciones.map(est => ({
    value: est.id,
    label: est.nombre
  }));

  return (
    <Select 
      options={opciones}
      onChange={onChange}
      placeholder="Buscar estación..."
      isSearchable
      noOptionsMessage={() => "No se encontraron estaciones"}
    />
  );
}