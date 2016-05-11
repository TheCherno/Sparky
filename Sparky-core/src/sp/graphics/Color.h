#pragma once

#include "sp\Common.h"
#include "sp\maths\vec3.h"
#include "sp\maths\vec4.h"
#include "sp\maths\maths_func.h"


namespace sp { namespace graphics {

	class SP_API Color
	{
	private:
		maths::vec3 m_Color;

	public:
		/*
		Collection of useful colors.
		*/
		const static maths::vec3 AliceBlue;
		const static maths::vec3 Color::AntiqueWhite;
		const static maths::vec3 AquaMarine;
		const static maths::vec3 Azure;
		const static maths::vec3 Beige;
		const static maths::vec3 BakerChocolate;
		const static maths::vec3 Banana;
		const static maths::vec3 Black;
		const static maths::vec3 Blue;
		const static maths::vec3 BlueViolet;
		const static maths::vec3 Brass;
		const static maths::vec3 BrightGold;
		const static maths::vec3 Brown;
		const static maths::vec3 Bronze;
		const static maths::vec3 Bronze2;
		const static maths::vec3 CadetBlue;
		const static maths::vec3 CoolCopper;
		const static maths::vec3 Copper;
		const static maths::vec3 Coral;
		const static maths::vec3 CornFlower;
		const static maths::vec3 Crimson;
		const static maths::vec3 Cyan;
		const static maths::vec3 DarkBrown;
		const static maths::vec3 DarkGreen;
		const static maths::vec3 DarkGreenCopper;
		const static maths::vec3 DarkOliveGreen;
		const static maths::vec3 DarkOrchid;
		const static maths::vec3 DarkPurple;
		const static maths::vec3 DarkRed;
		const static maths::vec3 DarkSlateBlue;
		const static maths::vec3 DarkSlateGray;
		const static maths::vec3 DarkTan;
		const static maths::vec3 DarkTurquoise;
		const static maths::vec3 DarkWood;
		const static maths::vec3 DimGray;
		const static maths::vec3 DustyRose;
		const static maths::vec3 EggShell;
		const static maths::vec3 Feldspar;
		const static maths::vec3 FireBrick;
		const static maths::vec3 ForestGreen;
		const static maths::vec3 Gold;
		const static maths::vec3 GoldenRod;
		const static maths::vec3 Gray;
		const static maths::vec3 Green;
		const static maths::vec3 GreenCopper;
		const static maths::vec3 GreenYellow;
		const static maths::vec3 HunterGreen;
		const static maths::vec3 IndianRed;
		const static maths::vec3 Khaki;
		const static maths::vec3 LightBlue;
		const static maths::vec3 LightGray;
		const static maths::vec3 LightSteelBlue;
		const static maths::vec3 LightWood;
		const static maths::vec3 LimeGreen;
		const static maths::vec3 Magenta;
		const static maths::vec3 MandarinOrange;
		const static maths::vec3 Maroon;
		const static maths::vec3 MidAquaMarine;
		const static maths::vec3 MidBlue;
		const static maths::vec3 MidForestGreen;
		const static maths::vec3 MidGoldenRod;
		const static maths::vec3 MidOrchid;
		const static maths::vec3 MidSeaGreen;
		const static maths::vec3 MidSlateBlue;
		const static maths::vec3 MidSpringGreen;
		const static maths::vec3 MidTurquoise;
		const static maths::vec3 MidVioletRed;
		const static maths::vec3 MidWood;
		const static maths::vec3 MidnightBlue;
		const static maths::vec3 NavyBlue;
		const static maths::vec3 NeonBlue;
		const static maths::vec3 NeonPink;
		const static maths::vec3 NewMidnightBlue;
		const static maths::vec3 NewTan;
		const static maths::vec3 OldGold;
		const static maths::vec3 Orange;
		const static maths::vec3 OrangeRed;
		const static maths::vec3 Orchid;
		const static maths::vec3 PaleGreen;
		const static maths::vec3 Pink;
		const static maths::vec3 Plum;
		const static maths::vec3 Quartz;
		const static maths::vec3 Red;
		const static maths::vec3 RichBlue;
		const static maths::vec3 Salmon;
		const static maths::vec3 Scarlet;
		const static maths::vec3 SeaGreen;
		const static maths::vec3 SemiSweetChocolate;
		const static maths::vec3 Sepia;
		const static maths::vec3 Sienna;
		const static maths::vec3 Silver;
		const static maths::vec3 SkyBlue;
		const static maths::vec3 SlateBlue;
		const static maths::vec3 SpicyPink;
		const static maths::vec3 SpringGreen;
		const static maths::vec3 SteelBlue;
		const static maths::vec3 SummerSky;
		const static maths::vec3 Tan;
		const static maths::vec3 Teal;
		const static maths::vec3 Thistle;
		const static maths::vec3 Tomato;
		const static maths::vec3 Turquoise;
		const static maths::vec3 VeryDarkBrown;
		const static maths::vec3 VeryLightGrey;
		const static maths::vec3 Violet;
		const static maths::vec3 VioletRed;
		const static maths::vec3 Wheat;
		const static maths::vec3 White;
		const static maths::vec3 Yellow;
		const static maths::vec3 YellowGreen;


