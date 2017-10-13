#include "File_paths.h"


namespace Game
{
	namespace Io
	{
		namespace Paths
		{
			const std::string resources{"Resources/"};
			const std::string textures{resources + "Textures/"};
			const std::string preferences{"Preferences/"};

			const std::string user_settings_file{preferences + "user_settings.ini"};
			const std::string game_constants_file{preferences + "game_constants.ini"};
		}
	}
}
