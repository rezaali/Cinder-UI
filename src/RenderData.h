#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace reza {
namespace ui {

struct RenderData {
	glm::vec3 pos = glm::vec3( 0, 0, 0 );
	glm::vec2 uv = glm::vec2( 0, 0 );
	glm::vec4 color = glm::vec4( 0, 0, 0, 0 );
};
}
} // namespace reza::ui
