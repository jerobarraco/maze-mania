/*
 * Files.h
 *
 *  Created on: 24/07/2014
 *      Author: nande
 */

#ifndef FILES_H_
#define FILES_H_
#include "cocos2d.h"
#include "utils/Utils.h"
//static const unsigned short C_SKINS = 2;
//typedef const char * pcc;
//todo definir como extern despues
namespace files{
	namespace about{
		static pcc credits = "about/credits.png";
		static pcc moongate = "about/moongate.png";
		static pcc music = "about/music.png";
		static pcc graphics = "about/graphics.png";
		static pcc twitter = "about/twitter.png";
		static pcc facebook = "about/facebook.png";
		static pcc diaspora = "about/diaspora.png";
		static pcc patreon = "about/patreon.png";
		static pcc m_bgm = "music/bgm_about.ogg";
	}
	namespace com{//common
		static pcc loading = "menu/loading.png";
		static pcc pause = "menu/pause.png";
		static pcc play =  "menu/play.png";
		//s_ = sonidos
		static pcc s_btn = "sounds/snd_btn.wav";
		//m_ = music
		//f_ = fonts
		static pcc f_com = "fonts/common.fnt";
	}
	namespace help{
		//todo renombrar la carpeta tuto a help
		static pcc a_title = "tuto/title.anim";
		static pcc anims[] = {
			"tuto/goal.anim", "tuto/bomb.anim", "tuto/time.anim",
			"tuto/boost.anim", "tuto/adoor.anim", "tuto/wave.anim", "tuto/play.anim"};
		enum ANIMS {AN_GOAL, AN_BOMB, AN_RACE, AN_POINT};
		static pcc title = "tuto/title.png";
		static pcc title_glow = "tuto/title_glow.png";
	}
	namespace intro{
		static pcc a_logo = "intro/logo.anim";
		static pcc bg = "intro/bg.png";
		static pcc logo = "intro/logo.png";
		static pcc logo_glow = "intro/logo_glow.png";
		static pcc play = "intro/play.png";
	}
	namespace start_menu{
		static pcc a_logo = "start_menu/logo.anim";
		static pcc back = "start_menu/back.png";
		static pcc pjIms[] = {
			"start_menu/p0_off.png","start_menu/p1_off.png", "start_menu/p2_off.png", "start_menu/p3_off.png",
			"start_menu/p4_off.png", "start_menu/p5_off.png"};
		static pcc pjImsOn[] = {
			"start_menu/p0_on.png","start_menu/p1_on.png", "start_menu/p2_on.png", "start_menu/p3_on.png",
			"start_menu/p4_on.png", "start_menu/p5_on.png"};
		static pcc difficulties[] = {"start_menu/btn_easy.png", "start_menu/btn_medium.png", "start_menu/btn_hard.png", "start_menu/btn_survival.png"};
		static pcc help = "start_menu/help.png";
	}
	//todo rename a dev_menu
	namespace menu{
		static pcc a_title = "menu/title.anim";
		static pcc title = "menu/title.png";
		static pcc title_glow = "menu/title_glow.png";
		static pcc pjIms[] = {
			"menu/p0_off.png","menu/p1_off.png", "menu/p2_off.png", "menu/p3_off.png",
			"menu/p4_off.png", "menu/p5_off.png"};
		static pcc pjImsOn[] = {
			"menu/p0_on.png","menu/p1_on.png", "menu/p2_on.png", "menu/p3_on.png",
			"menu/p4_on.png", "menu/p5_on.png"};
		static pcc modes[] = {"menu/mode0.png", "menu/mode1.png", "menu/mode2.png",
				"menu/mode3.png", "menu/mode4.png", "menu/mode5.png", "menu/mode6.png",
				"menu/mode7.png", "menu/mode8.png", "menu/mode9.png"};
		static pcc modes_on[] = {"menu/mode0_on.png", "menu/mode1_on.png", "menu/mode2_on.png",
				"menu/mode3_on.png", "menu/mode4_on.png", "menu/mode5_on.png", "menu/mode6_on.png",
				"menu/mode7_on.png", "menu/mode8_on.png", "menu/mode9_on.png"};
		static cus C_MODES=10;
		//static pcc speeds[] = {"menu/v0.png", "menu/v1.png","menu/v2.png", "menu/v3.png", "menu/v4.png", "menu/v5.png", "menu/v6.png"};
		//static pcc speeds_off[] = {"menu/v0_off.png", "menu/v1_off.png","menu/v2_off.png", "menu/v3_off.png", "menu/v4_off.png", "menu/v5_off.png", "menu/v6_off.png"};
		static pcc help = "menu/help.png";
		static pcc less = "menu/less.png";
		static pcc menu = "menu/menu.png";
		static pcc more = "menu/more.png";

		static pcc skin = "menu/skin.png";

