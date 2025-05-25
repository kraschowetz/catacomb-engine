#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "../core/gamestate.hpp"
#include "../ecs/text.hpp"

class TextRenderer {
private:
	std::vector<EntityID> text_entities;
public:
	void render_text();
};

#endif // TEXTRENDERER_H
