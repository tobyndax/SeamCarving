#include <SeamCostCalculator.h>
#include <algorithm>
#include <array>
#include <limits>

SeamCostCalculator::SeamCostCalculator(std::shared_ptr<Image<unsigned char>> im)
{
	//Make empty cost / path variable of appropriate size
	cost->resize(im->getWidth(), im->getHeight());
	path->resize(im->getWidth(), im->getHeight());

	//Name the images
	cost->setName("cost");
	path->setName("path");

	//Copy the image (does this copy?)
	image = std::make_shared<Image<unsigned char>>(*im);
	image->setName("image in SeamCost");
}

void SeamCostCalculator::execute()
{
	calculateCost();
}

void SeamCostCalculator::calculateCost()
{
	std::array<int, 3> candidates{ { 0,0,0 } };
	//Initialize the cost map
	for (unsigned int i = 1; i < image->getWidth() - 1; i++) { //Ignore edges since filter does not calculate values there.
		unsigned int index = image->getWidth() + i; //Start at second row. 
		cost->at(index) = image->at(index);
	}

	//Start at row 2 instead of one due to how the sobel filter is implemented around the edges.
	for (unsigned int j = 2; j < image->getHeight() - 1; j++) {
		for (unsigned int i = 1; i < image->getWidth() - 1; i++) { //Ignore edges since filter does not calculate values there.

			unsigned int srcIndex = (j-1) * image->getWidth() + i;

			//special case 1 
			if (i == 1) {
				candidates[0] = std::numeric_limits<int>::max();
				candidates[1] = cost->at(srcIndex);
				candidates[2] = cost->at(srcIndex + 1);

			}
			//special case 2 
			else if (i == image->getWidth() - 1) {
				candidates[0] = cost->at(srcIndex - 1);
				candidates[1] = cost->at(srcIndex);
				candidates[2] = std::numeric_limits<int>::max();
			}
			//general case
			else {
				candidates[0] = cost->at(srcIndex - 1);
				candidates[1] = cost->at(srcIndex);
				candidates[2] = cost->at(srcIndex + 1);
			}

			unsigned int dstIndex = j * image->getWidth() + i;

			auto it = std::min_element(candidates.begin(), candidates.end()); //Get iterator to minimum element.
			int pos = it - candidates.begin(); //Get position of iterator.

			cost->at(dstIndex) = (*it + image->at(dstIndex)); //cumulative cost 
			path->at(dstIndex) = pos - 1; //offset by one so that the value can be used directly as index offset. 
		
		}
	}
}


