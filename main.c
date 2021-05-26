#include <stdint.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 270
#define SCALE 4

#define LEFT_PIXEL(x) x-1
#define RIGHT_PIXEL(x) x+1
#define UP_PIXEL(x) x + SCREEN_WIDTH
#define DOWN_PIXEL(x) x - SCREEN_WIDTH
#define UP_LEFT(x) UP_PIXEL(x) - 1
#define UP_RIGHT(x) UP_PIXEL(x) + 1
#define DOWN_LEFT(x) DOWN_PIXEL(x) - 1
#define DOWN_RIGHT(x) DOWN_PIXEL(x) + 1

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

//Display Grid. Stored in row major form. temp_grid is used as a buffer for changes.
unsigned int* grid;
unsigned int* temp_grid;

//Set all grid values to 0
void clearGrid( unsigned int* g)
{
	for(unsigned int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) g[i] = 0;
}

//Place a cell on mouse cursor
void place_cell()
{
	int x = 0, y = 0;
	SDL_GetMouseState( &x, &y);
	
	x /= SCALE;
	y /= SCALE;

	grid[(y * SCREEN_WIDTH) + x] = 0xFFFFFFFF;

}

//Calculate next generation 
void calculate()
{
	for(unsigned int i = 0; i < SCREEN_HEIGHT; ++i)
	{
		for(unsigned int j = 0; j < SCREEN_WIDTH; ++j)
		{
			unsigned int count = 0;
			unsigned int current = (i * SCREEN_WIDTH) + j;
			
			if(j != 0 && grid[LEFT_PIXEL(current)]) 					count++;
			if(j != SCREEN_WIDTH - 1 && grid[RIGHT_PIXEL(current)]) 			count++;
			if(i != SCREEN_HEIGHT -1 && grid[UP_PIXEL(current)]) 				count++;
			if(i != 0 && grid[DOWN_PIXEL(current)]) 					count++;
			if(j != 0 && i != SCREEN_HEIGHT - 1 && grid[UP_LEFT(current)]) 			count++;
			if(j != SCREEN_WIDTH - 1 && i != SCREEN_HEIGHT - 1 && grid[UP_RIGHT(current)]) 	count++;
			if(j != 0 && i != 0 && grid[DOWN_LEFT(current)]) 	  			count++;
			if(j != SCREEN_WIDTH - 1 && i != 0 && grid[DOWN_RIGHT(current)]) 		count++;

			if(grid[current] == 0xFFFFFFFF)
			{
				if(count < 2 || count > 3) temp_grid[current] = 0;
				else temp_grid[current] = 0xFFFFFFFF;
			}
			else 
			{
				if(count == 3) temp_grid[current] = 0xFFFFFFFF;	
				else temp_grid[current] = 0;
			}

		}
	}
	
	//Swapping Arrays
	unsigned int* temp = grid;
	grid = temp_grid;
	temp_grid = temp;


}

int main(int argc, char** arv)
{
	//To keep track of fps
	uint32_t start_time, frame_time;
	float fps;
	
	//Window title
	char title[30] = "Game of Life";

	//States
	int quit = 0, pause = 0, is_click = 0;
	
	//The bigger the slower
	grid = (unsigned int*)malloc(sizeof(unsigned int) * SCREEN_WIDTH * SCREEN_HEIGHT);
	temp_grid = (unsigned int*)malloc(sizeof(unsigned int) * SCREEN_WIDTH * SCREEN_HEIGHT);

	SDL_Event evnt;

	//Initializing SDL
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Could not initialize SDL");
		return 1;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	

	clearGrid(grid);
	clearGrid(temp_grid);

	while(!quit)
	{
		start_time = SDL_GetTicks();
		if (is_click == 1) place_cell();

		//Taking input
		while(SDL_PollEvent(&evnt))
		{
			switch(evnt.type)
			{
				case SDL_QUIT: quit = 1; break;
				case SDL_MOUSEBUTTONDOWN: 
					if(evnt.button.button == SDL_BUTTON_LEFT)
						pause = 1;
						is_click = 1;
					break;
				
				case SDL_MOUSEBUTTONUP:
					if(evnt.button.button == SDL_BUTTON_LEFT)
						is_click = 0;
						pause = 0;
					break;
				
				case SDL_KEYDOWN:
					if(evnt.key.keysym.sym == SDLK_ESCAPE) clearGrid(grid);
					break;

				default: break;
			}
		}
		
		//Calculating the generation
		if (!pause) calculate();
		
		//Display
		SDL_UpdateTexture(texture, NULL, (const void*)grid, sizeof(grid[0])*SCREEN_WIDTH);
   		SDL_RenderClear(renderer);
    		SDL_RenderCopy(renderer, texture, NULL, NULL);
    		SDL_RenderPresent(renderer);
		
		//Hardcoded Delay. 
		SDL_Delay(10);
		
		//Calculating FPS and displaying it
		frame_time = SDL_GetTicks() - start_time;
		fps = frame_time > 0 ? 1000.0f/frame_time : 0;
		snprintf(title, 29, "Game of Life FPS:%f", fps); 
		SDL_SetWindowTitle(window, title);


	}

	//Cleaning SDL
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	free(grid);
	free(temp_grid);
	return 0;
}
