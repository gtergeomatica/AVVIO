#! /bin/bash
### BEGIN INIT INFO
# Provides:          conf option update for narvalo at boot
# Required-Start:
# Required-Stop:     
# Should-Stop:       
# Default-Start:
# Default-Stop:      
# Short-Description: communicate with "Narvalo Concentratore" to start rtklib 
# Description:       this shell script starts communication with the "Concentratore" and 
#                    parses the info sent by it, in order to update the .conf file for 			     rtkrcv. 
#                    Then invoke rtkrcv process with the updated .conf file
### END INIT INFO

#variables containing paths to the configuration files 
PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/bin:/home/alessandro/git/NARVALO_ConcTerm
CONF=/home/pi/NARVALO/DATI/CONF
SRC_PARSE=/home/pi/NARVALO/AVVIO
ALL=/home/pi/NARVALO/ALLERTA_ALLARME
IMU=/home/pi/NARVALO/BERRYIMU
CONF_NAME=narv2.conf

sleep 10

python $ALL/messaggi_CC.py
