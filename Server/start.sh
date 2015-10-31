chmod 777 server
while true
do
    echo "Start server"
    ./server 10000 0.map 1
    chmod 777 logs/*
done
