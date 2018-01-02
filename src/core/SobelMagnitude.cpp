#include "SobelMagnitude.h"
#include <algorithm>
#include <iostream>
#include <PerfUtility.h>

SobelMagnitudeFilter::SobelMagnitudeFilter(std::shared_ptr<Image<unsigned char>> im)
{
	image = im;

	output->resize(image->getWidth(), image->getHeight());
	output->setName("output");
}


SobelMagnitudeFilter::~SobelMagnitudeFilter()
{
#if _DEBUG
	std::cout << "Filter destructor running" << std::endl;
#endif // DEBUG

}

void SobelMagnitudeFilter::execute()
{
	calculateMagnitude();
}


std::shared_ptr<Image<unsigned char>> SobelMagnitudeFilter::getResult() const
{
	return output;
}

void SobelMagnitudeFilter::update(std::set<Pixel> pixels)
{
	for (auto pix : pixels) {
		unsigned int j = pix.y;
		unsigned int i = pix.x;
		//Skip indices who corresponds to the edges of the image.
		if (j < 2 || j > image->getHeight() - 2
			|| i < 2 || i > image->getWidth() -2) {
			continue;
		}

		//Calculate gx
		unsigned int index = (j - 1) * image->getWidth() + i;
		int sum = (int)image->at(index - 1) - (int)image->at(index + 1);

		index = j * image->getWidth() + i;
		sum += 2 * (int)image->at(index - 1) - (int)2 * image->at(index + 1);

		index = (j + 1) * image->getWidth() + i;
		sum += (int)image->at(index - 1) - (int)image->at(index + 1);

		int gxS = sum;

		//Calculate gy
		index = (j - 1) * image->getWidth() + i;
		sum = (int)image->at(index - 1) + 2 * (int)image->at(index) + (int)image->at(index + 1);

		index = (j + 1) * image->getWidth() + i;
		sum -= (int)image->at(index - 1) + 2 * (int)image->at(index) + (int)image->at(index + 1);

		int gyS = sum;
		//Combine gx and gy
		int res  = sqrt(pow(gyS, 2) + pow(gxS, 2));
		output->at(j*image->getWidth() + i) = sqrt(pow(gyS, 2) + pow(gxS, 2));
	}
	//output->showNow();
}

void SobelMagnitudeFilter::calculateMagnitude()
{
	for (unsigned int j = 1; j < image->getHeight() - 1; j++)
	{
		for (unsigned int i = 1; i < image->getWidth() - 1; i++)
		{
			//Calculate gx
			unsigned int index = (j - 1) * image->getWidth() + i;
			int sum = (int)image->at(index - 1) - (int)image->at(index + 1);

			index = j * image->getWidth() + i;
			sum += 2 * (int)image->at(index - 1) - (int)2 * image->at(index + 1);

			index = (j + 1) * image->getWidth() + i;
			sum += (int)image->at(index - 1) - (int)image->at(index + 1);

			int gxS = sum;

			//Calculate gy
			index = (j - 1) * image->getWidth() + i;
			sum = (int)image->at(index - 1) + 2 * (int)image->at(index) + (int)image->at(index + 1);

			index = (j + 1) * image->getWidth() + i;
			sum -= (int)image->at(index - 1) + 2 * (int)image->at(index) + (int)image->at(index + 1);

			int gyS = sum;
			//Combine gx and gy
			output->at(j*image->getWidth() + i) = sqrt(pow(gyS, 2) + pow(gxS, 2));

		}
	}

}


