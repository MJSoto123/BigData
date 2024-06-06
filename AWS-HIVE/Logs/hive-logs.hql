-- Crear la tabla externa de logs
CREATE EXTERNAL TABLE IF NOT EXISTS logs (
  user_a STRING,
  time STRING,
  query STRING
)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY ','
LOCATION 's3://aws-logs-683656757445-us-east-1/Input/Logs/';

-- Crear la tabla de resultados y guardar en S3
CREATE TABLE result AS
SELECT user_a, COUNT(1) as log_entries
FROM logs
GROUP BY user_a
ORDER BY user_a;

-- Guardar los resultados en S3
INSERT OVERWRITE DIRECTORY 's3://aws-logs-683656757445-us-east-1/Output/Logs/'
ROW FORMAT DELIMITED
FIELDS TERMINATED BY ','
SELECT * FROM result;