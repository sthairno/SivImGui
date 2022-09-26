#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class Image : public Widget
	{
	public:

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(Image).hash_code(),
				.name = U"Image",
				.enableMouseOver = false
			};
			return info;
		}

	public:

		Image()
			: Widget(TypeInfo()) { }

	public:

		SIVIMGUI_LAYOUT_PROPERTY(Texture, texture, { });

		SIVIMGUI_LAYOUT_PROPERTY(bool, fitScale, false);

		SIVIMGUI_PROPERTY(ColorF, diffuse, Palette::White);

	protected:

		virtual Size region() const override
		{
			return fitScale ? Size{ 0, 0 } : texture->size();
		}

		virtual void draw(Rect rect) const override
		{
			if (fitScale)
			{
				const Size textureSize = texture->size();
				double scale = Min(rect.w / textureSize.x, rect.h / textureSize.y);
				texture->scaled(scale)
					.drawAt(rect.center(), diffuse);
			}
			else
			{
				texture->drawAt(rect.center(), diffuse);
			}
		}
	};
}
