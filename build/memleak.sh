cd /opt/mangos/bin
sudo -u mangos env HEAPPROFILE=/arc/lost+found/memleak/mangosd LD_PRELOAD=/usr/lib64/libtcmalloc.so /opt/mangos/bin/mangosd -c /opt/mangos/etc/mangosd.conf
tail -f ./world-server.log
