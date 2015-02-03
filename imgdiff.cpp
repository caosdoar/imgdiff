/*
Simple image diff tool
Oscar Sebio Cajaraville 2015
*/

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void printHelp()
{
	printf("A simple image differences tool.\n");
	printf("Usage:\n");
	printf("imgdiff file1 file2 [diff_file]\n");
}

int main(int argc, char *argv[])
{
	if (argc != 3 && argc != 4)
	{
		printHelp();
		return 0;
	}
	else
	{
		const char *path1 = argv[1];
		const char *path2 = argv[2];

		int img0w, img0h, img0bpp;
		unsigned char *img0data = stbi_load(path1, &img0w, &img0h, &img0bpp, 0);

		int img1w, img1h, img1bpp;
		unsigned char *img1data = stbi_load(path2, &img1w, &img1h, &img1bpp, 0);

		if (img0w != img1w && img0h != img1h && img0bpp != img1bpp)
		{
			stbi_image_free(img0data);
			stbi_image_free(img1data);
			return -1;
		}
		else
		{
			const int n = img0w * img0h * img0bpp;
			if (argc == 3) // Just compare
			{
				for (int i = 0; i < n; ++i)
				{
					if (img0data[i] != img1data[i])
					{
						stbi_image_free(img0data);
						stbi_image_free(img1data);
						return -1;
					}
				}
			}
			else // Output difference
			{
				bool equal = true;
				char *diffData = new char[n];
				for (int i = 0; i < n; ++i)
				{
					if (img0data[i] != img1data[i]) equal = false;
					const int diffVal = int(img0data[i]) - int(img1data[i]);
					diffData[i] = diffVal < 0 ? -diffVal : diffVal;
				}
				stbi_image_free(img0data);
				stbi_image_free(img1data);
				stbi_write_png(argv[3], img0w, img0h, img0bpp, diffData, 0);
				return equal ? 0 : -1;
			}
		}

		stbi_image_free(img0data);
		stbi_image_free(img1data);
		return 0;
	}
}