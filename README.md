# AVVIO
Tutte le procedure SW e codice per gestire in avvio le procedure di update del file di configurazione con info dal Concentratore e l'avvio di rtkrcv

Ricordarsi di fare da root (sudo -s):
 - fare un link degli script in /etc/init.d/ --> ln -s /home/pi/NARVALO/AVVIO/nome_script.sh /etc/init.d/ 
 - chmod +x /etc/init.d/nome_script.sh
 - update-rc.d nome_script.sh defaults

   
