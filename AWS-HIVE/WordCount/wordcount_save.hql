-- Crear la tabla de entrada
CREATE EXTERNAL TABLE IF NOT EXISTS input_table (
  line STRING
)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '\n'
LOCATION 's3://aws-logs-683656757445-us-east-1/Input/WordCount/';

-- Ejecutar el conteo de palabras y guardar los resultados directamente en S3
INSERT OVERWRITE DIRECTORY 's3://aws-logs-683656757445-us-east-1/Output/WordCount/'
ROW FORMAT DELIMITED
FIELDS TERMINATED BY ','
SELECT word, COUNT(1) as count
FROM (
  SELECT explode(split(line, ' ')) as word
  FROM input_table
) a
GROUP BY word;
