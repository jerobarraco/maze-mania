/*
 * Files.cpp
 *
 *  Created on: 24/07/2014
 *      Author: nande
 */

#include "Files.h"
#include "Utils.h"

#include "SimpleAudioEngine.h"

static pcc SKINS[] = {
	"pixel_mode", "sherman"
};

static char ret[256], _last_bgm[256]={0};
static bool _last_bgm_loop = false;

static cus C_SKINI=2;
static us _skins_i[C_SKINI] = {0, 0}; //skins to try if one fails

SkinDef SKIN_DEFS[C_SKINS] = {
	{
		9,//imagenes para timer
		"http://www.moongate.com.ar/p/acercaabout.html",//graphicsURL
		"http://www.SoundImage.org",//musicURL
		cocos2d::ccc4f(0.92156863, 0.64313725, 0.35686275, 0.75),
		cocos2d::ccc4f(1.0, 1.0, 1.0, 1.0),
		cocos2d::ccc4f(0.0, 0.0, 0.0, 0.5)
	},{
		7,//imagenes para timer
		"http://www.GreatStudio.tv",//graphicsURL
		"http://www.SoundImage.org",//musicURL
		cocos2d::ccc4f(0.0, 0.0, 0.0, 0.40),
		cocos2d::ccc4f(1.0, 1.0, 1.0, 1.0),
		cocos2d::ccc4f(0.0, 0.0, 0.0, 0.5)
	}
};

SkinDef *skinDef(){
	return &SKIN_DEFS[GLOBALS.skin];
}

char* fullPath(const char *file, bool full_full){
	//not thread safe nor reentrant
	//sets the first skin to try the global one
	//full_full returns the really full path, useful when trying to use a file from other methods than cocos2d (ie fopen)
	_skins_i[0] = GLOBALS.skin;
	unsigned short int i=0;
	cocos2d::CCFileUtils *futil = cocos2d::CCFileUtils::sharedFileUtils();//get it? futile
	while (i<C_SKINI){
		ret[0] = 0;//"cleans" the "string"
		strcat(ret, SKINS[_skins_i[i]]);
		strcat(ret, "/");
		strcat(ret, file);
		if (full_full){
			//lol be careful here
			//here be dragons and spiders! we are juggling with pointers using ret 3 times
			//one to pass it, the 2nd to pass the file name only (with no path)
			//strcpy(ret, futil->fullPathFromRelativeFile(ret, strrchr(ret, '/')+1));
			strcpy(ret, futil->fullPathForFilename(ret).c_str());
		}
		if(futil->isFileExist(ret)){
			#ifdef DEBUG
				CCLOG("found file %s", ret);
			#endif
			return ret;//the magic is in the ret
		}
		#ifdef DEBUG
		else{
			CCLOG("not found file %s", ret);
		}
		#endif

		i++;
	}
	//sharedFileUtils is the lousiest thing i have ever seen
	//they have macros, conditional includes, static stuff and yet, using this...
	//if i cant find any skin with that file, returns null (if this gets error, try the other one below)
	CCLOG("FILE NOT FOUND! i bet this crashes %s", file);
	return NULL;//fact if this code is ever reached, none have been found
}

int play(const char * path, bool sfx, bool loop, bool restart){
	//be carefull because play calls fullPath wich overwrites the char buffer
	if (!GLOBALS.audio){
		return -1;
	}
	char *t = fullPath(path);
	CocosDenshion::SimpleAudioEngine* sae = CocosDenshion::SimpleAudioEngine::sharedEngine();
	if (sfx){
		CCLOG("playing sfx %s", path);
		return sae->playEffect(t);
	}
	else{
		CCLOG("Playing Music %s", path);
		/////
		/// \brief strcpy
		/////caches the last played bgm for android suckyness
		//if the last song played was the one to play now, and it was looping
		//and if dont want to restart, then we keep playing

		if (!restart && _last_bgm_loop && sae->isBackgroundMusicPlaying() && (strcmp(_last_bgm, path)==0)){
			//!restart : only if you don't NEED to restart it
			//_last_bgm_loop : only if the previous was looping: because it could end before
			//	the time we expect it to. we could check if the "loop" parameter is also looping
			//	but that wont lead to a very buggy ending and we really don't need it.
			// strcmp: if it was the same
			// isBackground : and if there was actually something playing (important for paused stuff)
			CCLOG(" - won't play because it was already playing and they told me not to restart");
			CCLOG(" - but i will resume it just in case ;)");
			CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
			return -2;
		}
		strcpy(_last_bgm, path);

		_last_bgm_loop = loop;
		////end
		//if (exclusive){
			//cannot implement non-exclusive background music play (so calling the play function is MANDATORY
			//because guess whose favorite chinese team forgot to add the function to unload background music?
			//todo test what would happen under normal conditions (logical memory leak?)
			if(sae->isBackgroundMusicPlaying()){
				sae->stopBackgroundMusic(true);//false a release
			}
		//}
		sae->playBackgroundMusic(t, loop);
		return -1;
	}
	return -1;
}
/*The Cocos2D ‘SoundEngine’ class provides all of the functionality you’ll need. It groups audio into two main groups: ‘Sound’ and ‘Effect’. Effects are the explosions, jump sounds, and other general effects you have within games. Sound is the background music and is the only audio type that can be paused and resumed. Sound effects should be kept under 5 seconds long, and ideally they should be under 3 seconds.
and probably less than 1mb in total*/

