#include "c_animation.h"
#include "ecs.h"
#include "ecs_components.h"

static void _init(C_Animation *self, Entity entity) {
}

AnimationFrames create_animation(ivec2s *frames, u64 n_frames, f32 frame_duration) {
	AnimationFrames self;

	/* self.frames = staticlist_from(frames, n_frames, sizeof(ivec2s)); */
	self.current_frame = 0;
	self.frame_duration = frame_duration;
	self.elapsed_time = 0.f;

	return self;
}

void c_animation_init(struct ECS *ecs) {
	ECS_REGISTER(
		C_ANIMATION,
		C_Animation,
		ecs,
		((ECS_System){
			.init = (ECS_Subscriber) _init,
			.destroy = NULL,
			.render = NULL,
			.tick = NULL
		})
	);
}
