#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class Image : public Widget
	{
		SIVIMGUI_BUILDER_HELPER(Image);

		static Image& New(Builder& ctx, const Texture& texture)
		{
			auto& w = reinterpret_cast<Image&>(ctx.next(typeid(Image).hash_code(), [&] { return new Image(); }));
			w.texture = texture;
			return w;
		}

	public:

		Image() : Widget(typeid(Image).hash_code(), U"Image") { }

	public:

		Property<Texture> texture{ *this, { }, PropertyFlag::Layout};

		Property<bool> fitScale{ *this, false, PropertyFlag::Layout };

		Property<ColorF> diffuse{ *this, Palette::White };

	public:

		bool clicked() const { return m_clicked; }

	protected:

		bool m_clicked = false;

		virtual SizeF region() const override
		{
			return fitScale ? SizeF{ 0, 0 } : texture->size();
		}

		virtual void draw(RectF rect) const override
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
