# vim: noexpandtab 



download_trackuino:
	wget http://trackuino.googlecode.com/files/trackuino-ide-1.31.zip
	unzip -o trackuino-ide-1.31.zip

#patch: download_trackuino

diff: 
	-diff -Naur trackuino-ide-1.31/trackuino/sensors.h \
	   	sensors.h > sensors_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/trackuino.h \
   		trackuino.h     > trackuino_h.patch 
	-diff -Naur trackuino-ide-1.31/trackuino/debug.h \
		debug.h         > debug_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/gps.cpp \
		gps.cpp         > gps_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/buzzer.h \
		buzzer.h        > buzzer_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/radio.cpp \
 		radio.cpp       > radio_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/buzzer.cpp \
  		buzzer.cpp      > buzzer_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/radio.h \
		radio.h         > radio_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/ax25.h \
		ax25.h          > ax25_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/radio_mx146.h \
	 	radio_mx146.h   > radio_mx146_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/aprs.cpp \
		aprs.cpp        > aprs_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/modem.cpp \
 		modem.cpp       > modem_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/ax25.cpp \
		ax25.cpp        > ax25_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/gps.h \
		gps.h           > gps_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/radio_mx146.cpp \
	   	radio_mx146.cpp > radio_mx146_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/radio_hx1.h \
   		radio_hx1.h     > radio_hx1_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/config.h \
		config.h        > config_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/sensors.cpp \
   		sensors.cpp     > sensors_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/modem.h \
		modem.h         > modem_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/radio_hx1.cpp \
	 	radio_hx1.cpp   > radio_hx1_cpp.patch
	-diff -Naur trackuino-ide-1.31/trackuino/aprs.h \
		aprs.h          > aprs_h.patch
	-diff -Naur trackuino-ide-1.31/trackuino/debug.cpp \
 		debug.cpp       > debug_cpp.patch



