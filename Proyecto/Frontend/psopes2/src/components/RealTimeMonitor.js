import { useState, useEffect, useRef } from "react";
import React from "react";
import NavBar from "./MyNavBar";
import "../styles/RealTimeMonitor.css";
import Chart from "chart.js/auto";

const RealTimeMonitor = () => {
  const [procesos, setProcesos] = useState([]);
  const [errorDeConexion, setErrorDeConexion] = useState(false);
  const [msgerror, setMsgError] = useState("");

  const procesosRef = useRef(null);

  useEffect(() => {
    const intervalId = setInterval(() => {
      fetch("http://localhost:5000/grafico")
        .then((response) => response.json())
        .then((data) => {
          // Reiniciar el estado de error de conexión si la solicitud es exitosa
          setErrorDeConexion(false);
          setMsgError("");

          // Filtrar y formatear los datos de procesos
          const procesosData = data[0].slice(0, 10); // Tomar solo los primeros 10 procesos
          setProcesos(procesosData);

          console.log(data);
        })
        .catch((error) => {
          // Manejar errores de conexión
          setErrorDeConexion(true);
          setMsgError("Error de conexión. Inténtelo de nuevo más tarde.");
          console.error("Error de conexión:", error);
        });
    }, 2500);

    return () => clearInterval(intervalId);
  }, []);

  useEffect(() => {
    // Crear gráfico de dona para procesos
    if (procesos.length > 0) {
      const ctx = document.getElementById("donaChartProcesos").getContext("2d");
      procesosRef.current = new Chart(ctx, {
        type: "doughnut",
        data: {
          labels: procesos.map((proceso) => proceso.Nombre),
          datasets: [
            {
              label: "Porcentaje de Memoria",
              data: procesos.map((proceso) => proceso.Memoria),
              backgroundColor: ["#36a2eb", "#ff6384", "#ffcd56", "#4bc0c0", "#9966ff", "#ff7f50", "#66cdaa", "#f4a460", "#a9a9a9", "#d8bfd8"],
            },
          ],
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          plugins: {
            legend: {
              position: "bottom",
            },
            tooltip: {
              callbacks: {
                label: function (tooltipItem) {
                  return tooltipItem.label + ': ' + tooltipItem.raw + ' MB';
                }
              }
            }
          },
        },
      });
    }

    // Devuelve una función de limpieza para detener el intervalo cuando el componente se desmonta
    return () => {
      if (procesosRef.current) {
        procesosRef.current.destroy();
      }
    };
  }, [procesos]);

  // Función para renderizar la tabla de procesos
  const renderTablaProcesos = () => {
    return (
      <table className="table table-bordered">
        <thead>
          <tr>
            <th>PID</th>
            <th>Nombre</th>
            <th>Memoria</th>
            <th>Porcentaje</th>
          </tr>
        </thead>
        <tbody>
          {procesos.map((proceso, index) => (
            <tr key={index}>
              <td>{proceso.PID}</td>
              <td>{proceso.Nombre}</td>
              <td>{proceso.Memoria}MB</td>
              <td>{proceso.Porcentaje}%</td>
            </tr>
          ))}
        </tbody>
      </table>
    );
  };

  return (
    <div>
    <NavBar />
    <h1 className="titulo_rm">Monitoreo en Tiempo Real</h1>
    <div className="container">
      {errorDeConexion && <div className="error-message">{msgerror}</div>}
      <h2 className="titulo_rm">PROCESOS</h2>
      <div className="row">
        <div className="col">
          
          <div className="chart-container_rm">
            <canvas id="donaChartProcesos"></canvas>
          </div>
        </div>
        <div className="col">
          <div className="table table-hover">
            {renderTablaProcesos()}
          </div>
        </div>
      </div>
      
    </div>
  </div>
  
  );
};

export default RealTimeMonitor;
