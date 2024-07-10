call emcc -c -Iraylib\src src\AnimationMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\AudioMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\BattyEngine.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\ControllerMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Entity.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\EntityMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\EnvironmentalObject.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\GameCamera.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Ground.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\LevelMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Ui.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\BattyUtils.cpp -Os -Wall -DPLATFORM_WEB
emar rcs build\battyenginelib.a AnimationMgr.o AudioMgr.o 
BattyEngine.o ControllerMgr.o Entity.o EntityMgr.o EnvironmentalObject.o GameCamera.o Ground.o LevelMgr.o Ui.o BattyUtils.o