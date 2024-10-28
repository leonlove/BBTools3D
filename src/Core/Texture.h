#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace BBGLOBE {

/**
* @brief 自定义纹理类
*/
class Texture {
public:
	/**
	* \brief	纹理构造函数
	*/
	Texture();

	/**
	* \brief	纹理析构函数
	*/
	~Texture();

public:
	/**
	* \brief	纹理唯一标识
	*/
	std::string GetID() const;

	/**
	* \brief	设置纹理名称
	* \param[name]	纹理名称
	*/
	void SetName(const std::string& name);

	/**
	* \brief	获取纹理名称
	* \return	纹理名称
	*/
	std::string GetName() const;

	/**
	* \brief	设置纹理宽度
	* \param[width]	纹理宽度
	*/
	void SetWidth(unsigned int width);

	/**
	* \brief	获取纹理宽度
	* \return	纹理宽度
	*/
	unsigned int GetWidth() const;

	/**
	* \brief	设置纹理高度
	* \param[height]	纹理高度
	*/
	void SetHeight(unsigned int height);

	/**
	* \brief	获取纹理高度
	* \return	纹理高度
	*/
	unsigned int GetHeight() const;

	/**
	* \brief	设置纹理通道 3-RGB 4-RGBA
	* \param[format]	纹理通道
	*/
	void SetFormat(unsigned int format);

	/**
	* \brief	获取纹理通道 3-RGB 4-RGBA
	* \return	纹理通道
	*/
	unsigned int GetFormat() const;

	/**
	* \brief	设置纹理大小
	* \param[size]	纹理大小
	*/
	void SetSize(unsigned int size);

	/**
	* \brief	获取纹理大小
	* \return	纹理大小
	*/
	unsigned int GetSize() const;

	/**
	* \brief	设置纹理内存
	* \param[data]	纹理内存
	*/
	void SetData(unsigned char* data);

	/**
	* \brief	获取纹理内存
	* \return	纹理内存
	*/
	unsigned char* GetData() const;

	/**
	* \brief	设置纹理路径,并通过纹理路径，读取纹理
	* \param[filePath]	纹理路径
	*/
	void SetFilePath(const std::string& filePath);

	/**
	* \brief	获取纹理路径
	* \return	纹理路径
	*/
	std::string GetFilePath();

private:
	std::string				mID;					//纹理唯一标识
	std::string				mName;					//纹理名称
	unsigned int			mWidth;					//纹理宽度
	unsigned int			mHeight;				//纹理高度
	unsigned int			mFormat;				//纹理通道
	unsigned int			mSize;					//纹理大小
	unsigned char*			mData;					//纹理内存
	std::string				mFilePath;				//纹理路径
};

using TexturePtr = std::shared_ptr<Texture>;
using TexturePtrArray = std::vector<TexturePtr>;
using TexturePtrMapArray = std::map<unsigned int, TexturePtr>;

}// end of namespace BBGLOBE
