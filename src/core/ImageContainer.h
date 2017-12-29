#pragma once
#include <memory>
#include <string>
#include <libdef.h>
#include <vector>
#include <PNGViewer.h>
#include <lodepng.h>
#include <iostream>


template<typename T>
class EXPORT_SYMBOL Image {
public:
	////Constructor from dataptr
	//Image(unsigned char * aData, unsigned int aWidth, unsigned int aHeight) : width(aWidth), height(aHeight), data{std::make_shared<std::vector<unsigned char>>(aData,aWidth*aHeight) } {
	//	
	//}

	//Empty Image Constructor
	Image<T>(unsigned int aWidth, unsigned aHeight) : viewer(std::unique_ptr<PNGViewer>(new PNGViewer())) {
		width = aWidth;
		height = aHeight;
		data = std::vector<T>(width*height, 0);
	}

	//Constructor from file
	Image<T>(std::string filePath) : viewer(std::unique_ptr<PNGViewer>(new PNGViewer())) {
		
		unsigned char * dataPtr;

		/*load the PNG in one function call*/
		unsigned int error = lodepng_decode32_file(&dataPtr, &width, &height, filePath.c_str());
		/*stop if there is an error*/
		if (error)
		{
			printf("decoder error %u: %s\n", error, lodepng_error_text(error));
		}
		unsigned char * newData = new unsigned char[width*height];
		for (int i = 0; i < width*height; ++i) {
			newData[i] = dataPtr[i * 4 + 0];
		}

		data = std::vector<T>(newData, newData + width * height);
	}

	//Copy constructor
	Image<T>(const Image & other) : viewer(std::unique_ptr<PNGViewer>(new PNGViewer())) {
		this->width = other.width;
		this->height = other.height;
		data = other.data;
	}

	//Move constructor
	Image<T>(Image && other) : viewer(std::unique_ptr<PNGViewer>(new PNGViewer())) {
		this->width = other.width;
		this->height = other.height;
		data = other.data;
	}
	//Copy assignment operator
	Image& operator= (const Image& other) {
		Image tmp(other);
		*this = std::move(tmp);
		return *this; 
	}

	//Destructor
	~Image() {
		std::cout << "Image destructor running" << std::endl;

	}

	//Dimensions accessor
	unsigned int getWidth() const {
		return width;
	}
	//Dimensions accessor
	unsigned int getHeight() const {
		return height;
	}
	//Iterator accessor
	auto begin() {
		return data.begin();
	}
	//Iterator accessor
	auto end() {
		return data.end();
	}

	//Open a window and show the result. Wait for keypress until returning.
	void show() {
		std::vector<unsigned char> d = getCharData();
		viewer->setData(d.data(), width, height);
		viewer->showWaitForEsc();
		
	}

	T & at(unsigned int i) {
		return data.at(i);
	}

	const T & at(unsigned int i) const {
		return data.at(i);
	}

	std::vector<T> & getData() {
		return data;
	}

	void transpose() {
		std::vector<T> ping = std::vector<T>(data.size(),0);
		recursiveTranspose(data.data(), ping.data(), width, height, width, height);
		std::swap(data, ping);
	}

private:
	std::vector<unsigned char> getCharData() {
		return std::vector<unsigned char>(data.begin(), data.end());
	}

	std::vector<T> data = std::vector<T>(); //The image data itself.
	unsigned int width = 0;
	unsigned int height = 0;

	std::unique_ptr<PNGViewer> viewer;

	void baseTranspose(T* __restrict a, T * __restrict b, const int width, const int height, const int strideA, const int strideB)
	{
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				int index = i + j * strideA;
				int indexT = j + i * strideB;
				b[indexT] = a[index];
			}
		}
	}
	const static int TRANSPOSE_BLOCK_SIZE = 8;
	void recursiveTranspose(T * __restrict a, T * __restrict b, int widthA, int heightA, int strideA, int strideB)
	{
		// Base case 
		if (heightA <= TRANSPOSE_BLOCK_SIZE && widthA <= TRANSPOSE_BLOCK_SIZE) {
			baseTranspose(a, b, widthA, heightA, strideA, strideB);
			return;
		}

		// Recursive case
		{
			if (heightA > widthA) { //Split along height in A
				T* aTop = a;
				T* aBot = a + heightA * strideA / 2;

				//splitting along height in A means splitting along width in B
				T* bLeft = b;
				T* bRight = b + heightA / 2;

				recursiveTranspose(aTop, bLeft, widthA, heightA / 2, strideA, strideB);
				recursiveTranspose(aBot, bRight, widthA, heightA / 2, strideA, strideB);
			}
			else { //Split along width in A
				T* aLeft = a;
				T* aRight = a + widthA / 2;

				//splitting along width in A means splitting along height in B
				T* bTop = b;
				T* bBot = b + widthA * strideB / 2;

				recursiveTranspose(aLeft, bTop, widthA / 2, heightA, strideA, strideB);
				recursiveTranspose(aRight, bBot, widthA / 2, heightA, strideA, strideB);
			}
		}
	}
};

template class Image<int>;
template class Image<unsigned char>;