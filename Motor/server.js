const http = require("http");
const fs = require("fs");
const express = require("express");
const cors = require("cors");

// Variable para almacenar el índice invertido
let invertedIndex = null;
let pageRank = null;

// Función para cargar el contenido del archivo InvertedIndex.txt y construir el índice invertido
const cargarInvertedIndex = () => {
  return new Promise((resolve, reject) => {
    fs.readFile("./input/InvertedIndex.txt", "utf8", (err, data) => {
      if (err) {
        reject(err);
      } else {
        invertedIndex = construirInvertedIndex(data);
        console.log("Inverted Index Ready");
        resolve();
      }
    });
  });
};

// Función para construir el Inverted Index a partir del contenido del archivo
const construirInvertedIndex = (contenido) => {
  const index = {};
  const lineas = contenido.split("\n");
  lineas.forEach((linea) => {
    const partes = linea.split(/\s+/);
    const palabra = partes[0];
    partes.slice(1).forEach((archivo) => {
      if (!index[palabra]) {
        index[palabra] = [];
      }
      if (!index[palabra].includes(archivo)) {
        index[palabra].push(archivo);
      }
    });
  });
  return index;
};

const cargarPageRank = () => {
  return new Promise((resolve, reject) => {
    fs.readFile("./input/PageRank.txt", "utf8", (err, data) => {
      if (err) {
        console.error("Error al leer el archivo PageRank.txt:", err);
        reject(err); // Rechazar la promesa en caso de error
        return;
      }

      // Procesar los datos del archivo
      const lines = data.split("\n");
      const pageRankData = {};

      lines.forEach((line) => {
        const linestream = line.trim().split(" ");
        file = linestream[0]
        rank = linestream[1]
        pageRankData[file] = parseInt(rank);
      });
      pageRank = pageRankData;
      console.log("Page Rank Ready");

      resolve(); // Resolver la promesa una vez que se hayan cargado los datos
    });
  });
};

const app = express();

// Configurar CORS
app.use(
  cors({
    allowedHeaders: ["Content-Type", "palabra"],
  })
);

// Cargar el índice invertido al iniciar el servidor
cargarInvertedIndex()
  .then(() => cargarPageRank()) // Llamamos a cargarPageRank como una función dentro de then
  .then(() => {
    // Iniciar el servidor en el puerto 3000
    const PORT = 3000;
    app.listen(PORT, () => {
      console.log(`Servidor escuchando en el puerto ${PORT}`);
    });
    // console.log(invertedIndex);
    // console.log(pageRank);
  })
  .catch((err) => {
    console.error("Error al cargar el índice invertido:", err);
  });

app.options("/consultar", cors());

// Crear el servidor HTTP
app.get("/consultar", (req, res) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  // Permitir métodos y encabezados específicos
  res.setHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  res.setHeader("Access-Control-Allow-Headers", "Content-Type");

  // Manejar solicitudes OPTIONS
  if (req.method === "OPTIONS") {
    res.writeHead(200);
    res.end();
    return;
  }

  if (req.method === "GET" && req.url === "/consultar") {
    const palabra = req.headers["palabra"];
    console.log("Query for word: ", palabra);
    if (!palabra) {
      res.writeHead(200, { "Content-Type": "application/json" });
      res.end(JSON.stringify({ archivos: [] }));
      return;
    }
    if (!invertedIndex) {
      res.writeHead(200, { "Content-Type": "application/json" });
      res.end(JSON.stringify({ archivos: [] }));
      return;
    }
    if (invertedIndex[palabra]) {
      res.writeHead(200, { "Content-Type": "application/json" });
      const sortedByRank = invertedIndex[palabra]
        .filter((archivo) => archivo !== "") // Filtrar los archivos con nombre no vacío
        .map((archivo) => {
          return { nombre: archivo, pesoPageRank: pageRank[archivo] };
        })
        .sort((a, b) => {
          return b.pesoPageRank - a.pesoPageRank;
        });
      res.end(JSON.stringify({ archivos: sortedByRank }));
    } else {
      res.writeHead(200, { "Content-Type": "application/json" });
      res.end(JSON.stringify({ archivos: [] }));
    }
  } else {
    res.writeHead(404, { "Content-Type": "application/json" });
    res.end(JSON.stringify({ mensaje: "Ruta no encontrada" }));
  }
});
