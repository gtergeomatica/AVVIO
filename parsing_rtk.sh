#! /bin/bash
### BEGIN INIT INFO
# Provides:          conf option update for narvalo at boot
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
#CONF=/home/alessandro/Scaricati/rtklib_2.4.3/app/rtkrcv
#SRC_PARSE=/home/alessandro/Scaricati/AVVIO
#CONF_NAME=prova5.conf
CONF=/home/pi/NARVALO/DATI/CONF
SRC_PARSE=/home/pi/NARVALO/AVVIO
#ALL=/home/pi/NARVALO/ALLERTA_ALLARME
#IMU=/home/pi/NARVALO/BERRYIMU
CONF_NAME=narv2.conf

#static IP address of NARVALO "concentratore"
#IP_ADDRESS = "104.46.34.74"

#get the Json string from the "concentratore" in the file conf_prms 
wget --output-document=${SRC_PARSE}/conf_prms.json http://narvalo.dyndns.org/Narvalo/NarvaloWS/api/Narvalo/GetConfApparato/9

#copy the file .conf from the folder of rtkrcv to the folder of the parser 
cp ${CONF}/$CONF_NAME ${SRC_PARSE}/$CONF_NAME
if [ -e $CONF/$CONF_NAME ]   #remove the old .conf file 
then
	rm $CONF/$CONF_NAME
fi

#cut the updated .conf file in the right folder 
cp ${SRC_PARSE}/$CONF_NAME ${CONF}/$CONF_NAME   

cd $SRC_PARSE
#simple_example

comunication && cp ${SRC_PARSE}/$CONF_NAME ${CONF}/$CONF_NAME; 
cd $CONF     #s&& rtkrcv -o $CONF/$CONF_NAME  

#wait %!        #wait until comunication is not finished

#cut the updated .conf file in the right folder 
#cp ${SRC_PARSE}/$CONF_NAME ${CONF}/$CONF_NAME   

#move tho the right folder and start rtkrcv

#cd ${CONF}
#rtkrcv -s -o $CONF/$CONF_NAME  

# execute python scripts for recevin alarms msg from CC

#python $ALL/messaggi_CC.py
#python $ALL/allarme_generico.py
#python $IMU/Narvalo/invio_IMU_CC.py

#cd ${CONF}
#cd /etc/init.d

exit 0