		/*
		Lights based on illuminance.
		*/

		// Colors used when light is heated.
		const static maths::vec3 Illum_Candle;
		const static maths::vec3 Illum_40WTungsten;
		const static maths::vec3 Illum_100WTungsten;
		const static maths::vec3 Illum_Halogen;
		const static maths::vec3 Illum_CarbonArc;
		const static maths::vec3 Day_Noon;
		const static maths::vec3 Day_Sunlight;
		const static maths::vec3 Day_Overcast;
		const static maths::vec3 Day_ClearBlue;

		/*
		Light sources that produces a large amount of UV light
		from high voltage.
		*/
		const static maths::vec3 Flourescent;
		const static maths::vec3 Flourescent_BlackLight;
		const static maths::vec3 Flourescent_Blue;
		const static maths::vec3 Flourescent_CoolWhite;
		const static maths::vec3 Flourescent_FullSpectrum;
		const static maths::vec3 Flourescent_GlowLight;
		const static maths::vec3 Flouresent_Standard;
		const static maths::vec3 Flourescent_Warm;

		/*
		Light sources that involves gas and high voltage coils.
		*/
		const static maths::vec3 Gas_Mercury;
		const static maths::vec3 Gas_Sodium;
		const static maths::vec3 Gas_Halide;
		const static maths::vec3 Gas_Sodium_HighPressure;

		Color();
		Color(const maths::vec3& other);
		Color(float grayScale);
		Color(float red, float green, float blue);	
		~Color();

		/*
		User-created colors.
		*/
		inline maths::vec3 Get() const { return m_Color; }
		inline void Set(const maths::vec3& color) { m_Color = color; }
		inline void Set(float red, float green, float blue)
		{
			m_Color.x = red;
			m_Color.y = green;
			m_Color.z = blue;
		}

		static inline Color Lerp(const Color& color1, float alpha1, const Color& color2, float alpha2, float amount)
		{
			return Color(
				maths::MathFunc::lerp(color1.Get().x, color2.Get().x, amount),
				maths::MathFunc::lerp(color1.Get().y, color2.Get().y, amount),
				maths::MathFunc::lerp(color1.Get().z, color2.Get().z, amount));
		}

		static inline Color LerpPrecise(const Color& color1, float alpha1, const Color& color2, float alpha2, float amount)
		{
			return Color(
				maths::MathFunc::lerpPrecision(color1.Get().x, color2.Get().x, amount),
				maths::MathFunc::lerpPrecision(color1.Get().y, color2.Get().y, amount),
				maths::MathFunc::lerpPrecision(color1.Get().z, color2.Get().z, amount));
		}

		

		/*
		Modifies the color brilliance in varying degrees around base color.
		*/
		static inline maths::vec3 Color::Concentration(const maths::vec3& color, float temperature, float scaleFactor = 2.0f);
		static inline maths::vec3 Color::Concentration(const maths::vec3& color, float temperature, float factorRed, float factorGreen, float factorBlue);
		static inline maths::vec3 Color::Stylize(const maths::vec3& color, float temperature, float scaleFactor, float weight);
		static inline maths::vec3 Color::Stylize(const maths::vec3& color, float temperature, float factorRed, float factorGreen, float factorBlue, float weight);
		};	
	}
}
