# AVVIO
Tutte le procedure SW e codice per gestire in avvio le procedure di update del file di configurazione con info dal Concentratore e l'avvio di rtkrcv

# ! NOTA BENE:
# DA FARE OGNI volta che si fa un git pull!
- modificare l'identificativo che compare nello script parsing_rtk.sh nella riga dove si chiama il webservice del concentratore NARVALO

# DA fare solo alla prima installazione!

C'è un codice sorgente da compilare. 

Ci sono 5 script da inserire tra le procedure di avvio:
 - allarme_gen.sh 
 - invio_imu.sh
 - messaggi.sh
 - pasrsing_rtk.sh
 - rtk_start.sh

# Procedura manuale
1) make all

2) Autenticarsi come root (sudo -s)

3) make install  (per copiare i file in /usr/local/bin 

4) Per i 5 script di avvio sono da fare le seguenti operazioni:
 - fare un link degli script in /etc/init.d/ --> ln -s /home/pi/NARVALO/AVVIO/nome_script.sh /etc/init.d/ 
 - chmod +x /etc/init.d/nome_script.sh
 - update-rc.d nome_script.sh defaults

# Procedura automatica: script install.sh
Per velocizzare la procedura di installazione e' possibile usare uno script apppositamente creato chiamato install.sh! 
Va fatto girare come root quindi occorre prima un sudo -s.


# altre note
Per cambiare i nomi dei file bisogna agire nel file SRC/conf_update.c che gestisce il seguente parsing e ovviamente anche tutti gli script bash


