#include "texture.h"

CTexture2D::CTexture2D(const char* path, GLuint sides_, GLint cadres_, GLuint params)
{
	sides = sides_;
	cadres = cadres_;
	this->SetTexture(path, params);
}

bool CTexture2D::SetTexture(const char *path, GLuint params) 
{
	texture_path = "data\\assets\\images\\" + std::string(path) + ".png";
	isLoaded = LoadTexture();
	
    if (isLoaded) 
	{
		if (params)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
		return true;
    }
	return false;
}

bool CTexture2D::LoadTexture(GLenum image_format, GLint internal_format, GLint level, GLint border) 
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(texture_path.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(texture_path.c_str());
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, texture_path.c_str());
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

	FreeImage_FlipVertical(dib);

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return true;
}

CTexture2D::~CTexture2D() 
{
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if (gl_texID)
		glDeleteTextures(1, &gl_texID);
}

CTexture3D::CTexture3D(const char* path, GLuint sides_, GLint cadres_, GLuint params)
{
	sides = sides_;
	cadres = cadres_;
	this->SetTexture(path, params);
}

bool CTexture3D::SetTexture(const char *path, GLuint params)
{
	if (std::string(path).empty())
	{
		std::cerr << "No texture name." << std::endl;
		return false;
	}

	std::string cur_path = "data\\assets\\images\\" + std::string(path);
	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_CUBE_MAP, gl_texID);
	for (int i = 0; i < 6; i++)
	{
		//texture_path = cur_path + "-" + (char)('0' + i) + ".png";
		texture_path = cur_path + ".png";
		
		LoadTexture(i);

		if (params)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, params);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, params);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, -1);
		//glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	this->isLoaded = true;

#ifdef DEBUG
	std::cout << this->texture_path.c_str() << " texture loaded successfull." << std::endl;
#endif

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return isLoaded;
}

bool CTexture3D::LoadTexture(GLint side, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(texture_path.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(texture_path.c_str());
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, texture_path.c_str());
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return true;
}

CTexture3D::~CTexture3D()
{
	if (gl_texID)
		glDeleteTextures(1, &gl_texID);
}