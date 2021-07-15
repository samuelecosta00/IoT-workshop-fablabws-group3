
**COMUNICAZIONE TRA LE DUE SCHEDE**

Esp32 invia i valori ricevuti di temperatura e di umidità alla Raspberry e li carica su un 
server accessibile solo tramite la stessa rete wi-fi *(Visionare slide N° 21)*.
Per farle comunicare si usa il protocollo *"MQTT"* che dovrà essere installato su entrambe.

**Raspberry**

1)Collegarsi a PuTTY e digitare tutto quello elencato nel file allegato *"MQTT Raspberry"* e testarlo.

**Esp32**

1) Scaricare la libreria tramite *https://github.com/marvinroger/async-mqtt-client/archive/master.zip*
2) Seguire il file allegato *"MQTT Esp32"*.
3) Ripetere il procedimento per la seconda libreria tramite *https://github.com/me-no-dev/AsyncTCP/archive/master.zip*
4) Scrivere il codice presente nel file allegato *"sketch_ESP32"* per connettersi ai server MQTT in modo tale da inviare i dati del sensore.