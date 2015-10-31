cd /home/test
rm server
cp server.new server
chmod 777 server
while true
do
    echo "Start server"
    ./server 10000 0.map
    chmod 777 logs/*
done