FROM ubuntu:18.04
RUN apt update -y
RUN apt install apache2 -y
RUN apt install g++ -y
RUN apt install libcgicc3 -y
RUN apt install libcgicc-dev -y
RUN apt install make -y

copy index.html /var/www/html/
copy conf/apache2.conf /etc/apache2/apache2.conf
copy conf/cgid.conf /etc/apache2/mods-enabled/
copy conf/cgid.load /etc/apache2/mods-enabled/
copy conf/cgi.load /etc/apache2/mods-enabled/
copy src/serverRequest.js /var/www/html/src/serverRequest.js
copy src/index.css /var/www/html/src/index.css

copy src/_normalize.css /var/www/html/src/_normalize.css
copy src/_zeroing.css /var/www/html/src/_zeroing.css
copy src/particles.js /var/www/html/src/particles.js
copy src/app.js /var/www/html/src/app.js 
copy src/select.js /var/www/html/src/select.js
copy src/select.css /var/www/html/src/select.css
copy src/fb.png /var/www/html/src/fb.png
copy src/github.png /var/www/html/src/github.png
copy src/vk.png /var/www/html/src/vk.png
copy src/logo.png /var/www/html/src/logo.png
copy src/main.png /var/www/html/src/main.png
copy src/misprint.png /var/www/html/src/misprint.png
copy src/repetition.png /var/www/html/src/repetition.png
copy src/uniqueness.png /var/www/html/src/uniqueness.png
copy src/validation.png /var/www/html/src/validation.png

RUN mkdir /var/www/cgi-bin
copy Makefile /var/www/cgi-bin/
copy script.cpp /var/www/cgi-bin/
copy db.txt /var/www/cgi-bin/db.txt
WORKDIR /var/www/cgi-bin
RUN make
RUN chmod 755 script.cgi
RUN rm Makefile script.cpp
CMD apache2ctl -D FOREGROUND
