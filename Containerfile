FROM ubuntu:latest
MAINTAINER Ty contact@tyqualters.com
USER root

WORKDIR /app

# Updates
RUN apt-get update -y

# Pre-requisites
RUN apt install git gcc g++ cmake uuid-dev zlib1g-dev openssl libssl-dev libmariadb-dev-compat libmariadb-dev -y

# Copy all files not in .containerignore
COPY . . 

# Build Project
RUN mkdir build
RUN cmake -S . -B build
RUN make -C build

# Generate Certificate
RUN ./gen_cert.sh

EXPOSE 80
EXPOSE 443

CMD ["./run"]
