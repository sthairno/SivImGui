#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class Image : public Widget
	{
		SIVIMGUI_BUILDER_HELPER(Image);

		static Image& New(Builder& ctx, const Texture& texture)
		{
			auto& w = ctx.next<Image>();
			w.texture = texture;
			return w;
		}

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

		Property<Texture> texture{ this, { }, PropertyFlag::Layout};

		Property<bool> fitScale{ this, false, PropertyFlag::Layout };

		Property<ColorF> diffuse{ this, Palette::White };

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
