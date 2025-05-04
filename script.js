document.addEventListener('DOMContentLoaded', async () => {
    const origenSelect = document.getElementById('origen');
    const destinoSelect = document.getElementById('destino');
    const resultadoDiv = document.getElementById('resultado');
    const rutaForm = document.getElementById('rutaForm');
  
    // Cargar las estaciones al cargar la página
    try {
      const response = await fetch('/api/estaciones');
      const estaciones = await response.json();
  
      // Rellenar las opciones de estación
      estaciones.forEach(estacion => {
        const option = document.createElement('option');
        option.value = estacion.nombre;
        option.textContent = estacion.nombre;
        origenSelect.appendChild(option);
        destinoSelect.appendChild(option.cloneNode(true));  // Crear opción similar para destino
      });
    } catch (error) {
      console.error('Error al cargar las estaciones:', error);
    }
  
    // Manejar el envío del formulario
    rutaForm.addEventListener('submit', async (e) => {
      e.preventDefault();
  
      const origen = origenSelect.value;
      const destino = destinoSelect.value;
  
      if (!origen || !destino) {
        resultadoDiv.textContent = 'Por favor, selecciona ambas estaciones.';
        return;
      }
  
      try {
        const response = await fetch('/api/ruta', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ origen, destino })
        });
  
        const data = await response.json();
        resultadoDiv.textContent = `La ruta entre ${origen} y ${destino} es: ${data.ruta}`;
      } catch (error) {
        resultadoDiv.textContent = 'Error al calcular la ruta.';
        console.error('Error al calcular la ruta:', error);
      }
    });
  });
  