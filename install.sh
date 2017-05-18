

cd /etc/init.d

ln -s /home/pi/NARVALO/AVVIO/invio_imu.sh ./
chmod +x invio_imu.sh
update-rc.d invio_imu.sh defaults

#################################################
ln -s /home/pi/NARVALO/AVVIO/messaggi.sh ./
chmod +x messaggi.sh 
update-rc.d messaggi.sh defaults


#################################################
ln -s /home/pi/NARVALO/AVVIO/rtk_start.sh ./
chmod +x rtk_start.sh 
update-rc.d rtk_start.sh defaults

#################################################
ln -s /home/pi/NARVALO/AVVIO/parsing_rtk.sh ./
chmod +x parsing_rtk.sh 
update-rc.d parsing_rtk.sh defaults

#################################################
ln -s /home/pi/NARVALO/AVVIO/allarme_gen.sh ./
chmod +x allarme_gen.sh 
update-rc.d allarme_gen.sh defaults
