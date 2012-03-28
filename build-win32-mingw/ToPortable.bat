@ECHO OFF
cd "C:\Documents and Settings\Matthew\Desktop\Programming\C++\Diamonds"

del ".\DiamondsPortable\App\diamonds\*.*" /Q
del ".\DiamondsPortable\App\diamonds\data\*.*" /Q
del ".\DiamondsPortable\App\DefaultData\settings\*.*" /Q
rmdir ".\DiamondsPortable\Data\settings" /S /Q

xcopy ".\diamonds-0.5-src\data\diamonds.ico" ".\DiamondsPortable\App\AppInfo\appicon.ico" /Y
xcopy ".\diamonds-0.5-src\settings\*.txt" ".\DiamondsPortable\App\DefaultData\settings\*.txt" /Y
xcopy ".\diamonds-0.5-src\settings\*.ini" ".\DiamondsPortable\App\DefaultData\settings\*.ini" /Y
xcopy ".\diamonds-0.5-src\settings\*.dat" ".\DiamondsPortable\App\DefaultData\settings\*.dat" /Y
xcopy ".\diamonds-0.5-src\data\*.png" ".\DiamondsPortable\App\diamonds\data\*.png" /Y
xcopy ".\diamonds-0.5-src\data\*.ogg" ".\DiamondsPortable\App\diamonds\data\*.ogg" /Y
xcopy ".\diamonds-0.5-src\data\*.bmp" ".\DiamondsPortable\App\diamonds\data\*.bmp" /Y
xcopy ".\diamonds-0.5-src\data\*.ttf" ".\DiamondsPortable\App\diamonds\data\*.ttf" /Y
xcopy ".\diamonds-0.5-src\data\*.xpm" ".\DiamondsPortable\App\diamonds\data\*.xpm" /Y
xcopy ".\diamonds-0.5-src\*.*" ".\DiamondsPortable\App\diamonds\*.*" /Y

.\upx.exe ".\DiamondsPortable\App\diamonds\diamonds.exe"
.\upx.exe ".\DiamondsPortable\App\diamonds\editor.exe"
.\upx.exe ".\DiamondsPortable\App\diamonds\libfreetype-6.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\libogg-0.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\libpng12-0.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\libvorbis-0.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\libvorbisfile-3.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\SDL.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\SDL_image.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\SDL_mixer.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\SDL_ttf.dll"
.\upx.exe ".\DiamondsPortable\App\diamonds\zlib1.dll"

PAUSE