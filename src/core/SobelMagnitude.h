#pragma once

#include <ImageContainer.h>
#include <memory>
#include <set>

class SobelMagnitudeFilter {
public:
	SobelMagnitudeFilter(std::shared_ptr<Image<unsigned char>> im);
	~SobelMagnitudeFilter(); 
	void execute();
	std::shared_ptr<Image<unsigned char>> getResult() const;
	void setInput(std::shared_ptr<const Image<unsigned char>> aImage) { image = aImage; output->resize(image->getWidth(), image->getHeight());}

	void update(std::set<Pixel> pixels);

private: 
	
	void calculateMagnitude();

	std::shared_ptr<const Image<unsigned char>> image = nullptr; //The filter shouldn't hold memory access of the input image,
	std::shared_ptr<Image<unsigned char>> output = std::make_shared<Image<unsigned char>>(); //Shared_ptr output, 
};