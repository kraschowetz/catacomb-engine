#ifndef ANIMATION_H
#define ANIMATION_H

#include "../util/util.h"
#include <cglm/struct.h>

typedef struct {
	/* StaticList *frames;	// <ivec2s> */
	u64 current_frame;
	f32 frame_duration;
	f32 elapsed_time;
} AnimationFrames;

AnimationFrames create_animation(ivec2s *frames, u64 n_frames, f32 frame_duration);

typedef struct {
	i16 z_index;
	/* StaticList *animations;	// <AnimationFrames> */
	size_t current_animation;
	ivec2s size;
} C_Animation;

#endif // ANIMATION_H
