#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

typedef enum {false, true} bool;

const int SCR_W = 640;
const int SCR_H = 320;
const int SCR_BPP = 32;

SDL_Surface *scr = NULL;
SDL_Surface *bg = NULL;
SDL_Surface *msg = NULL;

SDL_Event event;

SDL_Color color = {0x0, 0x0, 0x0};
TTF_Font *font = NULL;

/* Mix_Music is used to music; Mix_Chunk is used for sfx. */
// the music that will be played
Mix_Music *music = NULL;
// the sfxs that will be used
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

SDL_Surface *load_img(char *);
void blit_surface(int, int, SDL_Surface *, SDL_Surface *);
bool init();
bool load_files();
void clean_up();

int main(int argc, char *argv[])
{
    bool quit = false;

    if (!init() || !load_files())
	return 1;

    // blit the background to the screen
    blit_surface(0, 0, bg, scr);
    // blit the input messages to the screen
    msg = TTF_RenderText_Solid(font,"Press 1, 2, 3, or 4 to play a sfx",color);
    blit_surface((SCR_W - msg->w)/2, (SCR_H - msg->h)/4, msg, scr);
    SDL_FreeSurface(msg);
    msg = TTF_RenderText_Solid(font,"Press 9 to play/pause the music",color);
    blit_surface((SCR_W - msg->w)/2, (SCR_H - msg->h)/4+50, msg, scr);
    SDL_FreeSurface(msg);
    msg = TTF_RenderText_Solid(font,"Press 0 to stop the music",color);
    blit_surface((SCR_W - msg->w)/2, (SCR_H - msg->h)/4+100, msg, scr);
    SDL_FreeSurface(msg);
    msg = TTF_RenderText_Solid(font,"Press q to quit",color);
    blit_surface((SCR_W - msg->w)/2, (SCR_H - msg->h)/4+150, msg, scr);
    SDL_FreeSurface(msg);

    if (SDL_Flip(scr) == -1)
	return 1;

    while (!quit) {
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT)
		quit = true;
	    else if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		    case SDLK_1:    // if 1 is pressed
			// play the scratch sfx
			if (Mix_PlayChannel(-1,scratch,0) == -1)
			    return 1;
			break;
		    case SDLK_2:    // if 2 is pressed
			// play the high hit sfx
			if (Mix_PlayChannel(-1,high,0) == -1)
			    return 1;
			break;
		    case SDLK_3:    // if 3 is pressed
			// play the med hit sfx
			if (Mix_PlayChannel(-1,med,0) == -1)
			    return 1;
			break;
		    case SDLK_4:    // if 4 is pressed
			// play the low hit sfx
			if (Mix_PlayChannel(-1,low,0) == -1)
			    return 1;
			break;
		    case SDLK_9:    // if 9 is pressed
			// if there's no music already playing
			if (Mix_PlayingMusic() == 0) {
			    if (Mix_PlayMusic(music,-1) == -1)
				return 1;
			// if music is already playing, pause/resume it
			} else {
			    if (Mix_PausedMusic() == 1)
				Mix_ResumeMusic();
			    else
				Mix_PauseMusic();
			}
			break;
		    case SDLK_0:    // if 0 is pressed
			// stop the music
			Mix_HaltMusic();
			break;
		    case SDLK_q:    // if q is pressed
			// quit the program
			quit = true;
			break;
		    default:;
		}
	    }
	}
    }

    clean_up();
    return 0;
}

SDL_Surface *load_img(char *filename) {
    SDL_Surface *img = NULL;
    if ((img=IMG_Load(filename)) != NULL)
	img = SDL_DisplayFormat(img);
    return img;
}

void blit_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, NULL, dst, &offset);
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
	fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
	return false;
    } if (TTF_Init() == -1) {
	fprintf(stderr, "TTF_Init() failed: %s\n", TTF_GetError());
	return false;
    } if ((scr=SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE))==NULL) {
	fprintf(stderr, "SDL_SetVideoMode() failed: %s\n", SDL_GetError());
	return false;
    // initialize SDL_Mixer
    } if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
	fprintf(stderr, "Mix_OpenAudio() failed: %s\n", Mix_GetError());
	return false;
    }
    SDL_WM_SetCaption("Play some sounds!", NULL);
    return true;
}

bool load_files() {
    if ((bg=load_img("background.png")) == NULL) {
	fprintf(stderr, "load_img(\"background.png\") failed to load\n");
	return false;
    } if ((font=TTF_OpenFont("lazy.ttf", 30)) == NULL) {
	fprintf(stderr, "TTF_OpenFont() failed: %s\n", TTF_GetError());
	return false;
    // load the music
    } if ((music=Mix_LoadMUS("beat.wav")) == NULL) {
	fprintf(stderr, "Mix_LoadMUS() failed: %s\n", Mix_GetError());
	return false;
    }
    // load the sfxs
    scratch = Mix_LoadWAV("scratch.wav");
    high = Mix_LoadWAV("high.wav");
    med = Mix_LoadWAV("medium.wav");
    low = Mix_LoadWAV("low.wav");
    if (scratch == NULL || high == NULL || med == NULL || low == NULL) {
	fprintf(stderr, "Mix_LoadWAV() failed: %s\n", Mix_GetError());
	return false;
    }
    return true;
}

void clean_up() {
    // free the sfxs
    Mix_FreeChunk(scratch);
    Mix_FreeChunk(high);
    Mix_FreeChunk(med);
    Mix_FreeChunk(low);
    // free the music
    Mix_FreeMusic(music);
    // quit the SDL_Mixer
    Mix_CloseAudio();

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_FreeSurface(bg);
    SDL_FreeSurface(msg);
    SDL_Quit();
}
