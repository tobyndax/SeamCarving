#pragma once
#include <string>
#include <memory>
#include <vector>

class SDL_Window;
typedef union SDL_Event;
class SDL_Rect;
class SDL_Renderer;
class SDL_Texture;

class PNGViewer {
public:
	PNGViewer();
	~PNGViewer(); //Explicit destructor

	PNGViewer(const PNGViewer&) = delete; //Disallow the copy constructor
	PNGViewer& operator=(const PNGViewer&) = delete; //Disallow copy assign

	PNGViewer(PNGViewer&&); //Allow move constructor since it is needed for static?
	PNGViewer& operator=(PNGViewer&&) = delete; //Disallow move assign


	unsigned char* getData();
	unsigned char* getDataSingleChannel();

	void setData(unsigned char* data, int width = 0, int height = 0);
	void setData(std::shared_ptr<std::vector<unsigned char>> data, int width = 0, int height = 0);
	void setDataFromFile(std::string& filename);
	void show();
	void showWaitForEsc();


	int getWidth() { return w; }
	int getHeight() { return h; }

private:
	void createWindow();
	void destroyWindow();
	void destroyTextures();
	void generateTexture();

	unsigned error = { 0 };
	unsigned char* image = nullptr;
	unsigned w = { 0 }, h = { 0 }, x = { 0 }, y = { 0 };
	SDL_Window* window = nullptr;
	SDL_Event* event = nullptr;
	int done = { 0 };
	size_t jump = 1;
	SDL_Rect* logo_dst = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* png_texture = nullptr;

};
