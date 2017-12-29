#pragma once

#include <ImageContainer.h>
#include <memory>


class SobelMagnitudeFilter {
public:
	SobelMagnitudeFilter(const Image<unsigned char>& im);
	~SobelMagnitudeFilter(); 
	void execute();
	std::shared_ptr<Image<unsigned char>> getResult() const;


private: 
	void performGx();
	void performGy();
	void combineGxGy();

	std::unique_ptr<Image<int>> gx = nullptr; //Shared_ptr output, 
	std::unique_ptr<Image<int>> gy = nullptr; //Shared_ptr output, 
	std::unique_ptr<Image<int>> ping = nullptr; //Ping buffer 


	std::shared_ptr<const Image<unsigned char>> image = nullptr; //The filter shouldn't hold memory access of the input image,
	std::shared_ptr<Image<unsigned char>> output = nullptr; //Shared_ptr output, 
};