well use targeted delegates for touches for the time being for simplicity. if it brings problems well change later.
the only touchable items are rectangles. nor the player nor the items are touchable directly


End Game Conditions
Race Mode:
	Win: When the first unit reaches the goal
	Lose: - (goal doesnt makes everyone lose)
	
Swarm Mode:
	Win : -
	Lose : -
	
Timer:
	Win: - (Timer doesnt make anyone win)
	Lose: When time runs out. Everybody loses for -> Timeout
	
Bomb:
	Win:
		Si + de 1 Team:
			solo 1 team vivo
	Lose:
		Si 1 team (Alone play) (con o sin swarm)
			0 unidades vivas -> gameover
			
Naming conventions:
	
Stuff To test: 


=========================================
AUDIO
=========================================
CocosDenshion.SimpleAudioEngine:
http://www.cocos2d-x.org/reference/native-cpp/V2.2.3/de/d8f/class_cocos_denshion_1_1_simple_audio_engine.html#a464d8b6b3d82c9a3fddd0cd83ba590ec

Probado con: .ogg y .wav
Parece que no soporta formato .mp3


Música:
	Start Menu
		- Intro Maze Mania
	
	InGame
		- BGM 
	
	Game Over
		- La misma música para los 3 casos, excepto para Pause
	
	Credits
	- Intro Maze Mania, la misma que el de Start Menu
	
FXs:
	General
	- Buttons
	- Animaciones (Transiciones)
	
	Start Menu
	- 
	
	InGame
	- Abrir puerta
	- Cerrar puerta
	- Timer (tic tac de los últimos segundos)
	- Colisiones:
		- Puerta (quedaría muy chocante (ja-ja) que suene cada vez que pasa (Swarm))
		- Boost Up
		- Boost Down
		- Bomb
		- Goal (ver gameover)
	
	Game Over
	
	