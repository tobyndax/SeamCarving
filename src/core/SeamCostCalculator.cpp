#include <SeamCostCalculator.h>
#include <algorithm>
#include <array>
#include <limits>

SeamCostCalculator::SeamCostCalculator(std::shared_ptr<Image<unsigned char>> im)
{
	setInput(im);
}

void SeamCostCalculator::execute()
{
	PerfUtility perf{};
	filter->setInput(image);
	filter->execute();
	filterImage = filter->getResult();
	calculateCost();
}

void SeamCostCalculator::setInput(std::shared_ptr<Image<unsigned char>> im)
{
	//Make empty cost / path variable of appropriate size
	cost->resize(im->getWidth(), im->getHeight());
	path->resize(im->getWidth(), im->getHeight());
	//Name the images
	cost->setName("cost");
	path->setName("path");

	//Copy the image (does this copy?)
	image = im;
	image->setName("image in SeamCost");

	filter = std::make_unique<SobelMagnitudeFilter>(image);
}
std::set<Pixel> SeamCostCalculator::calculateUpdateNeed(std::vector<Pixel> pixels, const unsigned int & stride) {
	std::set<Pixel> set{};
	for (auto pixel : pixels) {
		Pixel localPixel(pixel.x, pixel.y);
		set.insert(localPixel);
		localPixel.x--;
		set.insert(pixel);

		localPixel = pixel;
		localPixel.y--;
		set.insert(localPixel);
		localPixel.x--;
		set.insert(localPixel);

		localPixel = pixel;
		pixel.y++;
		set.insert(localPixel);
		localPixel.x--;
		set.insert(localPixel);
	}
	return set;
}

void SeamCostCalculator::update(std::set<Pixel> pixels) {
	filter->setInput(image);
	PerfUtility perf{};
	filter->update(pixels);
	filterImage = filter->getResult();
	calculateCost();
}

void SeamCostCalculator::removePixelsAndUpdate(std::vector<int> indices, std::vector<Pixel> pixels)
{
	if (indices.empty()) return;

	filterImage->removePixels(indices);
	filterImage->setSize(filterImage->getWidth() - 1, filterImage->getHeight());

	//cost->removePixels(indices);
	cost->setSize(filterImage->getWidth(), filterImage->getHeight());

	//path->removePixels(indices);
	path->setSize(filterImage->getWidth(), filterImage->getHeight());

	std::set<Pixel> pixelsFlaggedForUpdate = calculateUpdateNeed(pixels, filterImage->getWidth());
	update(pixelsFlaggedForUpdate);
}

void SeamCostCalculator::calculateCost()
{
	//Initialize the cost map, start at the second row because of how the filters are implemented
	std::copy(filterImage->begin() + filterImage->getWidth(), filterImage->begin() + filterImage->getWidth() * 2, cost->begin() + filterImage->getWidth());

	std::array<int, 3> candidates{ {0,0,0 } };
	std::vector<int> previousCost = std::vector<int>(filterImage->begin() + 2 * filterImage->getWidth(), filterImage->begin() + (2 + 1) * filterImage->getWidth());
	std::vector<int> localCost = std::vector<int>(filterImage->getWidth(), 0);
	std::vector<int> localPath = std::vector<int>(filterImage->getWidth(), 0);

	//Start at row 2 instead of one due to how the sobel filter is implemented around the edges.
	for (unsigned int j = 2; j < filterImage->getHeight() - 1; j++) {
		unsigned int dstIndex = j * filterImage->getWidth();
		for (unsigned int i = 1; i < filterImage->getWidth() - 1; i++) { //Ignore edges since filter does not calculate values there.
			unsigned int srcIndex = (j - 1) * filterImage->getWidth() + i;

			//special case 1 
			if (i == 1) {
				candidates[0] = std::numeric_limits<int>::max();
				candidates[1] = cost->at(srcIndex);
				candidates[2] = cost->at(srcIndex + 1);

			}
			//special case 2 
			else if (i == filterImage->getWidth() - 2) {
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

			auto it = std::min_element(candidates.begin(), candidates.end()); //Get iterator to minimum element.
			int pos = it - candidates.begin(); //Get position of iterator.

			localCost[i] = (*it + previousCost[i]); //cumulative cost 
			localPath[i] = pos - 1; //offset by one so that the value can be used directly as index offset. 
			previousCost[i] = filterImage->at(dstIndex + i);
		}
		std::copy(localCost.begin(), localCost.end(), cost->begin() + dstIndex);
		std::copy(localPath.begin(), localPath.end(), path->begin() + dstIndex);
	}
}


