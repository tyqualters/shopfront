FROM ubuntu:latest
MAINTAINER Ty contact@tyqualters.com
USER root

WORKDIR /app

# Updates
RUN apt-get update -y

# Pre-requisites
RUN apt install git gcc g++ cmake libjsoncpp-dev uuid-dev zlib1g-dev openssl libssl-dev -y

# Drogon
RUN git clone https://github.com/drogonframework/drogon drogon --recursive

# YAML-CPP
RUN git clone https://github.com/jbeder/yaml-cpp yaml-cpp --recursive

# Copy all files not in .containerignore
COPY . . 

# Build Project
RUN mkdir build

EXPOSE 80

CMD ["./run"]
