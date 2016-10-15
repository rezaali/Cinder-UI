#pragma once

namespace reza {
namespace ui {

enum class Trigger {
	NONE = 0x00,
	BEGIN = 0x01,
	CHANGE = 0x02,
	END = 0x04,
	ALL = 0x07
};
}
} // namespace reza::ui
