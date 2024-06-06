-- Crear la tabla de entrada
CREATE EXTERNAL TABLE IF NOT EXISTS input_table (
  line STRING
)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '\n'
LOCATION 's3://aws-logs-683656757445-us-east-1/Input/';

-- Crear la tabla para almacenar los resultados
CREATE TABLE IF NOT EXISTS word_counts (
  word STRING,
  count INT
);

-- Ejecutar el conteo de palabras
INSERT OVERWRITE TABLE word_counts
SELECT word, COUNT(1) as count
FROM (
  SELECT explode(split(line, ' ')) as word
  FROM input_table
) a
GROUP BY word;
