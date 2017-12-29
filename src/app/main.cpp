#include <string>
#include <ImageContainer.h>
#include <SobelMagnitude.h>
#include <SeamCostCalculator.h>

#ifdef __APPLE__
std::string pngPath = "../testdata/lenaGrayLarge.png";
#else
std::string pngPath = "..\\..\\testdata\\lenaGrayLarge.png";
#endif

int main(int argc, char * argv[])
{
	Image<unsigned char> img(pngPath);
	std::shared_ptr<Image<unsigned char>> output;
	
	{	// Scope to trigger destructor.
		SobelMagnitudeFilter filter(img);
		filter.execute();
		output = filter.getResult();
	}
	output->show();
	{
		SeamCostCalculator costCalculator(output);
		costCalculator.execute();
		costCalculator.showCost();
	}
	
	return 0;
}
