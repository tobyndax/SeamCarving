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

	std::unique_ptr<Image<int>> gx = std::make_unique<Image<int>>(); // 
	std::unique_ptr<Image<int>> gy = std::make_unique<Image<int>>(); //
	std::unique_ptr<Image<int>> ping = std::make_unique<Image<int>>(); //Ping buffer 


	std::shared_ptr<const Image<unsigned char>> image = nullptr; //The filter shouldn't hold memory access of the input image,
	std::shared_ptr<Image<unsigned char>> output = std::make_shared<Image<unsigned char>>(); //Shared_ptr output, 
};