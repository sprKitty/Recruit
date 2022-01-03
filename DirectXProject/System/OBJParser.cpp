#include "OBJParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OBJParser::OBJParser()
{
}

OBJParser::~OBJParser()
{
}

bool OBJParser::Load(const char * fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rt");
	if (!fp)
	{
		return false;
	}
	while (1)
	{
		char lineHeader[256];

		int res = fscanf(fp, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0)
		{
			Vector3 vtx;
			fscanf(fp, "%f %f %fn", &vtx.x, &vtx.y, &vtx.z);
			m_vtxList.emplace_back(vtx);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			Vector2 uv;
			fscanf(fp, "%f %fn", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			m_uvList.emplace_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			Vector3 normal;
			fscanf(fp, "%f %f %f n", &normal.x, &normal.y, &normal.z);
			m_normalList.emplace_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vtx1, vtx2, vtx3;
			unsigned int vtxIdx[3], uvIdx[3], normalIdx[3];
			int matches = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d n", &vtxIdx[0], &uvIdx[0], &normalIdx[0], &vtxIdx[1], &uvIdx[1], &normalIdx[1], &vtxIdx[2], &uvIdx[2], &normalIdx[2]);
			if (matches != 9)
				return false;

			m_vtxIndexList.emplace_back(vtxIdx[0]);
			m_vtxIndexList.emplace_back(vtxIdx[1]);
			m_vtxIndexList.emplace_back(vtxIdx[2]);
			m_uvIndexList.emplace_back(uvIdx[0]);
			m_uvIndexList.emplace_back(uvIdx[1]);
			m_uvIndexList.emplace_back(uvIdx[2]);
			m_normalIndexList.emplace_back(normalIdx[0]);
			m_normalIndexList.emplace_back(normalIdx[1]);
			m_normalIndexList.emplace_back(normalIdx[2]);
		}
	}
	fclose(fp);
	return true;
}
