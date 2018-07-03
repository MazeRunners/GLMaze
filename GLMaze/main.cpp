#include "GLPlatform.h"
#include "Game.h"

int main() {
	GLPlatform platform;
	Game game = Game(&platform);
	game.start();
	return 0;
}
