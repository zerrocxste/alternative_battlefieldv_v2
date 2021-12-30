enum RGBA
{
	RED,
	GREEN, 
	BLUE,
	ALPHA,
	RGBA_MAX_SIZE
};

class Color
{
public:
	Color();
	Color(std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A);
	Color(std::uint8_t R, std::uint8_t G, std::uint8_t B);
	Color(std::uint32_t hex);
	~Color();

	std::uint8_t& operator[](std::uint32_t idx);

	int AtByteArr();

	Color& MakeRand();

	static const Color Red();
	static const Color Green();
	static const Color Blue();
	static const Color Yellow();
	static const Color Magenta();
	static const Color Aqua();
	static const Color Grey();
	static const Color White();
	static const Color Black();
private:
	std::uint8_t r, g, b, a;
};