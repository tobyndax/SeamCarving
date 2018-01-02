#pragma once
#include <ImageContainer.h>
#include <SobelMagnitude.h>
#include <set>

class SeamCostCalculator {
public:
	SeamCostCalculator() = default; // constructor
	SeamCostCalculator(std::shared_ptr<Image<unsigned char>> im); // constructor
	
	//Perform the Seam Costcalculation, this includes the sobel magnitude filter.
	void execute();
	//Set Input, provide an image
	void setInput(std::shared_ptr<Image<unsigned char>> im);

	//Takes a list of pixels and removes them from the filtered image and recalculates the result.
	void removePixelsAndUpdate(std::vector<int> indices, std::vector<Pixel> pixels);
	
	void showCost() {
		cost->show(true);
	}

	std::unique_ptr<Image<int>> getCost() { return std::move(cost); }
	void setCost(std::unique_ptr<Image<int>> aCost) { cost = std::move(aCost); }
	std::unique_ptr<Image<int>> getPath() { return std::move(path); }
	void setPath(std::unique_ptr<Image<int>> aPath) { path = std::move(aPath); }

private:
	void calculateCost();
	std::set<Pixel> calculateUpdateNeed(std::vector<Pixel> pixels,const unsigned int& stride);
	void update(std::set<Pixel>);

	std::shared_ptr<Image<unsigned char>> image;
	std::shared_ptr<Image<unsigned char>> filterImage;
	std::unique_ptr<Image<int>> cost = std::make_unique<Image<int>>();
	std::unique_ptr<Image<int>> path = std::make_unique<Image<int>>();
	std::unique_ptr<SobelMagnitudeFilter> filter = nullptr;
};