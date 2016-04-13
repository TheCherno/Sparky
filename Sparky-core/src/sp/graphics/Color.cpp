#include "sp\sp.h"
#include "Color.h"


namespace sp { namespace graphics {

	const maths::vec3 Color::AliceBlue = maths::vec3(0.941f, 0.973f, 1.0f);
	const maths::vec3 Color::AntiqueWhite = maths::vec3(0.98f, 0.922f, 0.843f);
	const maths::vec3 Color::AquaMarine = maths::vec3(0.4392157f, 0.85882354f, 0.5764706f);
	const maths::vec3 Color::Azure = maths::vec3(0.941f, 1.0f, 1.0f);
	const maths::vec3 Color::Beige = maths::vec3(0.961f, 0.961f, 0.863f);
	const maths::vec3 Color::BakerChocolate = maths::vec3(0.36078432f, 0.2f, 0.09019608f);
	const maths::vec3 Color::Banana = maths::vec3(0.89f, 0.812f, 0.34f);
	const maths::vec3 Color::Black = maths::vec3(0.0f, 0.0f, 0.0f);
	const maths::vec3 Color::Blue = maths::vec3(0.0f, 0.0f, 1.0f);
	const maths::vec3 Color::BlueViolet = maths::vec3(0.62352943f, 0.37254903f, 0.62352943f);
	const maths::vec3 Color::Brass = maths::vec3(0.70980394f, 0.6509804f, 0.25882354f);
	const maths::vec3 Color::BrightGold = maths::vec3(0.8509804f, 0.8509804f, 0.09803922f);
	const maths::vec3 Color::Brown = maths::vec3(0.6509804f, 0.16470589f, 0.16470589f);
	const maths::vec3 Color::Bronze = maths::vec3(0.54901963f, 0.47058824f, 0.3254902f);
	const maths::vec3 Color::Bronze2 = maths::vec3(0.6509804f, 0.49019608f, 0.23921569f);
	const maths::vec3 Color::CadetBlue = maths::vec3(0.37254903f, 0.62352943f, 0.62352943f);
	const maths::vec3 Color::CoolCopper = maths::vec3(0.8509804f, 0.5294118f, 0.09803922f);
	const maths::vec3 Color::Copper = maths::vec3(0.72156864f, 0.4509804f, 0.2f);
	const maths::vec3 Color::Coral = maths::vec3(1.0f, 0.49803922f, 0.0f);
	const maths::vec3 Color::Crimson = maths::vec3(0.863f, 0.078f, 0.235f);
	const maths::vec3 Color::CornFlower = maths::vec3(0.25882354f, 0.25882354f, 0.43529412f);
	const maths::vec3 Color::Cyan = maths::vec3(0.0f, 1.0f, 1.0f);
	const maths::vec3 Color::DarkBrown = maths::vec3(0.36078432f, 0.2509804f, 0.2f);
	const maths::vec3 Color::DarkGreen = maths::vec3(0.18431373f, 0.30980393f, 0.18431373f);
	const maths::vec3 Color::DarkGreenCopper = maths::vec3(0.2901961f, 0.4627451f, 0.43137255f);
	const maths::vec3 Color::DarkOliveGreen = maths::vec3(0.30980393f, 0.30980393f, 0.18431373f);
	const maths::vec3 Color::DarkOrchid = maths::vec3(0.6f, 0.19607843f, 0.8039216f);
	const maths::vec3 Color::DarkPurple = maths::vec3(0.5294118f, 0.12156863f, 0.47058824f);
	const maths::vec3 Color::DarkRed = maths::vec3(0.545f, 0.0f, 0.0f);
	const maths::vec3 Color::DarkSlateBlue = maths::vec3(0.41960785f, 0.13725491f, 0.5568628f);
	const maths::vec3 Color::DarkSlateGray = maths::vec3(0.18431373f, 0.30980393f, 0.30980393f);
	const maths::vec3 Color::DarkTan = maths::vec3(0.5921569f, 0.4117647f, 0.30980393f);
	const maths::vec3 Color::DarkTurquoise = maths::vec3(0.4392157f, 0.5764706f, 0.85882354f);
	const maths::vec3 Color::DarkWood = maths::vec3(0.52156866f, 0.36862746f, 0.25882354f);
	const maths::vec3 Color::DimGray = maths::vec3(0.32941177f, 0.32941177f, 0.32941177f);
	const maths::vec3 Color::DustyRose = maths::vec3(0.52156866f, 0.3882353f, 0.3882353f);
	const maths::vec3 Color::EggShell = maths::vec3(0.988f, 0.902f, 0.788f);
	const maths::vec3 Color::Feldspar = maths::vec3(0.81960785f, 0.57254905f, 0.45882353f);
	const maths::vec3 Color::FireBrick = maths::vec3(0.5568628f, 0.13725491f, 0.13725491f);
	const maths::vec3 Color::ForestGreen = maths::vec3(0.13725491f, 0.5568628f, 0.13725491f);
	const maths::vec3 Color::Gold = maths::vec3(0.8039216f, 0.49803922f, 0.19607843f);
	const maths::vec3 Color::GoldenRod = maths::vec3(0.85882354f, 0.85882354f, 0.4392157f);
	const maths::vec3 Color::Gray = maths::vec3(0.7529412f, 0.7529412f, 0.7529412f);
	const maths::vec3 Color::Green = maths::vec3(0.0f, 1.0f, 0.0f);
	const maths::vec3 Color::GreenCopper = maths::vec3(0.32156864f, 0.49803922f, 0.4627451f);
	const maths::vec3 Color::GreenYellow = maths::vec3(0.5764706f, 0.85882354f, 0.4392157f);
	const maths::vec3 Color::HunterGreen = maths::vec3(0.12941177f, 0.36862746f, 0.12941177f);
	const maths::vec3 Color::IndianRed = maths::vec3(0.30588236f, 0.18431373f, 0.18431373f);
	const maths::vec3 Color::Khaki = maths::vec3(0.62352943f, 0.62352943f, 0.37254903f);
	const maths::vec3 Color::LightBlue = maths::vec3(0.7529412f, 0.8509804f, 0.8509804f);
	const maths::vec3 Color::LightGray = maths::vec3(0.65882355f, 0.65882355f, 0.65882355f);
	const maths::vec3 Color::LightSteelBlue = maths::vec3(0.56078434f, 0.56078434f, 0.7411765f);
	const maths::vec3 Color::LightWood = maths::vec3(0.9137255f, 0.7607843f, 0.6509804f);
	const maths::vec3 Color::LimeGreen = maths::vec3(0.19607843f, 0.8039216f, 0.19607843f);
	const maths::vec3 Color::Magenta = maths::vec3(1.0f, 0.0f, 1.0f);
	const maths::vec3 Color::MandarinOrange = maths::vec3(0.89411765f, 0.47058824f, 0.2f);
	const maths::vec3 Color::Maroon = maths::vec3(0.5568628f, 0.13725491f, 0.41960785f);
	const maths::vec3 Color::MidAquaMarine = maths::vec3(0.19607843f, 0.8039216f, 0.6f);
	const maths::vec3 Color::MidBlue = maths::vec3(0.19607843f, 0.19607843f, 0.8039216f);
	const maths::vec3 Color::MidForestGreen = maths::vec3(0.41960785f, 0.5568628f, 0.13725491f);
	const maths::vec3 Color::MidGoldenRod = maths::vec3(0.91764706f, 0.91764706f, 0.68235296f);
	const maths::vec3 Color::MidOrchid = maths::vec3(0.5764706f, 0.4392157f, 0.85882354f);
	const maths::vec3 Color::MidSeaGreen = maths::vec3(0.25882354f, 0.43529412f, 0.25882354f);
	const maths::vec3 Color::MidSlateBlue = maths::vec3(0.49803922f, 0.0f, 1.0f);
	const maths::vec3 Color::MidSpringGreen = maths::vec3(0.49803922f, 1.0f, 0.0f);
	const maths::vec3 Color::MidTurquoise = maths::vec3(0.4392157f, 0.85882354f, 0.85882354f);
	const maths::vec3 Color::MidVioletRed = maths::vec3(0.85882354f, 0.4392157f, 0.5764706f);
	const maths::vec3 Color::MidWood = maths::vec3(0.6509804f, 0.5019608f, 0.39215687f);
	const maths::vec3 Color::MidnightBlue = maths::vec3(0.18431373f, 0.18431373f, 0.30980393f);
	const maths::vec3 Color::NavyBlue = maths::vec3(0.13725491f, 0.13725491f, 0.5568628f);
	const maths::vec3 Color::NeonBlue = maths::vec3(0.3019608f, 0.3019608f, 1.0f);
	const maths::vec3 Color::NeonPink = maths::vec3(1.0f, 0.43137255f, 0.78039217f);
	const maths::vec3 Color::NewMidnightBlue = maths::vec3(0.0f, 0.0f, 0.6117647f);
	const maths::vec3 Color::NewTan = maths::vec3(0.92156863f, 0.78039217f, 0.61960787f);
	const maths::vec3 Color::OldGold = maths::vec3(0.8117647f, 0.70980394f, 0.23137255f);
	const maths::vec3 Color::Orange = maths::vec3(1.0f, 0.49803922f, 0.0f);
	const maths::vec3 Color::OrangeRed = maths::vec3(1.0f, 0.14117648f, 0.0f);
	const maths::vec3 Color::Orchid = maths::vec3(0.85882354f, 0.4392157f, 0.85882354f);
	const maths::vec3 Color::PaleGreen = maths::vec3(0.56078434f, 0.7372549f, 0.56078434f);
	const maths::vec3 Color::Pink = maths::vec3(0.7372549f, 0.56078434f, 0.56078434f);
	const maths::vec3 Color::Plum = maths::vec3(0.91764706f, 0.6784314f, 0.91764706f);
	const maths::vec3 Color::Quartz = maths::vec3(0.8509804f, 0.8509804f, 0.9529412f);
	const maths::vec3 Color::Red = maths::vec3(1.0f, 0.0f, 0.0f);
	const maths::vec3 Color::RichBlue = maths::vec3(0.34901962f, 0.34901962f, 0.67058825f);
	const maths::vec3 Color::Salmon = maths::vec3(0.43529412f, 0.25882354f, 0.25882354f);
	const maths::vec3 Color::Scarlet = maths::vec3(0.54901963f, 0.09019608f, 0.09019608f);
	const maths::vec3 Color::SeaGreen = maths::vec3(0.13725491f, 0.5568628f, 0.40784314f);
	const maths::vec3 Color::Sepia = maths::vec3(0.369f, 0.149f, 0.071f);
	const maths::vec3 Color::SemiSweetChocolate = maths::vec3(0.41960785f, 0.25882354f, 0.14901961f);
	const maths::vec3 Color::Sienna = maths::vec3(0.5568628f, 0.41960785f, 0.13725491f);
	const maths::vec3 Color::Silver = maths::vec3(0.9019608f, 0.9098039f, 0.98039216f);
	const maths::vec3 Color::SkyBlue = maths::vec3(0.19607843f, 0.6f, 0.8f);
	const maths::vec3 Color::SlateBlue = maths::vec3(0.0f, 0.49803922f, 1.0f);
	const maths::vec3 Color::SpicyPink = maths::vec3(1.0f, 0.10980392f, 0.68235296f);
	const maths::vec3 Color::SpringGreen = maths::vec3(0.0f, 1.0f, 0.49803922f);
	const maths::vec3 Color::SteelBlue = maths::vec3(0.13725491f, 0.41960785f, 0.5568628f);
	const maths::vec3 Color::SummerSky = maths::vec3(0.21960784f, 0.6901961f, 0.87058824f);
	const maths::vec3 Color::Tan = maths::vec3(0.85882354f, 0.5764706f, 0.4392157f);
	const maths::vec3 Color::Teal = maths::vec3(0.0f, 0.502f, 0.502f);
	const maths::vec3 Color::Thistle = maths::vec3(0.84705883f, 0.7490196f, 0.84705883f);
	const maths::vec3 Color::Tomato = maths::vec3(1.0f, 0.388f, 0.278f);
	const maths::vec3 Color::Turquoise = maths::vec3(0.6784314f, 0.91764706f, 0.91764706f);
	const maths::vec3 Color::VeryDarkBrown = maths::vec3(0.36078432f, 0.2509804f, 0.2f);
	const maths::vec3 Color::VeryLightGrey = maths::vec3(0.8039216f, 0.8039216f, 0.8039216f);
	const maths::vec3 Color::Violet = maths::vec3(0.30980393f, 0.18431373f, 0.30980393f);
	const maths::vec3 Color::VioletRed = maths::vec3(0.8f, 0.19607843f, 0.6f);
	const maths::vec3 Color::Wheat = maths::vec3(0.84705883f, 0.84705883f, 0.7490196f);
	const maths::vec3 Color::White = maths::vec3(1.0f, 1.0f, 1.0f);
	const maths::vec3 Color::Yellow = maths::vec3(1.0f, 1.0f, 0.0f);
	const maths::vec3 Color::YellowGreen = maths::vec3(0.6f, 0.8f, 0.19607843f);

