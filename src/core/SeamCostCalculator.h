#pragma once
#include <ImageContainer.h>

class SeamCostCalculator {
public:
	SeamCostCalculator(std::shared_ptr<Image<unsigned char>> im); // constructor
	//Use default destructor

	
	void execute();
	
	void showCost() {
		cost->show(true);
	}

	std::unique_ptr<Image<int>> getCost() { return std::move(cost); }
	std::unique_ptr<Image<int>> getPath() { return std::move(path); }

private:
	void calculateCost();

	std::shared_ptr<Image<unsigned char>> image;
	std::unique_ptr<Image<int>> cost = std::make_unique<Image<int>>();
	std::unique_ptr<Image<int>> path = std::make_unique<Image<int>>();
};