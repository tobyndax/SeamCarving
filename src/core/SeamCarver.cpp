#include <SeamCarver.h>
#include <PerfUtility.h>
#include <algorithm>

SeamCarver::SeamCarver(const std::shared_ptr<Image<unsigned char>> im)
{
	//Copy the image
	image = std::make_shared<Image<unsigned char>>(*im);

	//Initialize the costCalculator
	costCalculator = std::make_unique<SeamCostCalculator>();

}

void SeamCarver::removeColumns(unsigned int its, unsigned int updateFrequency)
{
	//Set up initial seamCarve
	costCalculator->setInput(image);
	costCalculator->execute();


	//Sobel filter doesn't handle non-multiples of two images right now.
	for (int i = 0; i < its; i++) {
		perf.measureFunction([&] {

			performSeamRemoval(its == i - 1);
			if (updateFrequency != 0 && i % updateFrequency == 0) {
				image->showNow();
			}
		}, "Time per Seam Removal");
	}
}

std::shared_ptr<Image<unsigned char>> SeamCarver::getResult()
{
	return image;
}

void SeamCarver::ShowResult() {
	image->show();
}

void SeamCarver::performSeamRemoval(bool finalIteration)
{
	unsigned int offset;
	cost = costCalculator->getCost();
	path = costCalculator->getPath();
	//find the max pixel from the final row
	unsigned int index = (cost->getHeight() - 2)*(cost->getWidth());
	auto it = cost->begin() + index; // get iterator to first pixel of last row.
	std::vector<int> test = std::vector<int>(it, it + cost->getWidth());
	auto maxEl = std::min_element(it + 1, it + cost->getWidth() - 1);
	offset = maxEl - it; //offset to pixel with maximum element.

	std::vector<int> elementsToRemove{};
	std::vector<Pixel> pixelsToRemove{};
	elementsToRemove.reserve(cost->getHeight() - 1);
	//For now, just color the images for this row black.
	for (int j = cost->getHeight() - 1; j >= 0; j--) {
		//image->removePixel(j*image->getWidth() + offset); // lets change this to a std::remove_if
		elementsToRemove.push_back(j*image->getWidth() + offset);
		pixelsToRemove.push_back(Pixel(offset, j));
		offset += path->at(j*image->getWidth() + offset);
	}
	image->removePixels(elementsToRemove);

	image->setSize(image->getWidth() - 1, image->getHeight());


	//Return the cost and path variables, we are only borrowing them.
	costCalculator->setCost(std::move(cost));
	costCalculator->setPath(std::move(path));
	if (!finalIteration) { //Ask the filter to update.
		costCalculator->removePixelsAndUpdate(elementsToRemove, pixelsToRemove);
	}
	else {

	}
}

