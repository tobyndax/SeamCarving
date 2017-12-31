#pragma once 
#include <memory>
#include <ImageContainer.h>

class SeamCarver{
public:
	//Default constructor
	SeamCarver(std::shared_ptr<Image<unsigned char>> im);
	//We should be fine with the default destructor.

	//perform seam removal its number of times
	void removeColumns(unsigned int its);
	void removeColumnsDebug(unsigned int its);

	std::shared_ptr<Image<unsigned char>> getResult();

	void ShowResult();

private:
	//Perform a single pass of the 
	void performSeamRemoval();
	void performSeamRemovalDebug();

	//Image data, also this one that is being resized.
	std::shared_ptr<Image<unsigned char>> image;
};