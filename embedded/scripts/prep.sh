#!/bin/sh
echo «Hello, this utility tune your system ...»
echo "Change IP ..."
ip addr add 192.168.0.2/255.255.255.0 dev eth0
echo "Change route ..."
ip route add default via 192.168.0.1
ifconfig
#echo -e "tigger\ntigger\n" | passwd root
#echo "New password = tigger"
