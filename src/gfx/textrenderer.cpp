#include "textrenderer.hpp"
#include "renderer.hpp"

static const i16 TEXT_Z_INDEX = 9;

static glm::ivec2 _char_to_alphabet_coords(char c) {
	glm::ivec2 pos;
	
	const u8 _all_caps_root = 65;
	const u8 _regular_root = 97;

	if(c >= _regular_root) {
		pos.y = 0;
		pos.x = c - _regular_root;
		return pos;
	}
	pos.y = 1;
	pos.x = c - _all_caps_root;
	return pos;
}

void TextRenderer::render_text() {
	auto view = gamestate::ecs->view<Text, Transform>();

	view.foreach([&](EntityID id, Text& text, Transform& root) {
		char *ptr = (char *)text.text;
		glm::ivec2 pos = {0, 0};
		while(*ptr) {
			if(*ptr == ' ') {
				pos.x++;
				if(pos.x >= text.bounds.x && text.bounds.x > 0) {
					pos.x = 0;
					pos.y++;
					if(pos.y >= text.bounds.y && text.bounds.y > 0) {
						break;
					}
				}
				ptr++;
				continue;
			}
			Sprite spr = {
				.z_index = TEXT_Z_INDEX,
				.atlas_coords = _char_to_alphabet_coords(*ptr),
				.size = {text.size, text.size},
				.atlas_index = TEXT_ATLAS,
			};
			Transform trans = {
				.position = {
					root.position.x + (text.size*pos.x) * root.scale.x,
					root.position.y + (text.size*pos.y) * root.scale.y,
				},
				.scale = root.scale,
				.rotation = root.rotation
			};

			gamestate::renderer->add_sprite_to_batch(spr, trans);
			pos.x++;
			if(pos.x >= text.bounds.x && text.bounds.x > 0) {
				pos.x = 0;
				pos.y++;
				if(pos.y >= text.bounds.y && text.bounds.y > 0) {
					break;
				}
			}
			ptr++;
		}
	});
}
