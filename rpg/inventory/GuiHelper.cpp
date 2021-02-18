#include "GuiHelper.hpp"
#include "../../vendor/rapidjson/document.h"
#include "../../vendor/rapidjson/stringbuffer.h"

#include <fstream>
#include <iostream>

GuiHelper::GuiHelper(std::string xmlFile)
{
	this->m_xmlFile = xmlFile;
}

SpriteInfo GuiHelper::getSpriteInfo(std::string key)
{
	std::ifstream t(m_xmlFile);

	std::string json((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	rapidjson::Document document;
	document.Parse(json.c_str());

	if (document.HasMember("SubTexture")) {

		const rapidjson::Value &subTextures = document["SubTexture"];
		for (rapidjson::SizeType i = 0; i < subTextures.Size(); i++) { // Uses SizeType instead of size_t
			if (subTextures[i]["name"].GetString() == key) {
				return {subTextures[i]["x"].GetInt(), subTextures[i]["y"].GetInt(), subTextures[i]["width"].GetInt(),
				        subTextures[i]["height"].GetInt()};
			}
		}
	}
	return {0, 0, 0, 0};
}
