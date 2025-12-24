cd /opt/mangos/zero/cur/server
rm -f /opt/mangos/bin/*.log
rm -f /opt/mangos/bin/realmd
rm -f /opt/mangos/bin/mangosd
rm -f src/realmd/realmd
rm -f src/mangosd/mangosd
cmake . -DPLAYERBOTS=1 -DBUILD_REALMD=1 -DBUILD_TOOLS=0 -DCMAKE_INSTALL_PREFIX=/opt/mangos -DCMAKE_INSTALL_SYSCONFDIR=/opt/mangos/etc
make -j8
echo Stopping servers...
systemctl stop realmd
systemctl stop mangosd
echo Copying new binaries over...
rm -f /opt/mangos/bin/*.log
cp -f src/realmd/realmd /opt/mangos/bin/.
cp -f src/mangosd/mangosd /opt/mangos/bin/.
echo Restarting servers...
chown mangos:mangos /opt/mangos/bin/*
systemctl start realmd
systemctl start mangosd