	const maths::vec3 Color::Illum_Candle = maths::vec3(0.831f, 0.921f, 1.0f);
	const maths::vec3 Color::Illum_40WTungsten = maths::vec3(0.956f, 1.0f, 0.980f);
	const maths::vec3 Color::Illum_100WTungsten = maths::vec3(1.0f, 0.956f, 0.898f);
	const maths::vec3 Color::Illum_Halogen = maths::vec3(0.831f, 0.921f, 1.0f);
	const maths::vec3 Color::Illum_CarbonArc = maths::vec3(0.956f, 1.0f, 0.980f);
	const maths::vec3 Color::Day_Noon = maths::vec3(1.0f, 0.956f, 0.898f);
	const maths::vec3 Color::Day_Sunlight = maths::vec3(0.831f, 0.921f, 1.0f);
	const maths::vec3 Color::Day_Overcast = maths::vec3(0.956f, 1.0f, 0.980f);
	const maths::vec3 Color::Day_ClearBlue = maths::vec3(1.0f, 0.956f, 0.898f);

	const maths::vec3 Color::Flourescent = maths::vec3(0.956f, 1.0f, 0.980f);
	const maths::vec3 Color::Flourescent_BlackLight = maths::vec3(0.655f, 0.0f, 1.0f);
	const maths::vec3 Color::Flourescent_Blue = maths::vec3(0.831f, 0.921f, 1.0f);
	const maths::vec3 Color::Flourescent_CoolWhite = maths::vec3(0.831f, 0.922f, 1.0f);
	const maths::vec3 Color::Flourescent_FullSpectrum = maths::vec3(1.0f, 0.957f, 0.949f);
	const maths::vec3 Color::Flourescent_GlowLight = maths::vec3(1.0f, 0.937f, 0.969f);
	const maths::vec3 Color::Flouresent_Standard = maths::vec3(0.957f, 1.0f, 0.98f);
	const maths::vec3 Color::Flourescent_Warm = maths::vec3(1.0f, 0.956f, 0.898f);

