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

#static IP address of NARVALO "concentratore"
#IP_ADDRESS = "104.46.34.74"

#get the Json string from the "concentratore" in the file conf_prms 
wget --output-document=${SRC_PARSE}/conf_prms.json http://40.68.169.138/Narvalo/NarvaloWS/api/Narvalo/GetConfApparato/5

#copy the file .conf from the folder of rtkrcv to the folder of the parser 
cp ${CONF}/$CONF_NAME ${SRC_PARSE}/$CONF_NAME
if [ -e $CONF/$CONF_NAME ]   #remove the old .conf file 
then
	rm $CONF/$CONF_NAME
fi

cd $SRC_PARSE
#simple_example
./comunication      #invoke the SW suite for parsing the info sent by the Concentratore

#cut the updated .conf file in the right folder 
mv ${SRC_PARSE}/$CONF_NAME ${CONF}/$CONF_NAME   

#move tho the right folder and start rtkrcv
cd ${CONF}

rtkrcv -s -o $CONF/$CONF_NAME 

# execute python scripts for recevin alarms msg from CC

python $ALL/messaggi_CC.py
python $ALL/allarme_generico.py
python $IMU/Narvalo/invio_IMU_CC.py

#cd ${CONF}
#cd /etc/init.d



