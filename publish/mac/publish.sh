rm -rf gamer
rm gamer_osx.tar.gz
mkdir gamer
cp -r ../data/* gamer
cp ../../build/Gamer.app/Contents/MacOS/Gamer gamer/gamer
tar -cvzf gamer_osx.tar.gz gamer
scp gamer_osx.tar.gz leuat@www.irio.co.uk:www/gamer
