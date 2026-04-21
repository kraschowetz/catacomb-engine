#include <cat/config.hpp>
#include <cat/util/math.hpp>
#include <cat/core/memory.hpp>

namespace cat
{

struct cTransform
{
	glm::vec3 position;	// global position, in pixels
	glm::vec3 scale;	// global scale
	glm::vec3 rotation;	// global rotation, in degrees
};

struct cLocalTransform
{
	glm::vec3 position;		// local position, in pixels
	glm::vec3 scale;		// local scale
	glm::vec3 rotation;		// local rotation, in degrees
	Watcher<cTransform> parent;	
};

}
