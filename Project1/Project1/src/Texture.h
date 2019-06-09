#pragma once

#include "Renderer.h"
#include <string>

class Texture {
public:
	Texture(const std::string& filePath);
	~Texture();


	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer; // the buffer which is held by the texture
	int m_Width, m_Height, m_BitsPerPixel;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

