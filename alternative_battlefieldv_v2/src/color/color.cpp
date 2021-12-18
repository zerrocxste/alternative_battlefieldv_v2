#include "../alternative.h"

Color::Color() : r(0), g(0), b(0), a(0)
{

}

Color::Color(std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A) : r(R), g(G), b(B), a(A)
{

}

Color::Color(std::uint8_t R, std::uint8_t G, std::uint8_t B) : r(R), g(G), b(B), a(255)
{

}

Color::Color(std::uint32_t hex)
{
	r = g = b = a = 0;
	memcpy(this, &hex, 4);
}

Color::~Color()
{

}

std::uint8_t& Color::operator[](std::uint32_t idx)
{
	return ((std::uint8_t*)(this))[idx];
}

int Color::AtByteArr()
{
	int Color = 0;
	std::uint8_t* ByteArrColor = (std::uint8_t*)&Color;
	memcpy(&Color, this, 4);
	return Color;
}

Color& Color::MakeRand()
{
	this->r = rand() % 255;
	this->g = rand() % 255;
	this->b = rand() % 255;
	return *this;
}

Color Color::Red()
{
	return Color(255, 0, 0);
}

Color Color::Green()
{
	return Color(0, 255, 0);
}

Color Color::Blue()
{
	return Color(0, 0, 255);
}

Color Color::Yellow()
{
	return Color(255, 255, 0);
}

Color Color::Magenta()
{
	return Color(255, 0, 255);
}

Color Color::Aqua()
{
	return Color(0, 255, 255);
}