#pragma once
#include "DB.hpp"

namespace SivImGui::Reflection
{
	namespace detail
	{
		Texture TextureNameParser(StringView str);

		Font FontNameParser(StringView str);
	}
	
	class XMLParser
	{
	public:

		XMLParser(const DB& db)
			: m_db(db) {}

	public:

		std::function<Texture(StringView)> textureNameParser = detail::TextureNameParser;

		std::function<Font(StringView)> fontNameParser = detail::FontNameParser;

		std::unique_ptr<WidgetBase> parse(const XMLElement& elem);

	private:

		const DB& m_db;

		std::any parseProperty(std::type_index type, const StringView str) const;
	};
}
