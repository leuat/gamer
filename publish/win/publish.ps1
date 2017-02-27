Remove-Item Gamer -recurse

mkdir Gamer
cp -r ../../build/release/* Gamer
rm Gamer/*.cpp
rm Gamer/*.o
rm Gamer/*.h
cp -r ../data/* Gamer
Add-Type -Assembly "System.IO.Compression.FileSystem" ;
[System.IO.Compression.ZipFile]::CreateFromDirectory("Gamer", "Gamer_win.zip") ;
d:\work\pscp Gamer_win.zip leuat@www.irio.co.uk:www.irio.co.uk/gamer/

Send-PowerShellServerFile -Server leuat@irio.co.uk -RemoteFile www.irio.co.uk/gamer/Gamer_win.zip 
-LocalFile Gamer_win.zip