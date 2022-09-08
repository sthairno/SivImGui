#include "ParseCSSLikeColor.hpp"

namespace SivImGui::Util
{
	const static HashTable<String, Color> colorDic = {
		{ U"transparent", Color::Zero() },
		{ U"black", Palette::Black },
		{ U"dimgray", Palette::Dimgray },
		{ U"dimgrey", Palette::Dimgrey },
		{ U"gray", Palette::Gray },
		{ U"grey", Palette::Grey },
		{ U"darkgray", Palette::Darkgray },
		{ U"darkgrey", Palette::Darkgrey },
		{ U"silver", Palette::Silver },
		{ U"lightgray", Palette::Lightgray },
		{ U"lightgrey", Palette::Lightgrey },
		{ U"gainsboro", Palette::Gainsboro },
		{ U"whitesmoke", Palette::Whitesmoke },
		{ U"white", Palette::White },
		{ U"snow", Palette::Snow },
		{ U"ghostwhite", Palette::Ghostwhite },
		{ U"floralwhite", Palette::Floralwhite },
		{ U"linen", Palette::Linen },
		{ U"antiquewhite", Palette::Antiquewhite },
		{ U"papayawhip", Palette::Papayawhip },
		{ U"blanchedalmond", Palette::Blanchedalmond },
		{ U"bisque", Palette::Bisque },
		{ U"moccasin", Palette::Moccasin },
		{ U"navajowhite", Palette::Navajowhite },
		{ U"peachpuff", Palette::Peachpuff },
		{ U"mistyrose", Palette::Mistyrose },
		{ U"lavenderblush", Palette::Lavenderblush },
		{ U"seashell", Palette::Seashell },
		{ U"oldlace", Palette::Oldlace },
		{ U"ivory", Palette::Ivory },
		{ U"honeydew", Palette::Honeydew },
		{ U"mintcream", Palette::Mintcream },
		{ U"azure", Palette::Azure },
		{ U"aliceblue", Palette::Aliceblue },
		{ U"lavender", Palette::Lavender },
		{ U"lightsteelblue", Palette::Lightsteelblue },
		{ U"lightslategray", Palette::Lightslategray },
		{ U"lightslategrey", Palette::Lightslategrey },
		{ U"slategray", Palette::Slategray },
		{ U"slategrey", Palette::Slategrey },
		{ U"steelblue", Palette::Steelblue },
		{ U"royalblue", Palette::Royalblue },
		{ U"midnightblue", Palette::Midnightblue },
		{ U"navy", Palette::Navy },
		{ U"darkblue", Palette::Darkblue },
		{ U"mediumblue", Palette::Mediumblue },
		{ U"blue", Palette::Blue },
		{ U"dodgerblue", Palette::Dodgerblue },
		{ U"cornflowerblue", Palette::Cornflowerblue },
		{ U"deepskyblue", Palette::Deepskyblue },
		{ U"lightskyblue", Palette::Lightskyblue },
		{ U"skyblue", Palette::Skyblue },
		{ U"lightblue", Palette::Lightblue },
		{ U"powderblue", Palette::Powderblue },
		{ U"paleturquoise", Palette::Paleturquoise },
		{ U"lightcyan", Palette::Lightcyan },
		{ U"cyan", Palette::Cyan },
		{ U"aqua", Palette::Aqua },
		{ U"turquoise", Palette::Turquoise },
		{ U"mediumturquoise", Palette::Mediumturquoise },
		{ U"darkturquoise", Palette::Darkturquoise },
		{ U"lightseagreen", Palette::Lightseagreen },
		{ U"cadetblue", Palette::Cadetblue },
		{ U"darkcyan", Palette::Darkcyan },
		{ U"teal", Palette::Teal },
		{ U"darkslategray", Palette::Darkslategray },
		{ U"darkslategrey", Palette::Darkslategrey },
		{ U"darkgreen", Palette::Darkgreen },
		{ U"green", Palette::Green },
		{ U"forestgreen", Palette::Forestgreen },
		{ U"seagreen", Palette::Seagreen },
		{ U"mediumseagreen", Palette::Mediumseagreen },
		{ U"mediumaquamarine", Palette::Mediumaquamarine },
		{ U"darkseagreen", Palette::Darkseagreen },
		{ U"aquamarine", Palette::Aquamarine },
		{ U"palegreen", Palette::Palegreen },
		{ U"lightgreen", Palette::Lightgreen },
		{ U"springgreen", Palette::Springgreen },
		{ U"mediumspringgreen", Palette::Mediumspringgreen },
		{ U"lawngreen", Palette::Lawngreen },
		{ U"chartreuse", Palette::Chartreuse },
		{ U"greenyellow", Palette::Greenyellow },
		{ U"lime", Palette::Lime },
		{ U"limegreen", Palette::Limegreen },
		{ U"yellowgreen", Palette::Yellowgreen },
		{ U"darkolivegreen", Palette::Darkolivegreen },
		{ U"olivedrab", Palette::Olivedrab },
		{ U"olive", Palette::Olive },
		{ U"darkkhaki", Palette::Darkkhaki },
		{ U"palegoldenrod", Palette::Palegoldenrod },
		{ U"cornsilk", Palette::Cornsilk },
		{ U"beige", Palette::Beige },
		{ U"lightyellow", Palette::Lightyellow },
		{ U"lightgoldenrodyellow", Palette::Lightgoldenrodyellow },
		{ U"lemonchiffon", Palette::Lemonchiffon },
		{ U"wheat", Palette::Wheat },
		{ U"burlywood", Palette::Burlywood },
		{ U"tan", Palette::Tan },
		{ U"khaki", Palette::Khaki },
		{ U"yellow", Palette::Yellow },
		{ U"gold", Palette::Gold },
		{ U"orange", Palette::Orange },
		{ U"sandybrown", Palette::Sandybrown },
		{ U"darkorange", Palette::Darkorange },
		{ U"goldenrod", Palette::Goldenrod },
		{ U"peru", Palette::Peru },
		{ U"darkgoldenrod", Palette::Darkgoldenrod },
		{ U"chocolate", Palette::Chocolate },
		{ U"sienna", Palette::Sienna },
		{ U"saddlebrown", Palette::Saddlebrown },
		{ U"maroon", Palette::Maroon },
		{ U"darkred", Palette::Darkred },
		{ U"brown", Palette::Brown },
		{ U"firebrick", Palette::Firebrick },
		{ U"indianred", Palette::Indianred },
		{ U"rosybrown", Palette::Rosybrown },
		{ U"darksalmon", Palette::Darksalmon },
		{ U"lightcoral", Palette::Lightcoral },
		{ U"salmon", Palette::Salmon },
		{ U"lightsalmon", Palette::Lightsalmon },
		{ U"coral", Palette::Coral },
		{ U"tomato", Palette::Tomato },
		{ U"orangered", Palette::Orangered },
		{ U"red", Palette::Red },
		{ U"crimson", Palette::Crimson },
		{ U"mediumvioletred", Palette::Mediumvioletred },
		{ U"deeppink", Palette::Deeppink },
		{ U"hotpink", Palette::Hotpink },
		{ U"palevioletred", Palette::Palevioletred },
		{ U"pink", Palette::Pink },
		{ U"lightpink", Palette::Lightpink },
		{ U"thistle", Palette::Thistle },
		{ U"magenta", Palette::Magenta },
		{ U"fuchsia", Palette::Fuchsia },
		{ U"violet", Palette::Violet },
		{ U"plum", Palette::Plum },
		{ U"orchid", Palette::Orchid },
		{ U"mediumorchid", Palette::Mediumorchid },
		{ U"darkorchid", Palette::Darkorchid },
		{ U"darkviolet", Palette::Darkviolet },
		{ U"darkmagenta", Palette::Darkmagenta },
		{ U"purple", Palette::Purple },
		{ U"indigo", Palette::Indigo },
		{ U"darkslateblue", Palette::Darkslateblue },
		{ U"blueviolet", Palette::Blueviolet },
		{ U"mediumpurple", Palette::Mediumpurple },
		{ U"slateblue", Palette::Slateblue },
		{ U"mediumslateblue", Palette::Mediumslateblue }
	};

	Optional<ColorF> ParseCSSLikeColor(const StringView s)
	{
		if (s.empty())
		{
			return none;
		}

		String str{ s };
		str.remove_if([](char32_t c) {
			return c == U' ' || c == U'\t'; // Whitespace
		});
		str.lowercase();

		// named-color
		{
			auto itr = colorDic.find(str);
			if (itr != colorDic.cend())
			{
				return itr->second;
			}
		}

		// hex-color
		if (str.starts_with(U'#'))
		{
			switch (str.size())
			{
			case 4: // #rgb
			case 5: // #rgba
			case 7: // #rrggbb
			case 9: // #rrggbbaa
				return Color{ str };
			default:
				return none;
			}
		}
		
		return none;
	}
}
