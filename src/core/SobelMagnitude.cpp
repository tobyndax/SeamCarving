#include "SobelMagnitude.h"
#include <algorithm>
#include <iostream>

SobelMagnitudeFilter::SobelMagnitudeFilter(std::shared_ptr<Image<unsigned char>> im)
{
	image = im;

	gx->resize(image->getWidth(), image->getHeight());
	gy->resize(image->getWidth(), image->getHeight());
	
	gx->setName("gx");
	gy->setName("gy");
	
	ping->resize(image->getWidth(), image->getHeight());
	ping->setName("ping");

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
	performGx();
	performGy();
	combineGxGy();
}

std::shared_ptr<Image<unsigned char>> SobelMagnitudeFilter::getResult() const
{
	return output;
}

void SobelMagnitudeFilter::performGx()
{
	//No handling of edges yet.
	for (unsigned int j = 1; j < gx->getHeight() - 1; j++)
	{
		for (unsigned int i = 1; i < gx->getWidth() - 1; i++)
		{
			//first row of filter
			unsigned int index = (j - 1) * gx->getWidth() + i;
			int sum = (int)image->at(index - 1) - (int)image->at(index + 1);

			index = j * gx->getWidth() + i;
			sum += 2 * (int)image->at(index - 1) - (int)2 * image->at(index + 1);

			index = (j + 1) * gx->getWidth() + i;
			sum += (int)image->at(index - 1) - (int)image->at(index + 1);

			gx->at(j*gx->getWidth() + i) = sum;
		}
	}

	//No handling of edges yet.
	/*for (unsigned int j = 0; j < gx->getHeight() - 0; j++)
	{
		for (unsigned int i = 1; i < gx->getWidth() - 1; i++)
		{
			unsigned int index = i + j * image->getWidth();
			ping->at(index) = (int)image->at(index - 1) - (int)image->at(index + 1);
		}
	}
	
	ping->transpose();

	for (unsigned int j = 0; j < gx->getHeight() - 0; j++)
	{
		for (unsigned int i = 1; i < gx->getWidth() - 1; i++)
		{
			unsigned int index = i + j * gx->getWidth();
			gx->at(index) = ping->at(index - 1) + 2 * ping->at(index) + ping->at(index + 1);

		}
	}*/

}

void SobelMagnitudeFilter::performGy()
{
	//No handling of edges yet.
	for (unsigned int j = 1; j < gy->getHeight() - 1; j++)
	{
		for (unsigned int i = 1; i < gy->getWidth() - 1; i++)
		{
			//first row of filter
			unsigned int index = (j - 1) * gy->getWidth() + i;
			int sum = (int)image->at(index - 1) + 2 * (int)image->at(index) + (int)image->at(index + 1);

			index = (j + 1) * gy->getWidth() + i;
			sum -= (int)image->at(index - 1) + 2 * (int)image->at(index) + (int)image->at(index + 1);

			gy->at(j*gy->getWidth() + i) = sum;
		}
	}

	

	//for (unsigned int j = 1; j < gy->getHeight() - 1; j++)
	//{
	//	for (unsigned int i = 1; i < gy->getWidth() - 1; i++)
	//	{
	//		//first row of filter
	//		unsigned int index = j * gy->getWidth() + i;
	//		ping->at(index) = (int)image->at(index - 1) + 2 * (int)image->at(index) + (int)image->at(index + 1);
	//	}
	//}

	//ping->transpose();

	//for (unsigned int j = 1; j < gy->getHeight() - 1; j++)
	//{
	//	for (unsigned int i = 1; i < gy->getWidth() - 1; i++)
	//	{
	//		//first row of filter
	//		unsigned int index = i + j * gy->getWidth();
	//		gy->at(index) = (int)ping->at(index - 1) - (int)ping->at(index + 1);

	//	}
	//}
}

void SobelMagnitudeFilter::combineGxGy()
{
	//std::transform(gy->begin(), gy->end(), gx->begin(), output->begin(), [](int a, int b) {int res = sqrt(a*a + b * b); return res < 0 ? 0 : (unsigned char)res; });
	std::transform(gy->begin(), gy->end(), gx->begin(), output->begin(), [](int a, int b) {int res = std::sqrt(a*a + b*b); return res < 0 ? 0 : (unsigned char)res; });
	//output->transpose();
}