int playSndBtn(){
	return play (files::com::s_btn);
}

cocos2d::CCTexture2D *preloadTX(const char *path){
	//carefull it changes the return of fullpath
	CCLOG("preloading texture %s", path);
	return cocos2d::CCTextureCache::sharedTextureCache()->addImage(fullPath(path));
}

void preloadAudio(const char *path, bool sfx){
	CCLOG("preloading %s", path);
	char *t = fullPath(path);
	if (sfx){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(t);
	}else{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(t);
	}
}

void unloadTX(const char *path){
	char *t = fullPath(path);
	CCLOG("Unloading tx %s", t);
	cocos2d::CCTextureCache::sharedTextureCache()->removeTextureForKey(t);
}

void unloadSFX(const char *path){
	char *t = fullPath(path);
	CCLOG("Unloading sfx %s", t);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(t);
}

void destroyAudio(){
	CCLOG("destroying audio engine");
	CocosDenshion::SimpleAudioEngine* sae =  CocosDenshion::SimpleAudioEngine::sharedEngine();
	sae->end();
}

void pauseAudio(){
	CCLOG("pausing bg music");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void resumeAudio(){
	CCLOG("resuming bg music");
	//if the game is paused it should NOT resume the music, that is handled by the Game object
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void replayBGM(){
	/*theres an extremely aweful bug on cocos2d, when the app goes to background on Android, the music pauses
	 * but when the app goes to the Foreground, no music get resumed...... sucks
	 * to fix that, we tried this http://discuss.cocos2d-x.org/t/background-music-not-resume-after-applicationwillenterforeground-android-4-x/3872/5
	 * which of course doesnt work....
	 * so we implement this, that replays the last played music (if any*1).
	 * this sucks but there is no other alternative for now, and modifying cocos2d is not a nice alternative
	 *
	*/
	CCLOG("Replaying bgm {loop:'%s', path:'%s'}", _last_bgm_loop?"yes":"no", _last_bgm);
	if (strlen(_last_bgm)==0){//thanks android 4.3
		CCLOG("- Crazy shit, no bgm to replay!!");
		return;
	}
	play(_last_bgm, false, _last_bgm_loop, true);//be sure to restart because nothing will actually be playing!
	//capaz con un resume andaba bien... aunque este inheritamente va a llamar a play,
	//el cual respeta si en las opciones hay pausa o no
}

void preloadTXCache(const int cant, const char *files[], cocos2d::CCTexture2D *txs[]){
	/*
	 * Used to preload an array of CCTexture2D
	 */
	//only loads if it is not loaded
	if (txs[0]==NULL){
		for (us i=0; i<cant; i++){
			txs[i] = preloadTX(files[i]);
			txs[i]->retain();
		}
	}
}

void unloadTXCache(const int cant, const char *files[], cocos2d::CCTexture2D *txs[]){
	/* Unloads and release a texture cache, careful because it also unloads the texture! */
	//if(txs[0]!=NULL){
		for (us i =0; i<cant; i++){
			//notice we set to NULL to ALL txs
			//this is a good practice, and also they should have been initialized to null first (all of them)
			//this way it will catch if the skin changed before the unload
			RELEASE(txs[i]);
			unloadTX(files[i]);
		}
	//}
}
