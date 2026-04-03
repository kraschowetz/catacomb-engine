#pragma once

// @file Canvas is an interface that represents a rendering target

#include <string_view>
#include <cat/config.hpp>

namespace cat
{
struct CanvasInfo
{
	std::string_view title;
	u32 width, height; 

	struct {
		u32 major, minor;
	} version;
};

struct iCanvas
{
	virtual void begin_frame() {};
	virtual void end_frame() {};
	virtual void poll_events() {};

	virtual ~iCanvas() = default;
};


class OffscreenCanvas : public iCanvas
{
public:
	virtual void begin_frame() override;
	virtual void end_frame() override;

	OffscreenCanvas(CanvasInfo& info);
	~OffscreenCanvas();

private:
};

}
