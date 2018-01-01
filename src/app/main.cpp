#include <string>
#include <ImageContainer.h>
#include <SeamCarver.h>

#ifdef __APPLE__
std::string pngPath = "../testdata/lenaGrayLarge.png";
#else
std::string pngPath = "..\\..\\testdata\\Broadway_tower_grayscale.png";
#endif


int main(int argc, char * argv[])
{

	{
		std::shared_ptr<Image<unsigned char>> img = std::make_shared<Image<unsigned char>>(pngPath);
		img->setName("img");
		{
			std::shared_ptr<Image<unsigned char>> img2 = std::make_shared<Image<unsigned char>>(pngPath);
			img2->show();
		}
		SeamCarver seamCarver(img);
		seamCarver.removeColumns(img->getWidth() / 2, true);

		seamCarver.getResult()->showNow();

		//Sanity check that we haven't touched the original data
		img->show();

	}
	return 0;
}
