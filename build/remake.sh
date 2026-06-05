cd /opt/mangos/zero/cur/server
rm -f /opt/mangos/bin/realmd
rm -f /opt/mangos/bin/mangosd
rm -f src/realmd/realmd
rm -f src/mangosd/mangosd
cmake . -DPLAYERBOTS=1 -DBUILD_REALMD=1 -DBUILD_TOOLS=0 -DCMAKE_INSTALL_PREFIX=/opt/mangos -DCMAKE_INSTALL_SYSCONFDIR=/opt/mangos/etc  -DCMAKE_CXX_FLAGS="-w"
make -j8
echo Stopping servers...
sudo systemctl stop realmd
sudo systemctl stop mangosd
echo Copying new binaries over...
cp -f src/realmd/realmd /opt/mangos/bin/.
cp -f src/mangosd/mangosd /opt/mangos/bin/.
echo Restarting servers...
sudo systemctl start realmd
sudo systemctl start mangosd

