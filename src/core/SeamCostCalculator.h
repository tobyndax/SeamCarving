#pragma once
#include <ImageContainer.h>

class SeamCostCalculator {
public:
	SeamCostCalculator(std::shared_ptr<Image<unsigned char>> im); // constructor
	~SeamCostCalculator() {}; // destructor
	
	void execute();
	
	void showCost() {
		cost->show(true);
	}
	std::shared_ptr<Image<unsigned char>> getResult() {
		return image;
	}
	
private:
	void calculateCost();

	std::shared_ptr<Image<unsigned char>> image;
	std::unique_ptr<Image<int>> cost = std::make_unique<Image<int>>();
	std::unique_ptr<Image<int>> path = std::make_unique<Image<int>>();
};