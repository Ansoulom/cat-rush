#pragma once

#include <unordered_map>


namespace Game
{
	namespace Io
	{
		class Ini
		{
		public:
			explicit Ini(const std::string& document);
			std::string_view value(const std::string& section, const std::string& key) const;
			template<typename Type>
			Type value(const std::string& section, const std::string& key) const;

		private:
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>> content_;
		};


		template<typename Type>
		Type Ini::value(const std::string& section, const std::string& key) const
		{
			auto stream = std::istringstream{std::string{value(section, key)}};
			auto parsed_value = Type{};
			stream >> parsed_value;
			return parsed_value;
		}
	}
}
