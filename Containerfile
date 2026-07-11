FROM ubuntu:latest
MAINTAINER Ty contact@tyqualters.com
USER root

WORKDIR /opt/app

# Updates
RUN apt-get update -y

# Pre-requisites
RUN apt install git gcc g++ cmake uuid-dev zlib1g-dev openssl libssl-dev libmariadb-dev-compat libmariadb-dev libc-ares-dev libbrotli-dev libgtest-dev -y

# Copy all files not in .containerignore
COPY . . 

EXPOSE 80
EXPOSE 443

CMD ["./run"]
