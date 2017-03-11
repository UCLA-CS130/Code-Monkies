FROM ubuntu:14.04

RUN apt-get update
RUN apt-get install -y libboost-all-dev make g++
WORKDIR /opt/webserver
COPY . /opt/webserver
RUN make clean && make

EXPOSE 8080:8080

CMD tar -cf - bin/webserver
