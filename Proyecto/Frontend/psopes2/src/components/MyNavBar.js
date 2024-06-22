import React from "react";
import "bootstrap/dist/css/bootstrap.min.css";
import { NavLink } from "react-router-dom";
import "../styles/MyNavBar.css";
const MyNavbar = () => {
  return (
    <div>
      <div className="navbar">
        <NavLink to="/" className="nav-button">
          Inicio
        </NavLink>
      </div>
      <div className="navbar">
        <NavLink to="/realtimemonitor" className="nav-button">
          Monitoreo en Tiempo Real
        </NavLink>
      </div>
      <div className="navbar">
        <NavLink to="/historymonitor" className="nav-button">
          Monitoreo a lo largo del tiempo
        </NavLink>
      </div>
    </div>
  );
};

export default MyNavbar;