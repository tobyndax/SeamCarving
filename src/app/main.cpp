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
	{
		std::shared_ptr<Image<unsigned char>> img = std::make_shared<Image<unsigned char>>(pngPath);
		img->setName("img");
		std::shared_ptr<Image<unsigned char>> output;

		{	// Scope to trigger destructor.
			SobelMagnitudeFilter filter(img);
			filter.execute();
			output = filter.getResult();
			output->show();
		}

		{
			SeamCostCalculator costCalculator(output);
			costCalculator.execute();
			costCalculator.showCost();
		}

		{
			//Sanity check that img is untouched 
			img->show();
		}
	}
	return 0;
}
