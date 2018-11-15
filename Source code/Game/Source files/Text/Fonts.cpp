#include "Fonts.h"


namespace Game
{
	namespace Text
	{
		Font::Font(const std::filesystem::path& file_path, int size)
			: sdl_font_{TTF_OpenFont(file_path.string().c_str(), size), Sdl_deleter{}} { }
	}
}
