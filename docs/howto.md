descargar cocos2dx
descomprir

cd /home/nande/dev/work/maze-mania/tools/project-creator
python create_project.py -project mm -package com.moongate.mazemania -language cpp

importarlo en el workspace de momentics
	root directory /home/nande/dev/work/maze-mania/
	importar solo proyectos con proj.blackberry y ningun sample ni template (Box2D chipmunk cocos2dx CocosDenshion extensions mm)

si al compilar el puto cocos2dx da error en la extension en GUI/algoWP8 borrarlo a la mierda y listo

Para android:
	1) abrir el proyecto con netbeans, 
	ir a projects/mazemania e importar proj.android
	
	2) Poner el target y el sdk en local.properties
		sdk.dir=/home/nande/programas/adt/sdk
		target=android-10
		
	3) Copiar el local.properties a la carpeta de cocos2dx
		/home/nande/dev/work/maze-mania/cocos2dx/platform/android/java
		
	4) Poner el target correcto en el manifest de cocos y el proyecto
	
	5) Agregar archivos .cpp al Android.mk en 
		/home/nande/dev/work/maze-mania/projects/mazemania/proj.android/jni
	
	6) Modificar el Application.mk y agregar -fexceptions
		APP_CPPFLAGS := -frtti -fexceptions -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1

	7) Modificar build_native.sh agregar directorios
		/home/nande/dev/work/maze-mania/projects/mazemania/proj.android/build_native.sh
		# TODO definir estas variables
		NDK_ROOT="/home/nande/programas/adt/ndk-r9"
		ANDROID_NDK_ROOT="/home/nande/programas/adt/ndk-r9"
		ANDROID_SDK_ROOT="/home/nande/programas/adt/sdk"
		COCOS2DX_ROOT="/home/nande/dev/work/maze-mania"
		
	8) Ejecutar finalmente el build_native.sh cruzar los dedos rezar 3 ave marias y esperar que compile
	
	9) Darle clean+build en netbeans
	10) finalmente Darle run en netbeans
	
	facil no?
	
	En cuanto al simulador, no olvidarse de crear un dispositivo con android 4.3 (podria ser 2.3 pero 2.3 no anda con host gpu necesaria para cocos2d)
	
	
Para QtCreator:
	Abrir el project que está en el root. (MazeMania.creator), Sino crear un proyecto importando un proyecto de Make
	Instalar las siguientes libs (caso de linux):
		libGLEW-dev (instala libglu1-mesa-dev | libglu1-mesa-dev)   y todas sus dependencias jorribles) No instalar con apt-get porque marca las dependencias como manuales
		libglfw-dev (depende tambien de libglu-dev)
		libfontconfig-dev (depende en freetype png expat y pkg-config)
		fglrx-updates-dev
	Configurar el proyecto:
		Crear configuración debug y release
		en debug agregar parametro de make "DEBUG=1" en release no (o DEBUG=0)
		Crear además un paso custom después del build, ejecutar "link.sh" en la carpeta donde está. y como parámetro poner "debug" en caso debug y "release" en caso release.
		En el run solo falta que ejecute el link en la carpeta "run" y listo.