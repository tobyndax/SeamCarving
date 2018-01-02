#pragma once
#include <memory>
#include <string>
#include <libdef.h>
#include <vector>
#include <PNGViewer.h>
#include <lodepng.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <PerfUtility.h>



EXPORT_SYMBOL struct Pixel {
	//Assumption concerning maximum image width. Not sure about this one yet.
	const static unsigned int largeValue{ 1048576 };
	unsigned int x;
	unsigned int y;
	Pixel(unsigned int aX, unsigned int aY) : x(aX), y(aY) {

	}
	
	bool operator<(const Pixel& other) const {
		return((this->y * largeValue + this->x) < (other.y * largeValue + other.x));
	}
};

template<typename T>
class EXPORT_SYMBOL Image {

public:
	


	//Default Image Constructor
	Image<T>() {
		width = 0;
		height = 0;
		data = std::vector<T>();
	}

	//Empty Image Constructor
	Image<T>(unsigned int aWidth, unsigned aHeight) {
		width = aWidth;
		height = aHeight;
		data = std::vector<T>(width*height, 0);
	}

	//Constructor from file
	Image<T>(std::string filePath) {

		unsigned char * dataPtr;

		/*load the PNG in one function call*/
		unsigned int error = lodepng_decode32_file(&dataPtr, &width, &height, filePath.c_str());
		/*stop if there is an error*/
		if (error)
		{
			printf("decoder error %u: %s\n", error, lodepng_error_text(error));
		}
		data = std::vector<T>(width*height);
		for (int i = 0; i < width*height; ++i) {
			data[i] = dataPtr[i * 4 + 0];
		}
		delete dataPtr;
	}

	//Copy constructor
	Image<T>(const Image & other) {
		this->width = other.width;
		this->height = other.height;
		this->name = other.name;
		data = other.data;
	}

	//Move constructor
	Image<T>(Image && other) {
		this->width = other.width;
		this->height = other.height;
		this->name = other.name;
		data = other.data;
	}
	//Copy assignment operator
	Image& operator= (const Image& other) {
		Image tmp(other);
		*this = std::move(tmp);
		return *this;
	}

	void removePixel(unsigned int i) {
		data.erase(data.begin() + i);
	}
	void removePixels(std::vector<int> elements) {
		//Sort the elements so that the smallest index is at the back of the vector 
		std::sort(elements.begin(), elements.end(), std::greater<int>());
		//erase in the vector where remove_if indicates
		data.erase(
			std::remove_if(data.begin(), data.end(), [&](const T& d) {
			//Get the index of the current iterations in data
			int index = &d - &*data.begin();
			//Flag remove as false if no more elements to remove, else flag as true if index matches last element in elements
			bool remove = elements.empty() ? false : (index == elements.back());
			// remove the last element if we found it. 
			if (remove) {
				elements.pop_back();
			}
			return remove; })

			, data.end());
	}

	//Destructor
	~Image() {
#if _DEBUG
		std::cout << "Image destructor running: " << name << std::endl;
#endif
	}

	//Clone
	Image clone() {
		return Image(*this);
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

	void resize(unsigned int aWidth, unsigned int aHeight) {
		width = aWidth;
		height = aHeight;
		data.resize(aWidth*aHeight);
	}

	void setSize(unsigned int aWidth, unsigned int aHeight) {
		width = aWidth;
		height = aHeight;
	}

	//Open a window and show the result. Wait for keypress until returning.
	void show(bool dynamicScale = false) {
		std::vector<unsigned char> d = getCharData(dynamicScale);
		viewer.setData(d.data(), width, height);
		viewer.showWaitForEsc();
	}
	//Open a window and show the result. Wait for keypress until returning.
	void showNow(bool dynamicScale = false) {
		std::vector<unsigned char> d = getCharData(dynamicScale);
		viewer.setData(d.data(), width, height);
		viewer.show();
	}

	inline T & at(unsigned int i) {
		return data.at(i);
	}

	inline const T & at(unsigned int i) const {
		return data.at(i);
	}

	void transpose() {
		if (ping.size() != data.size())
			ping = std::vector<T>(data.size(), 0);

		PerfUtility perf{};

		perf.measureFunction([&] {
			recursiveTranspose(data.data(), ping.data(), width, height, width, height);
		}, "Transpose");
		std::swap(data, ping);
		setSize(getHeight(), getWidth());
	}

	void setName(const std::string& aName) { name = aName; };

private:
	PNGViewer viewer{};

	std::vector<unsigned char> getCharData(const bool dynamicScale) {
		if (dynamicScale) {
			auto it = std::max_element(data.begin(), data.end());
			T val = *it / 254; //When max value is 255 we get some pixels completely black, not sure why, leave for future investigation.

			std::vector<unsigned char> res;
			std::transform(data.begin(), data.end(), std::back_inserter(res), [&val](T a) {return (unsigned char)(a / val); });

			return res;
		}
		else {

			return std::vector<unsigned char>(data.begin(), data.end());
		}
	}


	const static int TRANSPOSE_BLOCK_SIZE = 8;
	void baseTranspose(T* __restrict a, T * __restrict b, const int width, const int height, const int strideA, const int strideB)
	{
		__assume(height <= TRANSPOSE_BLOCK_SIZE);
		for (int j = 0; j < height; ++j) {
			__assume(width <= TRANSPOSE_BLOCK_SIZE);
			for (int i = 0; i < width; ++i) {
				int index = i + j * strideA;
				int indexT = j + i * strideB;
				b[indexT] = a[index];
			}
		}
	}
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

				unsigned int split = (double)widthA / 2.0;

				T* aTop = a;
				T* aBot = a + split * strideA;

				//splitting along height in A means splitting along width in B
				T* bLeft = b;
				T* bRight = b + split;

				recursiveTranspose(aTop, bLeft, widthA, split, strideA, strideB);
				recursiveTranspose(aBot, bRight, widthA, heightA - split, strideA, strideB);
			}
			else { //Split along width in A

				unsigned int split = (double)widthA / 2.0;

				T* aLeft = a;
				T* aRight = a + split;

				//splitting along width in A means splitting along height in B
				T* bTop = b;
				T* bBot = b + split * strideB;

				recursiveTranspose(aLeft, bTop, split, heightA, strideA, strideB);
				recursiveTranspose(aRight, bBot, widthA - split, heightA, strideA, strideB);
			}
		}
	}

	//Data members

	std::vector<T> data = std::vector<T>(); //The image data itself.
	std::vector<T> ping;
	unsigned int width = 0;
	unsigned int height = 0;
	std::string name = "";


};

template class Image<int>;
template class Image<unsigned char>;