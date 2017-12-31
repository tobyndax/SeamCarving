#include <string>
#include <ImageContainer.h>
#include <SeamCarver.h>

#ifdef __APPLE__
std::string pngPath = "../testdata/lenaGrayLarge.png";
#else
std::string pngPath = "..\\..\\testdata\\lenaGray.png";
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
		//seamCarver.removeColumnsDebug(1);
		seamCarver.removeColumns(img->getWidth()/4);
		//seamCarver.removeColumnsDebug(1);
		seamCarver.ShowResult();


		{
			//Sanity check that img is untouched 
			img->show();
		}
	}
	return 0;
}
