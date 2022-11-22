#include "ParseCSSLikeColor.hpp"

namespace SivImGui::Util
{
	const static std::map<StringView, ColorF> ColorDic {
		{ U"transparent", ColorF::Zero() },
		{ U"black", Linear::Palette::Black },
		{ U"dimgray", Linear::Palette::Dimgray },
		{ U"dimgrey", Linear::Palette::Dimgrey },
		{ U"gray", Linear::Palette::Gray },
		{ U"grey", Linear::Palette::Grey },
		{ U"darkgray", Linear::Palette::Darkgray },
		{ U"darkgrey", Linear::Palette::Darkgrey },
		{ U"silver", Linear::Palette::Silver },
		{ U"lightgray", Linear::Palette::Lightgray },
		{ U"lightgrey", Linear::Palette::Lightgrey },
		{ U"gainsboro", Linear::Palette::Gainsboro },
		{ U"whitesmoke", Linear::Palette::Whitesmoke },
		{ U"white", Linear::Palette::White },
		{ U"snow", Linear::Palette::Snow },
		{ U"ghostwhite", Linear::Palette::Ghostwhite },
		{ U"floralwhite", Linear::Palette::Floralwhite },
		{ U"linen", Linear::Palette::Linen },
		{ U"antiquewhite", Linear::Palette::Antiquewhite },
		{ U"papayawhip", Linear::Palette::Papayawhip },
		{ U"blanchedalmond", Linear::Palette::Blanchedalmond },
		{ U"bisque", Linear::Palette::Bisque },
		{ U"moccasin", Linear::Palette::Moccasin },
		{ U"navajowhite", Linear::Palette::Navajowhite },
		{ U"peachpuff", Linear::Palette::Peachpuff },
		{ U"mistyrose", Linear::Palette::Mistyrose },
		{ U"lavenderblush", Linear::Palette::Lavenderblush },
		{ U"seashell", Linear::Palette::Seashell },
		{ U"oldlace", Linear::Palette::Oldlace },
		{ U"ivory", Linear::Palette::Ivory },
		{ U"honeydew", Linear::Palette::Honeydew },
		{ U"mintcream", Linear::Palette::Mintcream },
		{ U"azure", Linear::Palette::Azure },
		{ U"aliceblue", Linear::Palette::Aliceblue },
		{ U"lavender", Linear::Palette::Lavender },
		{ U"lightsteelblue", Linear::Palette::Lightsteelblue },
		{ U"lightslategray", Linear::Palette::Lightslategray },
		{ U"lightslategrey", Linear::Palette::Lightslategrey },
		{ U"slategray", Linear::Palette::Slategray },
		{ U"slategrey", Linear::Palette::Slategrey },
		{ U"steelblue", Linear::Palette::Steelblue },
		{ U"royalblue", Linear::Palette::Royalblue },
		{ U"midnightblue", Linear::Palette::Midnightblue },
		{ U"navy", Linear::Palette::Navy },
		{ U"darkblue", Linear::Palette::Darkblue },
		{ U"mediumblue", Linear::Palette::Mediumblue },
		{ U"blue", Linear::Palette::Blue },
		{ U"dodgerblue", Linear::Palette::Dodgerblue },
		{ U"cornflowerblue", Linear::Palette::Cornflowerblue },
		{ U"deepskyblue", Linear::Palette::Deepskyblue },
		{ U"lightskyblue", Linear::Palette::Lightskyblue },
		{ U"skyblue", Linear::Palette::Skyblue },
		{ U"lightblue", Linear::Palette::Lightblue },
		{ U"powderblue", Linear::Palette::Powderblue },
		{ U"paleturquoise", Linear::Palette::Paleturquoise },
		{ U"lightcyan", Linear::Palette::Lightcyan },
		{ U"cyan", Linear::Palette::Cyan },
		{ U"aqua", Linear::Palette::Aqua },
		{ U"turquoise", Linear::Palette::Turquoise },
		{ U"mediumturquoise", Linear::Palette::Mediumturquoise },
		{ U"darkturquoise", Linear::Palette::Darkturquoise },
		{ U"lightseagreen", Linear::Palette::Lightseagreen },
		{ U"cadetblue", Linear::Palette::Cadetblue },
		{ U"darkcyan", Linear::Palette::Darkcyan },
		{ U"teal", Linear::Palette::Teal },
		{ U"darkslategray", Linear::Palette::Darkslategray },
		{ U"darkslategrey", Linear::Palette::Darkslategrey },
		{ U"darkgreen", Linear::Palette::Darkgreen },
		{ U"green", Linear::Palette::Green },
		{ U"forestgreen", Linear::Palette::Forestgreen },
		{ U"seagreen", Linear::Palette::Seagreen },
		{ U"mediumseagreen", Linear::Palette::Mediumseagreen },
		{ U"mediumaquamarine", Linear::Palette::Mediumaquamarine },
		{ U"darkseagreen", Linear::Palette::Darkseagreen },
		{ U"aquamarine", Linear::Palette::Aquamarine },
		{ U"palegreen", Linear::Palette::Palegreen },
		{ U"lightgreen", Linear::Palette::Lightgreen },
		{ U"springgreen", Linear::Palette::Springgreen },
		{ U"mediumspringgreen", Linear::Palette::Mediumspringgreen },
		{ U"lawngreen", Linear::Palette::Lawngreen },
		{ U"chartreuse", Linear::Palette::Chartreuse },
		{ U"greenyellow", Linear::Palette::Greenyellow },
		{ U"lime", Linear::Palette::Lime },
		{ U"limegreen", Linear::Palette::Limegreen },
		{ U"yellowgreen", Linear::Palette::Yellowgreen },
		{ U"darkolivegreen", Linear::Palette::Darkolivegreen },
		{ U"olivedrab", Linear::Palette::Olivedrab },
		{ U"olive", Linear::Palette::Olive },
		{ U"darkkhaki", Linear::Palette::Darkkhaki },
		{ U"palegoldenrod", Linear::Palette::Palegoldenrod },
		{ U"cornsilk", Linear::Palette::Cornsilk },
		{ U"beige", Linear::Palette::Beige },
		{ U"lightyellow", Linear::Palette::Lightyellow },
		{ U"lightgoldenrodyellow", Linear::Palette::Lightgoldenrodyellow },
		{ U"lemonchiffon", Linear::Palette::Lemonchiffon },
		{ U"wheat", Linear::Palette::Wheat },
		{ U"burlywood", Linear::Palette::Burlywood },
		{ U"tan", Linear::Palette::Tan },
		{ U"khaki", Linear::Palette::Khaki },
		{ U"yellow", Linear::Palette::Yellow },
		{ U"gold", Linear::Palette::Gold },
		{ U"orange", Linear::Palette::Orange },
		{ U"sandybrown", Linear::Palette::Sandybrown },
		{ U"darkorange", Linear::Palette::Darkorange },
		{ U"goldenrod", Linear::Palette::Goldenrod },
		{ U"peru", Linear::Palette::Peru },
		{ U"darkgoldenrod", Linear::Palette::Darkgoldenrod },
		{ U"chocolate", Linear::Palette::Chocolate },
		{ U"sienna", Linear::Palette::Sienna },
		{ U"saddlebrown", Linear::Palette::Saddlebrown },
		{ U"maroon", Linear::Palette::Maroon },
		{ U"darkred", Linear::Palette::Darkred },
		{ U"brown", Linear::Palette::Brown },
		{ U"firebrick", Linear::Palette::Firebrick },
		{ U"indianred", Linear::Palette::Indianred },
		{ U"rosybrown", Linear::Palette::Rosybrown },
		{ U"darksalmon", Linear::Palette::Darksalmon },
		{ U"lightcoral", Linear::Palette::Lightcoral },
		{ U"salmon", Linear::Palette::Salmon },
		{ U"lightsalmon", Linear::Palette::Lightsalmon },
		{ U"coral", Linear::Palette::Coral },
		{ U"tomato", Linear::Palette::Tomato },
		{ U"orangered", Linear::Palette::Orangered },
		{ U"red", Linear::Palette::Red },
		{ U"crimson", Linear::Palette::Crimson },
		{ U"mediumvioletred", Linear::Palette::Mediumvioletred },
		{ U"deeppink", Linear::Palette::Deeppink },
		{ U"hotpink", Linear::Palette::Hotpink },
		{ U"palevioletred", Linear::Palette::Palevioletred },
		{ U"pink", Linear::Palette::Pink },
		{ U"lightpink", Linear::Palette::Lightpink },
		{ U"thistle", Linear::Palette::Thistle },
		{ U"magenta", Linear::Palette::Magenta },
		{ U"fuchsia", Linear::Palette::Fuchsia },
		{ U"violet", Linear::Palette::Violet },
		{ U"plum", Linear::Palette::Plum },
		{ U"orchid", Linear::Palette::Orchid },
		{ U"mediumorchid", Linear::Palette::Mediumorchid },
		{ U"darkorchid", Linear::Palette::Darkorchid },
		{ U"darkviolet", Linear::Palette::Darkviolet },
		{ U"darkmagenta", Linear::Palette::Darkmagenta },
		{ U"purple", Linear::Palette::Purple },
		{ U"indigo", Linear::Palette::Indigo },
		{ U"darkslateblue", Linear::Palette::Darkslateblue },
		{ U"blueviolet", Linear::Palette::Blueviolet },
		{ U"mediumpurple", Linear::Palette::Mediumpurple },
		{ U"slateblue", Linear::Palette::Slateblue },
		{ U"mediumslateblue", Linear::Palette::Mediumslateblue }
	};

