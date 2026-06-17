#include <cat/gfx/texture_loader.hpp>

#include <cat/gfx/gfx_util.hpp>
#include <cat/error.hpp>

#define STBI_ASSERT(x) CAT_ASSERT(x)
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


using namespace cat;

TextureLoader::TextureLoader()
{
    stbi_set_flip_vertically_on_load(false);
}

Texture TextureLoader::load(const std::string &path) const THROWS
{
    glm::ivec2 texture_size;
    u32 handle;

    byte_t* raw_bytes = stbi_load(
        path.c_str(),
        &texture_size.x,
        &texture_size.y,
        NULL,
        STBI_rgb_alpha
    );

    if(!raw_bytes)
    {
        throw Exception{eErrorCode::FILE_NOT_FOUND, "failed to load texture"};
    }

    glGenTextures(1, &handle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		texture_size.x,
		texture_size.y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		raw_bytes
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(raw_bytes);

    return Texture{texture_size, handle, GL_TEXTURE_2D};
}

hash_t TextureLoader::hash(const std::string &path) const noexcept
{
    return m_hasher(path);
}

void TextureLoader::unload(Texture *texture) const
{
    u32 temp_handle = texture->get_handle();
    glDeleteTextures(1, &temp_handle);
    texture->get_handle_ref() = 0;
}
