#pragma once

#include <unordered_map>
#include <boost/filesystem.hpp>


namespace Game
{
	namespace Io
	{
		class Ini
		{
		public:
			explicit Ini(const std::string& document);
			const std::string& value(const std::string& section, const std::string& key) const;
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


		template<>
		inline std::string Ini::value<std::string>(const std::string& section, const std::string& key) const
		{
			return value(section, key);
		}


		template<>
		inline bool Ini::value<bool>(const std::string& section, const std::string& key) const
		{
			auto stream = std::istringstream{std::string{value(section, key)}};
			auto parsed_value = bool{};
			stream >> std::boolalpha >> parsed_value;
			return parsed_value;
		}


		Ini load_ini_from_file(const boost::filesystem::path& file);
	}
}
