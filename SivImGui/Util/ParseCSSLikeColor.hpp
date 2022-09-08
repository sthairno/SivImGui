#pragma once
#include <Siv3D.hpp>

// Ref: https://developer.mozilla.org/ja/docs/Web/CSS/color

namespace SivImGui::Util
{
	Optional<ColorF> ParseCSSLikeColor(const StringView str);
}
