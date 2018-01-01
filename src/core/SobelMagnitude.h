#pragma once

#include <ImageContainer.h>
#include <memory>


class SobelMagnitudeFilter {
public:
	SobelMagnitudeFilter(std::shared_ptr<Image<unsigned char>> im);
	~SobelMagnitudeFilter(); 
	void execute();
	std::shared_ptr<Image<unsigned char>> getResult() const;

private: 
	
	void calculateMagnitude();

	std::shared_ptr<const Image<unsigned char>> image = nullptr; //The filter shouldn't hold memory access of the input image,
	std::shared_ptr<Image<unsigned char>> output = std::make_shared<Image<unsigned char>>(); //Shared_ptr output, 
};