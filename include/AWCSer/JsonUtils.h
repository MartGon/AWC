#include <json.hpp>

using Json = nlohmann::json;

namespace JsonUtils
{
	bool HasKey(const Json& json, const std::string& key);

	template <typename T>
	static T GetValueUnsafe(const Json& jsonObj)
	{
		static_assert(std::is_integral<T>::value || std::is_enum<T>::value || std::is_floating_point<T>::value || 
					std::is_base_of<bool, T>::value || std::is_base_of<std::string, T>::value || 
					std::is_same<Json::array_t, T>::value || std::is_same<Json, T>::value, "Template type was not valid");

		if (jsonObj.is_number_integer())
		{
			return jsonObj.get<T>();
		}
		else
			throw Json::type_error::create(0, "Object was not of type integer");
	}

	template<>
	uint32_t GetValueUnsafe<uint32_t>(const Json& jsonObj)
	{
		if (jsonObj.is_number())
		{
			return jsonObj.get<uint32_t>();
		}
		else
			throw Json::type_error::create(0, "Object was not of type uint32_t");
	}

	template<>
	float GetValueUnsafe<float>(const Json& jsonObj)
	{
		if (jsonObj.is_number())
		{
			return jsonObj.get<float>();
		}
		else
			throw Json::type_error::create(0, "Object was not of type float");
	}

	template<> 
	std::string GetValueUnsafe<std::string>(const Json& jsonObj)
	{
		if (jsonObj.is_string())
		{
			return jsonObj.get<std::string>();
		}
		else
			throw Json::type_error::create(0, "Object was not of type std::string");
	}

	template<>
	bool GetValueUnsafe<bool>(const Json& jsonObj)
	{
		if (jsonObj.is_boolean())
		{
			return jsonObj.get<bool>();
		}
		else
			throw Json::type_error::create(0, "Object was not of type bool");
	}

	template<>
	Json::array_t GetValueUnsafe<Json::array_t>(const Json& jsonObj)
	{
		if (jsonObj.is_array())
		{
			return jsonObj.get<Json::array_t>();
		}
		else
			throw Json::type_error::create(0, "Object was not of type Json::array");
	}

	template<>
	Json GetValueUnsafe<Json>(const Json& jsonObj)
	{
		if (jsonObj.is_object())
		{
			return jsonObj;
		}
		else
			throw Json::type_error::create(0, "Object was not of type Json");
	}

	// Get value

	template <typename T> 
	static T GetValue(const Json& json, const std::string& key, const T& def)
	{
		T val = def;
		if (JsonUtils::HasKey(json, key))
		{
			Json obj = json.at(key);
			val = GetValueUnsafe<T>(obj);
		}

		return val;
	}

	template <typename T> 
	static T GetValue(const Json& json, const std::string& key)
	{
		if (JsonUtils::HasKey(json, key))
		{
			Json obj = json.at(key);
			return GetValueUnsafe<T>(obj);
		}
		else
			throw Json::other_error::create(0, "JsonUtils::GetValue: Key " + key + " did not exist");
	}

	template <typename T>
	static T GetValue(const Json& json, uint32_t index, const T& def)
	{
		T val = def;
		if (json.is_array() && json.size() > index)
		{
			Json obj = json.at(index);
			val = GetValueUnsafe<T>(obj);
		}
		else
			val = def;

		return val;
	}

	template <typename T>
	static T GetValue(const Json& json, uint32_t index)
	{
		if (json.is_array() && json.size() > index)
		{
			Json obj = json.at(index);
			return GetValueUnsafe<T>(obj);
		}
		else
			throw Json::out_of_range::create(0, "Index " + std::to_string(index) + " was out of range");
	}
}