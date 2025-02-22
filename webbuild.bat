call emcc -c -I%RAYLIB_DIR%\src src\AnimationMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\AudioMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\BattyEngine.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\BattyUtils.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\ControllerMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\Entity.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\EntityMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\EnvironmentalObject.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\GameCamera.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\GeneralEntity.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\Ground.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\LevelMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -I%RAYLIB_DIR%\src src\Ui.cpp -Os -Wall -DPLATFORM_WEB
emar rcs build\battyenginelib.a AnimationMgr.o AudioMgr.o BattyEngine.o BattyUtils.o ControllerMgr.o Entity.o EntityMgr.o EnvironmentalObject.o GameCamera.o GeneralEntity.o Ground.o LevelMgr.o Ui.o