	const maths::vec3 Color::Gas_Mercury = maths::vec3(1.0f, 0.956f, 0.898f);
	const maths::vec3 Color::Gas_Sodium = maths::vec3(1.0f, 0.956f, 0.898f);
	const maths::vec3 Color::Gas_Halide = maths::vec3(1.0f, 0.956f, 0.898f);
	const maths::vec3 Color::Gas_Sodium_HighPressure = maths::vec3(1.0f, 0.956f, 0.898f);
	
	Color::Color()
		: m_Color(1.0f, 1.0f, 1.0f)
	{

	}

	Color::Color(const maths::vec3& other)
		: m_Color(other)
	{

	}

	Color::Color(float grayScale)
		: m_Color(grayScale, grayScale, grayScale)
	{

	}

	Color::Color(float red, float green, float blue)
		: m_Color(red, green, blue)
	{

	}

	Color::~Color()
	{

	}
	
	maths::vec3 Color::Concentration(const maths::vec3& color, float temperature, float scaleFactor)
	{
		maths::vec3 result;
		result.x = (temperature + color.x) / scaleFactor;
		result.y = (temperature + color.y) / scaleFactor;
		result.z = (temperature + color.z) / scaleFactor;

		return result;
	}

	maths::vec3 Color::Concentration(const maths::vec3& color, float temperature, float factorRed, float factorGreen, float factorBlue)
	{
		maths::vec3 result;
		result.x = (temperature + color.x) / factorRed;
		result.y = (temperature + color.y) / factorGreen;
		result.z = (temperature + color.z) / factorBlue;

		return result;
	}

	maths::vec3 Color::Stylize(const maths::vec3& color, float temperature, float scaleFactor, float weight)
	{
		maths::vec3 result;
		result.x = (temperature + color.x) / (scaleFactor + weight);
		result.y = (temperature + color.y) / (scaleFactor + weight);
		result.z = (temperature + color.z) / (scaleFactor + weight);

		return result;
	}

	maths::vec3 Color::Stylize(const maths::vec3& color, float temperature, float factorRed, float factorGreen, float factorBlue, float weight)
	{
		maths::vec3 result;
		result.x = (temperature + color.x) / (factorRed + weight);
		result.y = (temperature + color.y) / (factorGreen + weight);
		result.z = (temperature + color.z) / (factorBlue + weight);

		return result;
	}
} }