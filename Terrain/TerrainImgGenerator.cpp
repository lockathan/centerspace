#include <Terrain/TerrainImgGenerator.h>
#include <Templates/BattlefieldTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <Terrain/Divider.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>

TerrainImgGenerator::TerrainImgGenerator()
{

}

TerrainImgGenerator::~TerrainImgGenerator()
{

}

void TerrainImgGenerator::smooth(int* data, int originalX, int originalZ, int size)
{
	if( originalX < 0 || originalZ < 0 || originalX >= size || originalZ >= size)
		return;
	
	if((size*originalZ) + originalX >= size * size)
		return;
	int x, z, previousX, previousZ;
	int value, oldValue;
	for(int zDirection = -1;zDirection <= 1;++zDirection)
	{
		for(int xDirection = -1;xDirection <= 1;++xDirection)
		{
			if(xDirection == 0 && zDirection == 0)
				continue;

			x = originalX;
			z = originalZ;
			previousX = originalX;
			previousZ = originalZ;
			x+=xDirection;
			z+=zDirection;

			while(x >= 0 && x < size && z >= 0 && z < size)
			{
				value = data[(size*z) + x];
				oldValue = data[(size*previousZ) + previousX];
				data[(size*z) + x] = (int)(0.75f * data[(size*previousZ) + previousX] + 0.25f * data[(size*z) + x]);
				value = data[(size*z) + x];

				if(xDirection != 0 && zDirection == 0)
				{
					smooth(data, x, z, xDirection, -1, size);
					smooth(data, x, z, xDirection, 0, size);
					smooth(data, x, z, xDirection, 1, size);

				}

				if(xDirection == 0 && zDirection != 0)
				{
					smooth(data, x, z, -1, zDirection, size);
					smooth(data, x, z, 0, zDirection, size);
					smooth(data, x, z, 1, zDirection, size);
				}

				if(xDirection != 0 && zDirection != 0)
				{
					smooth(data, x, z, xDirection, zDirection, size);
					smooth(data, x, z, 0, zDirection, size);
					smooth(data, x, z, xDirection, 0, size);
				}

				x+=xDirection;
				z+=zDirection;
			}
		}
	}

}

void TerrainImgGenerator::smooth(int* data, int x, int z, int xDirection, int zDirection, int size)
{
	int previousX, previousZ;
	int oldValue;
	for(zDirection = -1;zDirection <= 1;++zDirection)
	{
		for(xDirection = -1;xDirection <= 1;++xDirection)
		{
			if(xDirection == 0 && zDirection == 0)
				continue;

			previousX = x;
			previousZ = z;
			x+=xDirection;
			z+=zDirection;

			while(x >= 0 && x < size && z >= 0 && z < size && previousX >= 0 && previousX < size && previousZ >= 0 && previousZ < size)
			{
				oldValue = data[(size*previousZ) + previousX];
				data[(size*z) + x] = (int)(0.75f * data[(size*previousZ) + previousX] + 0.25f * data[(size*z) + x]);

				x+=xDirection;
				z+=zDirection;
			}
		}
	}
}

void TerrainImgGenerator::writeToFile(int* data, int length, int count, int hillHeight, int x , int z)
{
	return;

	std::stringstream filename;
	filename << "output" << count << ".txt";
	std::ofstream outputFile;

	outputFile.open(filename.str(),std::ios_base::out);

	if(hillHeight != 0)
	{
		outputFile << "Height: " << hillHeight << " x " << x << " z " << z << std::endl;
	}
	for(int i = 0;i<length;++i)
	{
		for(int j = 0;j<length; ++j)
		{
			outputFile << data[(length*j)+i] << ' ';

		}
		outputFile << std::endl;
	}
	outputFile.close();
}

int* TerrainImgGenerator::GenerateTerrainIntArray(int size, int minHeight, int maxHeight, int maxHills, int maxRidges)
{
	int* data;
	int count = 0;
	data = new int[size*size];
	
	int range = maxHeight - minHeight;
	int plainHeight = rand() % range + minHeight - rand() % 5;

	for(int i = 0;i<size*size;++i)
	{
		data[i] = plainHeight;
	}

	int hills = rand() % maxHills + 1;
	int x = 0, z = 0;
	int value;
	writeToFile(data,33,count);
	for(int i = 0;i<hills;++i)
	{
		x = rand() % size;
		z = rand() % size;
		data[(size*z) + x] = rand() % (maxHeight - plainHeight) + plainHeight;
		value = data[(size*z) + x];
		smooth(data,x,z,size);
		count++;
		writeToFile(data,33,count,value,x,z);
	}

	
	return data;
}

