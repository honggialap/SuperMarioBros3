#include "pugixml.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
	pugi::xml_document sceneFile; 
	sceneFile.load_file(argv[1]);
	printf(argv[0]);

	int gridWidth = 0;
	gridWidth = atoi(argv[2]);
	printf("\n %d", gridWidth);

	int gridHeight = 0;
	gridHeight = atoi(argv[3]);
	printf("\n %d", gridHeight);

	for (
		auto gameObject = sceneFile.child("Scene").child("GameObject");
		gameObject;
		gameObject = gameObject.next_sibling("GameObject")
		)
	{
		float posX = gameObject.attribute("x").as_float();
		printf("\n %f", posX);

		float posY = gameObject.attribute("y").as_float();
		printf(" %f", posY);

		int gridX = 0;
		if (int(posX) > (gridWidth / 2))
		{
			gridX = (int(posX - (gridWidth / 2))/ gridWidth) + 1;
		}
		else if (int(posX) < (-gridWidth / 2))
		{
			gridX = (int(posX + (gridWidth / 2))/ gridWidth) - 1;
		}
		printf(" %d", gridX);

		int gridY = 0;
		if (int(posY) > (gridHeight / 2))
		{
			gridY = (int(posY - (gridHeight / 2)) / gridHeight) + 1;
		}
		else if (int(posY) < (-gridHeight / 2))
		{
			gridY = (int(posY + (gridHeight / 2)) / gridHeight) - 1;
		}
		printf(" %d", gridY);

		gameObject.attribute("gridX").set_value(gridX);
		gameObject.attribute("gridY").set_value(gridY);
	}
	sceneFile.save_file(argv[1]);
}