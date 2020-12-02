#include <json.hpp>

using Json = nlohmann::json;

namespace JsonUtils
{
	bool HasKey(const Json& json, const std::string& key);

	template <typename T>
	static T GetValueUnsafe(const Json& jsonObj, const T& def)
	{
		static_assert(std::is_integral<T>::value || std::is_enum<T>::value || std::is_floating_point<T>::value || 
					std::is_base_of<bool, T>::value || std::is_base_of<std::string, T>::value || 
					std::is_same<Json::array_t, T>::value || std::is_same<Json, T>::value, "Template type was not valid");

		if (jsonObj.is_number_integer())
		{
			return jsonObj.get<T>();
		}

		return def;
	}

	template<>
	uint32_t GetValueUnsafe<uint32_t>(const Json& jsonObj, const uint32_t& def)
	{
		if (jsonObj.is_number())
		{
			return jsonObj.get<uint32_t>();
		}

		return def;
	}

	template<>
	float GetValueUnsafe<float>(const Json& jsonObj, const float& def)
	{
		if (jsonObj.is_number())
		{
			return jsonObj.get<float>();
		}

		return def;
	}

	template<> 
	std::string GetValueUnsafe<std::string>(const Json& jsonObj, const std::string& def)
	{
		if (jsonObj.is_string())
		{
			return jsonObj.get<std::string>();
		}

		return def;
	}

	template<>
	bool GetValueUnsafe<bool>(const Json& jsonObj, const bool& def)
	{
		if (jsonObj.is_boolean())
		{
			return jsonObj.get<bool>();
		}

		return def;
	}

	template<>
	Json::array_t GetValueUnsafe<Json::array_t>(const Json& jsonObj, const Json::array_t& def)
	{
		if (jsonObj.is_array())
		{
			return jsonObj.get<Json::array_t>();
		}

		return def;
	}

	template<>
	Json GetValueUnsafe<Json>(const Json& jsonObj, const Json& def)
	{
		if (jsonObj.is_object())
		{
			return jsonObj;
		}

		return def;
	}

	template <typename T> 
	static T GetValue(const Json& json, const std::string& key, const T& def)
	{
		T val = def;
		if (JsonUtils::HasKey(json, key))
		{
			Json obj = json.at(key);
			val = GetValueUnsafe<T>(obj, def);
		}

		return val;
	}

	template <typename T>
	static T GetValue(const Json& json, uint32_t index, const T& def)
	{
		T val = def;
		if (json.is_array() && json.size() > index)
		{
			Json obj = json.at(index);
			val = GetValueUnsafe<T>(obj, def);
		}

		return val;
	}
}