//To calculate suitable source side length + expansion values:
//SourcesideLength should be 2^n+1
//expansion should be 2^x-1
//Some experimentation will reveal suitable values for n and x.

float* TerrainImgGenerator::GenerateTerrainData(int* source, long sourceSideLength, long sourceSize, int expansion)
{
	Divider* divider = new Divider();
	int newSideLength = ((sourceSideLength - 1) * expansion) + sourceSideLength;
	
	int resultSize = newSideLength * newSideLength;
	float* result;
	result = new float[resultSize];
	int topLeft = 0, topRight = 0, bottomLeft = 0, bottomRight = 0, currentRow = 0, startOfRow = 0, previous = 0;
	int dTopLeftTopRight = 0;
	int dTopLeftBottomLeft = 0;
	int dBottomLeftBottomRight = 0;
	int dTopRightBottomRight = 0;

	topRight = topLeft + expansion + 1;
	bottomLeft = topLeft + (newSideLength * (expansion + 1));
	bottomRight = bottomLeft + expansion + 1;

	int i = 0, j = 0, temp = 0;
		
	while(i < sourceSize)
	{
		temp = source[i];
		result[topLeft + j] = source[i];
		i++;
		j+= expansion + 1;
		if(j >= newSideLength)
		{
			currentRow+= expansion + 1;
			j = 0;
			topLeft = currentRow * newSideLength;
		}
	}
	topLeft = 0;
	currentRow = 0;
		
	while(bottomRight <= resultSize)
	{
			
		//do work
		dTopLeftBottomLeft = result[topLeft] - result[bottomLeft];
		dTopLeftTopRight = result[topLeft] - result[topRight];
		dTopRightBottomRight = result[topRight] - result[bottomRight];
		dBottomLeftBottomRight = result[bottomLeft] - result[bottomRight];
			
		divider->initialize(dTopLeftBottomLeft, expansion);
		previous = topLeft;
		for(int k = topLeft +  newSideLength; k < bottomLeft;k+= newSideLength)
		{
			result[k] = result[previous] - divider->pop();
			float test = result[k];
			previous = k;
		}
			
		divider->initialize(dTopRightBottomRight, expansion);
		previous = topRight;
		for(int k = topRight +  newSideLength; k < bottomRight;k+= newSideLength)
		{
			result[k] = result[previous] - divider->pop();
			float test = result[k];
			previous = k;
		}
			
			
		for(int k = topLeft; k <= bottomLeft;k+= newSideLength)
		{
			previous = k;
			divider->initialize(result[k] - result[k + expansion + 1], expansion);
			for(int l = k + 1; l <= k + expansion;++l)
			{
				result[l] = result[previous] - divider->pop();
				previous = l;
			}
		}
			
		//increment counters
		topLeft+= expansion + 1;
		topRight+= expansion + 1;
		bottomLeft += expansion + 1;
		bottomRight += expansion + 1;
			
		if(topRight - startOfRow >= newSideLength)
		{
			currentRow+= expansion + 1;
			startOfRow = newSideLength * currentRow;
			topLeft = (currentRow * newSideLength);
			topRight = topLeft + expansion + 1;
			bottomLeft = topLeft + (newSideLength * (expansion + 1));
			bottomRight = bottomLeft + expansion + 1;
		}
	}
		
		
	i = newSideLength + 1;
		
	while(i < resultSize - (newSideLength + 1))
	{
		result[i] = result[i] + result[i - 1] + result[i + 1] + result[i + newSideLength + 1] + result[i - newSideLength + 1] + result[i + newSideLength - 1] + result[i - newSideLength - 1] + result[i + newSideLength] + result[i - newSideLength];
		result[i]/=9;
		i++;
	}

	/*
	for(int i = 0;i < resultSize;++i)
	{

	}
	*/
	delete [] source;
	delete divider;
	return result;
}


