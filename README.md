This code is not maintained, it's on Github only for educational purposes. The interesting part of this project is the **Webserver** running on the Arduino. The webserver had to run without blocking the main loop, in order to keep buttons and lcd responsive to user input. Therefore every request is processed in multiple passes to prevent blocking the loop while processing the http requests.

Feel free to do whatever you want whit this code. License: [MIT](LICENSE)

# Projet integrateur 2015 - ECAM

Ceci est le code Arduino pour notre projet 2015 à l'Ecam 
(Haute école Leonard De Vinci - études d'ingénieurs industriel).

### Énoncé du projet

Il nous a été demandé de construire un système hydroponique avec un minimum d'automatisation via l'arduino.
L'hydroponie est la culture des plantes hors-sol, dans un (faible) courant d'eau continu.

On nous a imposé de devoir mesurer plusieurs facteurs qui ont une influance sur la croissance des plantes via un Arduino.
*Ceux-ci sont: le pH, la température, la lumière, l'humiditée, ...*

Nous avons aussi été demandé de réguler au moins un de ces paramètres avec l'arduino.
