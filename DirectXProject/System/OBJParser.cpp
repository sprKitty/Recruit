#include "OBJParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <System/DebugLog.h>

const bool NewOBJParser::Load(const std::string& path, const std::string& name)
{
	// �t�@�C���ǂݍ���
	std::ifstream readfile;
	std::string filePath = path + name;
	readfile.open(filePath, std::ios::in);

	if (!readfile.is_open())
	{
		DebugLog::GetInstance().LoadFileError(std::string(path + name).c_str());
		return false; // �ǂݍ��ݎ��s
	}

	std::string line;

	while (std::getline(readfile, line))
	{
		std::istringstream i_stream(line);
		std::string word;

		if (std::getline(i_stream, word, ' '))
		{
			if (word.empty())continue; // �󔒂Ȃ�X�L�b�v

			if (word == "mtllib") // �}�e���A����ǂݍ���
			{
				if (std::getline(i_stream, word, ' '))
				{
					LoadMaterial(path + word);
				}
			}
			else if (word == "v") // ���_��ǂݍ���
			{
				Vector3 vertex;
				if (std::getline(i_stream, word, ' '))
				{
					vertex.x = std::stof(word);
				}
				if (std::getline(i_stream, word, ' '))
				{
					vertex.y = std::stof(word);
				}
				if (std::getline(i_stream, word, ' '))
				{
					vertex.z = std::stof(word);
				}
				m_vtxList.emplace_back(vertex);
			}
			else if (word == "vt") // texture(uv)��ǂݍ���
			{
				Vector2 uv;
				if (std::getline(i_stream, word, ' '))
				{
					uv.x = std::stof(word);
				}
				if (std::getline(i_stream, word, ' '))
				{
					uv.y = std::stof(word);
					uv.y = 1 - uv.y;
				}
				m_uvList.emplace_back(uv);
			}
			else if (word == "vn") // �@����ǂݍ���
			{
				Vector3 normal;
				if (std::getline(i_stream, word, ' '))
				{
					normal.x = std::stof(word);
				}
				if (std::getline(i_stream, word, ' '))
				{
					normal.y = std::stof(word);
				}
				if (std::getline(i_stream, word, ' '))
				{
					normal.z = std::stof(word);
				}
				m_normalList.emplace_back(normal);
			}
			else if (word == "usemtl") // �C���f�b�N�X��ǂݍ���
			{
				std::string name;
				if (std::getline(i_stream, name, ' '))
				{
					while (std::getline(readfile, line))
					{
						std::istringstream i_streamIndex(line);

						if (std::getline(i_streamIndex, word, ' '))
						{
							if (word == "usemtl") // �V�����}�e���A�����ݒ肳��Ă����烋�[�v�𔲂���
							{
								if (std::getline(i_streamIndex, name, ' '))
								{
									int fs = 0;
								}
								else
								{
									break;
								}
							}
							else if (word == "f") // �C���f�b�N�X��ǂݍ���
							{
								for (UINT8 i = 0; i < 3; ++i)
								{
									if (std::getline(i_streamIndex, word, ' '))
									{
										LoadIndex(name, word);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}

const bool NewOBJParser::LoadMaterial(const std::string& str)
{
	std::ifstream readfile;
	readfile.open(str, std::ios::in);

	if (!readfile.is_open())
	{
		DebugLog::GetInstance().LoadFileError(str.c_str());
		return false; // �ǂݍ��ݎ��s
	}

	std::string line;
	while (std::getline(readfile, line))
	{
		std::istringstream i_stream(line);
		std::string word;

		if (std::getline(i_stream, word, ' '))
		{
			if (word.empty())continue; // �󔒂Ȃ�X�L�b�v
			std::string matName; // �}�e���A���̖��O

			if (word == "newmtl")
			{
				Material mat;
				if (std::getline(i_stream, word, ' '))
				{
					matName = word;
				}

				while (std::getline(readfile, line))
				{
					std::istringstream i_streamMat(line);
					if (std::getline(i_streamMat, word, ' '))
					{
						if (word == "Ns")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ns = std::stof(word);
							}
						}
						else if (word == "Ka")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ka.x = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ka.y = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ka .z= std::stof(word);
							}
						}
						else if (word == "Kd")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Kd.x = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Kd.y = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Kd.z = std::stof(word);
							}
						}
						else if (word == "Ks")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ks.x = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ks.y = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ks.z = std::stof(word);
							}
						}
						else if (word == "Ke")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ke.x = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ke.y = std::stof(word);
							}
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ke.z = std::stof(word);
							}
						}
						else if (word == "Ni")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.Ni = std::stof(word);
							}
						}
						else if (word == "d")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.d = std::stof(word);
							}
						}
						else if (word == "illum")
						{
							if (std::getline(i_streamMat, word, ' '))
							{
								mat.illum = std::stoi(word);
							}
						}
					}
					else
					{
						break;
					}
				}
				m_materialMap[matName] = mat;
			}
			else
			{
				continue;
			}
		}
	}
	return true;
}

const bool NewOBJParser::LoadIndex(const std::string & str, const std::string & data)
{
	std::istringstream i_stream(data);
	std::string word;
	VectorInt3 val;

	if (std::getline(i_stream, word, '/')) val.x = std::stoi(word); // ���_�C���f�b�N�X�������Ă����(���f���ɖ�肪����)false��Ԃ�
	else return false;

	if (std::getline(i_stream, word, '/')) val.y = std::stoi(word); // texture���̓��f���ɐݒ肳��Ă��Ȃ��ꍇ������̂�return��false�͕Ԃ��Ȃ�

	if (std::getline(i_stream, word, '/')) val.z = std::stoi(word); // �@���C���f�b�N�X�������Ă����(���f���ɖ�肪����)false��Ԃ�
	else return false;

	m_indexMap[str].emplace_back(val);

	return true;
}

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