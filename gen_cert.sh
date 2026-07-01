#!/usr/bin/env bash

if [[ ! -d ssl ]]; then
	echo Generating certificate
	mkdir ssl; cd ssl
	openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes -subj "/CN=localhost"
else
	echo Certificate is present
fi
