#include "gfx.hpp"
#include "../core/gamestate.hpp"
#include "../ecs/components.hpp"
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>

void gfx::render_cycle() {
	gamestate::renderer->prepare();

	//sprite pass
	{
		auto view = gamestate::ecs->view<Transform, Sprite>();

		view.foreach([&](EntityID id, Transform& transform, Sprite& sprite){
			gamestate::renderer->add_sprite_to_batch(sprite, transform);
		});
		gamestate::renderer->clear_batch_buffer();
	}
	
	// text pass
	{
		auto view = gamestate::ecs->view<Transform, Text>();

		view.foreach([&](EntityID id, Transform& transform, Text& text){
			gamestate::renderer->add_text_to_batch(text, transform);
		});
		gamestate::renderer->clear_batch_buffer();
	}

	//camera update
	{
		using namespace gamestate;
		auto view = ecs->view<Transform, Camera2D>();

		view.foreach([&](EntityID id, Transform& transform, Camera2D& camera){
			glm::vec2 center_pos = transform.position - glm::vec2(400, 300);
			f32 delta = glm::distance(camera.position, center_pos);
			
			glm::vec2 dir = glm::normalize(
				center_pos - camera.position
			);

			if(delta <= camera.max_distance + 1) {
				camera.position += dir * camera.speed * delta_time;
			}
			else {
				camera.position += dir * (delta - camera.max_distance);
			}

			camera.view = glm::mat4(1);
			camera.view = glm::translate(
				camera.view,
				glm::vec3(
					-camera.position.x,
					-camera.position.y,
					-1.f
				)
			);

			camera.projection = glm::ortho(
				camera.min.x,
				camera.max.x,
				camera.max.y,
				camera.min.y,
				(f32)std::numeric_limits<i8>::min() - 1.f,
				(f32)std::numeric_limits<i8>::max() + 1.f
			);

			gamestate::renderer->bind_camera(camera.view, camera.projection);
		});
	}
	
	gamestate::renderer->display();
}
