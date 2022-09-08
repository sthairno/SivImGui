#pragma once
#include "../Reflection/DB.hpp"

namespace SivImGui::Util
{
	class XMLParser
	{
	public:

		XMLParser(const Reflection::DB& db)
			: m_db(db) {}

	public:

		std::unique_ptr<WidgetBase> parse(const XMLElement& elem);

	private:

		const Reflection::DB& m_db;
	};
}
