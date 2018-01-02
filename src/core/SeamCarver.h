#pragma once 
#include <memory>
#include <ImageContainer.h>
#include <SeamCostCalculator.h>

class SeamCarver{
public:
	//Default constructor
	SeamCarver(std::shared_ptr<Image<unsigned char>> im);
	//We should be fine with the default destructor.

	//perform seam removal its number of times
	void removeColumns(unsigned int its, unsigned int updateFrequency = 0);
	std::shared_ptr<Image<unsigned char>> getResult();

	void ShowResult();

private:
	//Perform a single pass of the 
	void performSeamRemoval(bool finalIteration);
	std::shared_ptr<Image<unsigned char>> output;
	std::unique_ptr<SeamCostCalculator> costCalculator = nullptr;
	//Image data, also this one that is being resized.
	std::shared_ptr<Image<unsigned char>> image;
	std::unique_ptr<Image<int>> cost;
	std::unique_ptr<Image<int>> path;
	PerfUtility perf{};

};