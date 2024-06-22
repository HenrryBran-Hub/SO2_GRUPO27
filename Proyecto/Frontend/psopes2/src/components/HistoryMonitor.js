import { useState, useEffect } from "react";
import NavBar from "./MyNavBar";
import "../styles/HistoryMonitor.css";

const HistoryMonitor = () => {
  const [errorDeConexion, setErrorDeConexion] = useState(false);
  const [msgerror, setMsgError] = useState("");
  const [dataTabla, setDataTabla] = useState([]);
  const [currentPage, setCurrentPage] = useState(1);
  const [rowsPerPage] = useState(10);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch("http://localhost:5000/tabla");
        if (!response.ok) {
          throw new Error("Error al obtener los datos");
        }
        const data = await response.json();
        if (data && Array.isArray(data)) {
          // El endpoint devuelve un array con datos en el primer elemento
          setDataTabla(data[0]); // Asignar los datos al estado
          setErrorDeConexion(false);
          setMsgError("");
        } else {
          throw new Error("Datos recibidos no válidos");
        }
      } catch (error) {
        setErrorDeConexion(true);
        setMsgError("Error de conexión. Inténtelo de nuevo más tarde.");
        console.error("Error de conexión:", error);
      }
    };

    fetchData(); // Llamar a la función para obtener los datos inicialmente

    // Establecer intervalo para llamar al endpoint cada 5 segundos
    const intervalId = setInterval(fetchData, 5000);

    // Limpiar intervalo cuando el componente se desmonte o se vuelva a montar
    return () => clearInterval(intervalId);
  }, []);

  // Función para obtener las filas que se mostrarán en la página actual
  const indexOfLastRow = currentPage * rowsPerPage;
  const indexOfFirstRow = indexOfLastRow - rowsPerPage;
  const currentRows = dataTabla.slice(indexOfFirstRow, indexOfLastRow);

  // Cambiar página
  const paginate = (pageNumber) => setCurrentPage(pageNumber);

  return (
    <div>
      <NavBar />
      <h1 className="titulo_hm">Monitoreo Histórico</h1>
      <div className="container_hm">
        {errorDeConexion && <div className="error-message">{msgerror}</div>}
        <h4 className="titulo-dos_hm">Tabla de Datos</h4>
        <div className="table-responsive">
          <table className="table table-striped">
            <thead>
              <tr>
                <th>ID</th>
                <th>PID</th>
                <th>Llamada</th>
                <th>Tamaño</th>
                <th>Fecha</th>
              </tr>
            </thead>
            <tbody>
              {currentRows.map((item) => (
                <tr key={item.id}>
                  <td>{item.id}</td>
                  <td>{item.PID}</td>
                  <td>{item.Llamada}</td>
                  <td>{item.Tamaño}</td>
                  <td>{new Date(item.Fecha).toLocaleString()}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
        {/* Paginación */}
        <nav>
          <ul className="pagination">
            {Array.from(
              { length: Math.ceil(dataTabla.length / rowsPerPage) },
              (_, index) => (
                <li
                  key={index}
                  className={`page-item ${
                    currentPage === index + 1 ? "active" : ""
                  }`}
                >
                  <button
                    onClick={() => paginate(index + 1)}
                    className="page-link"
                  >
                    {index + 1}
                  </button>
                </li>
              )
            )}
          </ul>
        </nav>
      </div>
    </div>
  );
};

export default HistoryMonitor;
