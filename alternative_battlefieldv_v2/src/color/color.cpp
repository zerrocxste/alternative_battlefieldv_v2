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
	memcpy(this, &hex, 4);
}

Color::~Color()
{

}

std::uint8_t& Color::operator[](std::uint32_t idx)
{
	if (idx > RGBA_MAX_SIZE)
		throw;

	return ((std::uint8_t*)(this))[idx];
}

int Color::AtByteArr()
{
	int Color = 0;
	memcpy(&Color, this, 4);
	return Color;
}

Color& Color::MakeRand()
{
	this->r = rand() % 255;
	this->g = rand() % 255;
	this->b = rand() % 255;
	this->a = 255;
	return *this;
}

const Color Color::Red()
{
	return Color(255, 0, 0);
}

const Color Color::Green()
{
	return Color(0, 255, 0);
}

const Color Color::Blue()
{
	return Color(0, 0, 255);
}

const Color Color::Yellow()
{
	return Color(255, 255, 0);
}

const Color Color::Magenta()
{
	return Color(255, 0, 255);
}

const Color Color::Aqua()
{
	return Color(0, 255, 255);
}

const Color Color::Grey()
{
	return Color(128, 128, 128);
}

const Color Color::White()
{
	return Color(255, 255, 255);
}

const Color Color::Black()
{
	return Color(0, 0, 0);
}