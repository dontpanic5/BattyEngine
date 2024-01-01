call emcc -c -Iraylib\src src\AnimationMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\BattyEngine.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Building.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\ControllerMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Entity.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\EntityMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\EnvironmentalObject.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\GameCamera.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Ground.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\LevelMgr.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Ui.cpp -Os -Wall -DPLATFORM_WEB
call emcc -c -Iraylib\src src\Utils.cpp -Os -Wall -DPLATFORM_WEB
emar rcs build\battyenginelib.a src\AnimationMgr.o src\BattyEngine.o src\Building.o src\ControllerMgr.o src\Entity.o src\EntityMgr.o src\EnvironmentalObject.o src\GameCamera.o src\Ground.o src\LevelMgr.o src\Ui.o src\Utils.o