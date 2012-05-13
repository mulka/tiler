#include <vips/vipscpp.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sys/time.h>

using namespace std;

int main(int argc, char **argv){
        try{
                VImage image("image.v", "r");
		char extension[20];
		strcpy(extension, argv[1]);
		if(strcmp(extension, "gif") == 0){
			strcpy(extension, "png");
		}
		int width = image.Xsize();
		int height = image.Ysize();
		int maxLength = max(width, height);
		int levels = (int) ceil(log(maxLength)/log(2)) - 7;
		if(levels > 10 || levels < 1){
			cout << "levels must be between 1 and 10" << endl;
			exit(1);
		}
		
		int w = (int) pow(2.0, levels + 7);
		int h = w;
		
		cout << "adding borders" << endl;
		int level = levels - 1;
		char fileName[20];
                sprintf(fileName, "image%d.v", level);
		image.embed(4, (int) floor((w - width)/2), 
				(int) floor((h - height)/2), 
				(int) pow(2.0, levels + 7), 
				(int) pow(2.0, levels + 7)
			).write(fileName);
		for(level = levels - 1; level >= 0;){
			cout << "tiling level " << level << endl;
			char fileName[20];
                        sprintf(fileName, "image%d.v", level);
                        VImage levelImage(fileName, "r");
			int max = (int) pow(2.0, level);
			int n = 0;
			int y = 0;
			for(int j = 0; j < max; j++){
				int x = 0;
				for(int i = 0; i < max; i++){
					cout << '.' << flush;
					char name[300];
					sprintf(name, "tile-%d-%d.%s", level, n, extension);
					//cout << "saveing " << name << endl;
					levelImage.extract_area(x, y, 256, 256).write(name);
					n++;
					x += 256;
				}
				y += 256;
			}
			level--;
			if(level >= 0){
				cout << endl << "shrinking for level " << level << endl;
				char fileName[20];
                        	sprintf(fileName, "image%d.v", level);
				levelImage.affine(.5, 0, 0, .5, 0, 0, 0, 0, 
						(int) floor(levelImage.Xsize() * 0.5), 
						(int) floor(levelImage.Ysize() * 0.5)
					).write(fileName);
			}
		}
        }catch(VError e){
                e.perror(argv[1]);
        }
}
