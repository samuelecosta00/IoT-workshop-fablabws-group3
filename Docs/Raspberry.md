
**CONFIGURAZIONE RASPBERRY - SSH:**

1) Collegare la memoria della scheda Raspberry al pc, andare in esplora risorse, successivamente su *"Boot"* e creiamo un file ssh privo di estensione.
2) Si crea un altro file *“wpa_supplicant.conf"*, si apre con il blocco note e si inserisce il codice ***(allegato nei file come: "codice Raspberry")*** modificando l'*"ssid"* e inserendo il proprio wi-fi.
3) Estrarre la memoria, inserirla nella Raspberry e collegarla all'alimentazione.
4) Scaricare *"PuTTY"* tramite *https://www.putty.org/* e collegandoci da remoto apparirà un terminale dove si effettuerà il login tramite l'indirizzo IP della scheda.
5) Avviandolo apparirà un secondo terminale dove ogni modifica verrà effettuata sulla scheda.
6) Scrivendo *"clear"* si pulirà tutta la schermata.

**CONFIGURAZIONE RASPBERRY - VNC:**

1) Collegarsi da remoto a PuTTY e configurare raspberry per utilizzare VNC.
2) Scaricare  *"RealVNC Viewer"* tramite *https://www.realvnc.com/en/connect/download/viewer/*.
3) Andare su file, successivamente su new coonnection e inserire indirizzo IP, username e password della raspberry.
