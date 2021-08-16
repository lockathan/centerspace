#ifndef TerrainImgGenerator_h
#define TerrainImgGenerator_h

class BattlefieldTemplate;
class TerrainTemplate;

class TerrainImgGenerator
{
public:
	
	TerrainImgGenerator();
	~TerrainImgGenerator();

	int* GenerateTerrainIntArray(int size, int minHeight, int maxHeight, int maxHills, int maxRidges);
	float* GenerateTerrainData(int* source,long sourceSideLength, long sourceArraySize, int expansion);
	int* GenerateSimpleTerrainData(int* source,long sourceSideLength, long sourceArraySize, int expansion);

	int* GenerateHeightData(BattlefieldTemplate* battlefield, TerrainTemplate* terrain);

	static TerrainImgGenerator* getSingletonPtr();
private:
	static TerrainImgGenerator* mTerrainImgGenerator;
	void smooth(int* data, int x, int z, int size);
	void smooth(int* data, int x, int z, int xDirection, int zDirection, int size);
	void writeToFile(int* data, int length, int count, int hillHeight = 0, int x = 0, int z = 0);

};
#endif