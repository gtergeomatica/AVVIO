#! /bin/bash
### BEGIN INIT INFO
# Provides:          invio_imu.sh
# Required-Start:
# Required-Stop:     
# Should-Stop:       
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: communicate with "Narvalo Concentratore" to start rtklib 
# Description:       this shell script starts communication with the "Concentratore" and 
#                    parses the info sent by it, in order to update the .conf file for 			     rtkrcv. 
#                    Then invoke rtkrcv process with the updated .conf file
### END INIT INFO

#variables containing paths to the configuration files 
PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/bin
#CONF=/home/pi/NARVALO/DATI/CONF
#SRC_PARSE=/home/pi/NARVALO/AVVIO
ALL=/home/pi/NARVALO/ALLERTA_ALLARME
IMU=/home/pi/NARVALO/BERRYIMU
#CONF_NAME=narv2.conf

sleep 10

$IMU/Narvalo/invio_IMU_CC.py
