#include <SeamCarver.h>
#include <SobelMagnitude.h>
#include <SeamCostCalculator.h>
#include <algorithm>
SeamCarver::SeamCarver(const std::shared_ptr<Image<unsigned char>> im)
{
	//Copy the image
	image = std::make_shared<Image<unsigned char>>(*im);
}

void SeamCarver::removeColumns(unsigned int its)
{
	//Sobel filter doesn't handle non-multiples of two images right now.
	for (int i = 0; i < its; i++) {
		performSeamRemoval();
	}
}

void SeamCarver::removeColumnsDebug(unsigned int its)
{
	for (int i = 0; i < its; i++) {
		performSeamRemovalDebug();
	}
}

std::shared_ptr<Image<unsigned char>> SeamCarver::getResult()
{
	return image;
}

void SeamCarver::ShowResult() {
	image->show();
}

void SeamCarver::performSeamRemoval()
{
	std::shared_ptr<Image<unsigned char>> output;

	{	// Scope to trigger destructor.
		SobelMagnitudeFilter filter(image);
		filter.execute();
		output = filter.getResult();
	}

	SeamCostCalculator costCalculator(output);
	costCalculator.execute();

	std::unique_ptr<Image<int>> cost = costCalculator.getCost();
	std::unique_ptr<Image<int>> path = costCalculator.getPath();

	//find the max pixel from the final row
	unsigned int index = (cost->getHeight() - 2)*(cost->getWidth());
	auto it = cost->begin() + index; // get iterator to first pixel of last row.
	std::vector<int> test = std::vector<int>(it, it + cost->getWidth());
	auto maxEl = std::min_element(it + 1, it + cost->getWidth() - 1);
	unsigned int offset = maxEl - it; //offset to pixel with maximum element.

	//cost->show(true);
	
	//For now, just color the images for this row black.
	for (int j = cost->getHeight() - 1; j >= 0; j--) {
		image->removePixel(j*image->getWidth() + offset); // lets change this to a std::remove_if
		offset += path->at(j*image->getWidth() + offset);
	}
	image->setSize(image->getWidth() - 1, image->getHeight());

	image->showNow();

}

void SeamCarver::performSeamRemovalDebug()
{
	std::shared_ptr<Image<unsigned char>> output;

	{	// Scope to trigger destructor.
		SobelMagnitudeFilter filter(image);
		filter.execute();
		output = filter.getResult();
	}

	SeamCostCalculator costCalculator(output);
	costCalculator.execute();

	std::unique_ptr<Image<int>> cost = costCalculator.getCost();
	std::unique_ptr<Image<int>> path = costCalculator.getPath();
	
	cost->show(true);

	//find the max pixel from the final row
	unsigned int index = (cost->getHeight() - 2)*(cost->getWidth());
	auto it = cost->begin() + index; // get iterator to first pixel of last row.
	std::vector<int> test = std::vector<int>(it, it + cost->getWidth());
	auto maxEl = std::min_element(it + 1, it + cost->getWidth() - 1);
	unsigned int offset = maxEl - it; //offset to pixel with maximum element.

									  //cost->show(true);

									  //For now, just color the images for this row black.
	for (int j = cost->getHeight() - 1; j >= 0; j--) {
		image->removePixel(j*image->getWidth() + offset); // lets change this to a std::remove_if
		offset += path->at(j*image->getWidth() + offset);
	}
	image->setSize(image->getWidth() - 1, image->getHeight());
	
	image->show();

}