int* TerrainImgGenerator::GenerateSimpleTerrainData(int* source,long sourceSideLength, long sourceArraySize, int expansion)
{
	
	if(expansion == 0)
	{
		return source;
	}

	IntegerDivider* divider = new IntegerDivider();
	int newSideLength = ((sourceSideLength - 1) * expansion) + sourceSideLength;
	
	int resultSize = newSideLength * newSideLength;
	int* result;
	result = new int[resultSize];
	int topLeft = 0, topRight = 0, bottomLeft = 0, bottomRight = 0, currentRow = 0, startOfRow = 0, previous = 0;
	int dTopLeftTopRight = 0;
	int dTopLeftBottomLeft = 0;
	int dBottomLeftBottomRight = 0;
	int dTopRightBottomRight = 0;

	topRight = topLeft + expansion + 1;
	bottomLeft = topLeft + (newSideLength * (expansion + 1));
	bottomRight = bottomLeft + expansion + 1;

	int i = 0, j = 0, temp = 0;
		
	while(i < sourceArraySize)
	{
		temp = source[i];
		result[topLeft + j] = source[i];
		i++;
		j+= expansion + 1;
		if(j >= newSideLength)
		{
			currentRow+= expansion + 1;
			j = 0;
			topLeft = currentRow * newSideLength;
		}
	}
	topLeft = 0;
	currentRow = 0;
		
	while(bottomRight <= resultSize)
	{
			
		//do work
		dTopLeftBottomLeft = result[topLeft] - result[bottomLeft];
		dTopLeftTopRight = result[topLeft] - result[topRight];
		dTopRightBottomRight = result[topRight] - result[bottomRight];
		dBottomLeftBottomRight = result[bottomLeft] - result[bottomRight];
			
		divider->initialize(dTopLeftBottomLeft, expansion);
		previous = topLeft;
		for(int k = topLeft +  newSideLength; k < bottomLeft;k+= newSideLength)
		{
			result[k] = result[previous] - divider->pop();
			int test = result[k];
			previous = k;
		}
			
		divider->initialize(dTopRightBottomRight, expansion);
		previous = topRight;
		for(int k = topRight +  newSideLength; k < bottomRight;k+= newSideLength)
		{
			result[k] = result[previous] - divider->pop();
			int test = result[k];
			previous = k;
		}
			
			
		for(int k = topLeft; k <= bottomLeft;k+= newSideLength)
		{
			previous = k;
			divider->initialize(result[k] - result[k + expansion + 1], expansion);
			for(int l = k + 1; l <= k + expansion;++l)
			{
				result[l] = result[previous] - divider->pop();
				previous = l;
			}
		}
			
		//increment counters
		topLeft+= expansion + 1;
		topRight+= expansion + 1;
		bottomLeft += expansion + 1;
		bottomRight += expansion + 1;
			
		if(topRight - startOfRow >= newSideLength)
		{
			currentRow+= expansion + 1;
			startOfRow = newSideLength * currentRow;
			topLeft = (currentRow * newSideLength);
			topRight = topLeft + expansion + 1;
			bottomLeft = topLeft + (newSideLength * (expansion + 1));
			bottomRight = bottomLeft + expansion + 1;
		}
	}
		
		
	i = newSideLength + 1;
		
	while(i < resultSize - (newSideLength + 1))
	{
		result[i] = result[i] + result[i - 1] + result[i + 1] + result[i + newSideLength + 1] + result[i - newSideLength + 1] + result[i + newSideLength - 1] + result[i - newSideLength - 1] + result[i + newSideLength] + result[i - newSideLength];
		result[i]/=9;
		i++;
	}

	/*
	for(int i = 0;i < resultSize;++i)
	{

	}
	*/
	delete [] source;
	delete divider;
	return result;
}

int* TerrainImgGenerator::GenerateHeightData(BattlefieldTemplate* battlefield, TerrainTemplate* terrain)
{
	return GenerateSimpleTerrainData(GenerateTerrainIntArray(battlefield->getInitialSize(),terrain->getMinHeight(),terrain->getMaxHeight(),5,1),battlefield->getInitialSize(),battlefield->getInitialSize()*battlefield->getInitialSize(),0);
}

TerrainImgGenerator* TerrainImgGenerator::mTerrainImgGenerator;

TerrainImgGenerator* TerrainImgGenerator::getSingletonPtr()
{
	if(!mTerrainImgGenerator)
	{
		mTerrainImgGenerator = new TerrainImgGenerator();
	}

	return mTerrainImgGenerator;
}