Remove-Item gamer -recurse

mkdir gamer
cp -r C:\Users\leuat\Documents\GitHub\GamerRelease\release/* gamer
rm gamer/*.cpp
rm gamer/*.o
rm gamer/*.obj
rm gamer/*.h
cp -r ../data/* Gamer
cd gamer
C:\Qt\6.4.0\msvc2019_64\bin\windeployqt.exe .
Add-Type -Assembly "System.IO.Compression.FileSystem" ;
cd ..
rm gamer_win.zip
[System.IO.Compression.ZipFile]::CreateFromDirectory("gamer", "./gamer_win.zip") ;
#d:\work\pscp Gamer_win.zip leuat@www.irio.co.uk:www.irio.co.uk/gamer/

#Send-PowerShellServerFile -Server leuat@irio.co.uk -RemoteFile www.irio.co.uk/gamer/Gamer_win.zip 
#-LocalFile Gamer_win.zip