	const static RegExp FuncRgx{ UR"(^([a-z]+)\((.*)\)$)" };

	const static RegExp HexColorRgx{ UR"(^(#[\d|a-f]{3,8})$)" };

	const static RegExp RgbArgsRgx{ UR"(^\s*(?<r>[\d\.]+)(?<p>%?)\s*,\s*(?<g>[\d\.]+)\k<p>\s*,\s*(?<b>[\d\.]+)\k<p>\s*(,\s*(?<a>[\d\.]+)(?<ap>%?)\s*)?$)" };

	Optional<ColorF> ParseCSSLikeColor(const StringView s)
	{
		if (s.empty())
		{
			return none;
		}

		String str{ s };
		str.trim();
		str.lowercase();

		// named-color
		{
			auto itr = ColorDic.find(str);
			if (itr != ColorDic.cend())
			{
				return itr->second;
			}
		}

		// hex-color
		if (HexColorRgx.fullMatch(str))
		{
			const StringView hex = StringView(str).substr(1);

			switch (str.size())
			{
			case 4: // #rgb
			case 5: // #rgba
			case 7: // #rrggbb
			case 9: // #rrggbbaa
				return Color{ str };
			}

			return none;
		}

		// abc( 1, 2, 3, def )
		// -> funcName: "abc"
		//    funcArg:  " 1, 2, 3, def "

		StringView funcName, funcArg;
		if (auto match = FuncRgx.match(str))
		{
			funcName = *match[1];
			funcArg = *match[2];
		}
		else
		{
			return none;
		}

		// rgb(r,g,b(,a)), rgba(r,g,b(,a))
		if (funcName == U"rgb" || funcName == U"rgba")
		{
			const MatchResults match = RgbArgsRgx.match(funcArg);

			if (match.isEmpty())
			{
				return none;
			}

			ColorF col;

			const auto r = ParseOpt<double>(*match[1]);
			const auto g = ParseOpt<double>(*match[3]);
			const auto b = ParseOpt<double>(*match[4]);

			if (!r || !g || !b)
			{
				return none;
			}

			{
				// 0% ~ 100%, 0 ~ 255
				const double mul = (match[2] == U"%" ? 1. / 100 : 1. / 255);
				col.r = Clamp(*r * mul, 0., 1.);
				col.g = Clamp(*g * mul, 0., 1.);
				col.b = Clamp(*b * mul, 0., 1.);
			}

			if (!match[5])
			{
				col.a = 1;
				return col;
			}

			const auto a = ParseOpt<double>(*match[5]);

			if (!a)
			{
				return none;
			}

			{
				// 0% ~ 100%, 0 ~ 1
				const double mul = (match[6] == U"%" ? 1. / 100 : 1.);
				col.a = Clamp(*a * mul, 0., 1.);
			}

			return col;
		}
		else if (funcName == U"hsl" || funcName == U"hsla")
		{
			return none;
		}

		return none;
	}
}
