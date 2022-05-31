#!/bin/bash
for passwrd in $(strings ./s3)
do
	printf "Trying: %s \n" "$passwrd"
	cp s3 temp.out
	./temp.out $passwrd
	printf "\n"
done