		static pcc play = "menu/play.png";
		static pcc replay = "menu/replay.png";
		static pcc player = "menu/player.png";
		static pcc player_on = "menu/player_on.png";
		static pcc players_icon = "menu/players_icon.png";
		static pcc speed = "menu/player_speed.png";
		static pcc timer = "menu/game_timer.png";
		static pcc wave = "menu/wave.png";
		static pcc mode = "menu/mode_icon.png";
		static pcc config = "start_menu/btn_config.png";
		static pcc about = "start_menu/btn_about.png";
		static pcc exit = "start_menu/btn_exit.png";
		//static pcc music_on = "start_menu/btn_music_on.png";
		//static pcc music_off = "start_menu/btn_music_off.png";
		static pcc music[] = {"start_menu/btn_music_off.png", "start_menu/btn_music_on.png" };
		//m_ = musicas
		static pcc m_bgm ="music/bgm_menu.ogg";
	}
	namespace game{
		static pcc bg = "items/bg.png";
		static pcc point = "items/point.png";
		static pcc doors[] = {"door/0.png", "door/1.png", "door/wall.png"};
		static pcc players[] = {"pjs/p0.png", "pjs/p1.png", "pjs/p2.png", "pjs/p3.png", "pjs/p4.png", "pjs/p5.png"};
		static pcc bomb = "items/bomb.png";
		static pcc goal = "items/goal.png";
		static pcc boosts[] = {"items/speedup.png", "items/speeddown.png"};
		static pcc timers[] = {"items/timer_0.png", "items/timer_1.png", "items/timer_2.png",
				"items/timer_3.png", "items/timer_4.png", "items/timer_5.png",
				"items/timer_6.png", "items/timer_7.png", "items/timer_8.png"};
		static pcc w_timers[] = {"items/timer_w_0.png", "items/timer_w_1.png", "items/timer_w_2.png",
				"items/timer_w_3.png", "items/timer_w_4.png", "items/timer_w_5.png",
				"items/timer_w_6.png", "items/timer_w_7.png", "items/timer_w_8.png"};
		static pcc corner = "door/corner.png";
		//sounds
		static pcc s_doors[] = {"sounds/snd_door0.wav", "sounds/snd_door1.wav"};
		static pcc s_goal = "sounds/snd_goal.wav";
		static pcc s_bomb = "sounds/snd_bomb.wav";
		static pcc s_boosts[] = {"sounds/snd_boost0.wav", "sounds/snd_boost1.wav"};
		static pcc s_timer = "sounds/snd_timer.wav";
		static pcc s_timeout = "sounds/snd_timeout.wav";
		static pcc s_point = "sounds/snd_point.wav";
		//musicas
		static pcc m_gameover = "music/bgm_gameover.ogg";
		static pcc m_bgm[] = {"music/bgm_game00.ogg", "music/bgm_game01.ogg"};
	}

	namespace gameover{
		static pcc a_players[] = {
				"game_over/p0.anim", "game_over/p1.anim", "game_over/p2.anim",
				"game_over/p3.anim", "game_over/p4.anim", "game_over/p5.anim"};
		static pcc a_winner = "game_over/winner.anim";
		static pcc a_cleared = "game_over/cleared.anim";
		static pcc a_finished = "game_over/finished.anim";
		static pcc bg = "game_over/bg.png";
		static pcc pause = "game_over/pause.png";
		static pcc timeout = "game_over/timeout.png";
		static pcc gameover = "game_over/gameover.png";
		static pcc winner = "game_over/winner.png";
		static pcc cleared = "game_over/cleared.png";
		static pcc finished = "game_over/finished.png";
		//btn
		static pcc resume = "game_over/resume.png";
		//wave labels
		static pcc wave = "game_over/wave.png";
		static pcc points = "game_over/points.png";
		static pcc time = "game_over/time.png";
		static pcc clicks = "game_over/clicks.png";
	}
}

static cus C_SKINS = 2;
char *fullPath(const char *file, bool full_full=false);
int play(const char *path, bool sfx=true, bool loop=true, bool restart=true);
void preloadAudio(const char *path, bool sfx=true);
cocos2d::CCTexture2D *preloadTX(const char *path);
void unloadTX(const char *path);
void unloadSFX(const char *path);
void preloadTXCache(const int cant, const char *files[], cocos2d::CCTexture2D *txs[]);
void unloadTXCache(const int cant, const char *files[], cocos2d::CCTexture2D *txs[]);

int playSndBtn();
void pauseAudio();
void resumeAudio();
void replayBGM();
void destroyAudio();

class SkinDef{
public:
	//cantidad de imagenes para el timer
	us c_timer;
	const char *graphicsURL;
	const char *musicURL;
	cocos2d::ccColor4F boardBG, boardBorder, overlay;
};
SkinDef *skinDef();
extern SkinDef SKIN_DEFS[C_SKINS];

#endif /* FILES_H_ */
